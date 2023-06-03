/*
 * Copyright (c) 2016 David Gwynne <dlg@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * severely mutilated by emmy emmycelium 2023
 *
 * I did my own impelementations of all the algorithms but the basic structure
 * and names come from David Gwynne's code in OpenBSD sys/kern/subr_tree.c
 */

#include <stddef.h>

#include "tree.h"

// gives color from an rbe pointer, NULL is considered black.
#define RBE_COLOR(rbe) (rbe ? rbe->color : BLACK)

static struct rb_entry *rbe_rotate_left(struct rb_entry *);
static struct rb_entry *rbe_rotate_right(struct rb_entry *);

static inline struct rb_entry *
rb_n2e(const struct rb_type *t, const void *node)
{
	return ((struct rb_entry *)(node + t->offset));
}

static inline void *
rb_e2n(const struct rb_type *t, const struct rb_entry *rbe)
{
	return ((void *)rbe) - t->offset;
}

/*
 * _rb_insert -- inserts an element elm into the tree rbt. if an element with
 * the same key is already in the tree it will be returned, otherwise NULL.
 */
void *
_rb_insert(const struct rb_type *t, rb_tree *rbt, void *elm)
{
	struct rb_entry *rbe = rb_n2e(t, elm);
	struct rb_entry *tmp, *parent, *uncle, *gp;
	void		*node;
	int		 cmp;
	enum { LEFT, RIGHT } ndir, pdir;

	tmp = *rbt;

	while (tmp != NULL) {
		parent = tmp;
		node = rb_e2n(t, tmp);

		cmp = t->compare(elm, node);
		if (cmp < 0)
			tmp = tmp->left;
		else if (cmp > 0)
			tmp = tmp->right;
		else
			return node; /* the node was already here */
	}

	rbe->parent = parent;
	rbe->left = NULL;
	rbe->right = NULL;
	rbe->color = RED;

	if (cmp < 0) {
		parent->left = rbe;
		ndir = LEFT;
	} else {
		parent->right = rbe;
		ndir = RIGHT;
	}

	/* now the element is in the tree but if parent is red then we have a
	 * color violation to fix */
rebalance:
	/* easiest case, no color violation */
	if (RBE_COLOR(parent) == BLACK)
		return NULL;

	gp = parent->parent;
	/* if there's no grandparent, we can just recolor the parent */
	if (gp == NULL) {
		parent->color = BLACK;
		return NULL;
	}

	if (gp->left == parent) {
		uncle = gp->right;
		pdir = LEFT;
	} else {
		uncle = gp->left;
		pdir = RIGHT;
	}

	/*
	 * if parent and uncle both red then we can make them black and make
	 * grandparent red, but now grandparent might have a color violation
	 */
	if (RBE_COLOR(uncle) == RED) {
		parent->color = BLACK;
		if (uncle)
			uncle->color = BLACK;
		gp->color = RED;
		parent = gp->parent;
		goto rebalance;
	}

	/*
	 * if uncle is black instead we need to do some rotating
	 *
	 * when. me when i. when u rotate
	 */

	/*
	 * current node is an "inner" grandchild, so we need to rotate so its
	 * not
	 */
	if (ndir != pdir) {
		if (ndir == RIGHT)
			parent = rbe_rotate_left(parent);
		else
			parent = rbe_rotate_right(parent);
		ndir = pdir;
	}
	/* now we know its an outer grandchild */
	if (ndir == RIGHT)
		rbe_rotate_left(gp);
	else
		rbe_rotate_right(gp);

	/* recolor now that everything is in the right place */
	gp->color = RED;
	parent->color = BLACK;

	return NULL;
}

void *
_rb_remove(const struct rb_type *t, rb_tree *rbt, void *elm)
{
	(void)t;
	(void)rbt;
	(void)elm;
	return NULL;
}

/*
 * _rb_find -- finds the node with a given key and returns it, or NULL if not
 * in the tree
 */
void *
_rb_find(const struct rb_type *t, rb_tree *rbt, const void *key)
{
	struct rb_entry *tmp = *rbt;
	void		*node;
	int		 cmp;

	while (tmp) {
		node = rb_e2n(t, tmp);
		cmp = t->compare(key, node);
		if (cmp < 0)
			tmp = tmp->left;
		else if (cmp > 0)
			tmp = tmp->right;
		else
			return node;
	}
	return NULL;
}

void *
_rb_left(const struct rb_type *t, const void *node)
{
	struct rb_entry *rbe = rb_n2e(t, node);
	rbe = rbe->left;
	return rbe == NULL ? NULL : rb_e2n(t, rbe);
}

void *
_rb_right(const struct rb_type *t, const void *node)
{
	struct rb_entry *rbe = rb_n2e(t, node);
	rbe = rbe->right;
	return rbe == NULL ? NULL : rb_e2n(t, rbe);
}

void *
_rb_parent(const struct rb_type *t, const void *node)
{
	struct rb_entry *rbe = rb_n2e(t, node);
	rbe = rbe->parent;
	return rbe == NULL ? NULL : rb_e2n(t, rbe);
}

/*
 * rbe_rotate_left -- rotates a tree left and returns the new element
 * occupying the place previously held by node.
 *
 * will fail if node has no right child to rotate into its place
 */
static struct rb_entry *
rbe_rotate_left(struct rb_entry *node)
{
	struct rb_entry *r_child = node->right;

	/* trying to rotate with a null child would break everything */
	if (r_child == NULL)
		return NULL;

	node->right = r_child->left;
	r_child->left = node;
	r_child->parent = node->parent;
	node->parent = r_child;

	return r_child;
}

/*
 * rbe_rotate_right -- rotates a tree right and returns the new element
 * occupying the place previously held by node
 *
 * will fail if node has no left child to rotate into its place
 */
static struct rb_entry *
rbe_rotate_right(struct rb_entry *node)
{
	struct rb_entry *l_child = node->left;

	/* trying to rotate with a null child would break everything */
	if (l_child == NULL)
		return NULL;

	node->left = l_child->right;
	l_child->right = node;
	l_child->parent = node->parent;
	node->parent = l_child;

	return l_child;
}

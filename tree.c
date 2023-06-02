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

static inline struct rb_entry *
rb_n2e(const struct rb_type *t, const void *node)
{
	return ((struct rb_entry *)(node + t->t_offset));
}

static inline void *
rb_e2n(const struct rb_type *t, const struct rb_entry *rbe)
{
	return ((void *)rbe) - t->t_offset;
}

void *
_rb_insert(const struct rb_type *t, rb_tree *rbt, void *elm)
{
	return NULL;
}
void *
_rb_remove(const struct rb_type *t, rb_tree *rbt, void *elm)
{
	return NULL;
}

void *
_rb_find(const struct rb_type *t, rb_tree *rbt, const void *elm)
{
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

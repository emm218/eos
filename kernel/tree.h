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

/* severely mutilated by emmy emmycelium 2023 */

#ifndef _TREE_H
#define _TREE_H
#include <stddef.h>

struct rb_type {
	int (*compare)(const void *, const void *);
	unsigned int offset;
};

struct rb_entry {
	struct rb_entry *left;
	struct rb_entry *right;
	struct rb_entry *parent;
	enum { RED, BLACK } color;
};

typedef struct rb_entry *rb_tree;

void *_rb_insert(const struct rb_type *, rb_tree *, void *);
void *_rb_remove(const struct rb_type *, rb_tree *, void *);
void *_rb_find(const struct rb_type *, rb_tree *, const void *);
void *_rb_left(const struct rb_type *, const void *);
void *_rb_right(const struct rb_type *, const void *);
void *_rb_parent(const struct rb_type *, const void *);

/* clang-format off */
/* clang-format can't make head or tails of this... */
#define RBT_GENERATE(name, type, field, cmp)			\
typedef rb_tree name;						\
								\
static int							\
name##_RBT_COMPARE(const void *lptr, const void *rptr)		\
{								\
	const type *l = lptr, *r = rptr;			\
	return cmp(l, r);					\
}								\
static const struct rb_type name##_RBT_INFO = {			\
	name##_RBT_COMPARE,					\
	offsetof(type, field),					\
}; 								\
const struct rb_type *const name##_RBT_TYPE = &name##_RBT_INFO;	\
								\
__attribute__((unused)) static inline type *			\
name##_RBT_INSERT(name *head, type *elm)			\
{								\
	return _rb_insert(name##_RBT_TYPE, head, elm);		\
}								\
								\
__attribute__((unused)) static inline type *			\
name##_RBT_REMOVE(name *head, type *elm)			\
{								\
	return _rb_remove(name##_RBT_TYPE, head, elm);		\
}								\
								\
__attribute__((unused)) static inline type *			\
name##_RBT_FIND(name *head, type *elm)				\
{								\
	return _rb_find(name##_RBT_TYPE, head, elm);		\
}								\
								\
__attribute__((unused)) static inline type *			\
name##_RBT_LEFT(type *elm)					\
{								\
	return _rb_left(name##_RBT_TYPE, elm);			\
}								\
								\
__attribute__((unused)) static inline type *			\
name##_RBT_RIGHT(type *elm)					\
{								\
	return _rb_right(name##_RBT_TYPE, elm);			\
}								\
								\
__attribute__((unused)) static inline type *			\
name##_RBT_PARENT(type *elm)					\
{								\
	return _rb_parent(name##_RBT_TYPE, elm);		\
}
/* clang-format on */
#endif /* _TREE_H */

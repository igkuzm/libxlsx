/**
 * File              : array.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 22.02.2022
 * Last Modified Date: 21.05.2024
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/**
 * array.h
 * Copyright (c) 2022 Igor V. Sementsov <ig.kuzm@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * C library for dynamic arrays
 * struct array_t * array_new(T) - allocate and return new array of types T
 * void array_append(struct array_t * array, T, T item) - add item to array with
 * type T
 * T item * array_at(struct array_t * array, T, int i) - return item from array
 * at position i
 * array_for_each(struct array_t * array, T, T item){} - iterate the array
 * void array_free(struct array_t * array) - free array
 */

#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

// Dynamyc array
typedef struct array {
  void *data;
  int len;
} array_t;

#define array_new(T, on_error)\
({\
  array_t *a = malloc(sizeof(array_t));\
  if (!a) {\
		on_error; \
	} else { \
		a->data = malloc(sizeof(T));\
		if (!a->data) {\
			on_error; \
		} \
		a->len = 0;\
	}\
  a;\
})

#define array_append(a, T, item, on_error)\
({\
	T *d = a->data;\
	d[a->len++] = item;\
	T *p = realloc(a->data, sizeof(T) * (a->len + 1));\
	if (!p) {\
		on_error; \
	} else { \
		a->data = p; \
	}\
})

#define array_for_each(a, T, item)\
for (T *p = (a->data), (item) = *p; p < &(((T*)(a->data))[a->len]);\
     p++, (item) = *p)

#define array_free(a)\
  free(a->data);\
free(a);

#endif // ARRAY_H

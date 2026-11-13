/*
 * Doubly-linked list implementation.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

typedef struct ListItem {
    struct ListItem *next;
    struct ListItem *prev;
} ListItem;

typedef struct {
    ListItem *first;
    ListItem *last;
} List;

#define listEnumerateItems(list, item) for ((item) = (list)->first; (void *) (item) != (void *) (list); (item) = (item)->next)

static inline void listItemRemove(ListItem *item) {
    item->prev->next = item->next;
    item->next->prev = item->prev;
}

static inline void listItemInsert(ListItem *item, ListItem *prev, ListItem *next) {
    item->prev = prev;
    item->prev->next = item;
    item->next = next;
    item->next->prev = item;
}

static inline void listReset(List *list) {
    list->first = (ListItem *) list;
    list->last = (ListItem *) list;
}

static inline void listAppend(List *list, ListItem *item) {
    listItemInsert(item, list->last, (ListItem *) list);
}

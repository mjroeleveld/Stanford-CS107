//
// Created by Joachim Roeleveld on 13/05/2020.
//

#include "hashset.h"
#include "articleset.h"
#include "article.h"

#ifndef RSS_NEWS_SEARCH_INDEX_H
#define RSS_NEWS_SEARCH_INDEX_H

typedef hashset Index;

typedef struct {
  char word[128];
  vector *articles;
} IndexItem;

void IndexNew(Index *i, int numBuckets);

void IndexDispose(Index *i);

void IndexEnter(Index *i, char *word, Article *article);

IndexItem * IndexLookup(Index *i, const char *word);

static int IndexHashFunction(const void *elemAddr, int numBuckets);

static int IndexCompareFunction(const void *elemAddr1, const void *elemAddr);

static void IndexFreeFunction(void *elemAddr);

static int ArticlePntrsCompareFunction(const void *address1, const void *address);

#endif //RSS_NEWS_SEARCH_INDEX_H

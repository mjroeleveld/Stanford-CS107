//
// Created by Joachim Roeleveld on 13/05/2020.
//

#include "stdlib.h"
#include "index.h"
#include "util.h"

void IndexNew(Index *i, int numBuckets)
{
  HashSetNew(i, sizeof(IndexItem), numBuckets, IndexHashFunction, IndexCompareFunction, IndexFreeFunction);
}

void IndexDispose(Index *i)
{
  HashSetDispose(i);
}

void IndexEnter(Index *i, char *word, Article *article)
{
  IndexItem item;
  strcpy(item.word, word);
  IndexItem *found = HashSetLookup(i, &item);
  // Word not yet in index; insert item
  if (found == NULL) {
    vector *articles = (vector*) malloc(sizeof(vector));
    VectorNew(articles, sizeof(Article*), NULL, 4);
    VectorAppend(articles, &article);
    item.articles = articles;
    HashSetEnter(i, &item);
  }
  // Word in index; append article
  else {
    if (VectorSearch(found->articles, &article, ArticlePntrsCompareFunction, 0, false) == -1)
      VectorAppend(found->articles, &article);
  }
}

IndexItem * IndexLookup(Index *i, const char *word)
{
  IndexItem item;
  strcpy(item.word, word);
  return HashSetLookup(i, &item);
}

static int IndexHashFunction(const void *elemAddr, int numBuckets)
{
  return StringHash(((IndexItem*) elemAddr)->word, numBuckets);
}

static int IndexCompareFunction(const void *elemAddr1, const void *elemAddr)
{
  return strcmp(((IndexItem*) elemAddr1)->word, ((IndexItem*) elemAddr)->word);
}

static void IndexFreeFunction(void *elemAddr)
{
  IndexItem *item = (IndexItem*) elemAddr;
  free(item->articles);
}

static int ArticlePntrsCompareFunction(const void *address1, const void *address)
{
  return (Article*) address1 == (Article*) address;
}

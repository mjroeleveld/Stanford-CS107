//
// Created by Joachim Roeleveld on 13/05/2020.
//

#include "stdlib.h"
#include "util.h"
#include "article.h"

int ArticleSetHashFunction(const void *elemAddr, int numBuckets)
{
  return StringHash((*((Article**)elemAddr))->url, numBuckets);
}

int ArticleSetCompareFunction(const void *elemAddr1, const void *elemAddr)
{
  return strcmp((*((Article**)elemAddr1))->url, (*((Article**)elemAddr))->url);
}

void ArticleSetFreeFunction(void *elemAddr)
{
  free(*((Article**)elemAddr));
}


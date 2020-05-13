//
// Created by Joachim Roeleveld on 13/05/2020.
//

#ifndef ASSN_4_RSS_ARTICLESET_H
#define ASSN_4_RSS_ARTICLESET_H

#include "hashset.h"

typedef hashset ArticleSet;

int ArticleSetHashFunction(const void *elemAddr, int numBuckets);

int ArticleSetCompareFunction(const void *elemAddr1, const void *elemAddr);

void ArticleSetFreeFunction(void *elemAddr);

#endif //ASSN_4_RSS_ARTICLESET_H

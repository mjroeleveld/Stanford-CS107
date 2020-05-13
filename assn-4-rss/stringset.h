//
// Created by Joachim Roeleveld on 13/05/2020.
//

#ifndef ASSN_4_RSS_STRINGSET_H
#define ASSN_4_RSS_STRINGSET_H

#include "hashset.h"

typedef hashset StringSet;

int StringSetHashFunction(const void *elemAddr, int numBuckets);

int StringSetCompareFunction(const void *elemAddr1, const void *elemAddr);

void StringSetFreeFunction(void *elemAddr);

#endif //ASSN_4_RSS_STRINGSET_H

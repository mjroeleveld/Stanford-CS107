//
// Created by Joachim Roeleveld on 13/05/2020.
//

#include "string.h"
#include "ctype.h"

#ifndef RSS_NEWS_SEARCH_UTIL_H
#define RSS_NEWS_SEARCH_UTIL_H

static const signed long kHashMultiplier = -1664117991L;
int StringHash(const char *s, int numBuckets);

#endif //RSS_NEWS_SEARCH_UTIL_H

//
// Created by Joachim Roeleveld on 13/05/2020.
//

#include "stdlib.h"
#include "util.h"

int StringSetHashFunction(const void *elemAddr, int numBuckets)
{
  return StringHash(*((char**) elemAddr), numBuckets);
}

int StringSetCompareFunction(const void *elemAddr1, const void *elemAddr)
{
  return strcmp(*((char**)elemAddr1), *((char**)elemAddr));
}

void StringSetFreeFunction(void *elemAddr)
{
  free(*((char**)elemAddr));
}


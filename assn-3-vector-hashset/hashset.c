#include "hashset.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void HashSetNew(hashset *h, int elemSize, int numBuckets,
		HashSetHashFunction hashfn, HashSetCompareFunction comparefn, HashSetFreeFunction freefn)
{
  assert(elemSize > 0);
  assert(numBuckets > 0);
  assert(hashfn != NULL);
  assert(comparefn != NULL);

  h->numBuckets = numBuckets;
  h->elemSize = elemSize;
  h->size = 0;
  h->freeFn = freefn;
  h->hashFn = hashfn;
  h->cmpFn = comparefn;

  h->buckets = malloc(numBuckets * sizeof(vector));
  for (int i = 0; i < numBuckets; i++) {
    vector vectorI;
    VectorNew(&vectorI, h->elemSize, freefn, 1);
    vector *bucket = h->buckets + i;
    *bucket = vectorI;
  }
}

void HashSetDispose(hashset *h)
{
  for (int b = 0; b < h->numBuckets; b++)
    VectorDispose(h->buckets + b);
}

int HashSetCount(const hashset *h)
{ return h->size; }

void HashSetMap(hashset *h, HashSetMapFunction mapfn, void *auxData)
{
  for (int b = 0; b < h->numBuckets; b++)
    VectorMap(h->buckets + b, mapfn, auxData);
}

void HashSetEnter(hashset *h, const void *elemAddr)
{
  assert(elemAddr != NULL);

  int hash = h->hashFn(elemAddr, h->numBuckets);
  assert(h->size > hash >= 0);
  vector* bucket = h->buckets + hash;

  int foundIndex = VectorSearch(bucket, elemAddr, h->cmpFn, 0, false);
  if (foundIndex == -1) {
    VectorAppend(bucket, elemAddr);
    h->size++;
  }
  else {
    VectorReplace(bucket, elemAddr, foundIndex);
  }
}

void *HashSetLookup(const hashset *h, const void *elemAddr)
{
  assert(elemAddr != NULL);

  int hash = h->hashFn(elemAddr, h->numBuckets);
  assert(h->size > hash >= 0);
  vector* bucket = h->buckets + hash;

  int foundIndex = VectorSearch(bucket, elemAddr, h->cmpFn, 0, false);
  return foundIndex != -1 ? VectorNth(bucket, foundIndex) : NULL;
}

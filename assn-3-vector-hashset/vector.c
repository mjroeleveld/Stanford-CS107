#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void VectorNew(vector *v, int elemSize, VectorFreeFunction freeFn, int initialAllocation)
{
  v->allocLength = initialAllocation;
  v->logLength = 0;
  v->elemSize = elemSize;
  v->freeFn = freeFn;
  v->elems = malloc(initialAllocation*elemSize);
  assert(v->elems != NULL);
}

void VectorDispose(vector *v)
{
  if (!v->freeFn)
    return;
  for (int i = 0; i < v->logLength; i++)
    v->freeFn(VectorNth(v, i));
}

int VectorLength(const vector *v)
{
  return v->logLength;
}

void *VectorNth(const vector *v, int position)
{
  assert(position >= 0);
  assert(position < v->logLength);
  char* charPtr = (char*)v->elems + position * v->elemSize;
  return (void*)charPtr;
}

void VectorReplace(vector *v, const void *elemAddr, int position)
{
  assert(position >= 0);
  assert(position < v->logLength);
  void* dest = VectorNth(v, position);
  if (v->freeFn)
    v->freeFn(dest);
  memcpy(dest, elemAddr, v->elemSize);
}

void VectorInsert(vector *v, const void *elemAddr, int position)
{
  assert(position >= 0);
  assert(position <= v->logLength);
  if (position == v->logLength)
    return VectorAppend(v, elemAddr);
  void* src = VectorNth(v, position);
  void* shiftDest = (void*)((char*)src + v->elemSize);
  int shiftCount = v->logLength - position;
  growElems(v);
  memmove(shiftDest, src, shiftCount * v->elemSize);
  memcpy(src, elemAddr, v->elemSize);
  v->logLength++;
}

void VectorAppend(vector *v, const void *elemAddr)
{
  growElems(v);
  v->logLength++;
  void* dest = VectorNth(v, v->logLength - 1);
  memcpy(dest, elemAddr, v->elemSize);
}

void VectorDelete(vector *v, int position)
{
  assert(position >= 0);
  assert(position < v->logLength);
  void* target = VectorNth(v, position);
  if (v->freeFn)
    v->freeFn(target);
  if (position != v->logLength - 1) {
    void* shiftTarget = VectorNth(v, position + 1);
    int shiftCount = v->logLength - (position + 1);
    memmove(target, shiftTarget, shiftCount * v->elemSize);
  }
  v->logLength--;
//  shrinkElems(v);
}

void VectorSort(vector *v, VectorCompareFunction compare)
{
  qsort(v->elems, v->logLength, v->elemSize, compare);
}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData)
{
  for (int i = 0; i < v->logLength; i++)
    mapFn(VectorNth(v, i), auxData);
}

static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)
{
  assert(startIndex >= 0);
  assert(startIndex < v->logLength);
  if (!isSorted) {
    for (int i = startIndex; i < v->logLength; i++) {
      if (searchFn(VectorNth(v, i), key) == 0)
        return i;
    }
  }
  else {
    void* result = bsearch(key, VectorNth(v, startIndex), v->logLength - startIndex, v->elemSize, searchFn);
    if (result == NULL)
      return -1;
    int i = ((char*)result - (char*)v->elems) / v->elemSize;
    return i;
  }
  return kNotFound;
}

static void growElems(vector *v)
{
  if (v->logLength + 1 <= v->allocLength)
    return;
  v->elems = realloc(v->elems, v->elemSize * v->allocLength * 2);
  assert(v->elems != NULL);
  v->allocLength *= 2;
}

static void shrinkElems(vector *v)
{
  if (v->logLength - 1 > v->allocLength / 4)
    return;
  int newSize = v->allocLength / 2;
  v->elems = realloc(v->elems, newSize);
  v->allocLength = newSize;
}

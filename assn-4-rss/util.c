//
// Created by Joachim Roeleveld on 14/05/2020.
//

static const signed long kHashMultiplier = -1664117991L;
int StringHash(const char *s, int numBuckets)
{
  int i;
  unsigned long hashcode = 0;

  for (i = 0; i < strlen(s); i++)
    hashcode = hashcode * kHashMultiplier + tolower(s[i]);

  return hashcode % numBuckets;
}

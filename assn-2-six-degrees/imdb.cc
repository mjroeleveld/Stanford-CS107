#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "imdb.h"
#include "imdb-utils.h"

const char *const imdb::kActorFileName = "actordata";
const char *const imdb::kMovieFileName = "moviedata";

imdb::imdb(const string& directory)
{
  const string actorFileName = directory + "/" + kActorFileName;
  const string movieFileName = directory + "/" + kMovieFileName;
  
  actorFile = acquireFileMap(actorFileName, actorInfo);
  movieFile = acquireFileMap(movieFileName, movieInfo);
}

bool imdb::good() const
{
  return !( (actorInfo.fd == -1) || 
	    (movieInfo.fd == -1) ); 
}

int imdb::findActorCmp(const void* key, const void* cmpOffset)
{
  SearchKeyActor* sKey = (SearchKeyActor*)key;
  string other(sKey->file + *((int*)cmpOffset));
  return sKey->name.compare(other);
}

char* imdb::findActor(const string& actor) const
{
  int* base = (int*)actorFile;
  int* offsets = base + 1;
  struct SearchKeyActor key = { actor, (char*)actorFile };
  int* offset = (int*) bsearch(&key, offsets, *base, sizeof(int), findActorCmp);
  if (offset == nullptr)
    return nullptr;
  return (char*) base + *offset;
}

struct film imdb::getFilm(const void *movieFile, int offset)
{
  char* movie = (char*) movieFile + offset;
  string title(movie);
  char* year = movie + title.length() + 1;
  char* noPlayersChar = year + 1;
  if (*noPlayersChar == '\0')
    noPlayersChar++;
  short noPlayers = *((short*)noPlayersChar);
  const struct film f = { title, (int)*year + 1900, noPlayers };
  return f;
}

int imdb::findMovieCmp(const void* key, const void* cmpOffset)
{
  SearchKeyMovie* sKey = (SearchKeyMovie*)key;
  struct film otherMovie = getFilm(sKey->file, *((int*)cmpOffset));
  if (sKey->movie == otherMovie)
    return 0;
  else
    return sKey->movie < otherMovie ? -1 : 1;
}

char* imdb::findMovie(const film& movie) const
{
  int* base = (int*)movieFile;
  int* offsets = base + 1;
  struct SearchKeyMovie key = { movie, (char*)movieFile };
  int* offset = (int*) bsearch(&key, offsets, *base, sizeof(int), findMovieCmp);
  if (offset == nullptr)
    return nullptr;
  return (char*) base + *offset;
}

bool imdb::getCredits(const string& player, vector<film>& films) const
{
  char* actor = findActor(player);
  if (actor == nullptr)
    return false;
  char* starredChar = actor + player.length() + 1;
  if (*starredChar == '\0')
    starredChar++;
  short* starredNo = (short*)starredChar;
  char* offsets = starredChar + 2;
  if (*offsets == '\0')
    offsets += 2;
  for (short i = 0; i < *starredNo; i++) {
    int* offset = (int*)offsets + i;
    struct film f = getFilm(movieFile, *offset);
    films.push_back(f);
  }
  return true;
}

bool imdb::getCast(const film& movie, vector<string>& players) const
{
  char* found = findMovie(movie);
  if (found == nullptr)
    return false;
  char* noPlayersChar = found + movie.title.length() + 2;
  if (*noPlayersChar == '\0')
    noPlayersChar++;
  short noPlayers = *((short*)noPlayersChar);
  char* offsets = noPlayersChar + 2;
  if (*offsets == '\0')
    offsets += 2;
  for (int i = 0; i < noPlayers; i++) {
    int* offset = (int*)offsets + i;
    char* actor = (char*)actorFile + *offset;
    string name(actor);
    players.push_back(name);
  }
  return true;
}

imdb::~imdb()
{
  releaseFileMap(actorInfo);
  releaseFileMap(movieInfo);
}

// ignore everything below... it's all UNIXy stuff in place to make a file look like
// an array of bytes in RAM.. 
const void *imdb::acquireFileMap(const string& fileName, struct fileInfo& info)
{
  struct stat stats;
  stat(fileName.c_str(), &stats);
  info.fileSize = stats.st_size;
  info.fd = open(fileName.c_str(), O_RDONLY);
  return info.fileMap = mmap(0, info.fileSize, PROT_READ, MAP_SHARED, info.fd, 0);
}

void imdb::releaseFileMap(struct fileInfo& info)
{
  if (info.fileMap != NULL) munmap((char *) info.fileMap, info.fileSize);
  if (info.fd != -1) close(info.fd);
}

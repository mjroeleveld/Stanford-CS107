#include <vector>
#include <list>
#include <set>
#include <string>
#include <iostream>
#include <iomanip>
#include "imdb.h"
#include "path.h"
using namespace std;

/**
 * Using the specified prompt, requests that the user supply
 * the name of an actor or actress.  The code returns
 * once the user has supplied a name for which some record within
 * the referenced imdb existsif (or if the user just hits return,
 * which is a signal that the empty string should just be returned.)
 *
 * @param prompt the text that should be used for the meaningful
 *               part of the user prompt.
 * @param db a reference to the imdb which can be used to confirm
 *           that a user's response is a legitimate one.
 * @return the name of the user-supplied actor or actress, or the
 *         empty string.
 */

static string promptForActor(const string& prompt, const imdb& db)
{
  string response;
  while (true) {
    cout << prompt << " [or <enter> to quit]: ";
    getline(cin, response);
    if (response == "") return "";
    vector<film> credits;
    if (db.getCredits(response, credits)) return response;
    cout << "We couldn't find \"" << response << "\" in the movie database. "
	 << "Please try again." << endl;
  }
}

bool generateShortestPath(imdb& db, const string& player1, const string& player2, path& path, int depth,
                          vector<string> markedPlayers)
{
  if (depth > 7) return 0;
  vector<film> films;
  db.getCredits(player1, films);
  vector<film>::const_iterator fit = films.begin();
  vector<vector<string> > players;
  vector<string>::const_iterator pit;
  // Loop over player's films
  for (; fit != films.end(); fit++) {
    vector<string> fPlayers;
    db.getCast(*fit, fPlayers);
    // Loop over film's players
    for (pit = fPlayers.begin(); pit != fPlayers.end(); pit++) {
      // Skip already checked players
      if (find(markedPlayers.begin(), markedPlayers.end(), *pit) != markedPlayers.end()) {
        pit = fPlayers.erase(pit);
        break;
      }
      markedPlayers.insert(markedPlayers.end(), *pit);
      if (*pit == player2) {
        path.addConnection(*fit, *pit);
        return 1;
      }
    }
    players.insert(players.end(), fPlayers);
  }
  vector<vector<string> >::const_iterator fpit = players.begin();
  // Recursively search through found players' movies
  for (fit = films.begin(); fit != films.end(); fit++) {
    // Loop over film's player
    for (pit = (*fpit).begin(); pit != (*fpit).end(); pit++) {
      bool found = generateShortestPath(db, *pit, player2, path, depth + 1, markedPlayers);
      if (found) {
        path.addConnection(*fit, *pit);
        return 1;
      }
    }
    fpit++;
  }
  return 0;
}

/**
 * DFS algorithm that finds shortest path between two players.
 *
 * @param db
 * @param player1
 * @param player2
 * @param path
 * @param depth
 * @return
 */
bool generateShortestPath(imdb& db, const string& player1, const string& player2, path& path)
{
  vector<string> markedPlayers;
  generateShortestPath(db, player1, player2, path, 1, markedPlayers);
  path.reverse();
}

/**
 * Serves as the main entry point for the six-degrees executable.
 * There are no parameters to speak of.
 *
 * @param argc the number of tokens passed to the command line to
 *             invoke this executable.  It's completely ignored
 *             here, because we don't expect any arguments.
 * @param argv the C strings making up the full command line.
 *             We expect argv[0] to be logically equivalent to
 *             "six-degrees" (or whatever absolute path was used to
 *             invoke the program), but otherwise these are ignored
 *             as well.
 * @return 0 if the program ends normally, and undefined otherwise.
 */

int main(int argc, const char *argv[])
{
  imdb db(determinePathToData(argv[1])); // inlined in imdb-utils.h
  if (!db.good()) {
    cout << "Failed to properly initialize the imdb database." << endl;
    cout << "Please check to make sure the source files exist and that you have permission to read them." << endl;
    exit(1);
  }

  while (true) {
    string source = promptForActor("Actor or actress", db);
    if (source == "") break;
    string target = promptForActor("Another actor or actress", db);
    if (target == "") break;
    if (source == target) {
      cout << "Good one.  This is only interesting if you specify two different people." << endl;
    } else {
      path shortestPath(source);
      generateShortestPath(db, source, target, shortestPath);
      cout << endl << shortestPath;
    }
  }

  cout << "Thanks for playing!" << endl;
  return 0;
}


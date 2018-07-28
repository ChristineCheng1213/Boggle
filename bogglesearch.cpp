/* Name: Xiaoyu Cheng (Christine)
 * Course: CS106B
 * Section: Thurs 3:30pm
 * Description: implements humanWordSearch and computerWordSearch using recursive backtracking.
 * Resources: Lecture slides & Stanford Library Documentations.
 */

#include <string>
#include "grid.h"
#include "lexicon.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "bogglegui.h"
using namespace std;


bool humanWordHelper(const Grid<char>& board, string word, Set<Vector<int> >& visitedLocation, int rowInd, int colInd);
void computerWordHelper(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords, Set<Vector<int> >& visitedLocation, Set<string>& result, string chosen, int rowInd, int colInd);
int pointsToAdd(string word);


/*
 * Finds whether the board contains the word human player entered (case-insensitive).
 * Specifically, it checks whether the input word is valid, then call its helper function.
 * Accepts a referenceto board amd lexicon, and a string.
 * Returns true and add points to human if the word is found, otherwise returns false.
 */
bool humanWordSearch(Grid<char>& board, Lexicon& dictionary, string word) {

    word = toUpperCase(word);
    Set<Vector<int> > visited;          //use vector<int> to represent location on board, and use set to store locations
    BoggleGUI::clearHighlighting();

    if (dictionary.contains(word) && word.length() >= BoggleGUI::MIN_WORD_LENGTH){

        for (int i = 0; i < board.numRows(); i++) {
            for (int j = 0; j < board.numCols(); j++) {

                BoggleGUI::setHighlighted(i, j, true);

                if (humanWordHelper(board, word, visited, i, j)) {
                    BoggleGUI::scorePointsHuman(pointsToAdd(word));
                    return true;
                }

                BoggleGUI::setHighlighted(i, j, false);
            }
        }
    }
    return false;
}


/*
 * Starting from a giving location on board, find if its neighbor contains the rest of the word using recursive backtracking.
 * Accepts a const reference to board, reference to visitedLocation, string of word to check, and location on board.
 * Returns true if the word is found, otherwise returns false.
 */
bool humanWordHelper(const Grid<char>& board, string word, Set<Vector<int> >& visitedLocation, int rowInd, int colInd){

    Vector<int> curLocation = {rowInd, colInd};

    if (word == "") {
        //base case: the word is found
        return true;
    } else if (!board.inBounds(rowInd, colInd) || board[rowInd][colInd] != word[0] || visitedLocation.contains(curLocation)) {
        //base case: out of bound or cannot form word we want or have visited this location before
        return false;
    } else {

        //choose
        BoggleGUI::setHighlighted(rowInd, colInd, true);
        visitedLocation.add(curLocation);

        //explore
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1 ; j++) {
                if (humanWordHelper(board, word.substr(1), visitedLocation, rowInd + i, colInd + j)) {
                    return true;
                }
            }
        }

        //un-choose
        visitedLocation.remove(curLocation);
        BoggleGUI::setHighlighted(rowInd, colInd, false);

        return false;
    }
}


/*
 * Finds all words on board that human has not found.
 * Specifically, it calls its helper function on every location on board, and adds points to computer.
 * Accepts a reference to board and lexicon, and a set of words human has found.
 * Returns a set of string representing words that computer has found.
 */
Set<string> computerWordSearch(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords) {

    Set<string> result;
    Set<Vector<int> > visitedLocation;
    string chosen = "";

    for (int i = 0; i < board.numRows(); i++) {
        for (int j = 0; j < board.numCols(); j++) {
            visitedLocation.clear();
            computerWordHelper(board, dictionary, humanWords, visitedLocation, result, chosen, i, j);
        }
    }

    for (string wordFound : result) {
        BoggleGUI::scorePointsComputer(pointsToAdd(wordFound));
    }

    return result;
}


/*
 * Starting from a giving location on board, find all valid words contained in its neighborhood using recursive backtracking.
 * Accepts a const reference to board, reference to lexicon, set of humanWords, visitedLocation, result, string of word chosen, and location on board.
 */
void computerWordHelper(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords, Set<Vector<int> >& visitedLocation, Set<string>& result, string chosen, int rowInd, int colInd){

    if (!dictionary.containsPrefix(chosen)) {
        //base case: chosen chars cannot form any word
        return;
    } else {

        if (dictionary.contains(chosen) && chosen.length() >= BoggleGUI::MIN_WORD_LENGTH && !humanWords.contains(chosen)) {
            //base case: found a valid word that human has not found, add it to result
            result.add(chosen);
        }

        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {

                Vector<int> curLocation = {rowInd + i, colInd + j};

                if (!visitedLocation.contains(curLocation) && board.inBounds(rowInd + i, colInd + j)) {

                    //choose
                    chosen += board[rowInd + i][colInd + j];
                    visitedLocation.add(curLocation);

                    //explore
                    computerWordHelper(board, dictionary, humanWords, visitedLocation, result, chosen, rowInd + i, colInd + j);

                    //un-choose
                    visitedLocation.remove(curLocation);
                    chosen = chosen.substr(0, chosen.size() - 1);
                }
            }
        }
        return;
    }
}


/*
 * Accepts a word, and returns how many points should we add following the game's rule.
 */
int pointsToAdd(string word) {

    if (word.size() >= 4 && word.size() < 7 ) {
        return word.size() - 3;
    } else if (word.size() == 7) {
        return 5;
    } else {
        return 11;
    }
}


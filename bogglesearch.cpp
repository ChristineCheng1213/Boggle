
#include <string>
#include "grid.h"
#include "lexicon.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "bogglegui.h"
using namespace std;

//highlight部分

bool humanWordHelper(const Grid<char>& board, string word, Set<Vector<int> >& visitedLocation, int rowInd, int colInd);
void computerWordHelper(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords, Set<Vector<int> >& visitedLocation, Set<string>& result, string chosen, int rowInd, int colInd);
int pointsToAdd(string word);

bool humanWordSearch(Grid<char>& board, Lexicon& dictionary, string word) {
    word = toUpperCase(word);
    Set<Vector<int> > visited;

    if (dictionary.contains(word) && word.length() >= BoggleGUI::MIN_WORD_LENGTH){

        for (int i = 0; i < board.numRows(); i++) {
            for (int j = 0; j < board.numCols(); j++) {

                if (humanWordHelper(board, word, visited, i, j)){
                    BoggleGUI::scorePointsHuman(pointsToAdd(word));
                    return true;
                }

            }
        }
    }

    return false;
}




bool humanWordHelper(const Grid<char>& board, string word, Set<Vector<int> >& visitedLocation, int rowInd, int colInd){

    if (word == "") {
        //base case: the word is found
        return true;
    } else if (!board.inBounds(rowInd, colInd) || board[rowInd][colInd] != word[0]) {
        //base case: out of bound or cannot form word we want
        return false;
    } else {
        //recursive case: try to find if neighbor contains next character of word

        for (int i = -1; i <= 1; i++){
            for (int j = -1; j <= 1 ; j++){
                Vector<int> curLocation = {rowInd + i, colInd + j};
                //choose
                visitedLocation.add(curLocation);

                //explore
                if (humanWordHelper(board, word.substr(1), visitedLocation, rowInd + i, colInd + j)){
                    return true;
                }

                //un-choose
                visitedLocation.remove(curLocation);

            }
        }

        return false;
    }
}




Set<string> computerWordSearch(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords) {

    Set<string> result;
    Set<Vector<int> > visitedLocation;
    string chosen = "";

    for (int i = 0; i < board.numRows(); i++){
        for (int j = 0; j < board.numCols(); j++){
            visitedLocation.clear();                    //if set visitedLocation = {{i, j}}; - PLONK will not be found - Why?
            computerWordHelper(board, dictionary, humanWords, visitedLocation, result, chosen, i, j);
        }
    }

    for (string wordFound : result) {
        BoggleGUI::scorePointsComputer(pointsToAdd(wordFound));
    }

    return result;
}


void computerWordHelper(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords, Set<Vector<int> >& visitedLocation, Set<string>& result, string chosen, int rowInd, int colInd){

    if (!dictionary.containsPrefix(chosen)) {
        //base case: chosen chars cannot form any word
        return;
    } else {

        if (dictionary.contains(chosen) && chosen.length() >= BoggleGUI::MIN_WORD_LENGTH && !humanWords.contains(chosen)) {
            result.add(chosen);            
        }

        for (int i = -1; i <= 1; i++){
            for (int j = -1; j <= 1; j++){
                Vector<int> curLocation = {rowInd + i, colInd + j};

                if (!visitedLocation.contains(curLocation) && board.inBounds(rowInd + i, colInd + j)){

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
    }

}


int pointsToAdd(string word) {
    if (word.size() >= 4 && word.size() < 7 ) {
        return word.size() - 3;
    } else if (word.size() == 7){
        return 5;
    } else {
        return 11;
    }
}


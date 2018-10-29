#ifndef _HINT_GENERATOR_H_
#define _HINT_GENERATOR_H_

#include "block.h"
#include "cell.h"
#include "difficulty.h"
#include <vector>
#include <memory>


// forward declaration avoiding cyclic dependency
class Grid;


class HintGenerator{
  // block to try for generating the hint
  Block block;
  // the smart pointer to give the dificulty of the level for using the difficulty right now
  std::shared_ptr <Difficulty> theLevel;
  // The board to observe
  std::vector <std::vector <Cell>> * board;
    
  static const int MAX_DEPTH = 1024;
  // The array used for DP memoisation. Basically a huge vector of cells used for
  // memoisation. Bad, but better alogrithm is hard, so no.
  std::vector<Cell> memoisations;
    
  // Helper memoisation functions
  Cell getMemoisation(int row, int col, int rotation);
  Cell setMemoisation(int row, int col, int rotation, Cell c);
  bool isMemoised(int row, int col, int rotation);

  double score(Cell c);          // Generates the score of a given cell to palce the block
    
  Cell generateHint(Block b, int depth = 0); // Generates hint for block at position given by Block b
 public:
  // ctor
  HintGenerator(std::shared_ptr <Difficulty> levels,
                std::vector <std::vector <Cell>> * board,
                Block currentBlock);    
    
  // Returns cell, containing the best place to place the given Block b
  // Best place is defined as the row which is nearest the bottom that the Block b can get to
  Block generateHint();        // Generates hint based on Block block (the class variable)
};

#endif


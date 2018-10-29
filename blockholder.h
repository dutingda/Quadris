#ifndef _BLOCK_HOLDER_H_
#define _BLOCK_HOLDER_H_

#include <vector>
#include <memory>
#include "type.h"
#include "block.h"
#include "difficulty.h"

// The blockholder class is generally for dealing with generating new blocks and manage the current block
// that has been genertated

class BlockHolder {
  
  // Current block to generate, next block to generate
  Block currentBlock, nextBlock;
  // count for block that has been generated
  int blockCounter = 0;
  // count for the block that has not been cleared
  int unclearedCounter = 0;
  // a pointer of Level to record to difficulty that it is currently in
  Level *difficultyLevel = nullptr;
    
  // std::vector<std::vector<Cell*>> board;
  std::vector<std::shared_ptr<Difficulty>> levels;
    
 public:
  // initialize the board, difficulty and the txt file to be generated from
  void init(std::vector <std::vector<Cell>> * board,
            Level *difficultyLevel,
            std::string loadPath = "sequence.txt");
    
  Block &getCurrentBlock();      // return the current block
  Block &getNextBlock();         // return the type of next block
  void mutate(Move cmd, const int num = 1);   // mutate the current block
  void changeShape(Shape s);                  // change shape of current block
  void switchToNextBlock();     // sets the currentblock to the next block
  std::shared_ptr<Difficulty> getDifficulty(); // Returns the difficulty decorator that generates new blocks
  // Sets the file path to make random levels not random
  void setLoadPath(std::string path);
    
  // Penalty for not clearing boards quick enough
  bool implementPenalty(bool rowCleared);
};

#endif


#include <vector>
#include <iostream>
#include "blockholder.h"

#include "levelzero.h"
#include "levelone.h"
#include "leveltwo.h"
#include "levelthree.h"
#include "levelfour.h"
using namespace std;

void BlockHolder::init(std::vector <std::vector<Cell>> * board,
                       Level *difficultyLevel,
                       string loadPath)
{
  /*
    for (int i=0; i<HEIGHT; i++){
    vector <Cell*> temp;
    for (int j=0; j<WIDTH; j++){
    temp.push_back(&(bd[i][j]));
    }
    board.emplace_back(temp);
    }
  */
  this->difficultyLevel = difficultyLevel;
  levels.clear();
  levels.emplace_back( make_shared<LevelZero>(board, loadPath) );
  levels.emplace_back( make_shared<LevelOne>(board, loadPath)  );
  levels.emplace_back( make_shared<LevelTwo>(board, loadPath)  );
  levels.emplace_back( make_shared<LevelThree>(board, loadPath));
  levels.emplace_back( make_shared<LevelFour>(board, loadPath) );
    
  levels[Level::lvl0]->setLoadPath(loadPath);
    
  currentBlock = getDifficulty()->newBlock();
  currentBlock.setId(blockCounter++);
  nextBlock = getDifficulty()->newBlock();
  nextBlock.setId(blockCounter++);
}

// return the next block
Block &BlockHolder::getNextBlock() { return nextBlock; }

// return the current block
Block &BlockHolder::getCurrentBlock() { return currentBlock; }

shared_ptr<Difficulty> BlockHolder::getDifficulty(){
  int theLevel = 0;
  switch (*difficultyLevel) {
    case Level::lvl0 : theLevel = 0; break;
    case Level::lvl1 : theLevel = 1; break;
    case Level::lvl2 : theLevel = 2; break;
    case Level::lvl3 : theLevel = 3; break;
    case Level::lvl4 : theLevel = 4; break;
  }
    
  return levels[theLevel];
}


// mutate the current block
void BlockHolder::mutate(Move cmd, const int num){
    
  // call the move in levels[thelevel] num times
  for(int i=0; i<num; ++i){
    switch(cmd){
      case Move::Left :
        currentBlock = getDifficulty()->moveLeft(currentBlock);
        break;
      case Move::Right :
        currentBlock = getDifficulty()->moveRight(currentBlock);
        break;
      case Move::Down :
        currentBlock =  getDifficulty()->moveDown(currentBlock);
        break;
      case Move::Drop :
        currentBlock = getDifficulty()->drop(currentBlock);
        break;
      case Move::CounterRotate :
        currentBlock = getDifficulty()->rotateCounter(currentBlock);
        break;
      case Move::Rotate :
        currentBlock = getDifficulty()->rotateClock(currentBlock);
        break;
    }
  }
}

// change shape of current block
void BlockHolder::changeShape(Shape s){
  Block backUp = currentBlock;
  currentBlock.setShape(s);
  if(!getDifficulty()->validPosition(currentBlock)) currentBlock = backUp;
}


void BlockHolder::switchToNextBlock(){
  currentBlock = nextBlock;
  nextBlock = getDifficulty()->newBlock();
  nextBlock.setId(blockCounter++);
}

void BlockHolder::setLoadPath(std::string path){
  for(int i=0; i<4; ++i) levels[i]->setLoadPath(path);
}

bool BlockHolder::implementPenalty(bool rowCleared){
  if (rowCleared) unclearedCounter = 0;
  else unclearedCounter++;
    
  if (unclearedCounter && unclearedCounter % 5 == 0) {
    unclearedCounter = 0;
    getDifficulty()->implementPenalty();
    return true;
  }
  return false;
}


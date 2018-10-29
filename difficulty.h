#ifndef _DIFFICULTY_H_
#define _DIFFICULTY_H_

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "type.h"
#include "block.h"
#include "cell.h"

class Difficulty {
 protected:
  //std::weak_ptr<Grid> theGrid;
  std::vector <std::vector <Cell>> * board;
    
  int counter = 0;
  // Sequence of shapes used to generate blocks
  std::vector <Shape> generationSequence;
  std::string loadPath;
    
  // Moves the given block down depending on the weight
  virtual Block moveByWeight(Block b);
    
  // Clockwise rotate. No downwards pull by weight
  virtual Block cwRotate(Block b);
    
 public:
  // default ctor
  Difficulty();
  Difficulty(std::vector <std::vector <Cell>> * bd,
             std::string loadPath = "sequence.txt");
  // virtual function for different level, makes the new block to be pure virtual and make the class abstract
  virtual Block newBlock() = 0;
  virtual Block rotateClock(Block b);
  virtual Block rotateCounter(Block b);
  virtual Block moveDown(Block b);
  virtual Block moveLeft(Block b);
  virtual Block moveRight(Block b);
  virtual Block drop(Block b);
    
  // Checks if given block position is valid
  virtual bool validPosition(Block b);
    
  // Loads from the file used to randomly generate blocks
  virtual void setLoadPath(std::string path) = 0;
    
  // Penalty for not clearing rows quick enough
  virtual void implementPenalty();
};

#endif /* difficulty_h */


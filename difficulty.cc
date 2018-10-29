#include "difficulty.h"
#include "grid.h"
#include <vector>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
using namespace std;

// ctor
Difficulty::Difficulty(vector <vector <Cell>> * bd, std::string loadPath):
  board{bd}, loadPath{loadPath}{}

// dtor
Difficulty::Difficulty(){}

bool Difficulty::validPosition(Block b){
  std::vector<Cell> area = b.getArea();
    
  for(int i=0; i<4; ++i){
    int row = area[i].row;
    int col = area[i].col;
        
    if (row < 0 || row >= HEIGHT || col < 0 || col >= WIDTH ||
        ((*board)[row][col].content != Content::Empty &&
         !(*board)[row][col].temp))
      return false;        
  } return true;
}

// MAGIC: DO NOT TOUCH!!!
Block Difficulty::cwRotate(Block b){
  int width = b.getWidth();
  Block rotated = b;
    
  std::vector<Cell> oldArea = b.getArea();
  std::vector<Cell> newArea;
  for(int i=0; i<4; ++i)
    // Following 3 lines is the key that makes the rotation work!
    //   First we find the smartest rectangle that contains the block;
    //   then rotate the rectangle without cells that have not be covered
    newArea.emplace_back(Cell{oldArea[i].content,
          (b.getRow() - width + 1) + (oldArea[i].col - b.getCol()),
          b.getCol() + (b.getRow() - oldArea[i].row)});
    
  rotated.setArea(newArea);
  rotated.setRotation((b.getRotation()+1) % 4);
    
  if(validPosition(rotated)) return rotated;
  return b;
}
Block Difficulty::moveByWeight(Block b){
  Block moved = b;
  moved.setRow(b.getRow()+b.getWeight());
    
  if(validPosition(moved)) return moved;
  return b;
}

Block Difficulty::rotateClock(Block b){
  return moveByWeight(cwRotate(b));
}

Block Difficulty::rotateCounter(Block b){
  return moveByWeight(cwRotate(cwRotate(cwRotate(b))));
}

Block Difficulty::moveDown(Block b){
  Block moved = b;
  moved.setRow(b.getRow()+1);
    
  if(validPosition(moved)) return moveByWeight(moved);
  return moveByWeight(b);
}

Block Difficulty::moveLeft(Block b){
  Block moved = b;
  moved.setCol(b.getCol()-1);
    
  if(validPosition(moved)) return moveByWeight(moved);
  return moveByWeight(b);
}

Block Difficulty::moveRight(Block b){
  Block moved = b;
  moved.setCol(b.getCol()+1);
    
  if(validPosition(moved)) return moveByWeight(moved);
  return moveByWeight(b);
}

Block Difficulty::drop(Block b){
  Block moved = moveDown(b);
  for(int prevRow = b.getRow(), currRow = moved.getRow();
      prevRow != currRow;){
    prevRow = currRow;
    moved = moveDown(moved);
    currRow = moved.getRow();
  }
  return moveByWeight(moved);
}

void Difficulty::implementPenalty(){}

void update(std::vector <std::vector <Cell*>> & bd);




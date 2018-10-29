#include "hintgenerator.h"
#include "grid.h"
#include <iostream>
using namespace std;
HintGenerator::HintGenerator(shared_ptr <Difficulty> levels,
                             vector <vector <Cell>> * board,
                             Block currentBlock):
  block{currentBlock}, theLevel{levels}, board{board}
{ memoisations.resize(WIDTH * HEIGHT * 4 + 1); }

Cell HintGenerator::getMemoisation(int row, int col, int rotation){
  return memoisations[(WIDTH * row + col)*4 + rotation];
}

Cell HintGenerator::setMemoisation(int row, int col, int rotation, Cell c){
  return memoisations[(WIDTH * row + col)*4 + rotation] = c;
}

bool HintGenerator::isMemoised(int row, int col, int rotation){
  return memoisations[(WIDTH * row + col)*4 + rotation].content
    != Content::Empty;
}

// Returns the score of how good a hint a cell is
double HintGenerator::score(Cell c){
  Block b = block;
  while(b.getRotation() != c.rotation)
    b = theLevel->rotateClock(b);

  double s = c.row - b.getHeight();
  
  if((b.getShape() == Shape::Jblock || b.getShape() == Shape::Lblock) &&
     c.rotation != 0) s -= 0.5;
  
  return s;
}

// Generates the hint
Block HintGenerator::generateHint(){
  Cell c = generateHint(block); // Get where the hint is  
  Block b = Block(block.getShape());
  
  while(b.getRotation() != c.rotation) // Set hint block rotation    
  b = theLevel->rotateClock(b);
  
  b.setRow(c.row); b.setCol(c.col); // Set hint block location
  
  return b;
}

// Returns cell, containing the best place to place the given Block b
// Best place is defined as the row which is nearest
// the bottom that the Block b can get to
Cell HintGenerator::generateHint(Block b, int depth){
  int b_Row = b.getRow();
  int b_Col = b.getCol();
  int b_Rotation = b.getRotation();
  std::vector<Cell> area = b.getArea();
  Cell temp = Cell{Content::Hint, b_Row, b_Col, b_Rotation};
    
  // If current spot with current rotation visited...
  if(isMemoised(b_Row, b_Col, b_Rotation) ){
    return getMemoisation(b_Row, b_Col, b_Rotation);
  }
    
  // If cannot move
  if(b_Row >= HEIGHT-1){
    return setMemoisation(b_Row, b_Col, b_Rotation, temp);
  }
    
  for(int i=0; i<4; ++i){
    int row = area[i].row;
    int col = area[i].col;
    if((*board)[row+1][col].content != Content::Empty &&
       !(*board)[row+1][col].temp){
      return setMemoisation(b_Row, b_Col, b_Rotation, temp);
    }
  }
    
  // Mark current as visited
  Cell best = setMemoisation(b_Row, b_Col, b_Rotation, temp);
    
  // If can move, try each move
  Cell potential = generateHint(theLevel->moveDown(b), depth+1);
  if (score(potential) > score(best)) best = potential;
    
  potential = generateHint(theLevel->moveLeft(b), depth+1);
  if (score(potential) > score(best)) best = potential;
    
  potential = generateHint(theLevel->moveRight(b), depth+1);
  if (score(potential) > score(best)) best = potential;
    
  potential = generateHint(theLevel->rotateClock(b), depth+1);
  if (score(potential) > score(best)) best = potential;
    
  potential = generateHint(theLevel->rotateCounter(b), depth+1);
  if (score(potential) > score(best)) best = potential;
    
  // Memoise and continue
  return setMemoisation(b_Row, b_Col, b_Rotation, best);
}





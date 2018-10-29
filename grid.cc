#include "grid.h"
#include <memory>
#include <iostream>
#include <map>
using namespace std;

Grid::Grid(Level d):
  difficultyLevel{d}{}

void Grid::init(string defaultPath, Level level){
  difficultyLevel = level;
  defaultLoadPath = defaultPath;
  board.clear();
  for(int row = 0; row < HEIGHT; ++row){
    vector <Cell> temp; 
    for(int col = 0; col < WIDTH; ++col)
      temp.emplace_back(Cell{Content::Empty, row, col});
	board.emplace_back(temp);
  }
  td.setGrid(&board);
  theHolder.init(&board, &difficultyLevel, defaultLoadPath);
  this->updateBoard();
}

void Grid::setupGraphic()
{ 
  this->gd = make_unique<GraphicsDisplay>(11, 18);
  gd->setGrid(&board);
}

Block & Grid::getNextBlock()
{ return theHolder.getNextBlock(); }

Block & Grid::getCurrentBlock()
{ return theHolder.getCurrentBlock(); }

shared_ptr<Difficulty> Grid::getDifficulty()
{ return theHolder.getDifficulty(); }

// tries to eliminate any rows
void Grid::eliminateRow(){
  // Stores which grids are currently on the board
  map<int, int> diffArr;
  for(int row = 0; row < HEIGHT; ++row)
    for(int col = 0; col < WIDTH; ++col)
      if(board[row][col].numBlock != -1)
        // Ends in 1 = in board
        diffArr[board[row][col].numBlock] = board[row][col].level*2+1;
    
  // Proceeds onto deleting
  for(int row = 0; row < HEIGHT; ++row){       // For each row... (go from top of grid (0) to down)
    int occupied = 0;                               // Count occupied cells
    for(int col = 0; col <WIDTH; ++col)
      if(board[row][col].content != Content::Empty) ++occupied;
        
    if(occupied < WIDTH) continue;            // If row is full...
        
    board.erase(board.begin() + row);              // Delete full row
    rowCleared = true;
    vector<Cell> newRow;                      // Make new empty row
    newRow.resize(WIDTH);
    board.insert(board.begin(), newRow);           // Insert new row
        
    score.addScore(difficultyLevel, 1);              // Awards score for eliminating row
  }
    
  // Sync up the rows and columns
  for(int row = 0; row < HEIGHT; ++row)
    for(int col = 0; col < WIDTH; ++col){
      board[row][col].row = row;
      board[row][col].col = col;
    }
    
  for(int row = 0; row < HEIGHT; ++row)
    for(int col = 0; col < WIDTH; ++col)
      if(board[row][col].numBlock != -1)
        // Ends in 1 = eliminated, 0 = still in board
        diffArr[board[row][col].numBlock] = board[row][col].level*2;
    
  // Tabulates score for eliminating blocks
  for(auto it : diffArr)
    if(((int) it.second) % 2 == 1)
      score.addScore((Level) (((int) it.second)/2));
}

// update() the board (may eliminate lines by calling eliminateRow())
void Grid::update(){
  //rowCleared = false;
    
  // Checks if need to request next block
  Block currentBlock = theHolder.getCurrentBlock();
  bool requestNext = false;
  if(currentBlock.getRow() == HEIGHT-1) requestNext = true;   // Request next block when current block at end of board
  else{                                                            // Or when resting on other blocks
    vector<Cell> area = currentBlock.getArea();
        
    for(int i=0; i<4; ++i){
      int row = area[i].row;
      int col = area[i].col;
      if(board[row+1][col].content != Content::Empty){
        requestNext = true;
        break;
      }
    }
  }
    
  // this part will only be call when the new block should be generated
  if(requestNext){
       
    // Place block into board
    vector<Cell> area = currentBlock.getArea();
    for(int i=0; i<4; ++i){
      board[area[i].row][area[i].col].content = area[i].content;
      board[area[i].row][area[i].col].numBlock = currentBlock.getId();
      board[area[i].row][area[i].col].level = currentBlock.getLevel();
    }
        
    eliminateRow();                           // Eliminate rows if possible
    // check if there is any penalty
    if (theHolder.implementPenalty(rowCleared)){
      eliminateRow();
    }
    theHolder.switchToNextBlock();            // Request next block
  }
}

// this function will only mutate the block temperatly
//    the function will be mutate back, when after the board has been display
//    (in TextDisplay::operator << )
void Grid::updateBoard(){
    
  Block currentBlock = this->getCurrentBlock();
  vector<Cell> currentArea = currentBlock.getArea();
    
  for (int i=0; i<4; i++){
    int row = currentArea[i].row;
    int col = currentArea[i].col;
    board[row][col].content = currentArea[i].content;
    board[row][col].temp = true;
  }
    
  if (showHint){
    Block hintBlock = this->getHint();
    vector<Cell> hintArea = hintBlock.getArea();
        
    for (int i=0; i<4; i++){
      int row = hintArea[i].row;
      int col = hintArea[i].col;
      board[row][col].content = Content::Hint;
      board[row][col].temp = true;
    }
    showHint = false;
  }
}

// clear() clear the board
void Grid::clear(){

  for (int row=0; row<HEIGHT; row++){
    for (int col=0; col<WIDTH; col++){
      board[row][col].content = Content::Empty;
      board[row][col].rotation = -1;
      board[row][col].level = Level::lvl0;
      board[row][col].numBlock = -1;
      board[row][col].temp = false;
    }
  }
  score.clear();
  theHolder = BlockHolder();
  theHolder.init(&board, &difficultyLevel, defaultLoadPath);
  td.setGrid(&board);
}


// hint() give the best solution of current block
void Grid::hint() { showHint = true; updateBoard(); }
Block Grid::getHint(){
  return HintGenerator{theHolder.getDifficulty(), &board,
      theHolder.getCurrentBlock()}.generateHint();
}


// gameOver() determine if the game is over
bool Grid::gameOver(){
  vector<Cell> area = theHolder.getCurrentBlock().getArea();

  if(theHolder.getCurrentBlock().getRow() == 3){
    for(size_t i=0; i<area.size(); ++i){
      int row = area[i].row + 1, col = area[i].col;
      if (!board[row][col].temp &&
          board[row][col].content != Content::Empty)
        return true;
    }
  }
    
  return false;
}

// changeLevel() change the difficult lvl
void Grid::changeLevel(int num)
{
  switch (num) {
    case 0: difficultyLevel = Level::lvl0; break;
    case 1: difficultyLevel = Level::lvl1; break;
    case 2: difficultyLevel = Level::lvl2; break;
    case 3: difficultyLevel = Level::lvl3; break;
    case 4: difficultyLevel = Level::lvl4; break;
  }
}

// mutate() change the current block
void Grid::mutate(Move cmd, const int num){
  theHolder.mutate(cmd, num);
  this->update();
}

void Grid::changeShape(Shape s){
  theHolder.changeShape(s);
  update();
}

ostream &operator<<(ostream &out, Grid &g){
  g.updateBoard();
  out << "Level:    ";
  int lvl_num = 0;
  switch (g.difficultyLevel) {
    case Level::lvl0 : lvl_num = 0; break;
    case Level::lvl1 : lvl_num = 1; break;
    case Level::lvl2 : lvl_num = 2; break;
    case Level::lvl3 : lvl_num = 3; break;
    case Level::lvl4 : lvl_num = 4; break;
  }
    
  if (g.gd != nullptr){
    (g.gd)->showInfo(lvl_num, (g.score).getScore(), (g.score).getHighScore());
    out << *(g.gd);
    (g.gd)->showNextBlock(g.getNextBlock());
    if(g.gameOver()) (g.gd)->showGameOver();
  }
  out << lvl_num << endl;
  out << g.score;
  out << g.td << "Next:" << endl;
  Shape nextBlock = g.getNextBlock().getShape();
    
  switch (nextBlock) {
    case Shape::Iblock: out << "   " << endl << "IIII"<< endl; break;
    case Shape::Jblock: out << "J  " << endl << "JJJ" << endl; break;
    case Shape::Lblock: out << "  L" << endl << "LLL" << endl; break;
    case Shape::Oblock: out << "OO " << endl << "OO " << endl; break;
    case Shape::Sblock: out << " SS" << endl << "SS " << endl; break;
    case Shape::Zblock: out << "ZZ " << endl << " ZZ" << endl; break;
    case Shape::Tblock: out << "TTT" << endl << " T " << endl; break;
  }
  return out << endl;
}

string Grid::getDefaultLoadPath(){ return defaultLoadPath; }
void Grid::setLoadPath(string path){ theHolder.setLoadPath(path); }

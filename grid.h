#ifndef _GRID_H_
#define _GRID_H_

#include <vector>
#include <iostream>
#include <memory>
#include "type.h"
#include "cell.h"
#include "blockholder.h"
#include "hintgenerator.h"
#include "textdisplay.h"
#include "graphicsdisplay.h"
#include "score.h"

class Grid : public std::enable_shared_from_this<Grid> {
    
  // the game board
  std::vector<std::vector <Cell>> board;

  // other classes inside of grid, forward declaration
  BlockHolder theHolder;
  TextDisplay td;
  ScoreCounter score;
  std::unique_ptr <GraphicsDisplay> gd = nullptr;
    
  // fields that help to run the program
  bool showHint = false;                  // determine if the hint should be show
  bool rowCleared = false;                // determine if the row has be cleared
  Level difficultyLevel = Level::lvl0;    // determine current difficult level
  std::string defaultLoadPath = "sequence.txt";   // the file of blocks for lvl0
    
  // private methods
  void eliminateRow();
  void updateBoard();
 public:
  Grid(Level d = Level::lvl0);
  void init(std::string defaultPath = "sequence.txt", Level level = Level::lvl0);
    
   
    
  Block & getNextBlock();
  Block & getCurrentBlock();
  void setupGraphic();
  void update();      // update() the board (may eliminate lines)
  //void updateBoard();
  void clear();       // clear() clear the board
  void hint();        // hint() give the best solution of current block
  Block getHint();
  bool gameOver();    // gameOver() determine if the game is over
  void changeLevel(int num);
  void mutate(Move cmd, const int num = 1);
  void changeShape(Shape s);
  std::shared_ptr<Difficulty> getDifficulty();
  void setLoadPath(std::string path);
  std::string getDefaultLoadPath();
    
  friend std::ostream &operator<<(std::ostream &out, Grid &g);
};

#endif /* grid_hpp */


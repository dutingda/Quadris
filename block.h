#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "cell.h"
#include "type.h"
#include <vector>
#include <iostream>

class Block{
  std::vector<Cell> area;
  int heaviness; // The heaviness of the cell
  int row, col; // row and column of the left down coordinate of smallest enclosed rectangle
  int rotation; // Rotation state. Used for recursive backtracking in hinting
  int blockId;  // Id of the current block
  Level levelGenerated; // The level that the blocks currently generate at
  Shape shape;
    
 public:
    
  // ctor
  Block(Shape s = Shape::Iblock,
        int heaviness = 0,
        int r = 3, int c = 0,
        Level lvl = Level::lvl0);
    
    
  // follow methods get the information of the object Block
  // get the block ID which is useful for calculating the score by differentiating the ID for block
  int getId() const;
  // getter for row
  int getRow() const;
  // getter for column
  int getCol() const;
  // getter for heaviness
  int getWeight() const;
  // getter for the width of board
  int getWidth() const;
  // getter for the height of the board
  int getHeight() const;
  // getter for the rotation state
  int getRotation() const;
  // getter for level
  Level getLevel() const;
  // getter for the shape of the block
  Shape getShape() const;
  std::vector<Cell> getArea() const;
    
    
  // following methods mutate the fields in block
  // setter for row
  void setRow(int r);
  // setter for column
  void setCol(int c);
  // setter for the block ID
  void setId(int id);
  // set the shape of the block
  void setShape(Shape s);
  // set the rotation state for the block
  void setRotation(int r);
  // set the new set of cells that the block contains
  void setArea(const std::vector<Cell> &newArea);
    
    
  // function overload
  friend std::ostream &operator<<(std::ostream &out, Block &b);
};

#endif


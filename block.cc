#include "block.h"

//// ctor

Block::Block(Shape s, int heaviness, int r, int c, Level lvl):
  heaviness{heaviness},
  row{r}, col{c},
  rotation{0},
  levelGenerated{lvl},
  shape{s}
{
  switch(s){
    case Shape::Iblock :
      area.emplace_back(Cell{Content::I, r, c});
      area.emplace_back(Cell{Content::I, r, c+1});
      area.emplace_back(Cell{Content::I, r, c+2});
      area.emplace_back(Cell{Content::I, r, c+3});
      break;
    case Shape::Jblock :
      area.emplace_back(Cell{Content::J, r-1, c});
      area.emplace_back(Cell{Content::J, r, c});
      area.emplace_back(Cell{Content::J, r, c+1});
      area.emplace_back(Cell{Content::J, r, c+2});
      break;
    case Shape::Oblock :
      area.emplace_back(Cell{Content::O, r, c});
      area.emplace_back(Cell{Content::O, r, c+1});
      area.emplace_back(Cell{Content::O, r-1, c+1});
      area.emplace_back(Cell{Content::O, r-1, c});
      break;
    case Shape::Lblock :
      area.emplace_back(Cell{Content::L, r-1, c+2});
      area.emplace_back(Cell{Content::L, r, c+2});
      area.emplace_back(Cell{Content::L, r, c+1});
      area.emplace_back(Cell{Content::L, r, c});
      break;
    case Shape::Sblock :
      area.emplace_back(Cell{Content::S, r, c});
      area.emplace_back(Cell{Content::S, r, c+1});
      area.emplace_back(Cell{Content::S, r-1, c+1});
      area.emplace_back(Cell{Content::S, r-1, c+2});
      break;
    case Shape::Zblock :
      area.emplace_back(Cell{Content::Z, r-1, c});
      area.emplace_back(Cell{Content::Z, r-1, c+1});
      area.emplace_back(Cell{Content::Z, r, c+1});
      area.emplace_back(Cell{Content::Z, r, c+2});
      break;
    case Shape::Tblock :
      area.emplace_back(Cell{Content::T, r-1, c});
      area.emplace_back(Cell{Content::T, r-1, c+1});
      area.emplace_back(Cell{Content::T, r-1, c+2});
      area.emplace_back(Cell{Content::T, r, c+1});
      break;
  }
}

///////////////////////// get functions ////////////////////////
////////////////////////////////////////////////////////////////

int Block::getRow() const { return row; }
int Block::getCol() const { return col; }


int Block::getWidth() const{
  int width = 0;
  for(size_t i=0; i<area.size(); ++i) width = std::max(width, area[i].col - col);
  return width+1;
}

int Block::getHeight() const{
  int height = 0;
  for(size_t i=0; i<area.size(); ++i) height = std::max(height, row - area[i].row);
  return height+1;
}

int Block::getId() const { return blockId; }

int Block::getWeight() const { return heaviness; }

Shape Block::getShape() const { return shape; }

std::vector<Cell> Block::getArea() const { return area; }

Level Block::getLevel() const{ return levelGenerated; }


///////////////////////// set functions ////////////////////////
////////////////////////////////////////////////////////////////


void Block::setRow(int r) {
  int offset = r - row;
    
  // Updates the area
  for(size_t i = 0; i < area.size(); ++i)
    area[i].row += offset;
    
  // Updates the row
  row += offset;
}

void Block::setCol(int c) {
  int offset = c - col;
    
  // Updates the area
  for(size_t i = 0; i < area.size(); ++i)
    area[i].col += offset;
    
  // Updates the column
  col += offset;
}

void Block::setShape(Shape s) {
  Block other = Block(s, heaviness, row, col);
  setArea(other.getArea());
  shape = s;
}

void Block::setArea(const std::vector<Cell> &newArea){
  // Updates the area
  area = newArea;
    
  // Updates the row and column
  row = area[0].row; col = area[0].col;
  for(size_t i = 0; i < area.size(); ++i){
    row = std::max(row, area[i].row);
    col = std::min(col, area[i].col);
  }
}

// Used for recursive backtracking in hinting
void Block::setRotation(int r){ rotation = r % 4; }
int Block::getRotation() const{ return rotation; }

void Block::setId(int id){ blockId = id; }


/////////////////////// operator overload //////////////////////
////////////////////////////////////////////////////////////////

std::ostream &operator<<(std::ostream &out, Block &b){
  std::vector<std::vector <char>> disp;
  disp.resize(b.getHeight());
  int width = b.getWidth();
    
  // Fills display array with space
  for(size_t row = 0; row < disp.size(); ++row)
    for(int col = 0; col < width; ++col)
      disp[row].emplace_back(' ');
    
  // Fills occupeid spaces with the correct character
  for(size_t i=0; i<b.area.size(); ++i)
    disp[b.row - b.area[i].row][b.area[i].col - b.col] = b.area[i].content;
    
  // Prints the block
  for(long row = disp.size()-1; row >= 0; --row){
    for(size_t col = 0; col < disp[row].size(); ++col)
      out << (char) disp[row][col];
    out << std::endl;
  }
    
  return out;
}



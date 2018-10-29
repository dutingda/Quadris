#include <iostream>
#include "grid.h"
#include "type.h"
#include "textdisplay.h"
using namespace std;

void TextDisplay::setGrid(vector<vector <Cell>> * board){
  this->board = board;
}

std::ostream &operator<<(std::ostream &out, TextDisplay &td){
    
  out << "  ";
  for (int col=0; col<WIDTH; col++) out << col % 10;
  out << endl;
  out << " +-----------" << endl;
    
  for (int row=0; row<HEIGHT; row++){
    out << row % 10 << "|";
    for (int col=0; col<WIDTH; col++){
      switch ( (*(td.board))[row][col].content ) {
        case Content::Empty : out << " "; break;
        case Content::Extra : out << "*"; break;
        case Content::Hint : out << "?"; break;
        case Content::I : out << "I"; break;
        case Content::J : out << "J"; break;
        case Content::L : out << "L"; break;
        case Content::T : out << "T"; break;
        case Content::O : out << "O"; break;
        case Content::S : out << "S"; break;
        case Content::Z : out << "Z"; break;
      }
    }
    out << endl;
  }
  out << " +-----------" << endl;
  for (int row=0; row<HEIGHT; row++){
    for (int col=0; col<WIDTH; col++){
            
      // if the cell is temporarily occuped by content (hint or currentBlock)
      if ( (*td.board)[row][col].temp ){
        (*td.board)[row][col].temp = false;
        (*td.board)[row][col].content = Content::Empty;
      }
    }
  }
  return out;
}


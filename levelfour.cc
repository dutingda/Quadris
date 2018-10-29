#include "levelfour.h"
#include "grid.h"
using namespace std;

LevelFour::LevelFour(vector <vector <Cell>> * bd, string path):
  Difficulty{bd, path}, board{bd}
{
  // this sequence provide the different probablity of block generator
  generationSequence.emplace_back(Shape::Iblock);
  generationSequence.emplace_back(Shape::Jblock);
  generationSequence.emplace_back(Shape::Oblock);
  generationSequence.emplace_back(Shape::Lblock);
  for(int i=0; i<2; ++i) generationSequence.emplace_back(Shape::Sblock);
  for(int i=0; i<2; ++i) generationSequence.emplace_back(Shape::Zblock);
  generationSequence.emplace_back(Shape::Tblock);
}

Block LevelFour::newBlock(){
  if(loadPath != ""){
    if(generationSequence.size() != 0){
      counter %= generationSequence.size();
      return Block(generationSequence[counter++], 1, 3, 0, Level::lvl0);
    } else return Block();
  } return Block(generationSequence[rand() % generationSequence.size()], 1, 3, 0, Level::lvl4);
}

// implementPenalty() place the barrier on the board
void LevelFour::implementPenalty(){
    
  for(int row = HEIGHT-1; row >= 0; --row){
    if((*board)[row][WIDTH/2].content == Content::Empty){
            
      // mutate the block
      (*board)[row][WIDTH/2].content = Content::Extra;
      (*board)[row][WIDTH/2].level = Level::lvl4;
      (*board)[row][WIDTH/2].numBlock = -1;
      (*board)[row][WIDTH/2].rotation = -1;
      break;
    }
  }
}

void LevelFour::setLoadPath(std::string path){
  loadPath = path; counter = 0;
  generationSequence.clear();
    
  try{
    std::ifstream reader(loadPath);
    char c = '0';
    while(reader >> c){
      Shape s = Shape::Iblock;
      switch(c){
        case Content::I : s = Shape::Iblock; break;
        case Content::J : s = Shape::Jblock; break;
        case Content::O : s = Shape::Oblock; break;
        case Content::L : s = Shape::Lblock; break;
        case Content::S : s = Shape::Sblock; break;
        case Content::Z : s = Shape::Zblock; break;
        case Content::T : s = Shape::Tblock; break;
      } generationSequence.emplace_back(s);
    }
  } catch(std::runtime_error &e){
    generationSequence.emplace_back(Shape::Iblock);
    generationSequence.emplace_back(Shape::Jblock);
    generationSequence.emplace_back(Shape::Oblock);
    generationSequence.emplace_back(Shape::Lblock);
    for(int i=0; i<2; ++i) generationSequence.emplace_back(Shape::Sblock);
    for(int i=0; i<2; ++i) generationSequence.emplace_back(Shape::Zblock);
    generationSequence.emplace_back(Shape::Tblock);
  }
}


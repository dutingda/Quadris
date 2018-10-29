#include "levelthree.h"
using namespace std;

LevelThree::LevelThree(vector <vector <Cell>> * bd, string path):
  Difficulty{bd, path}
{
  generationSequence.emplace_back(Shape::Iblock);
  generationSequence.emplace_back(Shape::Jblock);
  generationSequence.emplace_back(Shape::Oblock);
  generationSequence.emplace_back(Shape::Lblock);
  for(int i=0; i<2; ++i) generationSequence.emplace_back(Shape::Sblock);
  for(int i=0; i<2; ++i) generationSequence.emplace_back(Shape::Zblock);
  generationSequence.emplace_back(Shape::Tblock);
}

Block LevelThree::newBlock(){
  if(loadPath != ""){
    if(generationSequence.size() != 0){
      counter %= generationSequence.size();
      return Block(generationSequence[counter++], 1, 3, 0, Level::lvl0);
    } else {
      return Block();
    }
  } return Block(generationSequence[rand() % generationSequence.size()],
                 1, 3, 0, Level::lvl3);
}

void LevelThree::setLoadPath(std::string path){
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


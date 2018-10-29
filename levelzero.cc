#include "levelzero.h"
#include <iostream>
using namespace std;

LevelZero::LevelZero(vector <vector <Cell>> * bd, string path):
  Difficulty{bd, path}
{
  loadPath = path;
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
}
LevelZero::LevelZero(){}

Block LevelZero::newBlock(){
  if(generationSequence.size() != 0){
    counter %= generationSequence.size();
    return Block(generationSequence[counter++], 0, 3, 0, Level::lvl0);
  } else return Block();
}

void LevelZero::setLoadPath(std::string){}

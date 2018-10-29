#include "levelone.h"
using namespace std;

LevelOne::LevelOne(vector <vector <Cell>> * bd, string path):
  Difficulty{bd, path}
{
  for(int i=0; i<2; ++i) generationSequence.emplace_back(Shape::Iblock);
  for(int i=0; i<2; ++i) generationSequence.emplace_back(Shape::Jblock);
  for(int i=0; i<2; ++i) generationSequence.emplace_back(Shape::Oblock);
  for(int i=0; i<2; ++i) generationSequence.emplace_back(Shape::Lblock);
  generationSequence.emplace_back(Shape::Sblock);
  generationSequence.emplace_back(Shape::Zblock);
  for(int i=0; i<2; ++i) generationSequence.emplace_back(Shape::Tblock);
}

Block LevelOne::newBlock(){
  return Block(generationSequence[rand() % generationSequence.size()],
               0, 3, 0, Level::lvl1);
}

void LevelOne::setLoadPath(std::string){}


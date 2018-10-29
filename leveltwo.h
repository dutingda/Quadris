#ifndef _LEVEL_TWO_H_
#define _LEVEL_TWO_H_

#include "difficulty.h"

class LevelTwo: public Difficulty{
public:
  LevelTwo();
  LevelTwo(std::vector <std::vector <Cell>> * bd,
           std::string load = "" "sequence.txt");
  Block newBlock() override;
  void setLoadPath(std::string path) override;
};

#endif

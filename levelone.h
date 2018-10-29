#ifndef _LEVEL_ONE_H_
#define _LEVEL_ONE_H_

#include "difficulty.h"

class LevelOne: public Difficulty{
 public:
  LevelOne();
  LevelOne(std::vector <std::vector <Cell>> * bd,
           std::string path = "sequence.txt");
  Block newBlock() override;
  void setLoadPath(std::string path) override;
};

#endif


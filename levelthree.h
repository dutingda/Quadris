#ifndef _LEVEL_THREE_H_
#define _LEVEL_THREE_H_

#include "difficulty.h"

class LevelThree: public Difficulty{
public:
  LevelThree();
  LevelThree(std::vector <std::vector <Cell>> * bd,
             std::string load = "");
  Block newBlock() override;
  void setLoadPath(std::string path) override;
};

#endif

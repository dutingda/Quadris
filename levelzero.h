#ifndef _LEVEL_ZERO_H_
#define _LEVEL_ZERO_H_

#include "difficulty.h"

class LevelZero: public Difficulty{
 public:
  LevelZero();
  LevelZero(std::vector <std::vector <Cell>> * bd,
            std::string path = """sequence.txt");
  Block newBlock() override;
  void setLoadPath(std::string path) override;
};

#endif


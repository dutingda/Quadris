#ifndef _LEVEL_FOUR_H_
#define _LEVEL_FOUR_H_

#include "difficulty.h"

class LevelFour: public Difficulty{
  std::vector <std::vector <Cell>> * board;
 public:
  LevelFour();
  LevelFour(std::vector <std::vector <Cell>> * bd,
            std::string load = "");
  Block newBlock() override;
  void implementPenalty() override;
  void setLoadPath(std::string path) override;
};

#endif


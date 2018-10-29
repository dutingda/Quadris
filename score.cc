#include "score.h"
#include "type.h"
#include <iostream>
// add the new score
void ScoreCounter::clear() { totalScore = 0; }

//
void ScoreCounter::addScore(Level lvl, int lines)
{
  int lvl_num = 0;
  switch (lvl) {
    case Level::lvl0: lvl_num = 0; break;
    case Level::lvl1: lvl_num = 1; break;
    case Level::lvl2: lvl_num = 2; break;
    case Level::lvl3: lvl_num = 3; break;
    case Level::lvl4: lvl_num = 4; break;
  }
  std::cout << totalScore << std::endl;
  totalScore += (lvl_num + lines) * (lvl_num + lines);
  if ( maxSoFar < totalScore ) maxSoFar = totalScore;
}

void ScoreCounter::addScore(Level lvl)
{
  int addOn = 0;
  switch (lvl) {
    case Level::lvl0: addOn = 1; break;
    case Level::lvl1: addOn = 2; break;
    case Level::lvl2: addOn = 3; break;
    case Level::lvl3: addOn = 4; break;
    case Level::lvl4: addOn = 5; break;
  }
  std::cout << totalScore << std::endl;
  totalScore += addOn; 
  if ( maxSoFar < totalScore ) maxSoFar = totalScore;
}

int ScoreCounter::getScore(){ return totalScore; }
int ScoreCounter::getHighScore(){ return maxSoFar; }

std::ostream &operator<<(std::ostream &out, ScoreCounter &s){
  out << "Score:    " << s.maxSoFar << std::endl;
  out << "Hi Score: " << s.totalScore << std::endl;
  out << "-------------" << std::endl;
  return out;
}

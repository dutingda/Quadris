#ifndef SCORE_H
#define SCORE_H
#include <iostream>
#include "type.h"

class ScoreCounter {
  // the variable to record the maxmium score
  int maxSoFar = 0;
  // the variable to record the total score in the game
  int totalScore = 0;
 public:
  void clear();                           // totalScore back to 0
    
  // addScore() update the new score to ScoreCounter
  void addScore(Level lvl, int lines);    // line eliminate version
  void addScore(Level lvl);               // block eliminate version
    
  // getScore() return the current score.
  // getHighScore() return the current highest score.
  int getScore();
  int getHighScore();
    
  // operator overload
  friend std::ostream &operator<<(std::ostream &out, ScoreCounter &s);
};

#endif /* score_h */

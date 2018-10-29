#ifndef _TYPE_H_
#define _TYPE_H_

const int HEIGHT = 15;
const int WIDTH = 11;

// Difficulty level
enum Level {lvl0 = 0, lvl1 = 1, lvl2 = 2, lvl3 = 3, lvl4 = 4, maxLvl = 4};

// Shape of block.
enum Shape {Iblock = 0, Jblock = 1, Oblock = 2, Lblock = 3,
            Sblock = 4, Zblock = 5, Tblock = 6};

// the content of the cell
enum Content {Hint = '?', Empty = ' ', Extra = '*',
              I = 'I', J = 'J', O = 'O', L = 'L', S = 'S',
              Z = 'Z', T = 'T'};

// command pass to other function
enum Move {Left, Right, Down, Drop, CounterRotate, Rotate};

#endif /* type.h */

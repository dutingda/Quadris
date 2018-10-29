#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

#include "grid.h"
#include "graphicsdisplay.h"
#include "type.h"

using namespace std;

shared_ptr<Grid> g(new Grid());

map<string, vector<string>> aliases;
string input = ""; string previous = "";

bool enableBonus = true;
int currentLevel = 0;

const int SUCCESS = 0b001;
const int FAILURE = 0;

const int OUTPUT = 0b010;
const int NO_OUTPUT = 0;

const int PRIMITIVE = 0b100;
const int NOT_PRIMITIVE = 0;

int interpret(string input);
void help();

// Returns 0b[primitive?][show_output?][success?]
int interpretPrimitive(vector<string> command){  
  if(command[0] == "left"){ g->mutate(Move::Left); return PRIMITIVE | OUTPUT | SUCCESS; }
  if(command[0] == "right"){ g->mutate(Move::Right); return PRIMITIVE | OUTPUT | SUCCESS; }
  if(command[0] == "down"){ g->mutate(Move::Down); return PRIMITIVE | OUTPUT | SUCCESS; }
  
  if(command[0] == "clockwise"){ g->mutate(Move::Rotate); return PRIMITIVE | OUTPUT | SUCCESS; }
  if(command[0] == "counterclockwise"){ g->mutate(Move::CounterRotate); return PRIMITIVE | OUTPUT | SUCCESS; }
  
  if(command[0] == "drop"){ g->mutate(Move::Drop); return PRIMITIVE | OUTPUT | SUCCESS; }
  
  if(command[0] == "levelup"){
    g->changeLevel(currentLevel = min(currentLevel+1, 4));
    return PRIMITIVE | OUTPUT | SUCCESS;
  }
  if(command[0] == "leveldown"){
    g->changeLevel(currentLevel = max(currentLevel-1, 0));
    return PRIMITIVE | OUTPUT | SUCCESS;
  }
  
  if(command[0] == "norandom"){
    if(command.size() == 1){
      cout << "  Please provide a file name" << endl;
      return PRIMITIVE | NO_OUTPUT | FAILURE;
    }
    
    try{ g->setLoadPath(command[1]); } catch(runtime_error &e){
      cout << "  File loading from " << command[1] << " failed" << endl;
      return PRIMITIVE | NO_OUTPUT | FAILURE;
    }
    cout << "  Randomisation disabled for levels 3 and 4. Loads from " << command[1] << endl;
    return PRIMITIVE | NO_OUTPUT | SUCCESS;
  } if(command[0] == "random"){
    try{ g->setLoadPath(""); } catch(runtime_error &e){}
    cout << "  Randomisation enabled for levels 3 and 4" << endl;
    return PRIMITIVE | NO_OUTPUT | SUCCESS;
  }

  if(command[0] == "sequence"){
    try{
      std::ifstream reader(command[1]);
      string s = "";
      while(getline(reader, s)){
        if(!(interpret(s) & SUCCESS)){
          cout << "  Failed executing command \"" << s << "\"in file " << command[1] << endl;
          return PRIMITIVE | OUTPUT | FAILURE;
        }
      }
    } catch(runtime_error &e){
      cout << "  Failed executing commands found in file \"" << command[1] << "\"" << command[1] << endl;
      return PRIMITIVE | OUTPUT | FAILURE;
    }
    
    return PRIMITIVE | OUTPUT | SUCCESS;
  }

  if(command[0] == "I" || command[0] == "J" || command[0] == "O" ||
     command[0] == "L" || command[0] == "S" || command[0] == "Z" ||
     command[0] == "T"){
    Shape s = Shape::Iblock;
    switch(command[0][0]){
      case 'I' : s = Shape::Iblock; break;
      case 'J' : s = Shape::Jblock; break;
      case 'O' : s = Shape::Oblock; break;
      case 'L' : s = Shape::Lblock; break;
      case 'S' : s = Shape::Sblock; break;
      case 'Z' : s = Shape::Zblock; break;
      case 'T' : s = Shape::Tblock; break;
    }
    g->changeShape(s);
    return PRIMITIVE | OUTPUT | SUCCESS;
  }
  
  if(command[0] == "restart"){ g->clear(); return PRIMITIVE | OUTPUT | SUCCESS; }
  
  if(command[0] == "hint"){ g->hint(); return PRIMITIVE | OUTPUT | SUCCESS; }

  if(command[0] == "help"){ help(); return PRIMITIVE | NO_OUTPUT | SUCCESS; }

  if(command[0] == "disp") return PRIMITIVE | OUTPUT | SUCCESS;
  
  return NOT_PRIMITIVE | NO_OUTPUT | FAILURE;
}

int interpret(int repetition, vector<string> command){//, Grid g){
  try{
    if(command.size() == 0){
      cout << "  Nothing to do/repeat" << endl;
      return false;
    }

    stringstream in(command[0]); int r;
    if(in >> r) repetition = r;
    in >> command[0];
    
    if(command[0] == "def"){ // Command binding
      if(command.size() == 1){
        cout << "  Macro binding must have a name." << endl;
        return NOT_PRIMITIVE | NO_OUTPUT | FAILURE;
      }
      string keyword = command[1];
      command.erase(command.begin());
      command.erase(command.begin());

      // Null definitions
      if(command.size() == 0){
        cout << "  Macro binding must be non-empty for macro: " << keyword << endl;
        return NOT_PRIMITIVE | NO_OUTPUT | FAILURE;
      }
      
      // Rebinding keywords
      if(keyword == "def"){
        cout << "  Cannot create binding for keyword def" << endl;
        return NOT_PRIMITIVE | NO_OUTPUT | FAILURE;
      }
      for(auto s : command)
        if(s == "def"){
          cout << "  Cannot create binding for keyword def" << endl;
          return NOT_PRIMITIVE | NO_OUTPUT | FAILURE;
        }
      
      aliases[keyword] = command;
      cout << "  Macro " << keyword << " is now bound to: ";
      for(auto s : command) cout << s << "  ";
      cout << endl;
      return NOT_PRIMITIVE | NO_OUTPUT | FAILURE;
    }
    
    int prevExit = 0;
    for(int r = 0; r < repetition; ++r){
      // Checks if command is primitive
      int checkPrimitive = interpretPrimitive(command);
      if(checkPrimitive & PRIMITIVE){
        prevExit = checkPrimitive;
        continue;
      }
      
      // Sees if command is uniquely defined
      vector<string> suggestions, definition;
      for(auto it : aliases)
        if(it.first.substr(0, min(command[0].size(), it.first.size())) == command[0]){
          suggestions.emplace_back(it.first);
          definition = it.second;
        }

      // Executes only uniquely defined commands
      switch(suggestions.size()){
        case 0: // Undefined command
          cout << "  Undefined command " << command[0] << endl;
          return NOT_PRIMITIVE | NO_OUTPUT | FAILURE;
        case 1: // Uniquely defined command
          if(definition.size() == 0) definition.emplace_back(suggestions[0]);
          for(auto s : definition){
            prevExit = interpret(s);
            if(!(prevExit & SUCCESS)){
              cout << "  ^^^ Bad macro definition of macro " << command[0] << endl;
              cout << "      defined as: " << endl << "    ";
              for(auto s : definition) cout << "  " << s;
              cout << endl;
              return prevExit;
            }
          }
          continue;
        default: // Cannot uniquely determine command
          cout << "  Multiple possibilities for " << command[0] <<". Suggestions: " << endl;
          cout << "    ";
          for(auto suggestion : suggestions) cout << suggestion << "  ";
          cout << endl;
          return NOT_PRIMITIVE | NO_OUTPUT | FAILURE;
      }
    } return prevExit;
  } catch(runtime_error &e){ cout << e.what() << endl; return NOT_PRIMITIVE | NO_OUTPUT | FAILURE; }
}

int interpret(string input){
  try{
    if(input == "") return PRIMITIVE | NO_OUTPUT | FAILURE;

    if(input == "!!") return interpret(previous);
    
    stringstream in(input);

    int rep;
    if(!(in >> rep)) rep = 1;
    in.clear();
  
    string s;
    vector<string> cmd;
    while(in >> s) cmd.emplace_back(s);

    int exit = interpret(rep, cmd);
    if(exit & SUCCESS) previous = input;

    return exit;
  } catch(runtime_error &e){ cout << e.what() << endl; return NOT_PRIMITIVE | NO_OUTPUT | FAILURE; }
}

void help(){
  std::ifstream reader("helpfile");
  try{
    string s;
    while(getline(reader, s)) cout << "  " << s << endl;
  } catch(runtime_error &e){
    cout << "  Error displaying help file." << endl;
  }
}

int main(int argc, const char* argv[]){
  bool textOnly = false;
  Level startLevel = Level::lvl0;
  string defaultPath = "sequence.txt";

  for(int i=0; i<argc; ++i){
    if(string(argv[i]) == "-text") textOnly = true;
    else if(string(argv[i]) == "-nobonus") enableBonus = false;
    else if(string(argv[i]) == "-seed" || string(argv[i]) == "-level"){
      if(i+1 >= argc){
        cerr << "Please provide an integer as " << string(argv[i]).substr(1) << endl;
        return 1;
      }

      stringstream ins(argv[++i]);
      int n;
      if(ins >> n){
        if(string(argv[i]) == "-seed") srand(n);
        else startLevel = (Level) n;
      }
      else{
        cerr << "Please provide an integer as " << string(argv[i]).substr(1) << endl;
        return 1;
      }
    } else if(string(argv[i]) == "-scriptfile"){
      if(i+1 >= argc){
        cerr << "Please provide a scriptfile" << endl;
        return 1;
      } defaultPath = argv[i+1];      
    }
  }
  
  // Sets primitive commands to be defined
  aliases["left"] = aliases["right"] = aliases["down"] =
    aliases["clockwise"] = aliases["counterclockwise"] =
    aliases["drop"] = 
    aliases["levelup"] = aliases["leveldown"] =
    aliases["norandom"] = aliases["sequence"] = 
    aliases["I"] = aliases["J"] = aliases["O"] = aliases["L"] = aliases["S"] = aliases["Z"] = aliases["T"] =
    aliases["restart"] = aliases["hint"] =
    aliases["help"] = aliases["disp"] =
    vector<string>();

  // Initialises grid
  g->init(defaultPath, startLevel);

  if(!textOnly) g->setupGraphic();

  cout << *g;
  
  cout << endl << "Enter command (\"help\" for instructions and \"quit\" to quit): ";
  while(getline(cin, input)){
    if(input == "quit") break;

    if(interpret(input) & OUTPUT) cout << *g;
    
    if(g->gameOver()){
      cout << "  Sorry, game over. Press any key to continue..." << endl;
      g->clear();
      
      getchar();

      cout << *g;
    }   cout << endl << "Enter command (\"help\" for instructions and \"quit\" to quit): ";
  }

  cout << endl << "  Game ended." << endl;
}

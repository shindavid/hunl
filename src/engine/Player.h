#pragma once

#include "GameEvent.h"
#include "TypeDefs.h"

#include <string>

class Player {
private:
  player_id_t _id;  // eventually retrieve from a database
  std::string _name;

public:
  Player(const char* name) {
    _id = __next_id++;
    _name = name;
  }

  player_id_t getID() const { return _id; }
  const char* getName() const { return _name.c_str(); }
  
  virtual void handleEvent(const GameEvent& event) {}
  virtual PlayerEvent handleActionRequest() = 0;

private:
  static player_id_t __next_id;
};


#include "state.h"

state::state(character * me, character *target, int (*entryAction)(character*, character*, vector<node> * graph),
      int (*action)(character*, character*, vector<node> * graph),
      int (*exitAction)(character*, character*, vector<node> * graph), string name, vector<node> * graph) {
  this->me = me;
  this->target = target;
  this->entryAction = entryAction;
  this->action = action;
  this->exitAction = exitAction;
  this->name = name;
  this->graph = graph;
}

state::state(const state& orig) {
  this->me = orig.me;
  this->target = orig.target;
  this->entryAction = orig.entryAction;
  this->action = orig.action;
  this->exitAction = orig.exitAction;
  this->name = orig.name;
  this->graph = orig.graph;
}

state::~state() {
}


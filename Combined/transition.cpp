#include "transition.h"

transition::transition( int (*trigger)(character*, character*, vector<node> * graph), state* to ) {
  this->trigger = trigger;
  this->to = to;
}

transition::transition(const transition& orig) {
  this->trigger = orig.trigger;
  this->to = orig.to;
}

transition::~transition() {
}


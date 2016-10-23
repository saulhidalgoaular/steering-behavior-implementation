#ifndef TRANSITION_H
#define	TRANSITION_H

#include "character.h"
#include "state.h"
#include "node.h"

class state;

class transition {
public: 
  int (*trigger)(character*, character*, vector<node> * graph);
  state * to;
  
  transition( int (*trigger)(character*, character*, vector<node> * graph), state * to);
  transition(const transition& orig);
  virtual ~transition();
private:

};

#endif	/* TRANSITION_H */


#ifndef STATE_H
#define	STATE_H

#include "character.h"
#include "transition.h"
#include <vector>
#include "node.h"

using namespace std;

class transition;

class state {
public:
  character * me, * target;
  vector<node> * graph;
  int (*entryAction)(character*, character*, vector<node> * graph);
  int (*action)(character*, character*, vector<node> * graph);
  int (*exitAction)(character*, character*, vector<node> * graph);
  
  vector<transition *> succ;
  
  string name;
  
  state(character * me, character *target, int (*entryAction)(character*, character*, vector<node> * graph),
      int (*action)(character*, character*, vector<node> * graph),
      int (*exitAction)(character*, character*, vector<node> * graph), string name, vector<node> * graph);
  state(const state& orig);
  virtual ~state();
private:

};

#endif	/* STATE_H */


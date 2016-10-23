#ifndef EDGE_H
#define	EDGE_H

#include "vec3.hpp"
#include "character.h"
#include "node.h"
#include <vector>

class node;

class edge: public character{
public:
  /**
   * Costo del camino
   */
  GLfloat cost;
  
  /**
   * Apuntador al nodo inicio
   */
  node* init;
  
  /**
   * Apuntador al nodo fin.
   */
  node* end;
  
  edge();
  edge(node* _pred, node* _succ, GLfloat _cost , bool _draw);
  edge(const node& orig);

  /**
   * Metodo sobrecargado, en este caso dibuja una linea que une el init con el end.
   */
  void redraw();
  virtual ~edge();
private:

};

#endif	/* EDGE_H */

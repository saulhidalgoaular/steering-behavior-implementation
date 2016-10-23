#ifndef NODE_H
#define	NODE_H

#include "SDL.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "vec3.hpp"
#include "character.h"
#include "edge.h"
#include <vector>

class edge;

/**
 * Extiende a la clase character, para tener la posibilidad mover los nodos
 */
class node: public character {
public:
  vec3 color;
  
  int id;
  
  GLfloat ratio;
  std::vector<edge*> succ;
  
  node();
  node(int _id, vec3 p, vec3 c, GLfloat _ratio);
  node(const node& orig);
  
  GLfloat get_euclid_dist(node* e);
  
  /**
   * Metodo sobrecargado. En este caso dibuja un cuadrado.
   */
  void redraw();
  void add_succesor(edge& n);
  
  
  /**
   * Verifica si el objeto esta dentro de la region del 
   * nodo caracteristico.
   * @param object_position Posicion del otro objeto.
   * @return True si esta dentro de la region.
   */
  bool is_inside(vec3 & object_position);
  
  virtual ~node();
private:

};

#endif	/* NODE_H */


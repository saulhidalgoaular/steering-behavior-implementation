#ifndef PATH_H
#define	PATH_H

#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <stdio.h>
#include "SDL.h"
#include "vec3.hpp"

using namespace std;

class path {
public:
  
  /**
   * Vector con los puntos leidos.
   */
  vector<vec3> points_read;
  GLfloat delta_points; // Deprecada.
  
  /**
   * Archivo desde donde se leen los puntos.
   */
  string file_path;
  
  path();
  path(std::string file, GLfloat _delta_points);
  void draw();
  
  /**
   * De todos los puntos leidos, toma POINTS_TO_PICK puntos igualmente espaciados.
   */
  void get_points_to_pick();
  path(const path& orig);
  virtual ~path();
  
  /**
   * Dados dos puntos, devuelve la distancia entre ellos.
   * @param i indice del primer puntos.
   * @param j indice del segundo.
   * @return Distancia.
   */
  GLfloat get_dist( int i, int j );
  GLfloat get_dist( const vec3 & a, const vec3 & b);
  
  /**
   * Dada una posicion, y el indice del ultimo punto mas cercado tomado en cuenta, 
   * calcula punto mas cerca.
   * @param p
   * @param current_index
   * @return Indice del punto mas cercano.
   */
  int get_closest_point( const vec3& p , int current_index );
  
  /**
   * Dado el punto mas cercano, devuelve la posicion del punto al que se le hara seek.
   * @param current_segment 
   * @return 
   */
  vec3 get_next_target( int current_segment );

private:
  /**
   * Carga el archivo, cada linea debe contener 2 floats, separados por , indicando 
   * la posicion de ese punto en el camino
   * @param addr
   */
  void load_file(const char * addr);
  void generate_points(); // Deprecada.
};

#endif	/* PATH_H */


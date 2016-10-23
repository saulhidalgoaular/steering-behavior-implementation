#ifndef CHARACTER_H
#define	CHARACTER_H

#include "vec3.hpp"
#include "SDL.h"
#include <math.h>
#include <string>
#include "utils.hpp"
#include "config.hpp"
#include "path.h"
#include <iostream>
#include <vector>

/**
 * Representa a todos los personajes en el juego, por lo que implementa todos los comportamiento
 * basicos mencionados anteriormente.
 */
class character {
public:
  /**
   * Posicion actual del personaje
   */
  vec3 position;
  
  /**
   * Orientacion actual del personaje desde 0 a 360, siendo 0 el lado 'este'.
   */
  GLfloat orientation;
  
  /**
   Velocidad actual del personaje
   */
  vec3 velocity;
  
  /**
   * Velocidad angular del personaje
   */
  GLfloat rotation;
  
  /**
   * Utilizado en el path following, indica el ultimo punto del camino mas cercano
   * al personaje desde el path.
   */
  int current_index_path;
  
  /**
   * Indica si debe dibujarse en el mapa el personaje.
   * Si es false, no se dibujara ni se le hara el update
   */
  bool do_the_draw;
  
  /**
   * Velocidad maxima del personaje. Especificada en el contructor
   */
  GLfloat max_speed;
  
  /**
   * Aceleracion lineal.
   */
  vec3 linear;
  
  /**
   * Aceleracion angular.
   */
  GLfloat angular;
  
  float life;
 
  bool draw_life;
  
  /**
   * Direccion en el fs donde se encuentra la imagen.
   */
  std::string picture_addr;
  
  /**
   * Identificador de Textura.
   */
  GLuint texture;

  vector<int> path_in_graph;

  int current_path_index;
  
  /**
   * Orientacion utilizada en el wander mejorado.
   */
  GLfloat wander_orientation;
  
  character();
  character(vec3 p, GLfloat o, const std::string & pic, bool draw, GLfloat max_speed);
  character(const character& orig);
  
  /**
   * Basandose en las aceleraciones, actualiza las velocidades y las posiciones.
   * @param time Lapso de tiempo transcurrido.
   */
  void update(GLfloat time);
  
  /**
   * Calcula la orientacion basandose en la velocidad
   * @return 
   */
  GLfloat get_new_orientation();
  
  /**
   * Hace seek al punto mas cercano mas un offset.
   * @param the_path Camino representado como una coleccion de puntos.
   */
  void follow_the_path(path & the_path);
  
  /**
   * Dado un personaje, ajusta la aceleracion angular para alinearlos
   * @param target
   */
  void align(character & target);
  
  /**
   * Basandose en la funcion align, hace que el personaje vea hacia donde esta
   * desplazandose, basandose en la velocidad.
   */
  void look_where_you_are_going();
  
  /**
   * Metodo que indica como dibujar la imagen. Es sobreescrito para los personajes
   * como nodo y arista.
   */
  virtual void redraw();
  
  /**
   * Dado un target, le hace flee
   * @param target Target
   */
  void flee(vec3 & target);
  
  /**
   * Dado un target, le hace pursue utilizando el offset especificado en config.hpp
   * @param target
   */
  void pursue(character & target);
  
  /**
   * Dado una manada ( o banco dependiendo del personaje ), 
   * hace separation, modificando la aceleracion del character que esta en el indice especificado
   * @param flock Manada
   * @param my_index_in_flock Personaje que se modificara.
   */
  void separation( vector< character > & flock , int my_index_in_flock );
  
  /**
   * Hace arrive al target
   * @param target Target
   * @param current_position Es la posicion real actual del target
   * @param reduce_speed indica si debe o no reducir la velocidad al acercarse.
   * @return true si genero aceleracion. False en caso contrario.
   */
  bool classic_arrive( vec3 & target, vec3 & current_position, bool reduce_speed);
  
  /**
   * Carga la textura que se utiliza en el personaje
   * @return Idenfiticador de textura.
   */
  int LoadGLTextures();
  
  /**
   * Wandering mejorado
   */
  void do_wandering();
  
  /**
   * Dado la posicion donde se desea hacer el lanzamiento, devuelve el vector velocidad.
   * Devuelve el vec3() si no alcanza a position con los parametros especificados.
   */
  vec3 calculate_firing_solution( const vec3 & position);
  
  
  character& operator=(const character & b);
  
  virtual ~character();
private:
  
};

#endif	/* CHARACTER_H */


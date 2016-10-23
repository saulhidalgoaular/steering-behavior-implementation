#ifndef CONFIG_HPP
#define	CONFIG_HPP

#include <string>
#include <math.h>

#include "vec3.hpp"

#ifdef	__cplusplus
extern "C" {
#endif

  
  // Tama~no de la pantalla
  #define SCREEN_WIDTH  1280
  #define SCREEN_HEIGHT 1000
  #define SCREEN_BPP     16

  #define TRUE  1
  #define FALSE 0

  // DEBUG 0 para eliminar couts
  #define DEBUG 1

  #ifdef DEBUG
  #define dout cout
  #else
  #define dout 0 && cout
  #endif

  static const GLfloat EPS = 1e-5;
  static const GLfloat INF_SPINS = 10000; // Numero muy alto, utilizado para el map_to_range
  static const GLfloat RADIUS = 0.4f; // Radio del arrive
  static const GLfloat TIME_TO_TARGET = 1.1f;
  static const GLfloat MAX_SPEED = 3.;
  static const GLfloat MAX_ACCELERATION = 50.f;
  static const GLfloat MAX_ANGULAR_ACCELERATION = 100.f;
  static const GLfloat MAX_PREDICTION_TIME = 10.f;
  static const GLfloat SLOW_RADIUS = 0.2f; // Utilizado en el arrive
  static const GLfloat TIME_ELAPSED = 1.f/60.f; // Tiempo utilizado para realizar cada update.
  static const GLfloat MAX_ROTATION = 100;
  static const int TIME_TO_ROTATE = 1;
  static const GLfloat PI = acos(0) * 2;
  static const GLfloat DEGREE_PI = 180.f;
  static const GLfloat WANDER_OFFSET = 5.f;
  static const GLfloat WANDER_RADIUS = 5.f;
  static const GLfloat WANDER_RATE = 5.f;
  static const GLfloat POINTS_TO_PICK = 100; // Puntos tomados en el path.
  static const int PATH_OFFSET = 3; // Punto del offset al que se le hace seek.
  static const GLfloat Z_FLOOR = -5.f;
  static const GLfloat MUZZLE = 3.f; // Velocidad de lanzamiento del proyectil.
  static const GLfloat GRAVITY = -.4f;
  static const GLfloat THRESHOLD = 1.5f;
  static const GLfloat DECAY_COEF = 1.5f;
  static const GLfloat INF_GLFLOAT = 1e30;
  
  static const GLfloat EDGE_RATIO = 1.f;
  static const GLfloat LIFE_DECREASE = 5e-2;
  
  /**
   Direcciones de las texturas.
   */
  static const std::string FISH_ADDR = "data/star.bmp";
  static const std::string BACKGROUND_ADDR = "data/background.bmp";
  static const std::string PUDGE_ADDR = "data/pudge.bmp";
  static const std::string MEDUSA_ADDR = "data/dusa.bmp";
  static const std::string BALL_ADDR = "data/ball.bmp";
  static const std::string TARGET_ADDR = "data/target.bmp";
  static const std::string LOBSTER_ADDR = "data/lobster.bmp";

#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_HPP */


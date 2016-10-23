#include<ctime>

#include "SDL.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "config.hpp"

#ifndef UTILS_HPP
#define	UTILS_HPP

  GLfloat random_float();
  GLfloat randomBinomial();
  int resizeWindow( int width, int height );
  void Quit( int returnCode );
  int initGL();
  GLfloat map_to_range(GLfloat angle);
  GLfloat min(GLfloat a, GLfloat b);
  int random_int(int n);
  int myRand(int i, int f);

  
#endif	/* UTILS_HPP */


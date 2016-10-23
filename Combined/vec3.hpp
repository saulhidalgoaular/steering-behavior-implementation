#ifndef VEC3_HPP
#define	VEC3_HPP

#include <GL/glu.h>
#include <math.h>
#include <assert.h>
#include "config.hpp"
#include <iostream>

class vec3 {
public:
  GLfloat x,y,z;
  vec3();
  vec3(GLfloat xx, GLfloat yy, GLfloat zz);
  vec3(const vec3& orig);
  void add_scalar_from_vector(vec3 &v, GLfloat s);
  GLfloat get_length();
  void normalize();

  vec3 operator+(const vec3 & b);
  vec3 operator-(const vec3 & b);
  vec3 operator*(const GLfloat b);
  vec3 operator/(const GLfloat b);
  GLfloat operator*(const vec3 & b);
  
  vec3& operator=(const vec3 & b);
  vec3& operator/=(const GLfloat f);
  vec3& operator*=(const GLfloat f);
  vec3& operator+=(const vec3 & b);
  
  virtual ~vec3();
private:
  
};


std::ostream& operator<< (std::ostream& stream, vec3 &v );

#endif	/* VEC3_HPP */


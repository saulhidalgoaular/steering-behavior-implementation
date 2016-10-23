#include "vec3.hpp"

vec3::vec3() {
  this->x = this->y = this->z = .0f;
}

vec3::vec3(const vec3& orig) {
  this->x = orig.x;
  this->y = orig.y;
  this->z = orig.z;
}

vec3::~vec3() {
}

vec3::vec3(GLfloat xx, GLfloat yy, GLfloat zz) {
  this->x = xx;
  this->y = yy;
  this->z = zz;
}

void vec3::add_scalar_from_vector(vec3 &v, GLfloat s){
  this->x += v.x*s;
  this->y += v.y*s;
  this->z += v.z*s;
}

GLfloat vec3::get_length() {
  return sqrt(x*x + y*y + z*z);
}

void vec3::normalize() {
  GLfloat size = get_length();
  if ( size < EPS ){
    return;
  }
  
  x /= size; y /= size; z /= size;
}

vec3 vec3::operator +(const vec3& b) {
  vec3 ans(x+b.x, y+b.y, z+b.z);
  return ans;
}

vec3 vec3::operator -(const vec3& b) {
  vec3 ans(x-b.x, y-b.y, z-b.z);
  return ans;
}

vec3 vec3::operator *(const GLfloat b) {
  vec3 ans(x*b, y*b, z*b);
  return ans;
}

vec3 vec3::operator /(const GLfloat b) {
  vec3 ans(x/b, y/b, z/b);
  return ans;
}

GLfloat vec3::operator *(const vec3 & b){
  GLfloat ans = x * b.x + y * b.y + z * b.z;
  return ans;
}

vec3& vec3::operator =(const vec3& b) {
  if ( this == &b ){
    return *this;
  }
  x = b.x; y = b.y; z = b.z;
  return *this;
}

vec3& vec3::operator /=(const GLfloat f) {
  assert( fabs( f ) > EPS );
  
  x /= f; y /= f; z /= f;
  return *this;
}

vec3& vec3::operator *=(const GLfloat f) {
  x *= f; y *= f; z *= f;
  return *this;
}

vec3& vec3::operator +=(const vec3& b) {  
  x += b.x; y += b.y; z += b.z;
  return *this;
}

std::ostream& operator <<(std::ostream& stream, vec3 &v) {
  stream << "[" << v.x << "," << v.y << "," << v.z << "]";
  return stream;
}

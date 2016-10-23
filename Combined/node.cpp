#include "node.h"

node::node() {
}

node::node(const node& orig) {
  *this = orig;
}

node::~node() {
}

node::node(int _id, vec3 p, vec3 c, GLfloat _ratio): character(p, .0f, "", 1, .0f){
  id = _id;
  color = c;
  ratio = _ratio;
}

GLfloat node::get_euclid_dist(node* e) {
  return (position - e->position).get_length();
}

void node::redraw(){
  //glColor3f(1.f, 1.f, 1.f);
  glLoadIdentity( );
  glColor3f(color.x, color.y, color.z);
  //glColor3f(1.f,0.f,0.f);
  
  glTranslatef( position.x, position.y, position.z );
  
  glBegin( GL_QUADS );
    //glNormal3f( 0.0f, 0.0f, 1.0f );
    glVertex3f( -.1f, -.1f ,  0 );
    glVertex3f(  .1f, -.1f ,  0 );
    glVertex3f(  .1f,  .1f ,  0 );
    glVertex3f( -.1f ,  .1f ,  0 );
  glEnd();

  glBegin(GL_LINE_LOOP);
 
  for (int a = 0; a < 360; a += 360 / 20.0)
  {
    double heading = a * PI / 180;
    glVertex2d(cos(heading) * ratio, sin(heading) * ratio);
  }
 
glEnd();
//  glBegin(GL_QUADS); // Start drawing a quad primitive
//    glVertex3f(position.x-1.0f, position.y + -1.0f, Z_FLOOR); // The bottom left corner
//    glVertex3f(position.x-1.0f, position.y+ 1.0f, Z_FLOOR); // The top left corner
//    glVertex3f(position.x+1.0f, position.y+1.0f, Z_FLOOR); // The top right corner
//    glVertex3f(position.x+1.0f, position.y+-1.0f, Z_FLOOR); // The bottom right corner
//  glEnd();
}


bool node::is_inside(vec3& object_position){
  vec3 diff = object_position - position;
  GLfloat dist = diff.get_length();
  
  return dist < ratio;
}
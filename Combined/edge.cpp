#include "edge.h"
#include <GL/gl.h>
#include "node.h"

edge::edge() {
}

edge::edge(node* _pred,node* _succ, GLfloat _cost, bool _draw){
  init = _pred;
  end = _succ;
  cost = _cost;
  do_the_draw = _draw;
}

edge::edge(const node& orig) {
  *this = orig;
}

void edge::redraw() {
  glLoadIdentity( );
  glBegin(GL_LINES);
    glLineWidth(6.5f);
    glColor3f(1.f,0.f,0.f);
    glVertex3f( init->position.x, init->position.y, Z_FLOOR );
    glVertex3f( end->position.x, end->position.y, Z_FLOOR );
  glEnd();
  
//  vec3 mid = pred->position + succ->position;
//  mid /= 2.f;
//  GLfloat tmps = atan2(succ->position.y-pred->position.y, succ->position.x-pred->position.x) / PI * 180;
//  //glRotatef( tmps , 0.0f, 0.0f, 1.0f);
//  glBegin(GL_TRIANGLES);                      // Drawing Using Triangles
//    glVertex3f( mid.x+0.5f, mid.y, Z_FLOOR);              // Top
//    glVertex3f( mid.x-.5f/2,mid.y-.5f, Z_FLOOR);              // Bottom Left
//    glVertex3f( mid.x-.5f/2,mid.y+.5f, Z_FLOOR);              // Bottom Right
//  glEnd(); 
//  //glRotatef( -tmps , 0.0f, 0.0f, 1.0f);
}


edge::~edge() {
}


#include "path.h"

path::path() {
}

path::path(const path& orig) {
}

path::~path() {
}

path::path(std::string file, GLfloat _delta_points) {
  delta_points = _delta_points;
  file_path = file;
  
  load_file( file.c_str() );
}


void path::load_file(const char* addr) {
  ifstream file( addr );
  
  if ( file.is_open() ){
    string line;
    while( getline(file,line) ){
      vec3 cur_point;
      sscanf(line.c_str(), "%f,%f", &cur_point.x, &cur_point.y);
      cur_point.z = Z_FLOOR;
      points_read.push_back( cur_point );
    }
    get_points_to_pick();
  }else{
    cerr << "NO SE PUDO ABRIR EL ARCHIVO!!!" << endl;
  }
}

void path::get_points_to_pick() {
  vector<GLfloat> points;
  for (int i = 0; i < (int)POINTS_TO_PICK; i++) {
    points.push_back( (GLfloat)i/POINTS_TO_PICK  );
  }
  vector<vec3> points_tmp;
  for (int i = 0; i < POINTS_TO_PICK; i++) {
    int index = round( points[i] * points_read.size() );
    if ( index < (int)points_read.size() ){
      points_tmp.push_back( points_read[index] );
    }
  }
  points_read = points_tmp;
}

void path::draw() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glLoadIdentity( );
  dout << "DIBUJANDO PATH " << endl;
  
  for (int i = 1; i < (int)points_read.size(); i++) {
    glBegin(GL_LINES);
      glLineWidth(6.5f);
      glColor3f(1.f,0.f,0.f);
      glVertex3f( points_read[i-1].x, points_read[i-1].y, Z_FLOOR );
      glVertex3f( points_read[i].x, points_read[i].y, Z_FLOOR );
      //dout << "DESDE " << points_read[i-1].x << "," << points_read[i-1].y << " HASTA " << points_read[i].x << "," << points_read[i].y << " DISTANCIA = " << get_dist(i-1,i)<< endl;
    glEnd();
  }
  
  /*for (int i = 0; i < (int)points_read.size() ;i++) {
    glLoadIdentity( );
    //double dx = points_read[i].z;
    //double dx = point_generated[i].z;
    double dx = points_read[i].x;
    double dy = points_read[i].y;
    dout << "DX = " << dx << endl;
    //glTranslatef( i, points_read[i].x, -5 );
    glBegin(GL_TRIANGLES);
      glColor3f(1.0, 1.0, 1.0);
      glVertex3f(0+dx, 0+dy, -5);
      glVertex3f(1+dx, 0+dy, -5);
      glVertex3f(0+dx, 1+dy, -5);
    glEnd();

  }*/
}

GLfloat path::get_dist(int ii, int jj){
  assert ( ii < (int)points_read.size() );
  assert ( jj < (int)points_read.size() );
  return get_dist( points_read[ii] , points_read[jj] );
 }

GLfloat path::get_dist(const vec3 & a, const vec3 & b){
  GLfloat deltaX = a.x - b.x;
  GLfloat deltaY = a.y - b.y;
  GLfloat deltaZ = a.z - b.z;

  return sqrt( deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ );

}

int path::get_closest_point( const vec3 & p, int current_index ){
  int ans = 0;
  GLfloat best_dist = (float)(1 << 20);

  for ( int i = 0 ; i < (int)points_read.size() ; i++ ){
    GLfloat cur_dist = get_dist( p , points_read[i]  );
    if ( cur_dist < best_dist && abs(current_index - i) < PATH_OFFSET ) {
      ans = i;
      best_dist = cur_dist;
    }
  }

  dout << "CURRENT POINT = " << p.x << "," << p.y << " CLOSEST = " << points_read[ans].x << "," << points_read[ans].y << endl;

  glLoadIdentity( );
  /*glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(0+points_read[ans].x, 0+points_read[ans].y, -5);
    glVertex3f(1+points_read[ans].x, 0+points_read[ans].y, -5);
    glVertex3f(1+points_read[ans].x, 1+points_read[ans].y, -5);
    glVertex3f(0+points_read[ans].x, 1+points_read[ans].y, -5);
  glEnd();
*/
  /*glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-1+p.x, -1+p.y, -5);
    glVertex3f(1+p.x, -1+p.y, -5);
    glVertex3f(1+p.x, 1+p.y, -5);
    glVertex3f(-1+p.x, 1+p.y, -5);
  glEnd();*/
  return ans;
}

vec3 path::get_next_target( int current ){

  vec3 ans = points_read[ min((int)(current + PATH_OFFSET), (int)(points_read.size() - 1)) ];
  /*glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(0+ans.x, 0+ans.y, -5);
    glVertex3f(1+ans.x, 0+ans.y, -5);
    glVertex3f(0+ans.x, 1+ans.y, -5);
  glEnd();
  */
  dout << "NEXT_TARGET = " << ans << endl;
  return ans; 
}

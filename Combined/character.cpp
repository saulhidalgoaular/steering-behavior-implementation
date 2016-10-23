#include "character.h"
#include "path.h"

character::character() {
}

character::character(const character& orig) {
  *this = orig;
}

character::~character() {
}

void character::update(GLfloat time) {
  this->position.add_scalar_from_vector(this->velocity, time);
  std::dout << position << std::endl;
  this->orientation += this->rotation * time;
  
  this->velocity.add_scalar_from_vector( this->linear, time );
  
  if ( velocity.get_length() > max_speed ){
    velocity.normalize();
    velocity *= max_speed;
  }
  
  this->rotation += this->angular * time;
}

GLfloat character::get_new_orientation() {
  if ( this->velocity.get_length() > EPS ){
    return atan2( -velocity.x, velocity.y ) / PI * DEGREE_PI;
  }
  return orientation;
}

void character::look_where_you_are_going() {
  if ( velocity.get_length() < EPS ){
    return;
  }
  
  std::dout << " TIENE VELOCIDAD = " << velocity.get_length() << std::endl;
  
  character align_target;
  align_target.orientation = atan2( velocity.y , velocity.x ) * DEGREE_PI / PI;
  std::dout << "ALIGN Orientation = " << align_target.orientation << " " << velocity.x << "," << velocity.y << std::endl;
  
  align( align_target );
}

void character::separation( vector<character> & flock, int my_index_in_flock ){
  for (int i = 0; i < (int)flock.size(); i++) {
    if ( i != my_index_in_flock ){
      vec3 direction = flock[i].position - position;
      direction = direction * (-1);
      GLfloat distance = direction.get_length();
      
      if ( distance < THRESHOLD ){
        GLfloat strength = min( DECAY_COEF * distance * distance , MAX_ACCELERATION );
        direction.normalize();

        linear = direction * strength;
      }
    }
  }
}

void character::flee(vec3& target) {
  vec3 direction = position - target;
  std::dout << "Direccion = " << direction << " length = " << direction.get_length() << std::endl;
  GLfloat distance = direction.get_length();
  
  if ( distance < RADIUS ){
    velocity = linear = vec3();
    return;
  }
  
  GLfloat target_speed;
  if ( distance > SLOW_RADIUS ){
    target_speed = MAX_SPEED;
  }else{
    target_speed = MAX_SPEED * distance / SLOW_RADIUS;
  }
  
  std::dout << "target_speed = " << target_speed << std::endl;
  
  vec3 target_velocity = direction;
  target_velocity.normalize();
  target_velocity *= target_speed;
  
  std::dout << "target_velocity = " << target_velocity << std::endl;
  
  linear = target_velocity - velocity;
  linear /= TIME_TO_TARGET;
  
  if ( linear.get_length() > MAX_ACCELERATION ){
    linear.normalize();
    linear *= MAX_ACCELERATION;
  }
  
  std::dout << "linear = " << linear << std::endl;
}

void character::follow_the_path( path & the_path ) {
  int next_closest = the_path.get_closest_point( position , current_index_path );
  dout << "PUNTO ACTUAL = " << current_index_path << " next =  " << next_closest << endl;
  vec3 target = the_path.get_next_target( next_closest );

  /*glBegin(GL_QUADS); // Start drawing a quad primitive

    glVertex3f(target.x+-1.0f, target.y + -1.0f, Z_FLOOR); // The bottom left corner
    glVertex3f(target.x-1.0f, target.y+ 1.0f, Z_FLOOR); // The top left corner
    glVertex3f(target.x+1.0f, target.y+1.0f, Z_FLOOR); // The top right corner
    glVertex3f(target.x+1.0f, target.y+-1.0f, Z_FLOOR); // The bottom right corner

  glEnd();*/

  current_index_path = next_closest;
  vec3 direction = target - position;
  std::dout << "Direccion = " << direction << " length = " << direction.get_length() << std::endl;
  GLfloat distance = direction.get_length();
  
  if ( distance < RADIUS ){
    rotation = angular = 0.;
    velocity = linear = vec3();
    return;
  }
  
  GLfloat target_speed;
  if ( distance > SLOW_RADIUS ){
    target_speed = MAX_SPEED;
  }else{
    target_speed = MAX_SPEED * distance / SLOW_RADIUS;
  }
  
  std::dout << "target_speed = " << target_speed << std::endl;
  
  vec3 target_velocity = direction;
  target_velocity.normalize();
  target_velocity *= target_speed;
  
  std::dout << "target_velocity = " << target_velocity << std::endl;
  
  linear = target_velocity - velocity;
  linear /= TIME_TO_TARGET;
  
  //if ( linear.get_length() > MAX_ACCELERATION ){
    linear.normalize();
    linear *= MAX_ACCELERATION;
  //}
  
  std::dout << "linear = " << linear << std::endl;
  look_where_you_are_going();
}

void character::pursue( character & target ){
  vec3 direction = target.position - position;
  GLfloat distance = direction.get_length();
  
  GLfloat speed = velocity.get_length();
  
  GLfloat prediction;
  if ( speed <= distance / MAX_PREDICTION_TIME ){
    prediction = MAX_PREDICTION_TIME;
  }else{
    prediction = distance / speed;
  }
  
  vec3 pursue_position = target.position;
  pursue_position.add_scalar_from_vector( target.velocity , prediction );
  std::dout << " target name = " << target.picture_addr << std::endl;
  classic_arrive(pursue_position, target.position, true);
}

bool character::classic_arrive( vec3& target, vec3& current_object_position, bool reduce_speed){
  dout << " CLASSIC ARRIVE FROM = " << target << " to = " << current_object_position << endl;
  vec3 direction = target - position;
  std::dout << "Direccion = " << direction << " length = " << direction.get_length() << std::endl;
  GLfloat distance_to_target = direction.get_length();
  GLfloat distance_to_object = (current_object_position - position).get_length();
  
  std::dout << " Distance to object =  " << distance_to_object << std::endl;
  
  if ( distance_to_object < RADIUS ){
    if ( reduce_speed ){
    velocity = linear = vec3();
    rotation = angular = 0;
    }
    return false;
  }
  
  GLfloat target_speed;
  if ( distance_to_target > SLOW_RADIUS ){
    target_speed = MAX_SPEED;
  }else{
    target_speed = MAX_SPEED * distance_to_target / SLOW_RADIUS;
  }
  
  std::dout << "target_speed arrive = " << target_speed << std::endl;
  
  vec3 target_velocity = direction;
  target_velocity.normalize();
  target_velocity *= target_speed;
  
  std::dout << "target_velocity arrive = " << target_velocity << std::endl;
  
  linear = target_velocity - velocity;
  linear /= TIME_TO_TARGET;
  
  if ( linear.get_length() > MAX_ACCELERATION ){
    linear.normalize();
    linear *= MAX_ACCELERATION;
  }
  
  std::dout << "linear arrive = " << linear << std::endl;
  
  look_where_you_are_going();
  
  return true;
}

character& character::operator =(const character& b) {
  dout << "COPIANDO POSICION " << endl;
  position = b.position;
  orientation = b.orientation;
  velocity = b.velocity;
  rotation = b.rotation;
  linear = b.linear;
  angular = b.angular;
  picture_addr = b.picture_addr;
  current_index_path = b.current_index_path;
  texture = 0; // WAITING TO LOAD TEXTURE
  do_the_draw = b.do_the_draw;
  max_speed = b.max_speed;
  return *this;
}

void character::align(character& target) {
  GLfloat rotation_t = target.orientation - orientation;
  dout << "ALIGN MI TARGET ES " << target.orientation << " ACTUAL = " << orientation << endl;
  rotation_t = map_to_range( rotation_t );
  dout << "DIFERENCIA ES " << rotation_t << endl;
  GLfloat rotation_size = fabs(rotation_t);
  
  if ( rotation_size < RADIUS ){
    return;
  }
  
  GLfloat target_rotation;
  if ( rotation_size > SLOW_RADIUS ){
    target_rotation = MAX_ROTATION;
  }else{
    target_rotation = MAX_ROTATION * rotation_size / SLOW_RADIUS;  
  }
  
  target_rotation *= rotation_t / rotation_size;
  
  angular = target_rotation - rotation;
  
  angular /= TIME_TO_TARGET;
  
  GLfloat angular_acceleration = fabs(angular);
  if ( angular_acceleration > MAX_ANGULAR_ACCELERATION ){
    angular /= angular_acceleration;
    angular *= MAX_ANGULAR_ACCELERATION;
  }
  dout << "ALIGN = " << angular << " " << rotation << " " << orientation << " " << target.orientation << endl;
}

vec3 character::calculate_firing_solution( const vec3 & end  ){
  vec3 delta = position - end;
  delta = delta * (-1);
  vec3 gravity(.0f,.0f,GRAVITY);

  dout << "COMENZANDO EL FIRING SOL " << endl;
  dout << "[" << end.x << "," << end.y << "," << end.z << "]" << endl;
  dout << "[" << position.x << "," << position.y << "," << position.z << "]" << endl;

  GLfloat a = gravity * gravity;
  GLfloat b = (gravity * delta + MUZZLE * MUZZLE) * (-4);
  GLfloat c = delta * delta * 4;

  if ( 4 * a * c > b * b ) {
    dout << "NO PUEDE LLEGAR EL PROYECTIL!!" << endl;
    return vec3();
  }
  
  dout << "A = " << a << " B = " << b << " C = " << c << endl;
  dout << " TIME0 = " << ( -b + sqrt(b*b-4*a*c)) / (2 * a) << endl;
  dout << " TIME1 = " << ( -b - sqrt(b*b-4*a*c)) / (2 * a) << endl;
  GLfloat time0 = sqrt( ( -b + sqrt(b*b-4*a*c)) / (2 * a));
  GLfloat time1 = sqrt( ( -b - sqrt(b*b-4*a*c)) / (2 * a));

  GLfloat ttt = .0f;
  if ( time0 < 0 ) {
    if ( time1 < 0 ) {
      dout << "NO PUEDE LLEGAR EL PROYECTIL! AMBOS NEGATIVOS!" << endl;
      return vec3();
    }else{
      ttt = time1;
    }
  }else{
    if ( time1 < 0 ) {
      ttt = time0;
    }else{
      ttt = min( time0, time1 );
    }
  }

  return ( delta*2 - gravity * ttt * ttt ) / ( 2 * MUZZLE * ttt );
}

void character::redraw() {  
  glLoadIdentity( );
  glBindTexture( GL_TEXTURE_2D, texture );
  glColor3f(1.f,1.f,1.f);

  //dout << "DIBUJANDO RUBICK EN = " << position << endl;
  glTranslatef( position.x, position.y, position.z - 1 );
  glRotatef( orientation , 0.0f, 0.0f, 1.0f);

  glBegin( GL_QUADS );
    //glNormal3f( 0.0f, 0.0f, 1.0f );
    glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -1.0f, -1.0f ,  1.0f );
    glTexCoord2f( 0.0f, 0.0f ); glVertex3f(  1.0f, -1.0f ,  1.0f );
    glTexCoord2f( 0.0f, 1.0f ); glVertex3f(  1.0f,  1.0f ,  1.0f );
    glTexCoord2f( 1.0f, 1.0f ); glVertex3f( -1.0f ,  1.0f ,  1.0f );
  glEnd();
  
  if ( draw_life ){
    glLoadIdentity( );
    glBegin( GL_QUADS );
      glColor3f(0.f,1.f,0.f);
    //glNormal3f( 0.0f, 0.0f, 1.0f );
      glVertex3f( position.x-1.0f, position.y-1.0f ,  Z_FLOOR );
      glVertex3f( position.x+1.0f, position.y-1.0f ,  Z_FLOOR );
      glVertex3f( position.x+1.0f, position.y-.8f ,  Z_FLOOR );
      glVertex3f( position.x-1.0f , position.y-.8f ,  Z_FLOOR );
    glEnd();
    
    float dying = (100 - life)/100.0*2.0;
    glBegin( GL_QUADS );
      glColor3f(1.f,0.f,0.f);
    //glNormal3f( 0.0f, 0.0f, 1.0f );
      glVertex3f( position.x-1.0f+dying, position.y-1.0f ,  Z_FLOOR );
      glVertex3f( position.x+1.0f, position.y-1.0f ,  Z_FLOOR );
      glVertex3f( position.x+1.0f, position.y-.8f ,  Z_FLOOR );
      glVertex3f( position.x-1.0f+dying , position.y-.8f ,  Z_FLOOR );
    glEnd();
  }
}

character::character(vec3 p, GLfloat o, const std::string& pic, bool draw, GLfloat max_speed) {
  wander_orientation = .0f;
  this->position = p;
  this->orientation = o;
  current_index_path = 0;
  picture_addr = pic;
  do_the_draw = draw;
  angular = .0f;
  this->life = 100.0;
  this->draw_life = false;
  this->max_speed = max_speed;
  this->path_in_graph = vector<int>();
}

void character::do_wandering() {
  wander_orientation += randomBinomial() * WANDER_RATE;
  
  GLfloat target_orientation = wander_orientation;// + orientation;
  vec3 orientation_as_vector = vec3( cos( orientation / DEGREE_PI * PI ) , sin( orientation / DEGREE_PI * PI ) , 0 );
  
  vec3 target = position + orientation_as_vector * WANDER_OFFSET;
  target += vec3( cos( target_orientation / DEGREE_PI * PI ) , sin( target_orientation / DEGREE_PI * PI ) , 0 ) * WANDER_RADIUS;
  
  std::dout << " wander_orientation =  " << wander_orientation << std::endl;
  
  character dummy;
  dummy.orientation = wander_orientation;
  align(dummy);
  
  linear = vec3( cos( target_orientation / DEGREE_PI * PI ) , sin( target_orientation / DEGREE_PI * PI ) , 0 ) * MAX_ACCELERATION;
}

/* Tomado de NEHE.com. Tutorial de OpenGL + SDL.
 * 
 */
int character::LoadGLTextures( )
  {
    std::dout << "Loading texture for " << picture_addr << std::endl;
    
      /* Status indicator */
      int Status = FALSE;

      /* Create storage space for the texture */
      SDL_Surface *TextureImage[1]; 

      /* Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit */
      if ( ( TextureImage[0] = SDL_LoadBMP( picture_addr.c_str() ) ) )
          {

        /* Set the status to true */
        Status = TRUE;

        /* Create The Texture */
        glGenTextures( 1, &texture );

        /* Load in texture 1 */
        /* Typical Texture Generation Using Data From The Bitmap */
        glBindTexture( GL_TEXTURE_2D, texture );

        /* Generate The Texture */
        glTexImage2D( GL_TEXTURE_2D, 0, 3, TextureImage[0]->w,
          TextureImage[0]->h, 0, GL_BGR,
          GL_UNSIGNED_BYTE, TextureImage[0]->pixels );

        /* Nearest Filtering */
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
             GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
             GL_NEAREST );

          }

      /* Free up any memory we may have used */
      if ( TextureImage[0] )
        SDL_FreeSurface( TextureImage[0] );

      return Status;
  }

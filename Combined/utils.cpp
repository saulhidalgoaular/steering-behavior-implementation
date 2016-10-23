#include "utils.hpp"

GLfloat random_float(){
  return ((GLfloat)rand())/(RAND_MAX);
}

GLfloat randomBinomial(){
  return random_float() - random_float();
}

int random_int(int n){
  return rand() % n;
}

GLfloat min(GLfloat a , GLfloat b){
  if ( a < b ){
    return a;  
  }
  return b;
}

GLfloat map_to_range(GLfloat angle){
  if ( fabs(angle) < DEGREE_PI ){
    return angle;
  }
  angle += INF_SPINS * DEGREE_PI;
  angle = fmod( angle , 2 * DEGREE_PI );
  if ( angle < DEGREE_PI ){
    return angle;  
  }else{
    return -(2 * DEGREE_PI - angle);  
  }
}

// LA MAYOR PARTE DE ESTE CODIGO VIENE DE UN TEMPLATE DE NEHE.COM
// Se encarga de las funciones con SDL+GL;

/* function to reset our viewport after a window resize */
  int resizeWindow( int width, int height )
  {
      /* Height / width ration */
      GLfloat ratio;

      /* Protect against a divide by zero */
      if ( height == 0 )
    height = 1;

      ratio = ( GLfloat )width / ( GLfloat )height;

      /* Setup our viewport. */
      glViewport( 0, 0, ( GLint )width, ( GLint )height );

      /* change to the projection matrix and set our viewing volume. */
      glMatrixMode( GL_PROJECTION );
      glLoadIdentity( );

      /* Set our perspective */
      gluPerspective( 155.0f, ratio, 0.1f, 100.0f );

      /* Make sure we're chaning the model view and not the projection */
      glMatrixMode( GL_MODELVIEW );

      /* Reset The View */
      glLoadIdentity( );

      return( TRUE );
  }
  
  void Quit( int returnCode )
{
    /* clean up the window */
    SDL_Quit( );

    /* and exit appropriately */
    exit( returnCode );
}
  
int initGL()
{
    /* Enable Texture Mapping ( NEW ) */
    glEnable( GL_TEXTURE_2D );

    /* Enable smooth shading */
    glShadeModel( GL_SMOOTH );

    /* Set the background black */
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    /* Depth buffer setup */
    glClearDepth( 1.0f );

    /* Enables Depth Testing */
    glEnable( GL_DEPTH_TEST );

    /* The Type Of Depth Test To Do */
    glDepthFunc( GL_LEQUAL );

    /* Really Nice Perspective Calculations */
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    /* Enable Light One */
    glEnable( GL_LIGHT1 );

    return( TRUE );
}

// un numero al azar entre i y f
int myRand(int i, int f){
    return rand()%(f-i)+i;
}
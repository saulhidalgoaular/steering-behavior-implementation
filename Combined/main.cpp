#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include<cstdlib>
#include "SDL.h"
#include "vec3.hpp"
#include "config.hpp"
#include "character.h"
#include "path.h"
#include "graph_algorithms.hpp"

#include "node.h"
#include <iostream>
#include <queue>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

/* This is our SDL surface */
SDL_Surface *surface;

character fish( vec3(-20.f, -10.f, Z_FLOOR), 0.f, FISH_ADDR , true, MAX_SPEED);
character dusa( vec3(15.f, 0.f, Z_FLOOR), 0.f , MEDUSA_ADDR , false, MAX_SPEED);
character ball( vec3(15.f, 0.f, Z_FLOOR), 0.f , BALL_ADDR , false, MAX_SPEED);
character target( vec3(0.f, 0.f, Z_FLOOR), 0.f , TARGET_ADDR , false, MAX_SPEED);

vector<character> my_flock;
vector<node> graph;

path the_path("path.txt", .2f);

bool do_wandering = FALSE;
bool do_projectil = FALSE;

vector<int> path_in_graph;
int current_path_index;
bool reduce_speed = false;

vec3 color_blue = vec3(0.f, 0.f, 1.f);
vec3 color_green = vec3(0.f, 1.f, 0.f);

vector<character *> all_characters;

void do_all_updates(){
  dout << "ANGULAR AFTER = " << fish.angular << " " << fish.rotation << " " << fish.orientation << endl;
  for (int i = 0; i < (int)all_characters.size() ; i++) {
    character * c = all_characters[i];
    dout << all_characters[i]->picture_addr << "CHECANDO" << endl;
    if ( c->do_the_draw ){
      c->update( TIME_ELAPSED );
      c->redraw();
    }
  } 
}

int drawWithGraph(){
  glClearColor(.29f,.45f,.76f,1.f);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  if ( !path_in_graph.empty() ){
    bool go_arrive = fish.classic_arrive( graph[path_in_graph[current_path_index]].position, graph[path_in_graph[current_path_index]].position , current_path_index +1 == (int)path_in_graph.size());

    if ( !go_arrive && current_path_index + 1 < (int)path_in_graph.size() ){
      ++current_path_index;
    }
  }
  
  do_all_updates();
    
  SDL_GL_SwapBuffers( );
  
  return( TRUE );
}

int draw(){  
  dout << "SIZE FLOCK " << my_flock.size() << endl;
  glClearColor(.29f,.45f,.76f,1.f);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
 
  if ( do_projectil ){ 
    do_projectil = !throw_projectil( dusa ,  target.position , ball );
  }

  if ( ball.position.z <= Z_FLOOR - EPS  ){
    ball.do_the_draw = false;
  }

  if ( ball.do_the_draw ){
    dout << "POSITION FISH PATH " << fish.position.y << endl;
    character dummy_fish_path = fish;
    character dummy_fish_wander = fish;

    dout << "POSITION FISH PATH " << dummy_fish_path.position.y << endl;
    dummy_fish_path.follow_the_path( the_path );
    dummy_fish_wander.flee(ball.position);

    fish.linear = (dummy_fish_path.linear*.1f) + (dummy_fish_wander.linear * 1.f);
    //dout << "ANGULAR LINEAR Z = " << dummy_fish_path.linear.z << " " << dummy_fish_wander.linear.z << " " << fish.linear.z << endl;
    dout << "ANGULAR LINEAR Z = " << dummy_fish_path.angular << endl;
    fish.angular = (dummy_fish_path.angular*.1) + (dummy_fish_wander.angular * 1.f);

    GLfloat angular_acceleration = fabs(fish.angular);
    if ( angular_acceleration > MAX_ANGULAR_ACCELERATION ){
      fish.angular /= angular_acceleration;
      fish.angular *= MAX_ANGULAR_ACCELERATION;
    }

    if ( fish.linear.get_length() > MAX_ACCELERATION ){
      fish.linear.normalize();
      fish.linear *= MAX_ACCELERATION;
    }
    fish.current_index_path = dummy_fish_path.current_index_path;
    //fish.rotation = dummy_fish_path.rotation;
  }else{
    fish.follow_the_path( the_path );
  }

  for (int i = 0; i < (int)my_flock.size(); i++) {
    character dummy_flock_pursue = my_flock[i];
    character dummy_flock_separation = my_flock[i];

    dummy_flock_pursue.pursue( fish );
    dummy_flock_separation.separation( my_flock , i );

    my_flock[i].linear = ( dummy_flock_pursue.linear*1 ) + ( dummy_flock_separation.linear * 10.f)  ;
    my_flock[i].angular = ( dummy_flock_pursue.angular*1 );// + ( dummy_flock_separation.angular * 1.f)  ;
    
    GLfloat angular_acceleration = fabs(my_flock[i].angular);
    if ( angular_acceleration > MAX_ANGULAR_ACCELERATION ){
      my_flock[i].angular /= angular_acceleration;
      my_flock[i].angular *= MAX_ANGULAR_ACCELERATION;
    }

    if ( my_flock[i].linear.get_length() > MAX_ACCELERATION ){
      my_flock[i].linear.normalize();
      my_flock[i].linear *= MAX_ACCELERATION;
    }

    //my_flock[i].pursue( fish );
    //dout << "ANGULAR PURSUE " << my_flock[0].angular << endl;
    if ( i == 0 ){
      dout << "ANGULAR PURSUE " << my_flock[0].angular << " ORIENTATION " << my_flock[0].orientation << endl;
    }
  }

  do_all_updates();

  the_path.draw();
    
  SDL_GL_SwapBuffers( );
  
  return( TRUE );
}

void loadAllTextures(){
  for (int i = 0; i < (int)all_characters.size() ; i++) {
    character * c = all_characters[i];
    dout << "SIZE FLOCK " << my_flock.size() << endl;
    c->LoadGLTextures();
  }
}

int main( int argc, char **argv )
{
    srand( unsigned(time(NULL)));
    bool showFlock = 0;
    
    graph.push_back( node( graph.size(), vec3(-20.f, -10.f, Z_FLOOR), color_blue, 1.f ) );
    graph.push_back( node( graph.size(), vec3(5.f, -10.f, Z_FLOOR), color_blue, 1.f ) );
    graph.push_back( node( graph.size(), vec3(-7.f, -7.f, Z_FLOOR), color_blue, 1.f ) );
    graph.push_back( node( graph.size(), vec3(18.f, -2.f, Z_FLOOR), color_blue, 1.f ) );
    graph.push_back( node( graph.size(), vec3(-8.f, 3.f, Z_FLOOR), color_blue, 1.f ) );
    graph.push_back( node( graph.size(), vec3(5.f, 3.f, Z_FLOOR), color_blue, 1.f ) );
    graph.push_back( node( graph.size(), vec3(10.f, 8.f, Z_FLOOR), color_blue, 1.f) );
    graph.push_back( node( graph.size(), vec3(-17.f, 8.f, Z_FLOOR), color_blue, 1.f ) );
    graph.push_back( node( graph.size(), vec3(-1.f, 10.f, Z_FLOOR), color_blue, 1.f ) );
    graph.push_back( node( graph.size(), vec3(18.f, 10.f, Z_FLOOR), color_blue, 1.f ) );
    
    join( graph, 0, 1 , all_characters);
    join( graph, 0, 2 , all_characters);
    join( graph, 0, 7 , all_characters);
    join( graph, 0, 4 , all_characters);
    join( graph, 4, 7 , all_characters);
    join( graph, 2, 4 , all_characters);
    join( graph, 2, 3 , all_characters);
    join( graph, 2, 5 , all_characters);
    join( graph, 1, 3 , all_characters);
    join( graph, 3, 6 , all_characters);
    join( graph, 5, 6 , all_characters);
    join( graph, 7, 8 , all_characters);
    join( graph, 4, 8 , all_characters);
    join( graph, 8, 9 , all_characters);
    join( graph, 4, 9 , all_characters);
    join( graph, 3, 9 , all_characters);
    join( graph, 4, 5 , all_characters);
    join( graph, 5, 3 , all_characters);
    
    for (int i = 0; i < (int)graph.size(); i++) {
      all_characters.push_back( &graph[i] );
    }

    GLfloat percent_speed = .8f;
    my_flock.push_back( character(vec3(-19.f, -9.f, Z_FLOOR), 0.f, LOBSTER_ADDR , showFlock, MAX_SPEED*percent_speed)  );
    my_flock.push_back( character(vec3(-19.f, -11.f, Z_FLOOR), 0.f, LOBSTER_ADDR , showFlock, MAX_SPEED*percent_speed)  );
    my_flock.push_back( character(vec3(-19.f, -7.f, Z_FLOOR), 0.f, LOBSTER_ADDR , showFlock, MAX_SPEED*percent_speed)  );
    my_flock.push_back( character(vec3(-16.f, -9.f, Z_FLOOR), 0.f, LOBSTER_ADDR , showFlock, MAX_SPEED*percent_speed)  );
    my_flock.push_back( character(vec3(-21.f, -9.f, Z_FLOOR), 0.f, LOBSTER_ADDR , showFlock, MAX_SPEED*percent_speed)  );

    dout << "SIZE FLOCK " << my_flock.size() << endl;

    all_characters.push_back( &fish );
    all_characters.push_back( &dusa );
    all_characters.push_back( &ball );
    all_characters.push_back( &target );

    for (int i = 0; i < (int)my_flock.size(); i++) {
      all_characters.push_back( &my_flock[i] );
    }
  
    dijkstra(graph, 0, 9, path_in_graph, current_path_index);
    //dijkstra(graph, 3, 0);
    //a_star(graph, 0, 9);
    
    dout << "SIZE FLOCK " << my_flock.size() << endl;
    /* Flags to pass to SDL_SetVideoMode */
    int videoFlags;
    /* main loop variable */
    int done = FALSE;
    /* used to collect events */
    SDL_Event event;
    /* this holds some info about our display */
    const SDL_VideoInfo *videoInfo;
    /* whether or not the window is active */
    int isActive = TRUE;

    /* initialize SDL */
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
	    fprintf( stderr, "Video initialization failed: %s\n",
		     SDL_GetError( ) );
	    Quit( 1 );
	}

    /* Fetch the video info */
    videoInfo = SDL_GetVideoInfo( );

    if ( !videoInfo )
	{
	    fprintf( stderr, "Video query failed: %s\n",
		     SDL_GetError( ) );
	    Quit( 1 );
	}

    /* the flags to pass to SDL_SetVideoMode */
    videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
    videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
    videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
    videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */

    /* This checks to see if surfaces can be stored in memory */
    if ( videoInfo->hw_available )
	videoFlags |= SDL_HWSURFACE;
    else
	videoFlags |= SDL_SWSURFACE;

    /* This checks if hardware blits can be done */
    if ( videoInfo->blit_hw )
	videoFlags |= SDL_HWACCEL;

    /* Sets up OpenGL double buffering */
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    /* get a SDL surface */
    surface = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
				videoFlags );

    /* Verify there is a surface */
    if ( !surface )
	{
	    fprintf( stderr,  "Video mode set failed: %s\n", SDL_GetError( ) );
	    Quit( 1 );
	}

    /* initialize OpenGL */
    initGL( );
    
    loadAllTextures();

    bool temp1 = 0;
    dout << "SIZE FLOCK " << my_flock.size() << endl;
    /* resize the initial window */
    resizeWindow( SCREEN_WIDTH, SCREEN_HEIGHT );

    /* wait for events */
    while ( !done )
	{
	   /* handle the events in the queue */
      
	    while ( SDL_PollEvent( &event ) )
		{
		    switch( event.type )
			{
			case SDL_ACTIVEEVENT:
			    /* Something's happend with our focus
			     * If we lost focus or we are iconified, we
			     * shouldn't draw the screen
			     */
			    if ( event.active.gain == 0 )
				isActive = FALSE;
			    else
				isActive = TRUE;
			    break;			    
			case SDL_VIDEORESIZE:
			    /* handle resize event */
			    surface = SDL_SetVideoMode( event.resize.w,
							event.resize.h,
							16, videoFlags );
			    if ( !surface )
				{
				    fprintf( stderr, "Could not get a surface after resize: %s\n", SDL_GetError( ) );
				    Quit( 1 );
				}
			    resizeWindow( event.resize.w, event.resize.h );
			    break;
			case SDL_KEYDOWN:
			    /* handle key presses */
			    handleKeyPress( &event.key.keysym , target, do_wandering, do_projectil, surface, temp1, temp1);
			    break;
			case SDL_QUIT:
			    /* handle quit requests */
			    done = 1;
			    break;
			default:
			    break;
			}
		}

	    /* draw the scene */
	    if ( isActive )
		drawWithGraph( );
	}

    /* clean ourselves up and exit */
    Quit( 0 );

    /* Should never get here */
    return( 0 );
}

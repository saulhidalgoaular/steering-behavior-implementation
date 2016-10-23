#include <cstdlib>
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
#include <map>

using namespace std;

/* This is our SDL surface */
SDL_Surface *surface;
bool do_projectil;
bool do_wandering;

vector< node > graph;
vec3 color_blue = vec3(0.f, 0.f, 1.f);
vector<character *> all_characters;
vector<character *> edges;
vector<state> fsm;
map<string, int(*)(character*, character*, vector<node> * graph) > function_map;
map<string, character*> character_map;
state * current_state;
queue< int (*)(character*, character*, vector<node> * graph) > actions_queue;

//vector<int> path_in_graph;
//int current_path_index;

bool draw_characteristic_points = 0;
bool draw_area = 0;
bool draw_edges = 0;

//character fish( vec3(-18.f, -8.f, Z_FLOOR), 0.f, FISH_ADDR , true, MAX_SPEED);
//character fish( vec3(-18.f, -8.f, Z_FLOOR), 0.f, FISH_ADDR , true, MAX_SPEED);
character fish( vec3(-18.f, -8.f, Z_FLOOR), 0.f, FISH_ADDR , true, MAX_SPEED);
character medusa( vec3(18.f, 8.f, Z_FLOOR), 0.f, MEDUSA_ADDR , true, MAX_SPEED);
node target1(-1, vec3(-18.f, 8.f, Z_FLOOR), vec3(0.f, 0.f, 0.f), 1);
node target2(-1, vec3(0.f, 8.f, Z_FLOOR), vec3(0.f, 0.f, 0.f), 1);
character food1( vec3(-18.f, 8.f, Z_FLOOR), 0.f, LOBSTER_ADDR , true, MAX_SPEED/2.0);
character food2( vec3(0.f, 8.f, Z_FLOOR), 0.f, LOBSTER_ADDR , true, MAX_SPEED/2.0);
//character target1 ( vec3(-18.f, 8.f, Z_FLOOR), 0.f, "TARGET1" , false, MAX_SPEED);
//character target2 ( vec3(0.f, 8.f, Z_FLOOR), 0.f, "TARGET2" , false, MAX_SPEED);
character exit_door( vec3(-14.f, -8.f, Z_FLOOR), 0.f, TARGET_ADDR , true, MAX_SPEED);
character home( vec3(-18.f, -8.f, Z_FLOOR), 0.f, BALL_ADDR , true, MAX_SPEED);
//character medusa( vec3(-11.f, -7.f, Z_FLOOR), 0.f, MEDUSA_ADDR , true, MAX_SPEED);


character* everyone[] = {&food1, &food2, &medusa};

void do_all_updates(){
  glEnable(GL_TEXTURE_2D);
  for (int i = 0; i < (int)all_characters.size() ; i++) {
    character * c = all_characters[i];
    if ( c->do_the_draw ){
      c->update( TIME_ELAPSED );
      c->redraw();
    }
  } 
  
  if ( draw_edges ){
    for (int i = 0; i < (int)edges.size() ; i++) {
      character * c = edges[i];
      if ( c->do_the_draw ){
        c->update( TIME_ELAPSED );
        c->redraw();
      }
    } 
  }
  
  //target1.redraw();
  //target2.redraw();
  glDisable(GL_TEXTURE_2D);
}

void draw_graph(){
  if ( draw_characteristic_points ){
    for ( int i = 0 ; i < (int)graph.size() ; i++ ){
      graph[i].redraw();
    }
  }
//  if ( draw_area ){
//    for ( GLfloat ix = -20.f ; ix <= 20.f ; ix += 0.2 ){
//      for ( GLfloat iy = -10.f ; iy <= 10.f ; iy += 0.2 ){
//        int current_index = get_index_quantization( graph, ix, iy );
//        if ( current_index >= 0 ){
//
//          glLoadIdentity( );
//
//          vec3 & current_color = graph[current_index].color;
//          glColor3f( current_color.x, current_color.y, current_color.z );
//
//          glBegin(GL_QUADS);
//
//          glVertex3f(ix-.15,iy-.15, Z_FLOOR);
//          glVertex3f(ix-.15,iy+.15, Z_FLOOR);
//          glVertex3f(ix+.15,iy+.15, Z_FLOOR);
//          glVertex3f(ix+.15,iy-.15, Z_FLOOR);
//
//          glEnd();
//        }
//      }
//    }
//  }
}

void draw_walls(){
  glDisable(GL_TEXTURE_2D);
  glLoadIdentity( );
  
  glColor3f(1.f,1.f,0.f);
  //1
  glBegin(GL_QUADS);
  
  glVertex3f(-20,-10, Z_FLOOR);
  glVertex3f(20,-10, Z_FLOOR);
  glVertex3f(20,-9, Z_FLOOR);
  glVertex3f(-20,-9, Z_FLOOR);
  
  glEnd();
  //2
  glBegin(GL_QUADS);
  
  glVertex3f(20,-10, Z_FLOOR);
  glVertex3f(20,10, Z_FLOOR);
  glVertex3f(19,10, Z_FLOOR);
  glVertex3f(19,-10, Z_FLOOR);
  
  glEnd();
  
  //3
  glBegin(GL_QUADS);
  
  glVertex3f(-20,10, Z_FLOOR);
  glVertex3f(20,10, Z_FLOOR);
  glVertex3f(20,9, Z_FLOOR);
  glVertex3f(-20,9, Z_FLOOR);
  
  glEnd();
  
  //4
  glBegin(GL_QUADS);
  
  glVertex3f(-20,-10, Z_FLOOR);
  glVertex3f(-20,10, Z_FLOOR);
  glVertex3f(-19,10, Z_FLOOR);
  glVertex3f(-19,-10, Z_FLOOR);
  
  glEnd();
  
  //5
  glBegin(GL_QUADS);
  
  glVertex3f(-17,7, Z_FLOOR);
  glVertex3f(-16,7, Z_FLOOR);
  glVertex3f(-16,-6, Z_FLOOR);
  glVertex3f(-17,-6, Z_FLOOR);
  
  glEnd();
  
  //6
  glBegin(GL_QUADS);
  
  glVertex3f(-17,7, Z_FLOOR);
  glVertex3f(-9,7, Z_FLOOR);
  glVertex3f(-9,6, Z_FLOOR);
  glVertex3f(-17,6, Z_FLOOR);
  
  glEnd();
  
  //6.1
  glBegin(GL_QUADS);
  
  glVertex3f(2,7, Z_FLOOR);
  glVertex3f(9,7, Z_FLOOR);
  glVertex3f(9,6, Z_FLOOR);
  glVertex3f(2,6, Z_FLOOR);
  
  glEnd();
  
  
  //6.2
  glBegin(GL_QUADS);
  
  glVertex3f(-6,7, Z_FLOOR);
  glVertex3f(-2,7, Z_FLOOR);
  glVertex3f(-2,6, Z_FLOOR);
  glVertex3f(-6,6, Z_FLOOR);
  
  glEnd();
  
  //7
  glBegin(GL_QUADS);
  
  glVertex3f(15,10, Z_FLOOR);
  glVertex3f(15,-2, Z_FLOOR);
  glVertex3f(14,-2, Z_FLOOR);
  glVertex3f(14,10, Z_FLOOR);
  
  glEnd();
  //8
  glBegin(GL_QUADS);
  
  glVertex3f(8,-10, Z_FLOOR);
  glVertex3f(8,2, Z_FLOOR);
  glVertex3f(7,2, Z_FLOOR);
  glVertex3f(7,-10, Z_FLOOR);
  
  glEnd();
  
  //9
  glBegin(GL_QUADS);
  
  glVertex3f(0,0, Z_FLOOR);
  glVertex3f(0,-5, Z_FLOOR);
  glVertex3f(-7,-5, Z_FLOOR);
  glVertex3f(-7,0, Z_FLOOR);
  
  glEnd();
  
  
  
  //10
  glBegin(GL_QUADS);
  
  glVertex3f(-13,3, Z_FLOOR);
  glVertex3f(-2,3, Z_FLOOR);
  glVertex3f(-2,2, Z_FLOOR);
  glVertex3f(-13,2, Z_FLOOR);
  
  glEnd();
  
  //11
  glBegin(GL_QUADS);
  
  glVertex3f(-13,3, Z_FLOOR);
  glVertex3f(-12,3, Z_FLOOR);
  glVertex3f(-12,-6, Z_FLOOR);
  glVertex3f(-13,-6, Z_FLOOR);
  
  glEnd();
  
  
  //12
  glBegin(GL_QUADS);
  
  glVertex3f(5,-6, Z_FLOOR);
  glVertex3f(5,2, Z_FLOOR);
  glVertex3f(4,2, Z_FLOOR);
  glVertex3f(4,-6, Z_FLOOR);
  
  glEnd();
  
  
  //13
  glBegin(GL_QUADS);
  
  glVertex3f(11,-6, Z_FLOOR);
  glVertex3f(16,-6, Z_FLOOR);
  glVertex3f(16,-5, Z_FLOOR);
  glVertex3f(11,-5, Z_FLOOR);
  
  glEnd();
}

void do_logic_fsm(){
  if ( !actions_queue.empty() ){
    
    for (int i = 0; i < (int)current_state->succ.size(); i++) {
      if ( current_state->succ[i]->trigger(current_state->me, current_state->target, current_state->graph) ){
        actions_queue.pop();
        // exit
        int (* current_action )(character*, character*, vector<node> * graph) = actions_queue.front();
        current_action(current_state->me, current_state->target, current_state->graph);
        
        actions_queue.pop(); // pop Exit
        assert( actions_queue.empty() );
        
        current_state = current_state->succ[i]->to;
        actions_queue.push( current_state->entryAction );
        actions_queue.push( current_state->action );
        actions_queue.push( current_state->exitAction );
        
        
      }
    }
    
    int (* current_action )(character*, character*, vector<node> * graph) = actions_queue.front();
    int ans = current_action(current_state->me, current_state->target, current_state->graph);

    if ( !ans ){
      actions_queue.pop();
    }
  }
  int value = fsm_seek(&food1, &target1, current_state->graph);
  if ( !value ){
    int random = random_int(graph.size());
    target1.position = graph[random].position;
    fsm_exit_seek(&food1, &target1, current_state->graph);
    fsm_entry_seek(&food1, &target1, current_state->graph);
  }
  
  value = fsm_seek(&food2, &target2, current_state->graph);
  if ( !value ){
    int random = random_int(graph.size());
    target2.position = graph[random].position;
    fsm_exit_seek(&food2, &target2, current_state->graph);
    fsm_entry_seek(&food2, &target2, current_state->graph);
  }
}

bool valid_in_time(vector<int> path, double max_distance, vector<node> & graph, character* everyone[], vector<int> &all){
  if ( path.empty() ){
    return 1;
  }
  all.clear();
  vector<int> tmp_path;
  int index_path;
  
  int from = get_index_quantization(graph, fish.position.x, fish.position.y);
  int to = get_index_quantization(graph, everyone[path[0]]->position.x, everyone[path[0]]->position.y);

  a_star(graph, from, to, tmp_path, index_path);
  
  all.insert( all.end(), tmp_path.begin(), tmp_path.end() );
  
  for (int i = 1; i < (int)path.size(); i++) {
    from = get_index_quantization(graph, everyone[path[i-1]]->position.x, everyone[path[i-1]]->position.y);
    to = get_index_quantization(graph, everyone[path[i]]->position.x, everyone[path[i]]->position.y);
    a_star(graph, from, to, tmp_path, index_path);
  
    all.insert( all.end(), tmp_path.begin(), tmp_path.end() );
  }

  
  to = get_index_quantization(graph, fish.position.x, fish.position.y);
  from = get_index_quantization(graph, everyone[path[path.size()-1]]->position.x, everyone[path[path.size()-1]]->position.y);
  
  a_star(graph, from, to, tmp_path, index_path);
  
  all.insert( all.end(), tmp_path.begin(), tmp_path.end() );
  
  double total = 0;
  for (int i = 1; i < (int)all.size(); i++) {
    total += graph[all[i-1]].get_euclid_dist(&graph[all[i]]);
  }

  return total < max_distance;
}

void simulating_annealing(){
  // OBJETIVO:
  // COMERSE LA MAYOR CANTIDAD DE LANGOSTAS O MEDUSAS
  // y vuelve a casa ANTES DE QUE SE ACABE EL TIEMPO.
  
  /// Creamos un camino inicial.
  
  vector<int> current_path;
  
  vector<int> best_path;
  
  double temperature = 100;
  
  // Actualmente se hace en base a la distancia
  // Pero se puede hacer en funcion a la vida, si se haya una funcion
  // desde vida hasta distancia,
  double max_distance = 100;
  
  // 0 = food1
  // 1 = food2
  // 2 = medusa
  
  vector<int> all;
  
  while ( temperature >= 1 ){
    int rand = myRand(0,3);
    if ( find(current_path.begin(), current_path.end(), rand) == current_path.end() ){
      // Agregar.
      
      int index_to_insert = myRand(0,current_path.size()+1);
      vector<int> copy = current_path;
      copy.insert( copy.begin() + index_to_insert, rand);
      
      if ( valid_in_time(copy, max_distance, graph, everyone, all) ){
        current_path = copy;
      }
    }else{
      float p = random_float();
      
      if ( p < exp( ((double)-1)/temperature ) ){
        current_path.erase( find(current_path.begin(), current_path.end(), rand) );
      }
    }
    
    if ( current_path.size() > best_path.size() ){
      best_path = current_path;
    }
    
    temperature *= 0.999;
  }
  
  valid_in_time(best_path, max_distance, graph, everyone, all);
  fish.path_in_graph = all;
  fish.current_path_index = 0;
}


void do_logic_simple_simulated_annealing(){
  if ( fish.current_path_index < (int)(fish.path_in_graph.size()) ){
    fish.classic_arrive( graph[fish.path_in_graph[fish.current_path_index]].position, graph[fish.path_in_graph[fish.current_path_index]].position , false);

    int current_node = get_index_quantization(graph, fish.position.x, fish.position.y);
    if ( current_node == fish.path_in_graph[fish.current_path_index] ){
      ++(fish.current_path_index);
    }
  }else if ( !fish.path_in_graph.empty() ){
    fish.classic_arrive( home.position, home.position , true);
  }
  if ( fish.life > 0 ){
    fish.life -= LIFE_DECREASE;
  }
}

bool draw(){
  glClearColor(.29f,.45f,.76f,1.f);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
  draw_walls();
  draw_graph();
  
  //do_logic_fsm();
  do_logic_simple_simulated_annealing();

  do_all_updates();
    
  SDL_GL_SwapBuffers( );
  
  return( TRUE );
}

int main(int argc, char** argv) {
  
  fish.draw_life = true;
  
  {
    FILE *f = fopen("points.txt", "r+");

    assert(f);
    GLfloat a,b,c,d;
    while( fscanf(f,"%f,%f,%f,%f\n", &a, &b, &c, &d ) > 0 ){
      vec3 pos = vec3(a, b, c);
      fscanf(f,"%f , %f , %f", &a, &b, &c );
      vec3 cur_color = vec3( a, b, c );
      graph.push_back( node( graph.size(), pos, cur_color, d ) );
      
    }
  }  
  
  {
    FILE *f = fopen("joins.txt", "r+");

    assert(f);
    int a,b;
    while( fscanf(f,"%d,%d\n", &a, &b ) > 0 ){
      join(graph, a, b, edges);
      join(graph, b, a, edges);
    }
    fclose(f);
  }  
  
  character_map["graph"] = (character*)&graph;
  character_map["fish"] = &fish;
  character_map["dusa"] = &medusa;
  character_map["food1"] = &food1;
  character_map["food2"] = &food2;
  character_map["exit_door"] = &exit_door;
  character_map["home"] = &home;
  function_map["fsm_entry_seek"] = &fsm_entry_seek;
  function_map["fsm_seek"] = &fsm_seek;
  function_map["skip"] = &skip;
  function_map["fedEnough"] = &fedEnough;
  function_map["notFedEnough"] = &notFedEnough;
  function_map["fsm_exit_seek"] = &fsm_exit_seek;
  function_map["go_to_feed"] = &go_to_feed;
  function_map["target_reached"] = &target_reached;
  
  
  {
    loadFSM(fsm, string("fsm.txt"), function_map, character_map );
    current_state = &fsm[0];
    actions_queue.push( current_state->entryAction );
    actions_queue.push( current_state->action );
    actions_queue.push( current_state->exitAction );
    
  }
  
  all_characters.push_back(&fish);
  all_characters.push_back(&medusa);
  all_characters.push_back(&food1);
  all_characters.push_back(&food2);
  all_characters.push_back(&exit_door);
  all_characters.push_back(&home);
    
  
  simulating_annealing();
    
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
    
    loadAllTextures(all_characters);

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
			    handleKeyPress( &event.key.keysym , medusa, do_wandering, do_projectil, surface, draw_area, draw_characteristic_points, draw_edges);
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
        draw( );
	}

  /* clean ourselves up and exit */
  Quit( 0 );

  /* Should never get here */
  return( 0 );
}


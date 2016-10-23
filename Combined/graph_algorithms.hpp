#include <vector>
#include <queue>
#include "node.h"
#include <list>
#include <algorithm>
#include "state.h"
#include <map>


#ifndef GRAPH_ALGORITMS_HPP
#define	GRAPH_ALGORITMS_HPP

using namespace std;

void dijkstra(vector<node> & graph, int from, int to,vector<int> &path_in_graph,int &current_path_index);
void a_star(vector<node> & graph, int from, int to,vector<int> &path_in_graph,int &current_path_index);
bool throw_projectil( character & from , vec3 &to , character & who);
void handleKeyPress( SDL_keysym *keysym , character & my_character, bool &do_wandering, bool &do_projectil, SDL_Surface *surface, bool &draw_area, bool &draw_characteristic_points, bool & draw_edges);
int get_index_quantization( vector< node > & all , GLfloat x , GLfloat y );
void join( vector<node>& graph, int from, int to, vector< character*> & all_characters );
void loadAllTextures(vector< character*> &all_characters);
void loadFSM(vector<state> & s, string file_name, map<string, int(*)(character*, character*, vector<node> * graph) > & function_map, map<string, character*> & character_map);
void check_search( character * a, character * b, vector<node> & graph );
int fsm_seek(character* from , character * to, vector<node> * graph);
int fsm_entry_seek(character* from , character * to, vector<node> * graph);
int skip(character* from , character * to, vector<node> * graph);
int fedEnough(character* from , character * to, vector<node> * graph);
int notFedEnough(character* from , character * to, vector<node> * graph);
int fsm_exit_seek(character* from , character * to, vector<node> * graph);
int go_to_feed(character* from , character * to, vector<node> * graph);
int target_reached(character* from , character * to, vector<node> * graph);

#endif	/* GRAPH_ALGORITMS_HPP */


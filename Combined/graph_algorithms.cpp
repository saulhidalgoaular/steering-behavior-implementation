#include "graph_algorithms.hpp"

struct compare_operator{
  bool operator()(pair<GLfloat, int> a, pair<GLfloat, int> b){
    return a.first > b.first;
  }
};
 
/**
 * Dijkstra. Implementacion clasica. Deja el camino a seguir en la variable path_in_graph.
 * @param graph Grafo.
 * @param from Punto de partida
 * @param to Punto objetivo.
 */

void dijkstra(vector<node> & graph, int from, int to, vector<int> &path_in_graph, int &current_path_index){
  vector<GLfloat> best_distance(graph.size(), INF_GLFLOAT);
  vector<int> previous( graph.size(), -1 );
  
  best_distance[from] = 0.f;
  
  priority_queue< pair<GLfloat, int>, std::vector< pair<GLfloat, int> >, compare_operator > q;
  
  q.push( make_pair( 0.f, from ) );
  
  while (!q.empty()) {
    pair<GLfloat, int> current = q.top();
    dout << "Dijkstra EXPANDE " << current.second << endl;
    q.pop();
    
    if ( current.second == to ){
      break;
    }
    
    vector<edge*> & succ = graph[ current.second ].succ;
    for ( int i = 0 ; i < (int)succ.size() ; i++ ){
      GLfloat new_cost = current.first + succ[ i ]->cost;
      if ( new_cost < best_distance[ succ[i]->end->id ] ){
        best_distance[ succ[i]->end->id ] = new_cost;
        previous[ succ[i]->end->id ] = current.second;
        q.push( make_pair( new_cost, succ[i]->end->id ) );
      }
    }
  }
  
  if ( best_distance[ to ] >= INF_GLFLOAT ){
    path_in_graph.clear();
    return;
  }
  //assert( best_distance[ to ] < INF_GLFLOAT );
  
  path_in_graph.clear();
  int current_to_follow = to;
  while( current_to_follow != -1 ){
    path_in_graph.push_back( current_to_follow );
    current_to_follow = previous[current_to_follow];
  }
  
  reverse(path_in_graph.begin(), path_in_graph.end());
  current_path_index = 0;
  dout << "FIN" << endl;
}

/**
 * A* Basada en el pseudocodigo de wikipedia. Utiliza como heuristica la distancia
 * euclidiana. Deja el camino a seguir en la variable path_in_graph.
 * @param graph Grafo.
 * @param from Punto de partida
 * @param to Punto de salida.
 */
void a_star(vector<node> & graph, int from, int to, vector<int> &path_in_graph, int &current_path_index){
  
  vector<int> previous( graph.size(), -1 );
  vector<GLfloat> best_distance(graph.size(), INF_GLFLOAT);
  vector<GLfloat> estimated_cost(graph.size(), INF_GLFLOAT);
  
  list<int> open;
  open.push_back(from);
  list<int> closed;
  
  best_distance[from] = 0;
  estimated_cost[from] = graph[from].get_euclid_dist( &graph[to] );
  
  while( !open.empty() ){
    list<int>::iterator it;
    int current = -1;
    
    for ( it = open.begin() ; it != open.end() ; ++it ){
      if ( current == -1 || ( estimated_cost[*it] < estimated_cost[current] ) ){
        current = *it;
      }
    }
    dout << "A* EXPANDE " << current << endl;
    
    if ( current == to ){
      break;
    }
    
    open.remove(current);
    closed.push_back(current);
    
    vector<edge*> & succ = graph[ current ].succ;
    
    for ( int i = 0 ; i < (int)succ.size() ; i++ ){
      int neighbor = succ[i]->end->id;
      if ( find(closed.begin(), closed.end(), neighbor) != closed.end() ){
        continue;
      }
      
      GLfloat current_cost = best_distance[current] + succ[i]->cost;
      
      if ( find(open.begin(), open.end(), neighbor) == open.end() ||
             current_cost < best_distance[neighbor] ){
        previous[ neighbor ] = current;
        best_distance[ neighbor ] = current_cost;
        estimated_cost[ neighbor ] = best_distance[ neighbor ] + graph[neighbor].get_euclid_dist( &graph[to] );
        
        if ( find(open.begin(), open.end(), neighbor) == open.end() ){
          open.push_back( neighbor );
        }
      }
    }
  }
  
  
  if ( best_distance[ to ] >= INF_GLFLOAT ){
    path_in_graph.clear();
    return;
  }
  //assert( best_distance[ to ] < INF_GLFLOAT );
  
  path_in_graph.clear();
  int current_to_follow = to;
  while( current_to_follow != -1 ){
    path_in_graph.push_back( current_to_follow );
    current_to_follow = previous[current_to_follow];
  }
  
  reverse(path_in_graph.begin(), path_in_graph.end());
  current_path_index = 0;
  
  dout << "FIN" << endl;
}


bool throw_projectil( character & from , vec3 &to , character & who){
  vec3 ans = from.calculate_firing_solution( to );
  if ( ans.get_length() > EPS ){
    who.position = from.position;
    who.velocity = ans * MUZZLE;
    who.linear = vec3(.0f,.0f,GRAVITY);
    who.do_the_draw = true;
    return 1;
  }
  return 0;
}

/* function to handle key press events */
void handleKeyPress( SDL_keysym *keysym , character & my_character, bool &do_wandering, bool &do_projectil, SDL_Surface *surface, bool &draw_area, bool &draw_characteristic_points, bool & draw_edges)
{
    switch ( keysym->sym )
	{
	case SDLK_ESCAPE:
	    /* ESC key was pressed */
	    Quit( 0 );
	    break;
	case SDLK_UP:
	    /* Up arrow key was pressed
	     * this affects the x rotation
	     */
        my_character.velocity.y += 0.5f;
	    break;
	case SDLK_w:
	    /* Up arrow key was pressed
	     * this affects the x rotation
	     */
        do_wandering = !do_wandering;
	    break;
  case SDLK_f:
	    /* Up arrow key was pressed
	     * this affects the x rotation
	     */
        do_projectil = 1;
	    break;

	case SDLK_DOWN:
	    /* Down arrow key was pressed
	     * this affects the x rotation
	     */
        my_character.velocity.y -= 0.5f;
	    break;
	case SDLK_RIGHT:
	    /* Right arrow key was pressed
	     * this affects the y rotation
	     */
        my_character.velocity.x += 0.5f;
	    break;
	case SDLK_LEFT:
	    /* Left arrow key was pressed
	     * this affects the y rotation
	     */
        my_character.velocity.x -= 0.5f;
	    break;
        
    case SDLK_r:
        /* Right arrow key was pressed
         * this affects the y rotation
         */
        my_character.orientation += 10.01f;
        break;
    case SDLK_a:
        /* Right arrow key was pressed
         * this affects the y rotation
         */
        draw_area = !draw_area;
        break;
    case SDLK_s:
        /* Right arrow key was pressed
         * this affects the y rotation
         */
        draw_characteristic_points = !draw_characteristic_points;
        break;
    case SDLK_d:
        /* Right arrow key was pressed
         * this affects the y rotation
         */
        draw_edges = !draw_edges;
        break;
    case SDLK_e:
        /* Left arrow key was pressed
         * this affects the y rotation
         */
        my_character.orientation -= 10.0f;
        break;
	case SDLK_F1:
	    /* 'f' key was pressed
	     * this toggles fullscreen mode
	     */
	    SDL_WM_ToggleFullScreen( surface );
	    break;
	default:
	    break;
	}

    return;
}

int get_index_quantization( vector< node > & all , GLfloat x , GLfloat y ){
  int ans = -1;
  vec3 pos(x,y,Z_FLOOR);
  
  for (int i = 0; i < (int)all.size(); i++) {
    vec3 diff = pos - all[i].position;
    
    if ( all[i].is_inside( pos ) ){
      if ( ans == -1 ){
        ans = i;
      }else {
        vec3 previous_diff = pos - all[ans].position;
        if ( previous_diff.get_length() > diff.get_length() ){
          ans = i;
        }
      }
    }
  }
  
  if ( ans == -1 ){
    ans = 0;
    for (int i = 1; i < (int)all.size(); i++) {
      vec3 diff = pos - all[i].position;

      vec3 previous_diff = pos - all[ans].position;
      if ( previous_diff.get_length() > diff.get_length() ){
        ans = i;
      }
    }
  }
  return ans;
}

void join( vector<node>& graph, int from, int to, vector< character*> & all_characters ){
  GLfloat dist = ceil((graph[from].position - graph[to].position).get_length());
  edge *new_edge = new edge(&graph[from], &graph[to], dist, true);
  new_edge->do_the_draw = true;
  
  graph[from].succ.push_back(new_edge);
  
  all_characters.push_back(new_edge);
}


void loadAllTextures(vector< character*> &all_characters){
  //glEnable(GL_TEXTURE_2D);
  for (int i = 0; i < (int)all_characters.size() ; i++) {
    character * c = all_characters[i];
    c->LoadGLTextures();
  }
}

void loadFSM(vector<state> & s, string file_name, map<string, int(*)(character*, character*, vector<node> * graph) > & function_map,
    map<string, character*> & character_map ){
  FILE *f = fopen(file_name.c_str(), "r+");
  assert(f);
  
  int amount_states;
  fscanf(f, "%d", &amount_states);
  
  map<string, int> name_to_int;
  
  for (int i = 0; i < amount_states; i++) {
    char name[100];
    char me[100];
    char target[100];
    char entryAction[100];
    char action[100];
    char exitAction[100];
    
    fscanf(f, "%s %s %s %s %s %s", name, me, target, entryAction, 
      action, exitAction);
    
    string real_name(name);
    
//    state new_state(character_map[me], character_map[target], function_map[entryAction],
//      function_map[action], function_map[exitAction], function_map[trigger], real_name);
    state new_state(character_map[me], character_map[target], function_map[entryAction],
      function_map[action], function_map[exitAction], real_name, (vector<node> *)(character_map["graph"]));
    
    name_to_int[real_name] = i;
    s.push_back(new_state);
    
  }
  
  int amount_transitions;
  fscanf(f, "%d", &amount_transitions);
  for (int i = 0; i < amount_transitions; i++) {
    int from, to;
    char name[100];
    fscanf(f, "%d %d %s", &from, &to, name);
    transition * new_transition = new transition( function_map[name], &(s[to]) );
    s[from].succ.push_back(new_transition);

  }
  
  fclose(f);
}

void check_search( character * a, character * b, vector<node> & graph ){
  if ( a->path_in_graph.empty() && (b->position - a->position).get_length() > .1f ){
    // estan muy lejos y no hay camino. Hacemos la busqueda.
    
    int from = get_index_quantization(graph, a->position.x, a->position.y);
    int to = get_index_quantization(graph, b->position.x, b->position.y);
    
    a_star(graph, from, to, a->path_in_graph, a->current_path_index);
  }
}

/**
 * 
 * @param from
 * @param to
 * @param graph
 * @return se movio. Hizo algo. ans == 2 -> no se movio pero hubo cambio de nodo objetivo
 */
int fsm_seek(character* from , character * to, vector<node> * graph){
  static map< character*, int > count;
  int mod = 5;
  
  if ( count.find(from) == count.end() ){
    count[from] = -2;
  }
  count[from]++;
  
  if ( !(count[from] % mod) ){
    fsm_exit_seek(from, to, graph);
    fsm_entry_seek(from, to, graph);
  }
  
  dout << " ACTUAL = " << from->picture_addr << " to = " << to->picture_addr << endl;
  dout << " CURRENT PATH = " << from->current_path_index << endl;
  int ans;
  if ( from->current_path_index < (int)(from->path_in_graph.size()) ){
    dout << " OPTION 1 " << endl;
    from->classic_arrive( (*graph)[from->path_in_graph[from->current_path_index]].position, (*graph)[from->path_in_graph[from->current_path_index]].position , false);

    int current_node = get_index_quantization(*graph, from->position.x, from->position.y);
    ans = 1;
    if ( current_node == from->path_in_graph[from->current_path_index] ){
      ++(from->current_path_index);
      ans = 2;
    }
    if ( from->life > 0 ){
      from->life -= LIFE_DECREASE;
    }
  }else if ( !from->path_in_graph.empty() ){
    dout << " OPTION 2 " << endl;
    ans = from->classic_arrive( to->position, to->position , true);
  }else{
    dout << " OPTION 3 " << endl;
    //assert(false);
    ans = 0;
  }
  
  count[from] = count[from] % mod;
  dout << " ANS SEEK = " << ans << endl;
  return ans;
}

int go_to_feed(character* from , character * to, vector<node> * graph){
  int ans = fsm_seek(from , to, graph);
  if ( !ans ){
    from->life = 100.0;
  }
  return 1;
}

int target_reached(character* from , character * to, vector<node> * graph){
  return (to->position - from->position).get_length() < RADIUS;
}

int fsm_entry_seek(character* from , character * to, vector<node> * graph){
  check_search( from, to, *graph);
  return 0;
}

int fsm_exit_seek(character* from , character * to, vector<node> * graph){
  from->current_path_index = 0;
  from->path_in_graph.clear();
  return 0;
}

int skip(character* from , character * to, vector<node> * graph){
  return 0;
}

int fedEnough(character* from , character * to, vector<node> * graph){
  return from->life > 50.;
}

int notFedEnough(character* from , character * to, vector<node> * graph){
  return !fedEnough(from, to, graph);
}
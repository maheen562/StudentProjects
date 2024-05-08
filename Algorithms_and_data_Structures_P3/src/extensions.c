#include "extensions.h"
#include "options.h"

//////////////////////////////////////////////////////////////////////
// For sorting colors

int color_features_compare(const void* vptr_a, const void* vptr_b) {

	const color_features_t* a = (const color_features_t*)vptr_a;
	const color_features_t* b = (const color_features_t*)vptr_b;

	int u = cmp(a->user_index, b->user_index);
	if (u) { return u; }

	int w = cmp(a->wall_dist[0], b->wall_dist[0]);
	if (w) { return w; }

	int g = -cmp(a->wall_dist[1], b->wall_dist[1]);
	if (g) { return g; }

	return -cmp(a->min_dist, b->min_dist);

}

//////////////////////////////////////////////////////////////////////
// Place the game colors into a set order

void game_order_colors(game_info_t* info,
                       game_state_t* state) {

	if (g_options.order_random) {
    
		srand(now() * 1e6);
    
		for (size_t i=info->num_colors-1; i>0; --i) {
			size_t j = rand() % (i+1);
			int tmp = info->color_order[i];
			info->color_order[i] = info->color_order[j];
			info->color_order[j] = tmp;
		}

	} else { // not random

		color_features_t cf[MAX_COLORS];
		memset(cf, 0, sizeof(cf));

		for (size_t color=0; color<info->num_colors; ++color) {
			cf[color].index = color;
			cf[color].user_index = MAX_COLORS;
		}
    

		for (size_t color=0; color<info->num_colors; ++color) {
			
			int x[2], y[2];
			
			for (int i=0; i<2; ++i) {
				pos_get_coords(state->pos[color], x+i, y+i);
				cf[color].wall_dist[i] = get_wall_dist(info, x[i], y[i]);
			}

			int dx = abs(x[1]-x[0]);
			int dy = abs(y[1]-y[0]);
			
			cf[color].min_dist = dx + dy;
			
		

		}


		qsort(cf, info->num_colors, sizeof(color_features_t),
		      color_features_compare);

		for (size_t i=0; i<info->num_colors; ++i) {
			info->color_order[i] = cf[i].index;
		}
    
	}

	if (!g_options.display_quiet) {

		printf("\n************************************************"
		       "\n*               Branching Order                *\n");
		if (g_options.order_most_constrained) {
			printf("* Will choose color by most constrained\n");
		} else {
			printf("* Will choose colors in order: ");
			for (size_t i=0; i<info->num_colors; ++i) {
				int color = info->color_order[i];
				printf("%s", color_name_str(info, color));
			}
			printf("\n");
		}
		printf ("*************************************************\n\n");

	}

}



//////////////////////////////////////////////////////////////////////
// Check for dead-end regions of freespace where there is no way to
// put an active path into and out of it. Any freespace node which
// has only one free neighbor represents such a dead end. For the
// purposes of this check, cur and goal positions count as "free".

int game_check_deadends(const game_info_t* info,
                        const game_state_t* state) {

	//printf("inside extension:\n");
	//game_print(info,state);


	int cur_x,cur_y;
	pos_t new_x,new_y;
	pos_t new_dir_cell_x,new_dir_cell_y;
	pos_t neighbour_position = 0;
	int color_pos = state->last_color;


	pos_get_coords(state->pos[color_pos], &cur_x, &cur_y);
	new_x = cur_x;
	new_y = cur_y; 
	for(int cell_to_move_dir=DIR_LEFT;
					cell_to_move_dir<=DIR_DOWN;cell_to_move_dir++){
		new_x = cur_x + DIR_DELTA[cell_to_move_dir][0];
		new_y = cur_y + DIR_DELTA[cell_to_move_dir][1];
		neighbour_position = pos_from_coords(new_x, new_y);
		

		if (!coords_valid(info, new_x,new_y)){ // check to see if invalid
			continue;
		}
		else if(game_is_free(info,state,new_x,new_y)){ 
				// search for surroundings of free cell
			//check surroundings
			int three_corner_dead_end=0;
			int invalid = 0;
			for(int i=DIR_LEFT;i<=DIR_DOWN;i++){
				new_dir_cell_x = new_x + DIR_DELTA[i][0];
				new_dir_cell_y = new_y + DIR_DELTA[i][1];
				int new_neighbour_position = 
							pos_from_coords(new_dir_cell_x,new_dir_cell_y);
				if(!coords_valid(info, new_dir_cell_x,new_dir_cell_y)){
							 //check walls with free cell
					invalid++;
				}
				else if(!game_is_free(info,state,new_dir_cell_x,new_dir_cell_y)){ 
					//check for spaces with non free cells
					if(((cell_get_color(state->cells[neighbour_position])) 
								== (cell_get_color(state->cells[new_neighbour_position])))
						 			&& (new_neighbour_position !=
									 		 info->goal_pos[cell_get_color(
												  state->cells[neighbour_position])]))
					{
						three_corner_dead_end++;
					}

				}
			}

			if(three_corner_dead_end >= 3 ||
				 (invalid >= 1 && three_corner_dead_end == 2)){ 
				//returns 1 if invalid move
				return 1;
			}

		}
		else if ( (cell_get_color(state->cells[neighbour_position]) != color_pos)){ 
			// check to see if we blocked way of another color
			int check_neighbour_blocks = 0;
			int neighbour_color = cell_get_color(state->cells[neighbour_position]);
					for(int i=DIR_LEFT;i<=DIR_DOWN;i++){

						

						new_dir_cell_x = new_x + DIR_DELTA[i][0];
						new_dir_cell_y = new_y + DIR_DELTA[i][1];
						int new_neighbour_position = pos_from_coords(
											new_dir_cell_x,new_dir_cell_y);

						//printf("now at cell inside %d\n",new_neighbour_position);


						if(!coords_valid(info, new_dir_cell_x ,new_dir_cell_y)){
							check_neighbour_blocks++;
						}
						else if(!(game_is_free(info,state,new_dir_cell_x,new_dir_cell_y))){
							if(neighbour_color != cell_get_color(
												state->cells[new_neighbour_position])){
								check_neighbour_blocks++; //have to check goal, path is not previous
							}
						}
												
					}
					if (check_neighbour_blocks == 4){
						return 1; //path of neighbour blocked 
					}


				}

		}

	
	return 0;

}


                                         


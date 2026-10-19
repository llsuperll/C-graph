#ifndef HANDLERS_H
#define HANDLERS_H

#include "graph.h"

void display_menu();
int menu_choice(int, Graph**);
const char* error_message(ErrorCode);
void case_insert_vertex(Graph**);
void case_insert_edge(Graph**);
void case_change_vertex(Graph**);
void case_change_edge(Graph**);
void case_delete_edge(Graph**);
void case_delete_vertex(Graph**);
void case_import_to_file(Graph**);
void case_potential_friends(Graph**);
void case_shortest_relationship(Graph**);
void case_positive_groups(Graph**);
#endif

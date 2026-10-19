#include "../include/graph.h"
#include <stdio.h>
#include <stdlib.h>
#include "../include/ui.h"
#include "../include/input.h"

int main() {
    int choice = 0, status = 1;
    Graph* graph = malloc(sizeof(Graph));
    if (!graph) {
        return 1;
    }
    graph -> vertex_count = 0;
    graph -> capacity = N;
    graph -> vertices = malloc(N * sizeof(Vertex*));
    if (!graph->vertices) {
        free(graph);
        return 1;
    }
    for (int i = 0; i < N; i++) {
        graph -> vertices[i] = NULL;
    }
    while (status) {
        display_menu();
        int retval = scanf("%d", &choice);
        if (retval == -1) {
            status = 0;
        } else if (retval != 1) {
            scanf("%*[^\n]%*c");
            printf("Ошибка: Такой команды нет!\n");
        } else {
            status = menu_choice(choice, &graph);
        }
    }
    free_graph(graph);
    return 0;
}

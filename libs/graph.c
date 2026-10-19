#include "../include/graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/ui.h"
#include "../include/input.h"
#include "../include/queue.h"

ErrorCode add_vertex(Graph* graph, const char* name) {
    if (!graph) {
        return ERR_NULL_GRAPH;
    }
    for (int i = 0; i < graph -> vertex_count; i++) {
        if (strcmp(name, graph -> vertices[i] -> name) == 0) {
            return ERR_VERTEX_EXISTS;
        }
    }
    Vertex* vertex = malloc(sizeof(Vertex));
    if (!vertex) {
        return ERR_MEMORY;
    }
    vertex -> name = strdup(name);
    vertex -> edges = NULL;
    if (graph -> vertex_count >= graph -> capacity) {
        graph -> capacity += N;
        graph -> vertices = realloc(graph -> vertices, graph -> capacity * sizeof(Vertex));
        if (!graph -> vertices) {
            free(vertex -> name);
            free(vertex);
            return ERR_MEMORY;
        }
    }
    graph -> vertices[graph -> vertex_count] = vertex;
    graph -> vertex_count++;
    return OK;
}

ErrorCode add_edge(Graph* graph, int from, int to, int relationship) {
    if (!graph) {
        return ERR_NULL_GRAPH;
    }
    if (from < 0 || to < 0 || from >= graph -> vertex_count || to >= graph -> vertex_count) {
        return ERR_INCORRECT_INDEX;
    } else if (from == to) {
        return ERR_SAME_VERTEX;
    }
    Edge* cur = graph -> vertices[from] -> edges;
    while (cur) {
        if (cur -> to_vertex == to) {
            return ERR_EDGE_EXISTS;
        }
        cur = cur -> next;
    }
    Edge* edge = malloc(sizeof(Edge));
    if (!edge) {
        return ERR_MEMORY;
    }
    edge -> to_vertex = to;
    edge -> relationship = relationship;
    edge -> next = graph -> vertices[from] -> edges;
    graph -> vertices[from] -> edges = edge;
    return OK;
}

ErrorCode change_vertex(Graph* graph, int vertex, const char* name) {
    if (!graph) {
        return ERR_NULL_GRAPH;
    }
    if (vertex < 0 || vertex >= graph -> vertex_count) {
        return ERR_INCORRECT_INDEX;
    }
    for (int i = 0; i < graph -> vertex_count; i++) {
        if (strcmp(name, graph -> vertices[i] -> name) == 0) {
            return ERR_VERTEX_EXISTS;
        }
    }
    Vertex* vert = graph -> vertices[vertex];
    free(vert -> name);
    vert -> name = NULL;
    vert -> name = strdup(name);
    return OK;
}

ErrorCode change_edge(Graph* graph, int from, int to, int relationship) {
    if (!graph) {
        return ERR_NULL_GRAPH;
    }
    if (from < 0 || to < 0 || from >= graph -> vertex_count || to >= graph -> vertex_count) {
        return ERR_INCORRECT_INDEX;
    }
    Edge* edge = graph -> vertices[from] -> edges;
    while (edge) {
        if (edge -> to_vertex == to) {
            edge -> relationship = relationship;
            return OK;
        }
        edge = edge -> next;
    }
    return ERR_NOT_FOUND;
}

void free_graph(Graph* graph) {
    if (!graph) {
        return;
    }
    for (int i = 0; i < graph -> vertex_count; i++) {
        Edge* edge = graph -> vertices[i] -> edges;
        Edge* del = NULL;
        while (edge) {
            del = edge;
            edge = edge -> next;
            free(del);
        }
        free(graph -> vertices[i] -> name);
        graph -> vertices[i] -> name = NULL;
        free(graph -> vertices[i]);
        graph -> vertices[i] = NULL;
    }
    free(graph -> vertices);
    graph -> vertices = NULL;
    free(graph);
    graph = NULL;
}

void print_graph(Graph* graph) {
    printf("Граф:\n");
    if (!graph || graph -> vertex_count == 0) {
        return;
    }
    for (int i = 0; i < graph -> vertex_count; i++) {
        Vertex* vertex = graph -> vertices[i];
        Edge* edge = vertex -> edges;
        if (!edge) {
            printf("%s [%d] -> (нет связей)\n", vertex -> name, i);
            continue;
        }
        while (edge) {
            Vertex* dest = graph -> vertices[edge -> to_vertex];
            printf("%s [%d] -> %s [%d] (отношение: %d)\n", vertex -> name, i, dest -> name, edge -> to_vertex, edge -> relationship);
            edge = edge -> next;
        }
    }
}

ErrorCode delete_edge(Graph* graph, int from, int to) {
    if (!graph) {
        return ERR_NULL_GRAPH;
    }
    if (from < 0 || to < 0 || from >= graph -> vertex_count || to >= graph -> vertex_count) {
        return ERR_INCORRECT_INDEX;
    }
    Edge** edge = &(graph -> vertices[from] -> edges);
    while (*edge) {
        Edge* cur = *edge;
        if (cur -> to_vertex == to) {
            *edge = cur -> next;
            free(cur);
            return OK;
        }
        edge = &cur -> next;
    }
    return ERR_NOT_FOUND;
}

ErrorCode delete_vertex(Graph* graph, int vertex) {
    if (!graph) {
        return ERR_NULL_GRAPH;
    }
    if (vertex < 0 || vertex >= graph -> vertex_count) {
        return ERR_INCORRECT_INDEX;
    }
    Edge* edge = graph -> vertices[vertex] -> edges;
    Edge* del = NULL;
    while (edge) {
        del = edge;
        edge = edge -> next;
        free(del);
    }
    graph -> vertices[vertex] -> edges = NULL;
    for (int i = 0; i < graph -> vertex_count; i++) {
        if (i != vertex) {
            delete_edge(graph, i, vertex);
        }
    }
    free(graph -> vertices[vertex] -> name);
    graph -> vertices[vertex] -> name = NULL;
    free(graph -> vertices[vertex]);
    graph -> vertices[vertex] = NULL;
    for (int i = vertex; i < graph -> vertex_count - 1; i++) {
        graph -> vertices[i] = graph -> vertices[i + 1];
    }
    for (int i = 0; i < graph -> vertex_count - 1; i++) {
        Edge* edge = graph -> vertices[i] -> edges;
        while (edge) {
            if (edge -> to_vertex > vertex) {
                edge -> to_vertex--;
            }
            edge = edge -> next;
        }
    }
    graph -> vertex_count -= 1;
    return OK;
}

ErrorCode graphviz_graph(Graph* graph, const char* filename) {
    if (!graph) {
        return ERR_NULL_GRAPH;
    }
    if (!filename) {
        return ERR_FILE;
    }
    FILE* file = fopen(filename, "w");
    if (!file) {
        return ERR_FILE;
    }
    
    fprintf(file, "digraph G {\n");
    char* color;
    for (int i = 0; i < graph -> vertex_count; i++) {
        Vertex* vertex = graph -> vertices[i];
        fprintf(file, "    \"%s\";\n", vertex -> name);
    }
    for (int i = 0; i < graph -> vertex_count; i++) {
        Vertex* from = graph -> vertices[i];
        Edge* edge = from -> edges;

        while (edge) {
            Vertex* to = graph -> vertices[edge -> to_vertex];
            int r = edge -> relationship;
            if (r >= 5) {
                color = "forestgreen";
            } else if (r < 5 && r >= 0) {
                color = "chartreuse";
            } else if (r < -5) {
                color = "red";
            }  else {
                color = "orange";
            }
            fprintf(file, "    \"%s\" -> \"%s\" [label=\"%d\", color=\"%s\"];\n", from -> name, to -> name, edge -> relationship, color);
            edge = edge -> next;
        }
    }
    fprintf(file, "}\n");

    fclose(file);
        char* cmd = calloc(2 * strlen(filename) + 27, sizeof(char));
    if (!cmd) {
        return ERR_MEMORY;
    }
    sprintf(cmd, "dot -Tpng %s -o %s.png", filename, filename);
    pclose(popen(cmd, "r"));
    free(cmd);
    return OK;
}

ErrorCode shortest_relationship(Graph* graph, int from, int to, int** res, int* capacity) {
    if (!graph) {
        return ERR_NULL_GRAPH;
    }
    if (from < 0 || to < 0 || from >= graph -> vertex_count || to >= graph -> vertex_count) {
        return ERR_INCORRECT_INDEX;
    }
    int n = graph -> vertex_count;
    int* prev = malloc(sizeof(int) * n);
    if (!prev) {
        return ERR_MEMORY;
    }
    int* dist = malloc(sizeof(int) * n);
    if (!dist) {
        return ERR_MEMORY;
    }
    for (int i = 0; i < n; i++) {
        prev[i] = -1;
        dist[i] = INF;
    }
    dist[from] = 0;
    
    for (int i = 0; i < n - 1; i++) {
        for (int u = 0; u < n; u++) {
            Vertex* vertex = graph -> vertices[u];
            Edge* edge = vertex -> edges;
            while (edge) {
                if (dist[u] != INF && dist[edge -> to_vertex] > dist[u] + edge -> relationship) {
                    dist[edge -> to_vertex] = dist[u] + edge -> relationship;
                    prev[edge -> to_vertex] = u;
                }
                edge = edge -> next;
            }       
        }
    }

    for (int i = 0; i < n - 1; i++) {
        for (int u = 0; u < n; u++) {
            Vertex* vertex = graph -> vertices[u];
            Edge* edge = vertex -> edges;
            while (edge) {
                if (dist[u] != INF && dist[edge -> to_vertex] > dist[u] + edge -> relationship) {
                    free(dist);
                    free(prev);
                    return ERR_NEGATIVE_CYCLE;
                }
                edge = edge -> next;
            }       
        }
    }
    if (dist[to] == INF) {
        free(prev);
        free(dist);
        return ERR_NO_PATH;
    }
    int* arr = malloc(sizeof(int) * n);
    if (!arr) {
        return ERR_MEMORY;
    }
    int cur = to, cap = 0;
    while (cur != -1) {
        arr[cap] = cur;
        cur = prev[cur];
        cap++;
    }
    arr = realloc(arr, sizeof(int) * cap);
    for (int i = 0; i < cap / 2; i++) {
        int tmp = arr[i];
        arr[i] = arr[cap - 1 - i];
        arr[cap - 1 - i] = tmp;
    }
    *res = arr;
    *capacity = cap;
    free(dist);
    free(prev);
    return OK;
}

ErrorCode potential_friends(Graph* graph, int vertex, int** result, int* capacity) {
    if (!graph) {
        return ERR_NULL_GRAPH;
    } 
    if (vertex < 0 || vertex >= graph -> vertex_count) {
        return ERR_INCORRECT_INDEX;
    }
    int n = graph -> vertex_count, cap = 0, cur;
    int* res = malloc(sizeof(int) * n);
    if (!res) {
        return ERR_MEMORY;
    }
    int* vis = malloc(sizeof(int) * n);
    if (!vis) {
        free(res);
        return ERR_MEMORY;
    }
    for (int i = 0; i < n; i++) {
        vis[i] = 0;
    }
    Queue* q = malloc(sizeof(Queue));
    if (!q) {
        free(vis);
        free(res);
        return ERR_MEMORY;
    }
    q -> head = NULL, q -> tail = NULL;
    ErrorCode err = enqueue(q, vertex);
    if (err != OK) {
        free(vis);
        free(res);
        return err;
    }
    vis[vertex] = 1;
    while (q -> head) {
        cur = dequeue(q);
        Edge* edge = graph -> vertices[cur] -> edges;
        while (edge) {
            if (edge -> relationship >= 0 && vis[edge -> to_vertex] == 0) {
                err = enqueue(q, edge -> to_vertex);
                if (err != OK) {
                    free(vis);
                    free(res);
                    return err;
                }
                vis[edge -> to_vertex] = 1;
                res[cap] = edge -> to_vertex;
                cap += 1;
            } else if (edge -> relationship < 0) {
                vis[edge -> to_vertex] = 1;
            }
            edge = edge -> next;
        }
    }
    if (cap > 0) {
        res = realloc(res, sizeof(int) * cap);
    }
    *result = res;
    *capacity = cap;
    free(vis);
    free_queue(q);
    return OK;
}

ErrorCode positive_groups(Graph* graph, int*** groups, int** group_caps, int* group_amount) {
    if (!graph) {
        return ERR_NULL_GRAPH;
    } 
    int n = graph -> vertex_count;
    int** result = malloc(sizeof(int*) * n);
    int res_size = 0;
    if (!result) {
        return ERR_MEMORY;
    }
    int* group_sizes = malloc(sizeof(int) * n);
    if (!group_sizes) {
        free(result);
        return ERR_MEMORY;
    }
    int* vis = malloc(sizeof(int) * n);
    if (!vis) {
        free(result);
        free(group_sizes);
        return ERR_MEMORY;
    }
    for (int i = 0; i < n; i++) {
        vis[i] = 0;
    }
    for (int i = 0; i < n; i++) {
        if (vis[i] == 1) {
            continue;
        }
        Queue* q = malloc(sizeof(Queue));
        if (!q) {
            free(vis);
            free(result);
            free(group_sizes);
            return ERR_MEMORY;
        }
        q -> head = NULL, q -> tail = NULL;
        int* group = malloc(sizeof(int) * n);
        int group_size = 0;
        if (!group) {
            free(vis);
            free(result);
            free_queue(q);
            return ERR_MEMORY;
        }
        ErrorCode err = enqueue(q, i);
        if (err != OK) {
            free(vis);
            free(result);
            free_queue(q);
            return err;
        }
        vis[i] = 1;
        while (q -> head) {
            int vert = dequeue(q);
            group[group_size] = vert;
            group_size += 1;
            Edge* edge = graph -> vertices[vert] -> edges;
            while (edge) {
                if (edge -> relationship >= 0 && vis[edge->to_vertex] == 0) {
                    vis[edge->to_vertex] = 1;
                    err = enqueue(q, edge->to_vertex);
                    if (err != OK) {
                        free(result);
                        free(vis);
                        free_queue(q);
                        return err;
                    }
                }
                edge = edge -> next;
            }
        }
        result[res_size] = group;
        group_sizes[res_size] = group_size;
        res_size += 1;
        free_queue(q);
    }
    if (res_size > 0) {
        result = realloc(result, sizeof(int*) * res_size);
        group_sizes = realloc(group_sizes, sizeof(int) * res_size);
    }
    *groups = result;
    *group_caps = group_sizes;
    *group_amount = res_size;
    free(vis);
    return OK;
}

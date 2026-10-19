#ifndef TREE_H
#define TREE_H

typedef enum {
    OK,
    ERR_NULL_GRAPH,
    ERR_INCORRECT_INDEX,
    ERR_NOT_FOUND,
    ERR_MEMORY,
    ERR_FILE,
    ERR_EDGE_EXISTS,
    ERR_NOT_INITIALISED,
    ERR_SAME_VERTEX,
    ERR_NEGATIVE_CYCLE,
    ERR_NO_PATH,
    ERR_VERTEX_EXISTS,
} ErrorCode; 

typedef struct Edge {
    int to_vertex;
    int relationship;
    struct Edge* next;
} Edge;

typedef struct Vertex {
    char* name;
    Edge* edges;
} Vertex;

typedef struct Graph {
    int capacity;
    int vertex_count;
    Vertex** vertices;
} Graph;

#define N 5
#define INF 10e8

ErrorCode add_vertex(Graph* graph, const char* name);
ErrorCode add_edge(Graph* graph, int from, int to, int relationship);
ErrorCode change_vertex(Graph* graph, int vertex, const char* name);
ErrorCode change_edge(Graph* graph, int from, int to, int relationship);
void free_graph(Graph* graph);
void print_graph(Graph* graph);
ErrorCode delete_edge(Graph* graph, int from, int to);
ErrorCode delete_vertex(Graph* graph, int vertex);
ErrorCode graphviz_graph(Graph* graph, const char* filename);
ErrorCode shortest_relationship(Graph* graph, int from, int to, int** res, int* capacity);
ErrorCode potential_friends(Graph* graph, int vertex, int** result, int* capacity);
ErrorCode positive_groups(Graph* graph, int*** groups, int** group_caps, int* group_amount);
#endif

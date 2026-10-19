CC = gcc

INCLUDE_DIR = include
SRC_DIR = libs
APP_DIR = app
LIB_DIR = lib

GRAPH_SRC = $(SRC_DIR)/graph.c
QUEUE_SRC = $(SRC_DIR)/queue.c  
MAIN_SRC = $(APP_DIR)/main.c
UI_SRC = $(APP_DIR)/ui.c
INPUT_SRC = $(APP_DIR)/input.c	

LIB_TABLE = $(LIB_DIR)/libgraph.a

APP = graph

default: static

static:
	$(CC) -c $(GRAPH_SRC) -O0 -g -o $(SRC_DIR)/graph.o
	ar rcs $(LIB_TABLE) $(SRC_DIR)/graph.o
	$(CC) -I $(INCLUDE_DIR) -o $(APP) $(MAIN_SRC) $(QUEUE_SRC) $(UI_SRC) $(INPUT_SRC) $(LIB_TABLE)

shared:
	$(CC) -fPIC -c $(GRAPH_SRC) -o $(SRC_DIR)/graph.o
	$(CC) -shared -o $(LIB_DIR)/graph.so $(SRC_DIR)/graph.o
	$(CC) -I $(INCLUDE_DIR) -o $(APP) $(MAIN_SRC) $(UI_SRC) $(INPUT_SRC) -L $(LIB_DIR) -lgraph -Wl,-rpath,'$$ORIGIN/$(LIB_DIR)'

run:
	valgrind ./graph
clean:
	rm -f $(SRC_DIR)/*.o $(LIB_DIR)/* $(APP)

.PHONY: default static shared run clean

#include <stdio.h>
#include <stdlib.h>
#include "../include/graph.h"
#include "../include/ui.h"
#include "../include/input.h"

int menu_choice(int choice, Graph** graph) {
    switch(choice) {
        case 1:
            case_insert_vertex(graph);
            break;
        case 2:
            case_insert_edge(graph);
            break;
        case 3:
            case_delete_vertex(graph);
            break;
        case 4:
            case_delete_edge(graph);
            break;
        case 5:
            case_change_vertex(graph);
            break;
        case 6:
            case_change_edge(graph);
            break;
        case 7:
            print_graph(*graph);
            break;
        case 8:
            case_import_to_file(graph);
            break;
        case 9:
            case_potential_friends(graph);
            break;
        case 10:
            case_shortest_relationship(graph);
            break;
        case 11:
            case_positive_groups(graph);
            break;
        default:
            return 0;
    }
    return 1;
}

void display_menu() {
    printf(
        "\n=== Круг знакомств ===\n"
        "(1) Добавить вершину\n"
        "(2) Добавить ребро\n"
        "(3) Удалить вершину\n"
        "(4) Удалить ребро\n"
        "(5) Изменить данные вершины\n"
        "(6) Изменить данные ребра\n"
        "(7) Вывод в виде списка смежности\n"
        "(8) Вывод графа в файл\n"
        "(9) Обход потенциальных друзей\n"
        "(10) Кратчайший путь (наим. сумма уровня отношений)\n"
        "(11) Разбиение на положительные КСС\n"
        "Выберите действие (1-11): "
    );
}

const char* error_message(ErrorCode code) {
    switch(code) {
        case OK: 
            return "Успешно";
        case ERR_MEMORY: 
            return "Ошибка выделения памяти";
        case ERR_FILE: 
            return "Ошибка работы с файлом";
        case ERR_INCORRECT_INDEX:
            return "Неверный индекс";
        case ERR_NOT_FOUND:
            return "Не найдено";
        case ERR_NOT_INITIALISED:
            return "Не инициализированное значение";
        case ERR_EDGE_EXISTS:
            return "Такое ребро уже существует";
        case ERR_NEGATIVE_CYCLE:
            return "Существует отрицательный цикл";
        case ERR_NO_PATH:
            return "Нет пути";
        case ERR_SAME_VERTEX:
            return "Как к себе относится то?!?";
        case ERR_NULL_GRAPH:
            return "Граф не инициализирован";
        case ERR_VERTEX_EXISTS:
            return "Вершина с таким именем уже существует";
        default: 
            return "Неизвестная ошибка";
    }
}

void case_insert_vertex(Graph** graph) {
    scanf("%*c");
    char* name = my_getline("Введите имя: ", stdin);
    ErrorCode err = add_vertex(*graph, name);
    if (err == OK) {
        printf("Вершина добавлена\n");
    } else {
        printf("%s\n", error_message(err));
    }
    if (name) {
        free(name);
    }
}

void case_insert_edge(Graph** graph) {
    int relationship, from, to;
    if (input(&from, "Введите вершину исток (номер): ") == -1) {
        return;
    }
    if (input(&to, "Введите вершину сток (номер): ") == -1) {
        return;
    }
    if (input(&relationship, "Введите отношение (от -10 до 10): ") == -1) {
        return;
    }
    if (relationship > 10) {
        relationship = 10;
    } else if (relationship < -10) {
        relationship = -10;
    }

    ErrorCode err = add_edge(*graph, from, to, relationship);
    if (err == OK) {
        printf("Ребро добавлено\n");
    } else {
        printf("%s\n", error_message(err));
    }
}

void case_change_vertex(Graph** graph) {
    int vertex;
    if (input(&vertex, "Введите номер вершины: ") == -1) {
        return;
    }
    char* name = my_getline("Введите новое имя: ", stdin);
    ErrorCode err = change_vertex(*graph, vertex, name);
    if (err == OK) {
        printf("Имя успешно изменено\n");
    } else {
        printf("%s\n", error_message(err));
    }
    if (name) {
        free(name);
    }
}

void case_change_edge(Graph** graph) {
    int from, to, relationship;
    if (input(&from, "Введите номер вершины истока: ") == -1) {
        return;
    }
    if (input(&to, "Введите номер вершины стока: ") == -1) {
        return;
    }
    if (input(&relationship, "Введите новое отношение: ") == -1) {
        return;
    }
    if (relationship > 10) {
        relationship = 10;
    } else if (relationship < -10) {
        relationship = -10;
    }
    ErrorCode err = change_edge(*graph, from, to, relationship);
    if (err == OK) {
        printf("Отношение успешно изменено\n");
    } else {
        printf("%s\n", error_message(err));
    }
}

void case_delete_edge(Graph** graph) {
    int from, to, relate;
    if (input(&from, "Введите номер вершины истока: ") == -1) {
        return;
    }
    if (input(&to, "Введите номер вершины стока: ") == -1) {
        return;
    }
    ErrorCode err = delete_edge(*graph, from, to);
    if (err == OK) {
        printf("Ребро удалено\n");
    } else {
        printf("%s\n", error_message(err));
    }
}

void case_delete_vertex(Graph** graph) {
    int vertex;
    if (input(&vertex, "Введите номер вершины: ") == -1) {
        return;
    }
    ErrorCode err = delete_vertex(*graph, vertex);
    if (err == OK) {
        printf("Вершина %d удалена\n", vertex);
    } else {
        printf("%s\n", error_message(err));
    }
}

void case_import_to_file(Graph** graph) {
    scanf("%*c");
    char* filename = my_getline("Введите имя файла: ", stdin);
    ErrorCode err = graphviz_graph(*graph, filename);
    if (err != OK) {
        printf("%s\n", error_message(err));
        return;
    }
    printf("Граф сохранен в файл %s.png", filename);
    if (filename) {
        free(filename);
    }
}

void case_potential_friends(Graph** graph) {
    int vertex;
    if (input(&vertex, "Введите номер вершины: ") == -1) {
        return;
    }
    int* res = NULL;
    int cap = 0;
    ErrorCode err = potential_friends(*graph, vertex, &res, &cap);
    if (err != OK) {
        printf("%s\n", error_message(err));
        return;
    }
    printf("Потенциальные друзья: ");
    for (int i = 0; i < cap; i++) {
        printf("%d ", res[i]);
    }
    free(res);
}

void case_shortest_relationship(Graph** graph) {
    int from, to;
    if (input(&from, "Введите номер первой вершины: ") == -1) {
        return;
    }
    if (input(&to, "Введите номер второй вершины: ") == -1) {
        return;
    }
    int* res = NULL;
    int cap = 0;
    ErrorCode err = shortest_relationship(*graph, from, to, &res, &cap);
    if (err != OK) {
        printf("%s\n", error_message(err));
        return;
    }
    printf("Кратчайший путь: ");
    for (int i = 0; i < cap; i++) {
        printf("%d ", res[i]);
    }
    free(res);
}

void case_positive_groups(Graph** graph) {
    int size = 0;
    int* group_sizes = NULL;
    int** pos_groups = NULL;
    ErrorCode err = positive_groups(*graph, &pos_groups, &group_sizes, &size);
    if (err != OK) {
        printf("%s\n", error_message(err));
        return;
    }
    printf("Найдено %d позитивных групп\n", size);
    for (int i = 0; i < size; i++) {
        printf("Группа %d (%d человек): ", i + 1, group_sizes[i]);
        for (int j = 0; j < group_sizes[i]; j++) {
            printf("%s ", (*graph) -> vertices[pos_groups[i][j]] -> name);
        }
        printf("\n");
    }
    free(group_sizes);
    for (int i = 0; i < size; i++) {
        free(pos_groups[i]);
    }
    free(pos_groups);
    return;
}

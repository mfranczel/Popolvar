#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//#define chodnik 'C'
//#define porast 'H'
//#define prekazka 'N'
//#define drak 'D'
//#define princezna 'P'
//#define generator 'G'

struct node {
    int x;
    int y;
    int dist;
    struct node *prev;
};

void heap_insert(struct node **heap, int *heap_size, struct node *node){
    heap[*heap_size] = node;
    int index = *heap_size;
    (*heap_size)++;

    while(1) {
        if(heap[(index-1)/2] -> dist < heap[index] -> dist) {
            struct node *temp = heap[index];
            heap[index] = heap[(index-1)/2];
            heap[(index-1)/2] = temp;
            index -=1;
            index /= 2;
        } else {
            break;
        }
    }
}

struct node* heap_min(struct node **heap, int *heap_size) {
    struct node* min = heap[0];
    int index = --*heap_size;
    heap[0] = heap[index];
    heap[index] = NULL;

    int left_child;
    int right_child;
    int smaller;

    while(1) {
        left_child = 2 * index + 1;
        right_child = 2 * index + 2;
        smaller = index;

        if(right_child < *heap_size  && heap[smaller] -> dist > heap[right_child] -> dist) {
            smaller = right_child;
        }
        if(left_child < *heap_size  && heap[left_child] -> dist < heap[smaller] -> dist) {
            smaller = left_child;
        }

        if(smaller == index) {break;}

        struct node *temp = heap[index];
        heap[index] = heap[smaller];
        heap[smaller] = temp;
        index = smaller;
    }

    return min;
}

int is_in_heap(struct node **heap, int heap_size, int x, int y) {
    for(int i = 0; i < heap_size; i++) {
        if(heap[i] -> x == x && heap[i] == y) {
            return 1;
        }
    }
    return 0;
}

struct node* find(struct node**heap, int heap_size, int x, int y) {
    for(int i = 0; i < heap_size; i++) {
        if(heap[i] -> x == x && heap[i] -> y == y) {
            return heap[i];
        }
    }
    return NULL;
}

int get_cost(char **mapa, int x, int y) {
    if(mapa[y][x] == 'P') {
        return 1;
    } else if(mapa[y][x] == 'C') {
        return 1;
    } else if(mapa[y][x] == 'H') {
        return 2;
    } else if(mapa[y][x] == 'N') {
        return -1;
    }
    return 0;
}

void decrease_priority(struct node **heap, int heap_size, int x, int y, int new_priority) {
    struct node* temp = find(heap, heap_size, x, y);
    int i;

    for(i = 0; i < heap_size; i++) {
        if(heap[i] -> x == x && heap[i] -> y == y) {
            temp = heap[i];
        }
    }

    temp -> dist = new_priority;
    while(1) {
        if(heap[(i-1)/2] -> dist < heap[i] -> dist) {
            struct node *temp = heap[i];
            heap[i] = heap[(i-1)/2];
            heap[(i-1)/2] = temp;
            i -=1;
            i /= 2;
        } else {
            break;
        }
    }

    int left_child;
    int right_child;
    int smaller;

    while(1) {
        left_child = 2 * i + 1;
        right_child = 2 * i + 2;
        smaller = i;

        if(right_child < heap_size  && heap[smaller] -> dist > heap[right_child] -> dist) {
            smaller = right_child;
        }
        if(left_child < heap_size  && heap[left_child] -> dist < heap[smaller] -> dist) {
            smaller = left_child;
        }

        if(smaller == i) {break;}

        struct node *temp = heap[i];
        heap[i] = heap[smaller];
        heap[smaller] = temp;
        i = smaller;
    }


}

void shortest_dijkstra(char **mapa, int n, int m, int tbf[2]) {
    struct node **heap = (struct node**) malloc(n*m*sizeof(struct node*));
    struct node* start = (struct node*) malloc(sizeof(struct node*));
    start -> x = 0;
    start -> y = 0;
    start -> dist = 0;
    start -> prev = NULL;
    int heap_size = 0;
    int x;
    int y;
    int dist;
    heap_insert(heap, &heap_size, start);
    struct node* temp;
    int temp_dist;



    while(heap_size != 0) {
        struct node* min = heap_min(heap, &heap_size);
        x = min -> x;
        y = min -> y;
        dist = min -> dist;

        if(x == tbf[0] && y == tbf[1] && dist != INT_MAX) {
            while(min != NULL) {
                printf("%d %d %d\n", min -> x, min -> y, min -> dist);
                min = min -> prev;
            }
            return;
        }

        printf("%d %d %c\n", x, y, mapa[y][x]);

        if(x < m-1 && (mapa[y][x+1] == 'C' || mapa[y][x+1] == 'H' || mapa[y][x+1] == 'D')) {
            temp_dist = min -> dist + get_cost(mapa, min -> x, min -> y);
            temp = malloc(sizeof(struct node));
            temp -> x = x+1;
            temp -> y = y;
                temp -> dist = temp_dist;
                temp -> prev = min;
                struct node* temp_2 = find(heap, heap_size, x+1, y);
                if(temp_2 != NULL) {
                    if(temp_dist < temp_2 -> dist) {
                        decrease_priority(heap, heap_size, x+1, y, temp_dist);
                        temp_2 -> prev = min;
                    }
                } else {
                    heap_insert(heap, &heap_size, temp);
                }
        }

        if(x > 0 && (mapa[y][x-1] == 'C' || mapa[y][x-1] == 'H' || mapa[y][x-1] == 'D')) {
            temp_dist = min -> dist + get_cost(mapa, min -> x, min -> y);
            temp = malloc(sizeof(struct node));
            temp -> x = x-1;
            temp -> y = y;
                temp -> dist = temp_dist;
                temp -> prev = min;
                struct node* temp_2 = find(heap, heap_size, x-1, y);
                if(temp_2 != NULL) {
                    if(temp_dist < temp_2 -> dist) {
                        decrease_priority(heap, heap_size, x - 1, y, temp_dist);
                        temp_2->prev = min;
                    }
                } else {
                    heap_insert(heap, &heap_size, temp);
                }
        }

        if(y < n-1 && (mapa[y+1][x] == 'C' || mapa[y+1][x] == 'H' || mapa[y+1][x] == 'D')) {
            temp_dist = min -> dist + get_cost(mapa, min -> x, min -> y);
            temp = malloc(sizeof(struct node));
            temp -> x = x;
            temp -> y = y+1;
                temp -> dist = temp_dist;
                temp -> prev = min;
                struct node* temp_2 = find(heap, heap_size, x, y+1);
                if(temp_2 != NULL) {
                    if(temp_dist < temp_2 -> dist) {
                        decrease_priority(heap, heap_size, x, y + 1, temp_dist);
                        temp_2->prev = min;
                    }
                } else {
                    heap_insert(heap, &heap_size, temp);
                }
        }

        if(y > 0 && (mapa[y-1][x] == 'C' || mapa[y-1][x] == 'H' || mapa[y-1][x] == 'D')) {
            temp_dist = min -> dist + get_cost(mapa, min -> x, min -> y);
            temp = malloc(sizeof(struct node));
            temp -> x = x;
            temp -> y = y-1;
                temp -> dist = temp_dist;
                temp -> prev = min;
                struct node* temp_2 = find(heap, heap_size, x, y-1);
                if(temp_2 != NULL) {
                    if(temp_dist < temp_2 -> dist) {
                        decrease_priority(heap, heap_size, x, y - 1, temp_dist);
                        temp_2->prev = min;
                    }
                } else {
                    heap_insert(heap, &heap_size, temp);
                }
        }

    }
}


int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty)
{
    int drak[2];

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(mapa[i][j] == 'D') {
                drak[0] = j;
                drak[1] = i;
            }
        }
    }

    shortest_dijkstra(mapa, n, m, drak);


}

int main() {
    int n = 5;
    int m = 5;
    int dlzka = 0;
    char mapa[5][5] = {
            {'P','P','H','H','H'},
            {'H','C','H','H','C'},
            {'C','C','H','C','C'},
            {'C','H','C','H','C'},
            {'H','P','C','D','C'}
    };

    int **mapa_2 = (int **) malloc(5*sizeof(int*));

    for(int i = 0; i<5; i++) {
        mapa_2[i] = (int*) malloc(sizeof(int)*5);
    }

    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            mapa_2[i][j]= mapa[i][j];
        }
    }

    zachran_princezne(mapa_2, n, m, 10, &dlzka);
    return 0;
}
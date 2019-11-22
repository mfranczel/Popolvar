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
    int state;
    struct node *prev;
};


int is_gen_on(struct node* node) {
    int state = node -> state;
    if(state & 1 == 1) {
        return 1;
    }
}


void heap_insert(struct node **heap, int *heap_size, struct node *node){
    heap[*heap_size] = node;
    int index = *heap_size;
    (*heap_size)++;

    while(1) {
        if(heap[(index-1)/2] -> dist > heap[index] -> dist) {
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
    int index = --(*heap_size);
    heap[0] = heap[index];
    heap[index] = NULL;

    int left_child;
    int right_child;
    int smaller;
    index = 0;

    while(1) {
        left_child = 2 * index + 1;
        right_child = 2 * index + 2;
        smaller = index;

        struct node* l = heap[left_child];
        struct node* r = heap[right_child];

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
            break;
        }
    }

    temp -> dist = new_priority;
    while(1) {
        struct node *temp2 = heap[(i-1)/2];
        if(heap[(i-1)/2] -> dist > heap[i] -> dist) {
            struct node *temp = heap[i];
            heap[i] = heap[(i-1)/2];
            heap[(i-1)/2] = temp;
            i -=1;
            i /= 2;
        } else {
            break;
        }
    }

}

void next(char **mapa, int n, int m, int x, int y, int xx, int yy,struct node **heap, int heap_size, struct node* prev) {
    struct node* temp;
    if(mapa[y+yy][x+xx] == 'C' || mapa[y+yy][x+xx] == 'H' || mapa[y+yy][x+xx] == 'D' || mapa[y+yy][x+xx] == 'P') {
            temp = find(heap, heap_size, x+xx, y+yy);
            if(temp != NULL && prev -> dist != INT_MAX && temp -> dist > prev -> dist + get_cost(mapa, x, y)) {
                temp ->prev = prev;
                decrease_priority(heap, heap_size, x+xx, y+yy, prev -> dist + get_cost(mapa, x, y));
            }
       }
}

void shortest_dijkstra(char **mapa, int n, int m, int tbf[2]) {
    struct node **heap = (struct node**) malloc(n*m*sizeof(struct node*));
    int heap_size = 0;
    int x;
    int y;
    int dist;
    struct node* temp;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            temp = malloc(sizeof(struct node));
            temp -> x = j;
            temp -> y = i;
            temp -> dist = INT_MAX;
            temp -> prev = NULL;
            temp -> state = 0;
            if(i == 0 && j == 0) {
                temp -> dist = 0;
            }
            heap_insert(heap, &heap_size, temp);
        }
    }


    while(heap_size != 0) {
        struct node* min = heap[0];
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

        if(x < m-1) {
            next(mapa, n, m, x, y, 1, 0, heap, heap_size, min);
        }

        if(x > 0) {
            next(mapa, n, m, x, y, -1, 0, heap, heap_size, min);
        }

        if(y < n-1) {
            next(mapa, n, m, x, y, 0, 1, heap, heap_size, min);
        }

        if(y > 0 && (mapa[y-1][x] == 'C' || mapa[y-1][x] == 'H' || mapa[y-1][x] == 'D' || mapa[y-1][x] == 'P')) {
            temp = find(heap, heap_size, x, y-1);
            if(temp != NULL && min -> dist != INT_MAX && temp -> dist > min -> dist + get_cost(mapa, x, y)) {
                temp ->prev = min;
                decrease_priority(heap, heap_size, x, y-1, min -> dist + get_cost(mapa, x, y));
            }
        }

        heap_min(heap, &heap_size);

    }
}


int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty)
{
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            printf("%c ", mapa[i][j]);
        }
        printf("\n");
    }
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

    int *mapa_3[5];
    for(int i = 0; i<5; i++) {
        mapa_3[i] = mapa[i];
    }

    zachran_princezne(mapa_3, n, m, 10, &dlzka);
    return 0;
}
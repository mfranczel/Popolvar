#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#import <string.h>

//#define chodnik 'C'
//#define porast 'H'
//#define prekazka 'N'
//#define drak 'D'
//#define princezna 'P'
//#define generator 'G'

struct node {
    int x;
    int y;
    unsigned int dist;
    int state;
    struct node *prev;
};


int is_gen_on(struct node* node) {
    int state = node -> state;
    if(state & 1 == 1) {
        return 1;
    }
}

struct teleport {
    int x;
    int y;
    struct teleport* next;
};

struct teleport *t0, *t1, *t2, *t3, *t4, *t5, *t6, *t7, *t8, *t9 = NULL;
int princesses[5][2];

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

int get_state(int p5, int p4, int p3, int p2, int p1, int drak, int generator) {
    int state = 0;
    if(generator != 0) {state |= 1;}
    if(drak != 0) {state |= 2;}
    if(p1 != 0) {state |= 4;}
    if(p2 != 0) {state |= 8;}
    if(p3 != 0) {state |= 16;}
    if(p4 != 0) {state |= 32;}
    if(p5 != 0) {state |= 64;}
    return state;
}

struct node* find(struct node**heap, int heap_size, int x, int y, int state) {
    for(int i = 0; i < heap_size; i++) {
        if(heap[i] -> x == x && heap[i] -> y == y && heap[i] -> state == state) {
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
    } else if(mapa[y][x] == 'D') {
        return 1;
    } else if(mapa[y][x] >= '0' && mapa[y][x] <= '9') {
        return 1;
    }
    return 0;
}

void decrease_priority(struct node **heap, int heap_size, int x, int y, int state, int new_priority) {
    struct node* temp = find(heap, heap_size, x, y, state);
    int i;


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

struct teleport *find_teleport(char n) {
    switch(n) {
        case '0':
            return t0;
        case '1':
            return t1;
        case '2':
            return t2;
        case '3':
            return t3;
        case '4':
            return t4;
        case '5':
            return t5;
        case '6':
            return t6;
        case '7':
            return t7;
    }

}

int get_princess_mask(int x, int y) {
    int i;
    int j = 4;
    for(i = 0; i < 5; i++) {
        if(princesses[i][0] == x && princesses[i][1] == y) {
            break;
        }
        j *= 2;
    }
    return j;
}

int get_n_from_state(int state) {
    int n = 0;
    n += (state & 64)/64 + (state & 32)/32 + (state & 16)/16 + (state & 8)/8 + (state & 4)/4;
    return n;
}

void next(char **mapa, int n, int m, int x, int y, int state, int xx, int yy,struct node **heap, int heap_size, struct node* prev, int *princ_n) {
    struct node* temp;

    if(mapa[y+yy][x+xx] == 'C' || mapa[y+yy][x+xx] == 'H' || (mapa[y+yy][x+xx] == 'D'  && (state & 2) != 0) || (mapa[y+yy][x+xx] == 'P' &&  ((state & 2) == 0 || ((get_princess_mask(x+xx, y+yy) & state) !=  0))) || (mapa[y+yy][x+xx] >= '0' && mapa[y+yy][x+xx] <= '9' && (state & 1) == 1)) {
            temp = find(heap, heap_size, x+xx, y+yy, state);

            if(temp != NULL && prev -> dist != UINT_MAX && temp -> dist > prev -> dist + get_cost(mapa, x, y)) {
                temp ->prev = prev;
                decrease_priority(heap, heap_size, x+xx, y+yy, state,prev -> dist + get_cost(mapa, x, y));
            } else {return;}
    } else if(mapa[y+yy][x+xx] == 'G' && (state & 1) == 0) {
        temp = find(heap, heap_size, x+xx, y+yy, state | 1);
        if(temp != NULL && prev -> dist != UINT_MAX && temp -> dist > prev -> dist + get_cost(mapa, x, y)) {
            temp -> prev = prev;
            decrease_priority(heap, heap_size, x+xx, y+yy, temp -> state, prev -> dist + get_cost(mapa, x, y));
        } else {return;}
    } else if(mapa[y+yy][x+xx] == 'D' && (state & 2) == 0) {
        temp = find(heap, heap_size, x+xx, y+yy, state | 2);
        if(temp != NULL && prev -> dist != UINT_MAX && temp -> dist > prev -> dist + get_cost(mapa, x, y)) {
            temp -> prev = prev;
            decrease_priority(heap, heap_size, x+xx, y+yy, temp -> state, prev -> dist + get_cost(mapa, x, y));
        } else {return;}
    } else if(mapa[y+yy][x+xx] == 'P' &&  (state & 2) != 0 && ((get_princess_mask(x+xx, y+yy) & state) ==  0)) {
        temp = find(heap, heap_size, x+xx, y+yy, state | get_princess_mask(x+xx, y+yy));
        if(temp != NULL && prev -> dist != UINT_MAX && temp -> dist > prev -> dist + get_cost(mapa, x, y)) {
            temp -> prev = prev;
            decrease_priority(heap, heap_size, x+xx, y+yy, temp -> state , prev -> dist + get_cost(mapa, x, y));
        } else {return;}
    } else if(mapa[y+yy][x+xx] >= '0' && mapa[y+yy][x+xx] <= '9' && (state & 1) == 1) {
        struct teleport* t = find_teleport(mapa[y+yy][x+xx]);
        while(t != NULL) {
            temp = find(heap, heap_size, t ->x, t -> y, state);
            if(temp != NULL && prev -> dist != UINT_MAX && temp -> dist > prev -> dist + get_cost(mapa, x, y)) {
                temp -> prev = prev;
                decrease_priority(heap, heap_size, x+xx, y+yy, state , prev -> dist + get_cost(mapa, x, y));
            }
            t = t->next;
        }
    }


}

int* shortest_dijkstra(char **mapa, int n, int m, int princ_n, int *dlzka_cesty) {
    struct node **heap = (struct node**) malloc(n*m*(128)*sizeof(struct node*));
    memset(heap, 0, n*m*(128)*sizeof(struct node*));
    int heap_size = 0;
    int x, y;
    unsigned int dist;
    int state;
    struct node* temp;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            for(int k = 0; k < 128; k++) {
                temp = malloc(sizeof(struct node));
                temp -> x = j;
                temp -> y = i;
                temp -> prev = NULL;
                temp -> state = k;
                if(i == 0 && j == 0 && k == 0) {
                    temp -> dist = 0;
                } else {
                    temp -> dist = UINT_MAX;
                }
                heap_insert(heap, &heap_size, temp);
            }
        }
    }

    struct node **all_paths = malloc(500*sizeof(struct node*));
    int no = 0;


    while(heap_size != 0) {
        struct node* min = heap[0];
        x = min -> x;
        y = min -> y;
        state = min -> state;
        dist = min -> dist;


        if(x < m-1) {
            next(mapa, n, m, x, y, min -> state, 1, 0, heap, heap_size, min, &princ_n);
        }

        if(x > 0) {
            next(mapa, n, m, x, y, min -> state,-1, 0, heap, heap_size, min, &princ_n);
        }

        if(y < n-1) {
            next(mapa, n, m, x, y, min -> state, 0, 1, heap, heap_size, min, &princ_n);
        }

        if(y > 0 ) {
            next(mapa, n, m, x, y, min -> state,0, -1, heap, heap_size, min, &princ_n);
        }
        if(princ_n == get_n_from_state(state) && mapa[y][x] == 'P') {

            (*dlzka_cesty) = 0;
            int *cesta = malloc(sizeof(int)*5000);
            int i = 0;
            while(min != NULL) {
                //printf("%d %d %u %d\n", min -> x, min -> y, min -> dist, min -> state);
                cesta[i] = min -> x;
                cesta[i+1] = min -> y;
                i += 2;
                min = min -> prev;
                (*dlzka_cesty)++;
            }

            int temp;
            int j = 0;
            while(i != j) {
                temp = cesta[i];
                cesta[i] = cesta[j];
                cesta[j] = temp;
                i--;
                j++;
            }
            return cesta;
        }

        heap_min(heap, &heap_size);

    }

//    for(int i = 0; i < no; i++) {
//        if(all_paths[i] -> prev != NULL) {
//            struct node* min = all_paths[i];
//            while(min != NULL) {
//                printf("%d %d %d %d\n", min -> x, min -> y, min -> dist, min -> state);
//                min = min -> prev;
//            }
//            printf("------------------\n");
//        }
//    }
}

void add_teleport(struct teleport *t, int x, int y) {
    struct teleport *teleport = malloc(sizeof(teleport));
    teleport -> next = NULL;
    teleport -> x = x;
    teleport -> y = y;

    struct teleport *temp = t;
    t = teleport;
    t -> next = temp;
}


int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty)
{
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            printf("%c ", mapa[i][j]);
        }
        printf("\n");
    }
    memset(princesses, 0, sizeof(princesses));
    int drak[2];
    int princess_counter = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {

            switch (mapa[i][j]) {
                case 'D':
                    drak[0] = j;
                    drak[1] = i;
                    break;
                case '0':
                    add_teleport(t0,j,i);
                    break;
                case '1':
                    add_teleport(t1,j,i);
                    break;
                case '2':
                    add_teleport(t2,j,i);
                    break;
                case '3':
                    add_teleport(t3,j,i);
                    break;
                case '4':
                    add_teleport(t4,j,i);
                    break;
                case '5':
                    add_teleport(t5,j,i);
                    break;
                case '6':
                    add_teleport(t6,j,i);
                    break;
                case '7':
                    add_teleport(t7,j,i);
                    break;
                case '8':
                    add_teleport(t8,j,i);
                    break;
                case '9':
                    add_teleport(t9,j,i);
                    break;
                case 'P':
                    princesses[princess_counter][0] = j;
                    princesses[princess_counter][1] = i;
                    princess_counter++;
                    break;
            }


        }
    }

    return shortest_dijkstra(mapa, n, m, princess_counter, dlzka_cesty);


}

int main() {
    int n = 5;
    int m = 30;
    int dlzka = 0;
    /*char mapa[5][5] = {
            {'P','P','H','H','H'},
            {'H','C','H','H','C'},
            {'C','C','H','C','C'},
            {'C','H','C','H','C'},
            {'H','P','C','D','C'}
    };*/

    char mapa[5][30] = {
            {'H', 'H', 'H', 'H', 'H', 'C', 'H', 'C', 'C', 'C', 'H', 'H', 'H', 'C', 'H', 'H', 'H', 'H', 'P', 'D', 'H', 'C', 'C', 'C', 'C', 'H', 'C', 'C', 'C', 'H'},
            {'C', 'C', 'H', 'C', 'C', 'C', 'C', 'C', 'C', 'H', 'C', 'C', 'C', 'C', 'C', 'H', 'C', 'H', 'H', 'H', 'C', 'C', 'C', 'C', 'C', 'C', 'H', 'C', 'C', 'H'},
            {'C', 'C', 'C', 'C', 'C', 'H', 'P', 'C', 'C', 'C', 'H', 'H', 'C', 'C', 'H', 'C', 'C', 'C', 'C', 'H', 'C', 'C', 'C', 'C', 'H', 'C', 'C', 'C', 'C', 'H'},
            {'C', 'C', 'C', 'C', 'H', 'H', 'H', 'H', 'C', 'H', 'C', 'C', 'C', 'H', 'C', 'C', 'C', 'C', 'C', 'H', 'C', 'C', 'H', 'C', 'C', 'C', 'C', 'H', 'C', 'H'},
            {'C', 'P', 'H', 'H', 'C', 'C', 'H', 'H', 'C', 'C', 'C', 'H', 'H', 'C', 'H', 'H', 'H', 'C', 'C', 'H', 'C', 'H', 'C', 'H', 'H', 'C', 'C', 'H', 'C', 'C'}
    };

    int *mapa_3[5];
    for(int i = 0; i<5; i++) {
        mapa_3[i] = mapa[i];
    }

    int i, *cesta = zachran_princezne(mapa_3, n, m, 10, &dlzka);
    for (i=0;i<dlzka;++i)
        printf("%d %d\n", cesta[i*2], cesta[i*2+1]);
    //printf("%d\n", 1|2);
    return 0;
}
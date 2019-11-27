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
    short x;
    short y;
    unsigned short dist;
    unsigned short state;
    struct node *prev;
};


int is_gen_on(struct node* node) {
    unsigned short state = node -> state;
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
//short hsh[128][500][500];
short ***hsh;

void heap_insert(struct node **heap, int *heap_size, struct node *node){
    heap[*heap_size] = node;
    int index = *heap_size;
    (*heap_size)++;
    hsh[heap[index] -> state][heap[index] -> y][heap[index] -> x] = (*heap_size) - 1;

    while(1) {
        if(heap[(index-1)/2] -> dist > heap[index] -> dist) {
            hsh[heap[(index-1)/2] -> state][heap[(index-1)/2] -> y][heap[(index-1)/2] -> x] = index;
            hsh[heap[index] -> state][heap[index] -> y][heap[index] -> x] = (index-1)/2;
            struct node *temp = heap[index];
            heap[index] = heap[(index-1)/2];
            heap[(index-1)/2] = temp;
            index -=1;
            index /= 2;
        } else {
            break;
        }
    }

    hsh[node -> state][node -> y][node -> x] = index;
}

struct node* heap_min(struct node **heap, int *heap_size) {
    struct node* min = heap[0];
    int index = --(*heap_size);

    hsh[heap[0] -> state][heap[0] -> y][heap[0] -> x] = -1;
    heap[0] = heap[index];
    hsh[heap[index] -> state][heap[index] -> y][heap[index] -> x] = 0;
    heap[index] = NULL;

    int left_child;
    int right_child;
    int smaller;
    index = 0;

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

        hsh[heap[index] -> state][heap[index] -> y][heap[index] -> x] = smaller;
        hsh[heap[smaller] -> state][heap[smaller] -> y][heap[smaller] -> x] = index;
        struct node *temp = heap[index];
        heap[index] = heap[smaller];
        heap[smaller] = temp;
        index = smaller;
    }

    //printf("%d %d\n", hash[(heap[index] -> x) * (heap[index] -> y) * (heap[index] -> state)], index);
    return min;
}

int get_state(int p5, int p4, int p3, int p2, int p1, int drak, int generator) {
    unsigned short state = 0;
    if(generator != 0) {state |= 1;}
    if(drak != 0) {state |= 2;}
    if(p1 != 0) {state |= 4;}
    if(p2 != 0) {state |= 8;}
    if(p3 != 0) {state |= 16;}
    if(p4 != 0) {state |= 32;}
    if(p5 != 0) {state |= 64;}
    return state;
}

struct node* find(struct node**heap, int heap_size, int x, int y, unsigned short state, int *in) {
    if(hsh[state][y][x] == -1) {
        return NULL;
    } else {
        (*in) = hsh[state][y][x];
        return heap[hsh[state][y][x]];
    }

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

struct node* decrease_priority(struct node **heap, int heap_size, int x, int y, unsigned short state, int new_priority, struct node* prev) {
    int i;
    struct node* temp = find(heap, heap_size, x, y,state,&i);
    if(temp == NULL ) {
        return NULL;
    }

    if( temp -> dist < new_priority) {
        return temp;
    }
    temp -> prev = prev;
    temp -> dist = new_priority;
    while(1) {
        if(heap[(i-1)/2] -> dist > heap[i] -> dist) {
            hsh[heap[(i-1)/2] -> state][heap[(i-1)/2] -> y][heap[(i-1)/2] -> x] = i;
            hsh[heap[i] -> state][heap[i] -> y][heap[i] -> x] = (i-1)/2;
            struct node *temp = heap[i];
            heap[i] = heap[(i-1)/2];
            heap[(i-1)/2] = temp;
            i -=1;
            i /= 2;
        } else {
            break;
        }
    }
    return temp;

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
        case '8':
            return t8;
        case '9':
            return t9;
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

int get_n_from_state(unsigned short state) {
    int n = 0;
    n += (state & 64)/64 + (state & 32)/32 + (state & 16)/16 + (state & 8)/8 + (state & 4)/4;
    return n;
}

void next(char **mapa, int n, int m, int x, int y, unsigned short state, int xx, int yy,struct node **heap, int *heap_size, struct node* prev, int *princ_n) {
    struct node* temp;

    if(mapa[y+yy][x+xx] == 'C' || mapa[y+yy][x+xx] == 'H' || (mapa[y+yy][x+xx] == 'D'  && (state & 2) != 0) || (mapa[y+yy][x+xx] == 'P' &&  ((state & 2) == 0 || ((get_princess_mask(x+xx, y+yy) & state) !=  0))) || (mapa[y+yy][x+xx] >= '0' && mapa[y+yy][x+xx] <= '9' && (state & 1) == 1)) {
        temp = decrease_priority(heap, *heap_size, x+xx, y+yy, state,prev -> dist + get_cost(mapa, x, y), prev);

        if(temp == NULL){
            temp = malloc(sizeof(struct node));
            temp -> x = x+xx;
            temp -> y = y+yy;
            temp -> dist = prev -> dist + get_cost(mapa, x, y);
            temp -> prev = prev;
            temp -> state = state;
            heap_insert(heap, heap_size, temp);
        }
    } else if(mapa[y+yy][x+xx] == 'G' && (state & 1) == 0) {
        temp = decrease_priority(heap, *heap_size, x+xx, y+yy, state | 1, prev -> dist + get_cost(mapa, x, y), prev);

        if(temp == NULL){
            temp = malloc(sizeof(struct node));
            temp -> x = x+xx;
            temp -> y = y+yy;
            temp -> dist = prev -> dist + get_cost(mapa, x, y);
            temp -> prev = prev;
            temp -> state = state | 1;
            heap_insert(heap, heap_size, temp);
        }
    } else if(mapa[y+yy][x+xx] == 'D' && (state & 2) == 0) {
        temp = decrease_priority(heap, *heap_size, x+xx, y+yy, state | 2, prev -> dist + get_cost(mapa, x, y), prev);

        if(temp == NULL) {
            temp = malloc(sizeof(struct node));
            temp -> x = x+xx;
            temp -> y = y+yy;
            temp -> dist = prev -> dist + get_cost(mapa, x, y);
            temp -> prev = prev;
            temp -> state = state | 2;
            heap_insert(heap, heap_size, temp);
        }
    } else if(mapa[y+yy][x+xx] == 'P' &&  (state & 2) != 0 && ((get_princess_mask(x+xx, y+yy) & state) ==  0)) {
        temp = decrease_priority(heap, *heap_size, x+xx, y+yy, state | get_princess_mask(x+xx, y+yy), prev -> dist + get_cost(mapa, x, y), prev);

        if(temp == NULL) {
            temp = malloc(sizeof(struct node));
            temp -> x = x+xx;
            temp -> y = y+yy;
            temp -> dist = prev -> dist + get_cost(mapa, x, y);
            temp -> prev = prev;
            temp -> state = state | get_princess_mask(x+xx, y+yy);
            heap_insert(heap, heap_size, temp);
        }
    } else if(mapa[y+yy][x+xx] >= '0' && mapa[y+yy][x+xx] <= '9' && (state & 1) == 1) {
        struct teleport* t = find_teleport(mapa[y+yy][x+xx]);
        while(t != NULL) {
            int xxx = t -> x;
            int yyy = t -> y;
            temp = decrease_priority(heap, *heap_size, xxx, yyy, state, prev -> dist + get_cost(mapa, x, y), prev);
            if(temp == NULL){
                temp = malloc(sizeof(struct node));
                temp -> x = xxx;
                temp -> y = yyy;
                temp -> dist = prev -> dist + get_cost(mapa, x, y);
                temp -> prev = prev;
                temp -> state = state;
                heap_insert(heap, heap_size, temp);
            }
            t = t->next;
        }
    }


}

int* shortest_dijkstra(char **mapa, int n, int m, int princ_n, int *dlzka_cesty) {
    struct node **heap = (struct node**) malloc((n*m+1)*sizeof(struct node*));
    memset(heap, 0, (n*m+1)*sizeof(struct node*));
    int heap_size = 0;
    int x, y;
    unsigned int dist;
    unsigned short state;
    struct node* temp;

    temp = malloc(sizeof(struct node));
    temp -> x = 0;
    temp -> y = 0;
    temp -> prev = NULL;
    temp -> state = 0;
    temp -> dist = 0;
    heap_insert(heap, &heap_size, temp);
    char arr[128][n][m];
    memset(arr, 0, sizeof(arr));


    while(heap_size != 0) {
        struct node* min = heap[0];
        x = min -> x;
        y = min -> y;
        state = min -> state;
        dist = min -> dist;


        if(x < m-1 && arr[state][y][x] != 1) {
            next(mapa, n, m, x, y, min -> state, 1, 0, heap, &heap_size, min, &princ_n);
        }

        if(x > 0  && arr[state][y][x] != 1) {
            next(mapa, n, m, x, y, min -> state,-1, 0, heap, &heap_size, min, &princ_n);
        }

        if(y < n-1  && arr[state][y][x] != 1) {
            next(mapa, n, m, x, y, min -> state, 0, 1, heap, &heap_size, min, &princ_n);
        }

        if(y > 0  && arr[state][y][x] != 1) {
            next(mapa, n, m, x, y, min -> state,0, -1, heap, &heap_size, min, &princ_n);
        }
        if(princ_n == get_n_from_state(state) && mapa[y][x] == 'P') {

            (*dlzka_cesty) = 0;
            int *cesta = malloc(sizeof(int)*2500);
            int i = 0;
            while(min != NULL) {
                //printf("%d %d %u %d\n", min -> x, min -> y, min -> dist, min -> state);
                cesta[i] = min -> x;
                cesta[i+1] = min -> y;
                i += 2;
                min = min -> prev;
                (*dlzka_cesty)++;
            }

            return cesta;
        }

        heap_min(heap, &heap_size);
        arr[state][y][x] = 1;

    }


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
    hsh = (short***)malloc(sizeof(short**)*128);
    for(int j = 0; j < 128; j++) {
        hsh[j] =  (short**) malloc(n*sizeof(short*));
        for(int k = 0; k < n; k++) {
            hsh[j][k] =  (short*)malloc(m*sizeof(short));
            memset(hsh[j][k], -1, m*sizeof(short));
        }
    }



    memset(princesses, 0, sizeof(princesses));
    //memset(hsh, -1, sizeof(hsh));
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
    int m = 30; //30
    int dlzka = 0;
    /*
    char mapa[5][5] = {
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
    printf("%d\n", sizeof(hsh)/8/1024/1024);

    return 0;
}
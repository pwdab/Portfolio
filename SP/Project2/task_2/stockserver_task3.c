/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include <sys/time.h>
#include "csapp.h"

#define SBUFSIZE 1024
typedef struct {
    int *buf;                                   /* Buffer array */
    int n;                                      /* Maximum number of slots */
    int front;                                  /* buf[(front + 1) % n] is the first item */
    int rear;                                   /* buf[rear % n] is the last item */
    sem_t mutex;                                /* Protects accesses to buf */
    sem_t slots;                                /* Counts available slots */
    sem_t items;                                /* Counts available items */
} sbuf_t;

#define MAX_NODE 100
static int client_cnt = 0;
sem_t client_mutex;
static int end_cnt = 0;
sem_t end_mutex;
static int service_cnt = 0;
sem_t service_mutex;
static int start_flag = 0;
struct timeval startTime, endTime;

#define counter_goal 20
static float total_time = 0.0f;
static int counter = 0;

int node_cnt = 0;
typedef struct item {
    int ID;
    int left_stock;
    int price;
    int readcnt;
    struct item* left_Node;
    struct item* right_Node;
    sem_t mutex;
} itemNode;

FILE *fp;
static sem_t file_mutex;
sbuf_t sbuf;                                    /* Shared buffer of connected descriptors */
itemNode *root;

void sprint_Node(itemNode *Node, char* buf);
void fprint_Node(itemNode *Node, FILE* fp);
void print_Node(itemNode *Node);
itemNode* search_Node(itemNode* Node, int key);

void sbuf_init(sbuf_t *sp, int n);
void sbuf_deinit(sbuf_t *sp);
void sbuf_insert(sbuf_t *sp, int item);
int sbuf_remove(sbuf_t *sp);
void *thread(void *vargp);

int main(int argc, char **argv) {
    /* Stock file open */
    fp = fopen("stock.txt", "r");
    if (fp == NULL) {
        exit(0);
    }

    while (!feof(fp)) {
        /* Insert Node */
        itemNode *ptr;
        itemNode *new = (itemNode*)malloc(sizeof(itemNode));
        fscanf(fp, "%d %d %d\n", &new->ID, &new->left_stock, &new->price);
        new->readcnt = 0;
        new->left_Node = NULL;
        new->right_Node = NULL;
        sem_init(&new->mutex, 0, 1);     // 0 -> Lock, 1 -> Unlock

        if(root == NULL) {
            root = new;
        }
        else {
            ptr = root;
            while(ptr) {
                if(new->ID < ptr->ID) {
                    if(ptr->left_Node == NULL) {
                        ptr->left_Node = new;
                        break;
                    }
                    else {
                        ptr = ptr->left_Node;
                    }
                }
                else {
                    if(ptr->right_Node == NULL) {
                        ptr->right_Node = new;
                        break;
                    }
                    else {
                        ptr = ptr->right_Node;
                    }
                }
            }
        }
        node_cnt++;
    }

    fclose(fp);

    int listenfd, *connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;  /* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage
    char client_hostname[MAXLINE], client_port[MAXLINE];
    pthread_t tid;

    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
    }

    listenfd = Open_listenfd(argv[1]);
    sbuf_init(&sbuf, SBUFSIZE);
    Sem_init(&file_mutex, 0, 1);
    sem_init(&client_mutex, 0, 1);
    sem_init(&end_mutex, 0, 1);
    sem_init(&service_mutex, 0, 1);

    // Create a pool of worker threads /
    for (int i = 0; i < SBUFSIZE; i++) {
        Pthread_create(&tid, NULL, thread, NULL);
    }

    while (1) {
        // If listening descriptor is ready, add new client to pool /
        clientlen = sizeof(struct sockaddr_storage);
        connfd = (int*)malloc(sizeof(int));
        *connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        if(start_flag == 0) {
                gettimeofday(&startTime, NULL);
                start_flag++;
                //printf("Start\n");
        }
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, 
                    client_port, MAXLINE, 0);
        //printf("Connected to (%s, %s)\n", client_hostname, client_port);
        P(&client_mutex);
        client_cnt++;
        V(&client_mutex);
        // Insert connfd in buffer /
        sbuf_insert(&sbuf, *connfd);
    }
    exit(0);
    
}
/* $end echoserverimain */

void sprint_Node(itemNode *Node, char* buf) {
    char str[MAXLINE];

    if(Node != NULL) {
        sprint_Node(Node->left_Node, buf);

        P(&Node->mutex);
        sprintf(str, "%d %d %d\n", Node->ID, Node->left_stock, Node->price);
        strcat(buf, str);
        V(&Node->mutex);

        sprint_Node(Node->right_Node, buf);
    }
}

void fprint_Node(itemNode *Node, FILE* fp) {
    if(Node != NULL) {
        fprint_Node(Node->left_Node, fp);

        P(&Node->mutex);
        fprintf(fp, "%d %d %d\n", Node->ID, Node->left_stock, Node->price);
        V(&Node->mutex);

        fprint_Node(Node->right_Node, fp);
    }
}

void print_Node(itemNode *Node) {
    if(Node != NULL) {
        print_Node(Node->left_Node);
        printf("%d %d %d\n", Node->ID, Node->left_stock, Node->price);
        print_Node(Node->right_Node);
    }
}

itemNode* search_Node(itemNode* Node, int key) {
    if(Node == NULL) {
        return NULL;
    }

    if(Node->ID == key) {
        return Node;
    }
    else if(Node->ID > key) {
        search_Node(Node->left_Node, key);
    }
    else {
        search_Node(Node->right_Node, key);
    }
}

/* Create an empty, bounded, shared FIFO buffer with n slots */
void sbuf_init(sbuf_t *sp, int n) {
    sp->buf = (int *)Calloc(n, sizeof(int));
    sp->n = n;                                  /* Buffer holds max of n items */
    sp->front = sp->rear = 0;                   /* Empty buffer iff front == rear */
    Sem_init(&sp->mutex, 0, 1);                 /* Binary semaphore for locking */
    Sem_init(&sp->slots, 0, n);                 /* Initially, buf has n empty slots */   
    Sem_init(&sp->items, 0, 0);                 /* Initially, buf has 0 items */
}

/* Clean up buffer sp */
void sbuf_deinit(sbuf_t *sp) {
    Free(sp->buf);
}

/* Insert an item onto the rear of shared buffer sp */
void sbuf_insert(sbuf_t *sp, int item) {
    P(&sp->slots);                              /* Wait for available slot */
    P(&sp->mutex);                              /* Lock the buffer */
    sp->buf[(++sp->rear) % (sp->n)] = item;     /* Insert the item */
    V(&sp->mutex);                              /* Unlock the buffer */
    V(&sp->items);                              /* Announce available item */
}

/* Remove and return the first item from buffer sp */
int sbuf_remove(sbuf_t *sp) {
    int item;
    P(&sp->items);                              /* Wait for available item */
    P(&sp->mutex);                              /* Lock the buffer */
    item = sp->buf[(++sp->front) % (sp->n)];    /* Remove the item */
    V(&sp->mutex);                              /* Unlock the buffer */
    V(&sp->slots);                              /* Announce available slot */
    return item;
}

void *thread(void *vargp) {
    Pthread_detach(pthread_self());
    while(1) {
        /* Remove connfd from buf */
        int connfd = sbuf_remove(&sbuf);
        /* Service Client */
        //echo_cnt(connfd);
        int n;
        char buf[MAXLINE];
        rio_t rio;
        itemNode* ptr;

        Rio_readinitb(&rio, connfd);
        while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
            P(&service_mutex);
            service_cnt++;
            V(&service_mutex);
            //printf("thread %d received %d bytes on fd %d\n", (int)pthread_self(), n, connfd);
            if (!strncmp(buf, "show", 4)) {
                sprint_Node(root, buf);
            }
            else if(!strncmp(buf, "buy", 3)) {
                int buy_ID, buy_amount;
                sscanf(buf, "%*s %d %d\n", &buy_ID, &buy_amount);

                ptr = search_Node(root, buy_ID);
                //sem_wait(&Node[i].mutex);
                P(&ptr->mutex);
                if (ptr->left_stock >= buy_amount) {
                    ptr->left_stock -= buy_amount;
                    strcat(buf, "[buy] success\n");
                }
                else {
                    strcat(buf, "Not enough left stocks\n");
                }
                //sem_post(&Node[i].mutex);
                V(&ptr->mutex);
            }
            else if (!strncmp(buf, "sell", 4)) {
                int sell_ID, sell_amount;
                sscanf(buf, "%*s %d %d\n", &sell_ID, &sell_amount);

                ptr = search_Node(root, sell_ID);
                //sem_wait(&Node[i].mutex);
                P(&ptr->mutex);
                ptr->left_stock += sell_amount;
                strcat(buf, "[sell] success\n");
                //sem_post(&Node[i].mutex);
                V(&ptr->mutex);
            }
            Rio_writen(connfd, buf, MAXLINE);
        }
        /* EOF detected, remove descriptor from pool */
        P(&end_mutex);
        end_cnt++;
        V(&end_mutex);
        
        P(&file_mutex);
        fp = fopen("stock.txt", "w");
        fprint_Node(root, fp);
        fclose(fp);
        V(&file_mutex);

        Close(connfd);

        P(&end_mutex);
        P(&client_mutex);
        if((end_cnt != 0) && (end_cnt == client_cnt)) {
            gettimeofday(&endTime, NULL);
            printf("Total Clients : %d, ", end_cnt);
            printf("Total Services : %d, ", service_cnt);
            printf("%f s\n", ( endTime.tv_sec - startTime.tv_sec ) + ((float)( endTime.tv_usec - startTime.tv_usec ) / 1000000));
            total_time += ( endTime.tv_sec - startTime.tv_sec ) + ((float)( endTime.tv_usec - startTime.tv_usec ) / 1000000);

            start_flag = 0;
            client_cnt = 0;
            end_cnt = 0;
            service_cnt = 0;

            counter++;
        }
        V(&client_mutex);
        V(&end_mutex);

        if(counter == counter_goal) {
            printf("\nAverage Elapsed Time : %f\n", total_time / counter);
            counter++;
        }
    }
}
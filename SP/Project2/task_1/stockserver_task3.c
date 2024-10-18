/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include <sys/time.h>
#include "csapp.h"

int byte_cnt = 0;                           /* Counts total bytes received by server*/
typedef struct {                            /* Represents a pool of connected descriptors */
    int maxfd;                                  /* Largest descriptor in read_set*/
    fd_set read_set;                            /* Set of all active descriptors */
    fd_set ready_set;                           /* Subset of descriptors ready for reading */
    int nready;                                 /* Number of ready descriptors from select */
    int maxi;                                   /* High water index into client array */
    int clientfd[FD_SETSIZE];                   /* Set of active descriptors */
    rio_t clientrio[FD_SETSIZE];                /* Set of active read buffers */
} pool;

#define MAX_NODE 100
int client_cnt = 0;
sem_t client_mutex;
int end_cnt = 0;
sem_t end_mutex;
int service_cnt = 0;
sem_t service_mutex;

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

void sprint_Node(itemNode *Node, char* buf);
void fprint_Node(itemNode *Node, FILE* fp);
void print_Node(itemNode *Node);
itemNode* search_Node(itemNode* Node, int key);

void init_pool(int listenfd, pool *p);
void add_client(int connfd, pool *p);

int main(int argc, char **argv) {
    int start_flag = 0;
    struct timeval startTime, endTime;
    FILE *fp;
    sem_t file_mutex;
    itemNode* root = NULL;

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

        //printf("new->ID = %d\n", new->ID);
        //printf("new->left_stock = %d\n", new->left_stock);
        //printf("new->price = %d\n", new->price);

        if(root == NULL) {
            //printf("root is Null..\n");
            //printf("insert..\n");
            root = new;
        }
        else {
            //printf("root is not Null..\n");
            ptr = root;
            while(ptr) {
                if(new->ID < ptr->ID) {
                    //printf("new->ID(%d) < ptr->ID(%d)\n", new->ID, ptr->ID);
                    if(ptr->left_Node == NULL) {
                        //printf("ptr->left_Node is Null..\n");
                        //printf("insert..\n");
                        ptr->left_Node = new;
                        break;
                    }
                    else {
                        ptr = ptr->left_Node;
                    }
                }
                else {
                    //printf("new->ID(%d) > ptr->ID(%d)\n", new->ID, ptr->ID);
                    if(ptr->right_Node == NULL) {
                        //printf("ptr->right_Node is Null..\n");
                        //printf("insert..\n");
                        ptr->right_Node = new;
                        break;
                    }
                    else {
                        ptr = ptr->right_Node;
                    }
                }
            }
        }
        //printf("\n");
        node_cnt++;
    }
    //print_Node(root);

    fclose(fp);
    sem_init(&file_mutex, 0, 1);
    sem_init(&client_mutex, 0, 1);
    sem_init(&end_mutex, 0, 1);
    sem_init(&service_mutex, 0, 1);

    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;     /* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage
    char client_hostname[MAXLINE], client_port[MAXLINE];
    static pool pool;

    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
    }

    listenfd = Open_listenfd(argv[1]);
    init_pool(listenfd, &pool);

    while (1) {
        pool.ready_set = pool.read_set;     /* Wait for listening / connected descriptor(s) to become ready */
        pool.nready = Select(pool.maxfd + 1, &pool.ready_set, NULL, NULL, NULL);

        /* If listening descriptor is ready, add new client to pool */
        if (FD_ISSET(listenfd, &pool.ready_set)) {
            if(start_flag == 0) {
                gettimeofday(&startTime, NULL);
                start_flag++;
            }
            clientlen = sizeof(struct sockaddr_storage);
            connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
            Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, 
                        client_port, MAXLINE, 0);
            //printf("Connected to (%s, %s)\n", client_hostname, client_port);

            sem_wait(&client_mutex);
            client_cnt++;
            sem_post(&client_mutex);

            add_client(connfd, &pool);
        }

        /* Echo a text line from each ready connected descriptor */    
        int connfd, n;
        char buf[MAXLINE];
        rio_t rio;
        itemNode* ptr;

        for(int i = 0; (i <= pool.maxi) && (pool.nready > 0); i++) {
            connfd = pool.clientfd[i];
            rio = pool.clientrio[i];

            /* If the descriptor is ready, echo a text line from it */
            if ((connfd > 0) && (FD_ISSET(connfd, &pool.ready_set))) {
                pool.nready--;

                if ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
                    sem_wait(&service_mutex);
                    service_cnt++;
                    sem_post(&service_mutex);

                    byte_cnt += n;
                    //printf("server received %d bytes on fd %d\n", n, connfd);
                    if (!strncmp(buf, "show", 4)) {
                        sprint_Node(root, buf);
                    }
                    else if(!strncmp(buf, "buy", 3)) {
                        int buy_ID, buy_amount;
                        sscanf(buf, "%*s %d %d\n", &buy_ID, &buy_amount);

                        ptr = search_Node(root, buy_ID);
                        sem_wait(&ptr->mutex);
                        if (ptr->left_stock >= buy_amount) {
                            ptr->left_stock -= buy_amount;
                            strcat(buf, "[buy] success\n");
                        }
                        else {
                            strcat(buf, "Not enough left stocks\n");
                        }
                        sem_post(&ptr->mutex);
                    }
                    else if (!strncmp(buf, "sell", 4)) {
                        int sell_ID, sell_amount;
                        sscanf(buf, "%*s %d %d\n", &sell_ID, &sell_amount);

                        ptr = search_Node(root, sell_ID);
                        sem_wait(&ptr->mutex);
                        ptr->left_stock += sell_amount;
                        strcat(buf, "[sell] success\n");
                        sem_post(&ptr->mutex);
                    }
                    Rio_writen(connfd, buf, MAXLINE);
                }
                /* EOF detected, remove descriptor from pool */
                else {
                    sem_wait(&end_mutex);
                    end_cnt++;
                    sem_post(&end_mutex);

                    sem_wait(&file_mutex);
                    fp = fopen("stock.txt", "w");
                    fprint_Node(root, fp);
                    fclose(fp);
                    sem_post(&file_mutex);

                    Close(connfd);
                    FD_CLR(connfd, &pool.read_set);
                    pool.clientfd[i] = -1;

                    if((end_cnt != 0) && (end_cnt == client_cnt)) {
                        gettimeofday(&endTime, NULL);
                        printf("Total Clients : %d, ", end_cnt);
                        printf("Total Services : %d, ", service_cnt);
                        printf("%f s\n", ( endTime.tv_sec - startTime.tv_sec ) + ((float)( endTime.tv_usec - startTime.tv_usec ) / 1000000));
                        total_time += ( endTime.tv_sec - startTime.tv_sec ) + ((float)( endTime.tv_usec - startTime.tv_usec ) / 1000000);

                        start_flag = 0;
                        sem_wait(&client_mutex);
                        client_cnt = 0;
                        sem_post(&client_mutex);
                        sem_wait(&end_mutex);
                        end_cnt = 0;
                        sem_post(&end_mutex);
                        sem_wait(&service_mutex);
                        service_cnt = 0;
                        sem_post(&service_mutex);

                        counter++;
                    }

                    if(counter == counter_goal) {
                        printf("\nAverage Elapsed Time : %f\n", total_time / counter);
                    }
                }
            }
        }
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

void init_pool(int listenfd, pool *p) {
    /* Initially, there are no connected descriptors */
    p->maxi = -1;
    for (int i = 0; i < FD_SETSIZE; i++) {
        p->clientfd[i] = -1;
    }

    /* Initially, listenfd is only member of select read set */
    p->maxfd = listenfd;
    FD_ZERO(&p->read_set);
    FD_SET(listenfd, &p->read_set);
}

void add_client(int connfd, pool *p) {
    int i;
    p->nready--;
    /* Find an available slot */
    for (i = 0; i < FD_SETSIZE; i++) {
        /* If slot is empty */
        if (p->clientfd[i] < 0) {
            /* Add connected descriptor to the pool */
            p->clientfd[i] = connfd;
            Rio_readinitb(&p->clientrio[i], connfd);

            /* Add the descriptor to descriptor set */
            FD_SET(connfd, &p->read_set);

            /* Update max descriptor and pool high water mark */
            if (connfd > p->maxfd) {
                p->maxfd = connfd;
            }
            if ( i > p->maxi) {
                p->maxi = i;
            }
            break;
        }
    }
    
    /* Couldn't find an empty slot */
    if (i == FD_SETSIZE) {
        app_error("add_client error: Too many clients");
    }
}
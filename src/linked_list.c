#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINKED_LIST 20
typedef struct node_t_{
    int size;
    char *data;
    int key;
    struct node_t_ *next;
    struct node_t_ *prev;
}node_t;



int num_of_llists = 0;
node_t * heads[MAX_LINKED_LIST];
node_t * llist_indices[MAX_LINKED_LIST];
int test = 2;
int add_item_to_llist(int list_id, char *data, int size,int key){
    node_t *head = heads[list_id], *new_node;
    if(list_id >= MAX_LINKED_LIST) return -1;
    if(heads[list_id] == NULL) return -1;
    while(head->next != NULL)
        head = head->next;    
    
    new_node = malloc(sizeof(node_t));
    if(new_node == NULL) 
        return -1;
    new_node->next = NULL;
    new_node->data = malloc(size);
    if(new_node->data == NULL){
        free(new_node);
        return -1;
    }
    new_node->key = key;
    
    memcpy(new_node->data, data, size);
    
    head->next = new_node;    
    new_node->prev = head;
    return 0;
}

int delete_item_from_llist(int list_id, char *data, int size){

    return 0;
}


int create_llist(){
    if(num_of_llists > MAX_LINKED_LIST){
        return -1;
    }
    int i;
    for(i = 0; i < MAX_LINKED_LIST; i++){
        if(heads[i] == NULL){
            heads[i] = malloc(sizeof(node_t));
            llist_indices[i] = heads[i];
            break;
        }
    }
    if(i >= MAX_LINKED_LIST) return -2;
    num_of_llists++; 
    return i;
}

int destroy_list(int list_id){
    node_t *head, *temp;
    int num_of_deleted = 0;
    if(list_id >= MAX_LINKED_LIST)
        return -1;
    head = heads[list_id];

    while(head != NULL){
        temp = head->next;
        free(head->data);
        free(temp);
        head = temp;
        num_of_deleted++;
    }
    heads[list_id] = NULL;
    llist_indices[list_id] = NULL;
    return num_of_deleted;
}

int set_index_to_first(int list_id){
    if(list_id >= MAX_LINKED_LIST)
        return -1;
    if(heads[list_id] == NULL) 
        return -1;
    llist_indices[list_id] = heads[list_id];
    return 0;
}

char *get_next(int list_id, int *size){
    node_t *head = heads[list_id];
    node_t **current_index = &llist_indices[list_id];
    
    char *ret = NULL;
    if((head == NULL)||(current_index == NULL)){
        return NULL;
    }
    *size = (*current_index)->size;
    ret = (*current_index)->data;
    if((*current_index)->next != NULL){
        (*current_index) = (*current_index)->next;
        //printf("asd\n");
    }
    return ret;
}

char *get_from_key(int list_id, int key, int* size){
    node_t *head = heads[list_id];
    
    if(list_id >= MAX_LINKED_LIST) return NULL;
    if(heads[list_id] == NULL) return NULL;
    while(head->key != key){
        head = head->next;  
        if(head == NULL) return NULL;    
    }  
        printf("key\n");
    
    *size = head->size;
    return head->data;
}

char *get_prev(int list_id, int *size){
    node_t *head = heads[list_id];
    node_t **current_index = &llist_indices[list_id];
    
    char *ret = NULL;
    if((head == NULL)||(current_index == NULL)){
        return NULL;
    }
    *size = (*current_index)->size;
    ret = (*current_index)->data;
    if((*current_index)->next != NULL){
        (*current_index) = (*current_index)->prev;
        //printf("asd\n");
    }
    return ret;
}

int delete_last_item(int list_id){
    node_t *head = heads[list_id];
    if(list_id >= MAX_LINKED_LIST) return -1;
    if(heads[list_id] == NULL) return -1;
    while(head->next != NULL)
        head = head->next;    
    
    if(llist_indices[list_id] == head)
        llist_indices[list_id] = head->prev;

    head->prev->next = NULL;
    free(head->data);
    free(head);

    return 0;
}

int delete_first_item(int list_id){
    node_t  *temp ;
    if(list_id >= MAX_LINKED_LIST) return -1;
    if(heads[list_id] == NULL) return -1;
    if(heads[list_id]->next != NULL){
        if(heads[list_id]->next->next != NULL){
            temp = heads[list_id]->next->next;
            free(heads[list_id]->next->data);
            free(heads[list_id]->next);
            heads[list_id]->next = temp;
        }else{
            free(heads[list_id]->next->data);
            free(heads[list_id]->next);
            heads[list_id]->next = NULL;
        }
    } 

    return 0;
}


int times = 0;
int main2(){

    

    //printf("%p\n",heads[0]);
    int llist_array[200];

    

    for(int i = 0; i < 30; i++){
        llist_array[i] = create_llist();
        if(llist_array[i] >= 0){
            //printf("able to create a list %d\n",llist_array[i]);
        }else{
            //printf("not able to create a llsit\n");
        }
    }
    printf("created llists\n");
    for(int i = 0; i < 30; i++){
        for(int j = 0; j < 200; j++){
            int ret = add_item_to_llist(i, (char*)&j, sizeof(int),0);
            if(ret == 0){
                //printf("able to add to list to %dth list %d\n", i, j);
            }else{
                //printf("not able to add to list to %dth list %d\n", i, j);
            }
        }
    }
    printf("added to llists\n");
    int size;
    for(int i = 0; i < 30; i++){
        for(int j = 0; j < 200; j++){
            char *ret = get_next(i, &size);
            if(ret != NULL){
                //printf("readed from %dth linked list %dth try %d data %p\n",i,j,*(int*)ret, ret);
            }else{
                printf("empty list %d \n",i);
            }
        }
    }
    printf("readed from llists\n");
    for(int i = 0; i < 30; i++){
        destroy_list(i);
    }
    printf("destryoed llists\n");
    for(int i = 0; i < 30; i++){
        for(int j = 0; j < 200; j++){
            add_item_to_llist(i, (char*)&j, 4,0);
        }
    }
    times++;
    printf("times %d\n", times);

    return 0;
}

int delete_tests(){
    int i =  create_llist(), size;
    for(int j = 0; j < 20; j++){
        int ret = add_item_to_llist(i, (char*)&j, sizeof(int),0);
        if(ret == 0){
                printf("able to add to list to %dth list %d\n", i, j);
        }else{
                //printf("not able to add to list to %dth list %d\n", i, j);
        }
    }
    for(int t = 0; t < 21; t++){
        set_index_to_first(i);
        for(int j = 0; j < 21; j++){
            char *ret = get_next(i, &size);
            if(ret != NULL){
                printf("readed from %dth linked list %dth try %d data %p\n",i,j,*(int*)ret, ret);
            }else{
                printf("empty list %d \n",i);
            }
        }
        delete_first_item(i);
    }
    return 0;
}
/*
int main(){
    delete_tests();




return 0;
}
*/

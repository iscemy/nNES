
int create_llist();
int destroy_list(int list_id);
int delete_last_item(int list_id);
int add_item_to_llist(int list_id, char *data, int size,int key);
char *get_next(int list_id, int *size);
char *get_from_key(int list_id, int key, int* size);

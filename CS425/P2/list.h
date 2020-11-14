struct DLL{
    struct Node* head;
    struct Node* tail ;
    
    int count;
};

struct Node{
    struct Node *next;
    struct BPHead* val;
    struct Node *prev;
};

void initializeList(struct DLL* DLLlist){
    DLLlist->head = NULL;
    DLLlist->tail = NULL;
    DLLlist->count = 0;
    
}

void add(struct DLL* DLLlist, struct BPHead* addSeg){
     int internalCounter = 0;
     
     if(DLLlist->count == 0){
        DLLlist->head = malloc(sizeof(struct Node));
        DLLlist->tail = DLLlist->head;
        DLLlist->head->next = NULL;
        DLLlist->head->prev = NULL;

    }
    else if(DLLlist->count == 1){
        DLLlist->head = malloc(sizeof(struct Node));
        DLLlist->head->next = DLLlist->tail;
      
        DLLlist->tail->prev = DLLlist->head;
        DLLlist->head->prev = NULL;
        DLLlist->tail->next = NULL;
    }
    else{
      struct Node* temp = DLLlist->head;
      DLLlist->head = malloc(sizeof(struct Node));
      DLLlist->head->next = temp;
      temp->prev = DLLlist->head;
      temp->next = DLLlist->tail;
      DLLlist->tail->prev = temp;
      
    }
     DLLlist->head->val = malloc(sizeof(struct BPHead));
    *DLLlist->head->val = *addSeg;
    DLLlist->count = DLLlist->count + 1; 

   

    //sort((orderedList), count);

}
/**
 * If for some reason we received out of order we need to just check the list and ensure that it is sorted.
 */
void sort(int **orderedList, int *count){
    int *begList = *orderedList;
    for(int i = 1; i < *count; i++){
        int key = begList[i];
        int j = i - 1;

    while(j>=0 && begList[j] < key){
        begList[j+1] = begList[j];
        j = j-1;
    }
    begList[j+1] = key;

    }
}

void clearList(int **orderedList, int *count){
    for(int i = 0; i < *count; i++){
        free(*(orderedList + i));
    }
    *count = 0;
}

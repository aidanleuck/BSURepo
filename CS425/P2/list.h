struct DLL
{
    struct Node *head;
    struct Node *tail;

    int count;
};

struct Node
{
    struct Node *next;
    struct BPHead *val;
    struct Node *prev;
};

void initializeList(struct DLL *DLLlist)
{
    DLLlist->head = NULL;
    DLLlist->tail = NULL;
    DLLlist->count = 0;
}

void addToFront(struct DLL *DLLlist, struct BPHead *addSeg)
{

    if (DLLlist->count == 0)
    {
        DLLlist->head = malloc(sizeof(struct Node));
        DLLlist->tail = DLLlist->head;
        DLLlist->head->next = NULL;
        DLLlist->head->prev = NULL;
    }
    else if (DLLlist->count == 1)
    {
        DLLlist->head = malloc(sizeof(struct Node));
        DLLlist->head->next = DLLlist->tail;

        DLLlist->tail->prev = DLLlist->head;
        DLLlist->head->prev = NULL;
        DLLlist->tail->next = NULL;
    }
    else
    {
        struct Node *temp = DLLlist->head;
        DLLlist->head = malloc(sizeof(struct Node));
        DLLlist->head->next = temp;
        DLLlist->head->prev = NULL;
        temp->prev = DLLlist->head;
    }

    DLLlist->head->val = malloc(sizeof(struct BPHead));
    *DLLlist->head->val = *addSeg;
    DLLlist->count = DLLlist->count + 1;

    //sort((orderedList), count);
}
/**
 * If for some reason we received out of order we need to just check the list and ensure that it is sorted.
 */
void sort(struct DLL *list)
{

    struct Node *currValue = list->head->next;
    struct Node *sortPointer = list->head;

    while (currValue != NULL)
    {

        while (currValue != NULL)
        {
            struct Node tempSort = *sortPointer;
            struct Node tempCurr = *currValue;
            if (currValue->val->segNum > sortPointer->val->segNum)
            {
                printf("%d", currValue->val->segNum);

                if (sortPointer->next != currValue)
                {
                    currValue->prev->next = tempCurr.next;
                    if (sortPointer == list->head && currValue != list->tail)
                    {
                        list->head = currValue;
                        currValue->next->prev = currValue->prev;
                    }
                    else if (currValue == list->tail && sortPointer != list->head)
                    {
                        list->tail = currValue->prev;
                        sortPointer->prev->next = currValue;
                    }
                    else if (currValue == list->tail && sortPointer == list->head)
                    {
                        list->tail = currValue->prev;
                        list->head = currValue;
                    }
                    else
                    {
                        sortPointer->prev->next = currValue;

                        currValue->next->prev = currValue->prev;
                    }
                }
                else
                {
                    sortPointer->next = tempCurr.next;

                    if (sortPointer == list->head && currValue != list->tail)
                    {
                        list->head = currValue;
                        currValue->next->prev = sortPointer;
                    }
                    else if (currValue == list->tail && sortPointer != list->head)
                    {
                        list->tail = tempCurr.prev;
                        sortPointer->prev->next = currValue;
                    }
                    else if (currValue == list->tail && sortPointer == list->head)
                    {
                        list->tail = currValue->prev;
                        list->head = currValue;
                    }
                    else
                    {
                        sortPointer->prev->next = currValue;
                        currValue->next->prev = sortPointer;
                    }
                }

                sortPointer->prev = currValue;
                currValue->next = sortPointer;
                currValue->prev = tempSort.prev;

                sortPointer = currValue;
            }

            currValue = tempCurr.next;
        }
        sortPointer = sortPointer->next;
        currValue = sortPointer->next;
    }
}

void clearList(struct DLL *list)
{
    struct Node *curr = list->head;
    for (int i = 0; i < list->count; i++)
    {
        free(curr->val);
        struct Node *remove = curr;
        curr = curr->next;
        free(remove);
    }

    list->count = 0;
}

struct Node *getPointer(struct DLL *list, struct BPHead *addSeg)
{
    struct Node *curr = list->head;
    while (curr != NULL)
    {
        if (addSeg->segNum == curr->val->segNum)
        {
            return curr;
        }
        else
        {
            curr = curr->next;
        }
    }
    return curr;
}
void removeSegs(struct DLL *list, struct Node *pointer)
{
    list->tail = pointer->prev;
    if (pointer->prev)
    {
        pointer->prev->next = NULL;
    }
    while (pointer != NULL)
    {
        struct Node temp = *pointer;
        free(pointer->val);
        free(pointer);
        pointer = temp.next;
        list->count = list->count - 1;
    }
}
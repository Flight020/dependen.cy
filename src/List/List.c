#include "List.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct sList
{
    int iNodeCount;
    int iNodeDataSize;
    sNode_t* head;
    sNode_t* tail;
    fListCompare compareNodes;
    fListFreeNode freeNode;
}sList_t;

typedef struct sNode
{
    void* data;
    sNode_t* next;
    sNode_t* previous;
} sNode_t;

static sNode_t* ListCreateNode(sList_t* list, void* data);
static sNode_t* ListFind(sList_t* list, void* data);

sList_t* ListCreate(int dataSize, fListCompare compareCallback, fListFreeNode freeNodeCallback)
{
    if(dataSize == 0)
    {
        return NULL;
    }
    sList_t* newList = malloc(sizeof(sList_t));
    if(newList == NULL)
    {
        return NULL;
    }

    newList->iNodeCount = 0;
    newList->iNodeDataSize = dataSize;
    newList->compareNodes = compareCallback;
    newList->freeNode = freeNodeCallback;
    return newList;
}

/// @brief  Allocates an array for the address of the data element of each node.
//          The last element is NULL 
/// @param list 
/// @return Address of first data element
void** ListGetIterator(sList_t* list)
{
    if(list == NULL || list->iNodeCount || list->head == NULL)
    {
        return NULL;
    }

    ListIterator_t iteratorArray = calloc(sizeof(void*), (list->iNodeCount + 1)); // plus one to have one empty at the end
    sNode_t* node = list->head;
    int count = 0;
    iteratorArray[count++] = node->data;
    while(node->next != NULL)
    {
        node = node->next;
        iteratorArray[count++] = node->data;
    }
    return iteratorArray;
}

/// @brief Removes the first node of the list where data equals the nodes data
/// @param list 
/// @param data 
/// @return true if a node was removed, false else
bool ListRemove(sList_t* list, void* data)
{
    if(list == NULL || data == NULL || list->head == NULL || list->iNodeCount == 0)
    {
        return false;
    }

    bool match = false;
    sNode_t* node = list->head;
    do
    {
        //Use custom compare function if available
        if(list->compareNodes != NULL)
        {
            match = list->compareNodes(node->data, data);
        }
        else
        {
            match = node->data == data;
        }

        if(match)
        {
            sNode_t* prev = node->previous;
            sNode_t* next = node->next;
            if(next == NULL)//Tail
            {
                prev->next = NULL;
                list->tail = prev;
            }
            else if(prev == NULL) //Head
            {
                next->previous == NULL;
                list->head = next;
            }
            else
            {
                prev->next = next;
                next->previous = prev;
            }
            //Free data elemnt and memory of node itself
            if(list->freeNode != NULL)
            {
                list->freeNode(node->data);
            }
            else
            {
                free(node->data);
            }
            free(node);
            list->iNodeCount--;
            return true;
        }
        node = node->next;
    } while (node != NULL);
    return false; //No node to remove
}

/// @brief Add the given data element as new node to the end of the list
/// @param list 
/// @param data 
/// @return true if a new node was added, false else
bool ListAppend(sList_t* list, void* data)
{
    sNode_t* newNode = ListCreateNode(list, data);
    if (newNode == NULL) {
        return false;
    }

    if (list->head == NULL) {
        // Empty list, set the new node as both head and tail
        list->head = newNode;
        list->tail = newNode;
    } else {
        // Append the new node to the tail
        list->tail->next = newNode;
        newNode->previous = list->tail;
        list->tail = newNode;
    }

    list->iNodeCount++;
    return true;
}

/// @brief Add the given data element as new node to the start of the list
/// @param list 
/// @param data 
/// @return true if a new node was added, false else
bool ListPrepend(sList_t* list, void* data)
{
    sNode_t* newNode = ListCreateNode(list, data);
    if(newNode == NULL)
    {
        return false;
    }
    
    if(list->head == NULL)
    {
        //Empty List
        list->head = newNode;
        list->tail = newNode;
    }
    else
    {
        newNode->next = list->head;
        list->head = newNode;
        newNode->next->previous = newNode;
    }
    list->iNodeCount++;
}

/// @brief Insert data as new node after the node containing the target data
/// @param list 
/// @param target 
/// @param data 
/// @return true if a new node was added, false else
bool ListInsertAfter(sList_t* list, void* target, void* data)
{
    if(target == NULL)
    {
        return false;
    }
    sNode_t* newNode = ListCreateNode(list, data);
    if(newNode == NULL)
    {
        return false;
    }

    sNode_t* targetNode = ListFind(list, target);
    if(targetNode == NULL)
    {
        return false;
    }

    if(targetNode->next == NULL) //Tail
    {
        targetNode->next = newNode;
        newNode->previous = targetNode;
        list->tail = newNode;
    }
    else
    {
        newNode->previous = targetNode;
        newNode->next = targetNode->next;
        targetNode->next = newNode;
        newNode->next->previous = newNode;
    }
    list->iNodeCount++;
    return true;
}

/// @brief Insert data as new node before the given target node
/// @param list 
/// @param target 
/// @param data 
/// @return true if a new node was added, false else
bool ListInsertBefore(sList_t* list, void* target, void* data)
{
    if(target == NULL)
    {
        return false;
    }
    sNode_t* newNode = ListCreateNode(list, data);
    if(newNode == NULL)
    {
        return false;
    }

    sNode_t* targetNode = ListFind(list, target);
    if(targetNode == NULL)
    {
        return false;
    }

    if(targetNode->previous == NULL) //Head
    {
        targetNode->previous = newNode;
        newNode->next = targetNode;
        list->head = newNode;
    }
    else
    {
        newNode->previous = targetNode->previous;
        newNode->next = targetNode;
        targetNode->previous = newNode;
        newNode->previous->next = newNode;
    }
    list->iNodeCount++;
    return true;
}

/// @brief Returns
/// @param list
/// @param data
/// @return Returns the count of nodes that contain the same data, -1 in case of error
int ListContains(sList_t* list, void* data)
{
    if(list == NULL || data == NULL)
    {
        return -1;
    }

    if(list->head == NULL)
    {
        return -1; //Empty list
    }

    bool match = false;
    sNode_t* node = list->head;
    int count = 0;
    do
    {
        ///Use custom compare function if available
        if(list->compareNodes != NULL)
        {
            match = list->compareNodes(node->data, data);
        }
        else
        {
            match = node->data == data;
        }
        if(match)
        {
            count++;
            match = false;
        }
    } while (node != NULL);
    
    
}
sNode_t* ListFind(sList_t* list, void* data);
int ListRemoveDuplicates(sList_t* list);
bool ListFree(sList_t* list);


void* ListGetNodeData(sNode_t* node);
bool ListSetNodeData(sNode_t* node);

static sNode_t* ListCreateNode(sList_t* list, void* data)
{
    if(list == NULL || data == NULL)
    {
        return NULL;
    }

    sNode_t* newNode = malloc(sizeof(sNode_t));
    if(newNode == NULL)
    {
        return NULL;
    }

    newNode->data = malloc(list->iNodeDataSize);
    if(newNode->data == NULL)
    {
        return NULL;
    }
    return newNode;
}
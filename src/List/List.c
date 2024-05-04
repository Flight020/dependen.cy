#include "List.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static sNode_t* ListCreateNode(int size, void* data);
static sNode_t* ListFind(sList_t* list, void* data);

/// @brief Allocates a list which can be used as container for list nodes
/// @param dataSize size of the data element a list node
/// @param compareCallback pointer to custom compare function regarding the data element
/// @param freeNodeCallback  pointer to custom free function regarding the data element
/// @return returns a reference to the allocated 
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
    newList->head = NULL;
    newList->tail = NULL;
    return newList;
}

/// @brief  Allocates an array containing the address of the data element of each node. The last element is NULL 
/// @param list 
/// @return Address of first data element
void* ListGetIterator(sList_t* list)
{
    if(list == NULL || list->iNodeCount == 0 || list->head == NULL)
    {
        return NULL;
    }

    void* iteratorArray = calloc((list->iNodeCount + 1), list->iNodeDataSize); // plus one to have one empty at the end
    sNode_t* node = list->head;
    char* ptr;
    int count = 0;

    ptr = iteratorArray;
    memcpy(ptr, node->data, list->iNodeDataSize);
    while(node->next != NULL)
    {
        node = node->next;
        ptr += list->iNodeDataSize;
        memcpy(ptr, node->data, list->iNodeDataSize);
    }
    return iteratorArray;
}

bool ListFreeIterator(ListIterator_t iterator)
{
    if(iterator != NULL)
    {
        free(iterator);
        return(true);
    }
    else
    {
        return false;
    }
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
            match = *((int*)node->data) == *((int*)data);
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
    if(list == NULL)
    {
        return false;
    }
    sNode_t* newNode = ListCreateNode(list->iNodeDataSize, data);
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
    if(list == NULL)
    {
        return false;
    }
    sNode_t* newNode = ListCreateNode(list->iNodeDataSize, data);
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
    if(target == NULL || list == NULL)
    {
        return false;
    }
    sNode_t* newNode = ListCreateNode(list->iNodeDataSize, data);
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
    if(target == NULL || list == NULL)
    {
        return false;
    }
    sNode_t* newNode = ListCreateNode(list->iNodeDataSize, data);
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
            match = *((int*)node->data) == *((int*)data);
        }
        if(match)
        {
            count++;
            match = false;
        }
        node = node->next;
    } while (node != NULL);
    
    return count;
}

/// @brief parses each node and removed any duplicates
/// @param list 
/// @return count of removed duplicates, -1 in case of error
int ListRemoveDuplicates(sList_t* list)
{
    int removeCount = 0;
    if(list == NULL)
    {
        return -1;
    }

    bool match = false;
    sNode_t* referenceNode = list->head;
    sNode_t* compareNode;
    sNode_t* nextNode;
    while(referenceNode != NULL)
    {
        compareNode = referenceNode->next;
        while(compareNode != NULL)
        {
            //Use custom compare function if available
            if(list->compareNodes != NULL)
            {
                match = list->compareNodes(referenceNode->data, compareNode->data);
            }
            else
            {
                match = *((int*)referenceNode->data) == *((int*)compareNode->data);
            }

            if(match) //Duplicate found
            {
                nextNode = compareNode->next;
                if(compareNode->next == NULL) //Tail
                {
                    list->tail = compareNode->previous;
                    compareNode->previous->next = NULL;
                }
                else
                {
                    compareNode->previous->next = compareNode->next;
                    compareNode->next->previous = compareNode->previous;
                }
                //Free data elemnt and memory of node itself
                if(list->freeNode != NULL)
                {
                    list->freeNode(compareNode->data);
                }
                else
                {
                    free(compareNode->data);
                }
                free(compareNode);
                list->iNodeCount--;
                removeCount++;
                compareNode = nextNode;
            }
            else
            {
                compareNode = compareNode->next;
            } 
        }
        referenceNode = referenceNode->next;
    }
    return removeCount;
}

/// @brief frees the memory reserved for a list and its nodes
/// @param list 
/// @return true if successful, false if not
bool ListFree(sList_t* list)
{
    if(list == NULL)
    {
        return false;
    }

    if(list->head != NULL)
    {
        sNode_t* node = list->head;
        sNode_t* nextNode;
        while(node != NULL)
        {
            //Free data elemnt and memory of node itself
            if(list->freeNode != NULL)
            {
                list->freeNode(node->data);
            }
            else
            {
                free(node->data);
            }
            nextNode = node->next;
            free(node);
            list->iNodeCount--;
            node = nextNode;
        }
    }
    free(list);
    list = NULL;
return true;
}

/// @brief Returns the count of nodes of the list
/// @param list 
/// @return count of nodes, -1 if no valid list is given
int ListGetCount(sList_t* list)
{
    if(list == NULL)
    {
        return -1;
    }

    return list->iNodeCount;
}

/// @brief Returns the size of the data elemnts of list
/// @param list 
/// @return Size in bytes, -1 if no valid list is given
int ListGetDataSize(sList_t* list)
{
    if(list == NULL)
    {
        return -1;
    }

    return list->iNodeDataSize;
}

static sNode_t* ListCreateNode(int size, void* data)
{
    if(size == 0 || data == NULL)
    {
        return NULL;
    }

    sNode_t* newNode = malloc(sizeof(sNode_t));
    if(newNode == NULL)
    {
        return NULL;
    }

    newNode->data = malloc(size);
    if(newNode->data == NULL)
    {
        return NULL;
    }
    memcpy(newNode->data, data, size);
    newNode->next = NULL;
    newNode->previous = NULL;
    return newNode;
}

static sNode_t* ListFind(sList_t* list, void* data)
{
    if(list == NULL || data == NULL)
    {
        return NULL;
    }

    if(list->head == NULL || list->iNodeCount == 0)
    {
        return NULL;
    }

    bool match = false;
    sNode_t* node = list->head;
    while(node != NULL)
    {
        //Use custom compare function if available
        if(list->compareNodes != NULL)
        {
            match = list->compareNodes(node->data, data);
        }
        else
        {
            match = *((int*)node->data) == *((int*)data);
        }

        if(match)
        {
            return node;
        }
        else
        {
            node = node->next;
        }
    }
    return NULL;
}
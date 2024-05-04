/*******************************************************************************
 *    INCLUDED FILES
 ******************************************************************************/
 #include <stdio.h>
//-- unity: unit test framework
#include "unity.h"

//-- module being tested
#include "List.h"
 
 
/*******************************************************************************
 *    DEFINITIONS
 ******************************************************************************/
 
/*******************************************************************************
 *    PRIVATE TYPES
 ******************************************************************************/
 
/*******************************************************************************
 *    PRIVATE DATA
 ******************************************************************************/

 
/*******************************************************************************
 *    PRIVATE FUNCTIONS
 ******************************************************************************/
 
 
/*******************************************************************************
 *    SETUP, TEARDOWN
 ******************************************************************************/
 
void setUp(void)
{
}
 
void tearDown(void)
{
    
}
 
/*******************************************************************************
 *    TESTS
 ******************************************************************************/
 
void test_ListCreate_Valid(void)
{
    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);
    TEST_ASSERT_TRUE(ListFree(list));
    list = NULL;
}


void test_ListCreate_Invalid(void)
{
    sList_t* list = ListCreate(0, NULL, NULL);
    TEST_ASSERT_EQUAL(list, NULL);
}


void test_ListAppend_Valid(void)
{
    int a = 1;
    int b = 2;

    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_TRUE(ListAppend(list, (void*)&a));
    TEST_ASSERT_TRUE(ListAppend(list, (void*)&b));

    TEST_ASSERT_EQUAL(1, ListContains(list, &a));
    TEST_ASSERT_EQUAL(1, ListContains(list, &b));

    
    TEST_ASSERT_TRUE(ListFree(list));
    list = NULL;
}

void test_ListAppend_Valid2(void)
{
    int a = 1;

    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_TRUE(ListAppend(list, (void*)&a));
    TEST_ASSERT_TRUE(ListAppend(list, (void*)&a));

    TEST_ASSERT_EQUAL(2, ListContains(list, &a));

    
    TEST_ASSERT_TRUE(ListFree(list));
    
}

void test_ListAppend_IsAtTail(void)
{
    int a = 1;
    int b = 2;

    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_TRUE(ListAppend(list, (void*)&a));
    TEST_ASSERT_TRUE(ListAppend(list, (void*)&b));

    ListIterator_t iterator = ListGetIterator(list);
    int* it = (int*)iterator;
    TEST_ASSERT_EQUAL(1, *it);
    it++;
    TEST_ASSERT_EQUAL(2, *it);
    
    TEST_ASSERT_TRUE(ListFreeIterator(iterator));
    TEST_ASSERT_TRUE(ListFree(list));
}

void test_ListAppend_Invalid(void)
{
    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_FALSE(ListAppend(NULL, list));

    TEST_ASSERT_TRUE(ListFree(list));
}

void test_ListAppend_Invalid2(void)
{
    int a = 1;
    TEST_ASSERT_FALSE(ListAppend((void*)&a, NULL));
}

void test_ListPrepend_Valid(void)
{
    int a = 1;
    int b = 2;

    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_TRUE(ListPrepend(list, (void*)&a));
    TEST_ASSERT_TRUE(ListPrepend(list, (void*)&b));

    TEST_ASSERT_EQUAL(1, ListContains(list, &a));
    TEST_ASSERT_EQUAL(1, ListContains(list, &b));

    
    TEST_ASSERT_TRUE(ListFree(list));
    list = NULL;
}

void test_ListPrepend_Valid2(void)
{
    int a = 1;

    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_TRUE(ListPrepend(list, (void*)&a));
    TEST_ASSERT_TRUE(ListPrepend(list, (void*)&a));

    TEST_ASSERT_EQUAL(2, ListContains(list, &a));

    
    TEST_ASSERT_TRUE(ListFree(list));
    
}

void test_ListPrepend_IsAtHead(void)
{
    int a = 1;
    int b = 2;

    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_TRUE(ListPrepend(list, (void*)&a));
    TEST_ASSERT_TRUE(ListPrepend(list, (void*)&b));

    ListIterator_t iterator = ListGetIterator(list);
    int* it = (int*)iterator;
    TEST_ASSERT_EQUAL(2, *it);
    it++;
    TEST_ASSERT_EQUAL(1, *it);
    
    TEST_ASSERT_TRUE(ListFreeIterator(iterator));
    TEST_ASSERT_TRUE(ListFree(list));
}

void test_ListPrepend_Invalid(void)
{
    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_FALSE(ListPrepend(NULL, list));

    TEST_ASSERT_TRUE(ListFree(list));
}

void test_ListPrepend_Invalid2(void)
{
    int a = 1;
    TEST_ASSERT_FALSE(ListPrepend((void*)&a, NULL));
}

void test_ListInsertAfter_Invalid(void)
{
    int a = 10;
    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_FALSE(ListInsertAfter(list, NULL, (void*)&a));

    TEST_ASSERT_TRUE(ListFree(list));
}

void test_ListInsertAfter_Invalid2(void)
{
    int a = 10;
    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_FALSE(ListInsertAfter(list, (void*)&a, NULL));

    TEST_ASSERT_TRUE(ListFree(list));
}

void test_ListInsertAfter_Invalid3(void)
{
    int a = 10;

    TEST_ASSERT_FALSE(ListInsertAfter(NULL, (void*)&a, (void*)&a));
}

void test_ListInsertAfter_Invalid4(void)
{
    int a = 10;
    int b = 5;
    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_FALSE(ListInsertAfter(list, (void*)&b, (void*)&a));

    TEST_ASSERT_TRUE(ListFree(list));
}

void test_ListInsertAfter_Valid(void)
{
    int a = 10;
    int b = 5;
    int c = 13;
    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_TRUE(ListAppend(list, (void*)&a));

    TEST_ASSERT_TRUE(ListInsertAfter(list, (void*)&a, (void*)&b));
    TEST_ASSERT_TRUE(ListInsertAfter(list, (void*)&a, (void*)&c));

    int* iterator = (int*)ListGetIterator(list);
    TEST_ASSERT_NOT_NULL(iterator);

    TEST_ASSERT_EQUAL(a, iterator[0]);
    TEST_ASSERT_EQUAL(c, iterator[1]);
    TEST_ASSERT_EQUAL(b, iterator[2]);

    ListFreeIterator(iterator);
    TEST_ASSERT_TRUE(ListFree(list));
}

void test_ListInsertBefore_Invalid(void)
{
    int a = 10;
    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_FALSE(ListInsertBefore(list, NULL, (void*)&a));

    TEST_ASSERT_TRUE(ListFree(list));
}

void test_ListInsertBefore_Invalid2(void)
{
    int a = 10;
    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_FALSE(ListInsertBefore(list, (void*)&a, NULL));

    TEST_ASSERT_TRUE(ListFree(list));
}

void test_ListInsertBefore_Invalid3(void)
{
    int a = 10;

    TEST_ASSERT_FALSE(ListInsertBefore(NULL, (void*)&a, (void*)&a));
}

void test_ListInsertBefore_Invalid4(void)
{
    int a = 10;
    int b = 5;
    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_FALSE(ListInsertBefore(list, (void*)&b, (void*)&a));

    TEST_ASSERT_TRUE(ListFree(list));
}

void test_ListInsertBefore_Valid(void)
{
    int a = 10;
    int b = 5;
    int c = 13;
    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_TRUE(ListAppend(list, (void*)&a));

    TEST_ASSERT_TRUE(ListInsertBefore(list, (void*)&a, (void*)&b));
    TEST_ASSERT_TRUE(ListInsertBefore(list, (void*)&a, (void*)&c));

    int* iterator = (int*)ListGetIterator(list);
    TEST_ASSERT_NOT_NULL(iterator);

    TEST_ASSERT_EQUAL(a, iterator[2]);
    TEST_ASSERT_EQUAL(c, iterator[1]);
    TEST_ASSERT_EQUAL(b, iterator[0]);

    ListFreeIterator(iterator);
    TEST_ASSERT_TRUE(ListFree(list));
}

void test_ListContains_Invalid(void)
{
    int a = 10;

    TEST_ASSERT_EQUAL(-1, ListContains(NULL, (void*)&a));
}

void test_ListContains_Invalid2(void)
{
    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_EQUAL(-1, ListContains(list, NULL));

    TEST_ASSERT_TRUE(ListFree(list));
}

void test_ListContains_Valid_None(void)
{
    int a = 5;
    int b = 10;

    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_TRUE(ListAppend(list, &a));

    TEST_ASSERT_EQUAL(0, ListContains(list, &b));

    TEST_ASSERT_TRUE(ListFree(list));
}

void test_ListContains_Valid_One(void)
{
    int a = 5;
    int b = 10;

    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_TRUE(ListAppend(list, &a));
    TEST_ASSERT_TRUE(ListAppend(list, &b));

    TEST_ASSERT_EQUAL(1, ListContains(list, &b));

    TEST_ASSERT_TRUE(ListFree(list));
}

void test_ListContains_Valid_Several(void)
{
    int a = 5;
    int b = 10;

    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_TRUE(ListAppend(list, &b));
    TEST_ASSERT_TRUE(ListAppend(list, &a));
    TEST_ASSERT_TRUE(ListAppend(list, &b));

    TEST_ASSERT_EQUAL(2, ListContains(list, &b));

    TEST_ASSERT_TRUE(ListFree(list));
}

void test_ListRemoveDuplicates_Invalid(void)
{
    TEST_ASSERT_EQUAL(-1, ListRemoveDuplicates(NULL));
}

void test_ListRemoveDuplicates_Valid_None(void)
{
    int a = 5;
    int b = 10;

    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_TRUE(ListAppend(list, &a));
    TEST_ASSERT_TRUE(ListAppend(list, &b));

    TEST_ASSERT_EQUAL(0, ListRemoveDuplicates(list));

    TEST_ASSERT_TRUE(ListFree(list));
}

void test_ListRemoveDuplicates_Valid_One(void)
{
    int a = 5;
    int b = 10;

    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_TRUE(ListAppend(list, &a));
    TEST_ASSERT_TRUE(ListAppend(list, &b));
    TEST_ASSERT_TRUE(ListAppend(list, &b));

    TEST_ASSERT_EQUAL(1, ListRemoveDuplicates(list));
    TEST_ASSERT_EQUAL(1, ListContains(list, &a));
    TEST_ASSERT_EQUAL(1, ListContains(list, &b));

    TEST_ASSERT_TRUE(ListFree(list));
}

void test_ListRemoveDuplicates_Valid_Several(void)
{
    int a = 5;
    int b = 10;

    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(list, NULL);

    TEST_ASSERT_TRUE(ListAppend(list, &b));
    TEST_ASSERT_TRUE(ListAppend(list, &a));
    TEST_ASSERT_TRUE(ListAppend(list, &b));
    TEST_ASSERT_TRUE(ListAppend(list, &b));

    TEST_ASSERT_EQUAL(2, ListRemoveDuplicates(list));

    TEST_ASSERT_EQUAL(1, ListContains(list, &a));
    TEST_ASSERT_EQUAL(1, ListContains(list, &b));

    TEST_ASSERT_TRUE(ListFree(list));
}

void test_ListRemove_Invalid(void)
{
    int a = 5;

    TEST_ASSERT_FALSE(ListRemove(NULL, &a));
}

void test_ListRemove_Invalid2(void)
{
    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_NULL(list);

    TEST_ASSERT_FALSE(ListRemove(list, NULL));

    TEST_ASSERT_TRUE(ListFree(list));
}

void test_ListRemove_Valid(void)
{
    int a = 5;
    int b = 10;
    sList_t* list = ListCreate(sizeof(int), NULL, NULL);
    TEST_ASSERT_NOT_NULL(list);

    ListAppend(list, &a);
    ListAppend(list, &b);
    ListAppend(list, &b);
    ListAppend(list, &a);

    TEST_ASSERT_TRUE(ListRemove(list, &b));

    int* iterator = (int*)ListGetIterator(list);

    TEST_ASSERT_EQUAL(a, iterator[0]);
    TEST_ASSERT_EQUAL(b, iterator[1]);
    TEST_ASSERT_EQUAL(a, iterator[2]);

    TEST_ASSERT_EQUAL(3, ListGetCount(list));

    ListFreeIterator(iterator);
    TEST_ASSERT_TRUE(ListFree(list));
}

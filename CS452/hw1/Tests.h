#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "deq.h"

typedef enum  {PUT, GET, REM, ith}TestType;
typedef enum {Head, Tail} End;

typedef struct TestStep{
    End end;
    TestType type;
    int data;
}*TestStep;
typedef struct Test{
    TestStep* steps;
    int count;
    char *expected;
}*Test;

char* printInt(Data d);
bool checkHead(char, char);
bool checkTail(char, char);
bool compareEntireString(char*, char*);
void freeInt(Data d);
Test GenerateTest(TestType*, End*, int*, char*, int);
bool runTest();

bool Test_Put_AddToHeadEmptyList();
bool Test_Put_AddToHead2Item();
bool Test_Put_AddToHead3Item();

bool Test_Get_AddToHead2ItemGetHead();


bool Test_Put_AddToTailEmptyList();
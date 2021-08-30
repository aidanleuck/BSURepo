#include "Tests.h"

Test GenerateTest(TestType *type, End *ends, int* data, char* expected, int stepCount){   
    
    Test testList = malloc(sizeof(struct Test));
    testList->steps = malloc(sizeof(TestStep));
    TestStep* currentStep = malloc(sizeof(TestStep));
    *currentStep = malloc(sizeof(struct TestStep) * stepCount);
    TestStep stepHead = *currentStep;

    for(int i =0; i< stepCount; i++){

        TestType currentType = *type;
        TestType end = *ends;

        (*currentStep)->type = currentType;
        (*currentStep)->end = end;

        // Iterates the pointers to get the next value
        type += 1;
        ends += 1;
        if(currentType != GET){
            (*currentStep)->data = *data;
            data += 1;
        }
        
        *currentStep += 1;
    }
    *currentStep = stepHead;
     testList->steps = currentStep;
     testList->expected = expected;
     testList->count = stepCount;


     return testList;
    
}

bool runTest(Test testList){

    Deq q = deq_new();
    int** data = malloc(sizeof(int**));
    *data = malloc(sizeof(int*) * testList->count);
    for(int i = 0; i < testList->count; i++){
        struct TestStep step = *(*(testList->steps));
        *(data + i) = &(*(testList->steps))->data;

    
        if(step.type == PUT){
          
            if(step.end == Head){
                deq_head_put(q, *(data + i));
            }
            else{
                deq_tail_put(q, *(data + i));
            }
        }
        else if(step.type == GET){
            if(step.end == Head){
                deq_head_get(q);
            }
            else{
                deq_tail_get(q);
            }
        }
        else if(step.type == ith){
            if(step.end == Head){
                deq_head_ith(q, *(*(data + i)));
            }
            else{
                deq_tail_ith(q, *(*(data + i)));
            }
        }
        else if(step.type = REM){
            if(step.end = Head){
                deq_head_rem(q, *(data + i));
            }
            else{
                deq_tail_rem(q, *(data + i));
            }
            
        }

        *(testList->steps) = (*(testList->steps)+ 1); 


    }

    Data head = deq_head_ith(q, 0);
    Data tail = deq_tail_ith(q, 0);

    char headChar= printInt(head)[0];
    char tailChar = printInt(tail)[0];

    char expectedHead = testList->expected[0];
    char expectedTail = testList->expected[strlen(testList->expected)-1];

    checkHead(headChar, expectedHead);
    checkTail(tailChar, expectedTail);

    char* actual = deq_str(q, printInt);

    compareEntireString(actual, testList->expected);

    return false;
}

char* printInt(Data d){
  int value = *((int*)(d));
  int size = sizeof(char)/sizeof(value);
  char *string = malloc(sizeof(size));

  sprintf(string, "%d", value);

  return string;
}

bool compareEntireString(char* actual, char* expected){
    if(strcmp(actual, expected) == 0){
        printf("%s", "String comparison passed\n");
        return true;
    }
    fprintf(stderr,"%s%s%s%s", "String comparison failed\n Actual:", actual, "\nExpected:", expected);
    return false;
}

bool checkHead(char actual, char expected){

    if(actual == expected){
        printf("%s", "Head check passed\n");
        return true;
    }
    fprintf(stderr,"%s%c%s%c", "Actual head ", actual, "is not equal to expected\n", expected);
    return false;

}

bool checkTail(char actual, char expected){
    if(actual == expected){
        printf("%s", "Tail check passed\n");
        return true;
    }
    fprintf(stderr,"%s%c%s%c", "Actual tail ", actual, "is not equal to expected\n", expected);
    return false;

}

bool Test_Put_AddToHeadEmptyList(){

   printf("%s", "\nRunning function : Test_Put_AddToHeadEmptyList\n");
   int stepCount = 1;
   int data[] = {5};
   char* expected = "5";

   TestType types[] = {PUT};
   End end[] = {Head};

   Test test = GenerateTest(types, end, data, expected, stepCount);
   runTest(test);
}
bool Test_Put_AddToHead2Item(){

   printf("%s", "\nRunning function : Test_Put_AddToHead2Item\n");
   int stepCount = 2;
   int data[] = {5, 6};
   char* expected = "6 5";

   TestType types[] = {PUT, PUT};
   End end[] = {Head, Head};

   Test test = GenerateTest(types, end, data, expected, stepCount);
   runTest(test);
}
bool Test_Put_AddToHead3Item(){

   printf("%s", "\nRunning function : Test_Put_AddToHead3Item\n");
   int stepCount = 3;
   int data[] = {5, 6, 7};
   char* expected = "7 6 5";

   TestType types[] = {PUT, PUT, PUT};
   End end[] = {Head, Head, Head};

   Test test = GenerateTest(types, end, data, expected, stepCount);
   runTest(test);
}

bool Test_Get_AddToHead2ItemGetHead(){

   printf("%s", "\nRunning function : Test_Get_AddToHead2ItemGetHead\n");
   int stepCount = 3;
   int data[] = {5, 6};
   char* expected = "5";

   TestType types[] = {PUT, PUT, GET};
   End end[] = {Head, Head, Head};

   Test test = GenerateTest(types, end, data, expected, stepCount);
   runTest(test);
}






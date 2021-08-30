#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>

#include "Tests.h"


int main() {
  Test_Put_AddToHeadEmptyList();
  Test_Put_AddToHead2Item();
  Test_Put_AddToHead3Item();

  
  Test_Get_AddToHead2ItemGetHead();
}



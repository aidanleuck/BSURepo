#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"
#include "balloc.h"
static Balloc bp=0;


int main(){
 bp=bp ? bp : bnew(4096,4,12);
 printf("HELLO");

}


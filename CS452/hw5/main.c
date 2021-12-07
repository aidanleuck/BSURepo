#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"
#include "balloc.h"
#include "bitmap.h"
#include "freelist.h"

int main(){
Balloc b = bnew(4096, 2, 12);
void* addr = balloc(b, 100);
void* addr2 = balloc(b, 200);


}


#include <stdio.h>
#include <stdlib.h>
#include "common.h"

void exit_on_error(int status, const char *message) {
    if (status < 0) { 
        perror(message);
        exit(1); 
    }
}
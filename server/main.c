//
// Created by cristian-roman on 9/7/23.
//

#include <stdio.h>
#include "./custom-libraries/myLogger/myLogger.h"

int main() {

    InitMyLogger();
    FILE *fp;
    fp = fopen(CURRENT_LOG_FILE_PATH, "w+");
    fprintf(fp, "BUILD STARTED AT: %s\n", GetCurrentBuildTime());
}
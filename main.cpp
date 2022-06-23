#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <string>
#include <cstring>
#include <vector>
#include <ctime>
#include <queue>
#include <sqlite3.h>
#include <iostream>
#include "todo.h"
using namespace std;

int main(int argc, char * argv[]) {

#ifdef DEBUG
    fprintf(stdout, "\n:::::::: DEBUG ::::::::\n");
    fprintf(stdout, "arc: %d\n", argc);
    for (int i = 0; i < argc; ++i) {
        fprintf(stdout, "argv[%d]: %s\n", i, argv[i]);
    }
#endif
    sqlite3_open("todo.db", &db);
    char opt;
    while ((opt = getopt(argc, argv, "h")) != -1) {
        usage();
        sqlite3_close(db);
        return 0; // 发完Usage就下班
    }

    if (argc == 0) {
        usage();
        sqlite3_close(db);
        return 0;
    }


    args2cmdseq(argc - 1, argv + 1);
    if (cmdseq.front() == "run") {
        cmdseq.pop();
        run(1);
    }
    else {
        run(0);
    }
    sqlite3_close(db);
    return 0;
}
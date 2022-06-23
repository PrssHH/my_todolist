#ifndef TODO_H
#define TODO_H

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
using namespace std;




extern queue<string> cmdseq;

extern sqlite3 *db;
extern char *errmsg;
extern bool some_thing_wrong;

extern char PRIO[][4];
extern char CLAS[][7];
extern char DONE[];
extern int count_row;
extern vector <int> id2eid;


void usage();

void print_cmdseq();
void clear_cmdseq();
void args2cmdseq(int argc, char **argv);
void input2cmdseq();

static int callback(void *data, int argc, char **argv, char **azColName);
static int value_callback(void *data, int argc, char **argv, char **azColName);
bool task_empty(int uid);
int db_login(string user_name, string encryped_password);

int login(int interact);

void creat_user_table(int uid);


static int taskshow_callback(void *data, int argc, char **argv, char **azColName);
void showtask(int uid, int print_out);

string cmdseq_to_sql_time();
void addtask(int uid, int interact);

void donetask(int uid, int interact);

void deltask(int uid, int interact);

int handle(int uid, int interact);
void run(int interact);

#endif
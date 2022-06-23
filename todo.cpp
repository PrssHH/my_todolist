#ifndef TODO_C
#define TODO_C
#include "todo.h"
#include "passwd.h"


queue<string> cmdseq;

sqlite3 *db = NULL;
char *errmsg;
bool some_thing_wrong = false;

char PRIO[][4] = {"高", "中", "低"};
char CLAS[][7] = {"学习", "娱乐", "生活"};
char DONE[] = "  完成  ";
int count_row = 0;
vector <int> id2eid;

void print_cmdseq() {
    while (cmdseq.size()) {
        cout << cmdseq.front() << endl;
        cmdseq.pop();
    }
}

void clear_cmdseq() {
    queue<string> empty;
    swap(empty, cmdseq);
}

void args2cmdseq(int argc, char **argv) {
    string tmp = "";
    for (int i = 0; i < argc; ++i) {
        tmp = argv[i];
        cmdseq.push(tmp);

    }
}

void input2cmdseq() {
    string tmp = ""; char c;
    while(1) {
        cin >> tmp;
        cmdseq.push(tmp);
        c = getchar();
        if (c == '\n') {
            break;
        }
    }
}





static int callback(void *data, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    } 
    printf("\n");
    return 0;
}

static int value_callback(void *data, int argc, char **argv, char **azColName) {
    int *ptr = (int*)data;
    *ptr = atoi(argv[0]);
    return 0;
}

bool task_empty(int uid) {
    string sentence = "SELECT COUNT(*) FROM TB" + to_string(uid) + ";";
    int cnt;
    sqlite3_exec(db, sentence.c_str(), value_callback, &cnt, &errmsg);
    return !cnt;
}







int db_login(string user_name, string encryped_password) {
    int uid;

    sqlite3_stmt *stmt = NULL;
    string sentence = "SELECT * FROM USER WHERE NICK_NAME == '" + user_name
                    + "' AND PASSWORD == '" + encryped_password + "';";
    // cout << sentence << endl;

#ifdef DEBUG
    cout << sentence << endl;
#endif

    int rc;
    rc = sqlite3_prepare_v2(db, sentence.c_str(), -1, &stmt, NULL);

#ifdef DEBUG
    printf("DEBUG：登录数据库打开完毕 %d\n", rc);
#endif


    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            unsigned char uuid = *sqlite3_column_text(stmt, 0);
            uid = uuid - '0';
        }
        else {
            fprintf(stderr, "用户名或密码错误\n");
            return -1;
        }
    }
    else {
        fprintf(stderr, "查询数据库失败: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    return uid;
}


int login(int interact) {
    
    
    string user_name = "";
    string origin_password = "";
    string encryped_password = "";
    if (interact == 0 && cmdseq.size() < 2) {
        fprintf(stderr, "ERROR: 登录参数数量不正确\n");
        return -1; // single mode 参数数量不对 直接截了不交互
    }

    if (cmdseq.size() == 0) {
        fprintf(stdout, "请输入用户名: ");
        input2cmdseq();
    }
    user_name = cmdseq.front();
    cmdseq.pop();
    
    if (cmdseq.size() == 0) {
        fprintf(stdout, "请输入密码: ");
        input2cmdseq();
    }
    origin_password = cmdseq.front();
    cmdseq.pop();
    encryped_password = passwd(origin_password);


#ifdef DEBUG
    printf("\n:::::::: DEBUG ::::::::\n获取用户名与密码\n");
    cout << "USERNAME: " << user_name << endl;
    cout << "ORIGIN PASSWORD: " << origin_password << endl;
    cout << "ENCRYPT PASSWORD: " << encryped_password << endl;
#endif


    int uid = db_login(user_name, encryped_password);
    return uid;
}



void creat_user_table(int uid) {
    string sentence = "CREATE TABLE IF NOT EXISTS TB" + to_string(uid) 
                    + "(EID INT PRIMARY KEY UNIQUE, DONE INT, NAME TEXT, SETTIME TEXT, PRIO INT, CLASS INT, ALTIME TEXT);";
    int rc = sqlite3_exec(db, sentence.c_str(), callback, 0, &errmsg);
    if (rc != SQLITE_OK) {
        cout << errmsg << endl;
    }
}


static int taskshow_callback(void *data, int argc, char **argv, char **azColName){
    count_row++;
    id2eid.push_back(atoi(argv[0]));
    if (*(int*)data == 0) {
        // 传参表示只建表不打印
        return 0;
    }


    if (count_row == 1) {
        printf("|%-8s|%-9s|%-22s|%-20s|%-8s|%-8s|%-20s|\n", "事项ID", "完成状况", "事项", "设定时间", "优先级", "分类", "提醒时间");
        // id done name time prio class alarm
        printf("|------|--------|--------------------|----------------|------|------|----------------|\n");
    }
    argv[3][16] = 0;
    argv[6][16] = 0;
    int len = strlen(argv[2]);
    int cnt = 0;
    for (char *ptr = argv[2]; *ptr; ++ptr) {
        if (isascii(*ptr)) {
            cnt++;
        }
    }
    int cnchar = (len - cnt) / 3;
    int screen_len = cnchar * 2 + cnt;
    char emptystr[] = "";
    if (atoi(argv[1])) {
        argv[1] = DONE;
    }
    else {
        argv[1] = emptystr;
    }
    printf("|%-6d|%-8s|", count_row, argv[1]);
    printf("%-s", argv[2]);
    for (int i = 0; i < 20 - screen_len; ++i) {
        printf(" ");
    }
    printf("|%-16s|%-7s|%-8s|%-16s|\n", argv[3], PRIO[atoi(argv[4])], CLAS[atoi(argv[5])], argv[6]);
    return 0;
}


void showtask(int uid, int print_out) {
    string sentence = "SELECT * FROM TB" + to_string(uid) + " ORDER BY DONE, SETTIME, PRIO ASC;";
                                                            // 未完成，开始时间，优先级，分类。
    count_row = 0;
    id2eid.clear();
    sqlite3_exec(db, sentence.c_str(), taskshow_callback, &print_out, &errmsg);
    if (count_row == 0) {
        printf("没有任何计划事项捏。\n");
    }
}



string cmdseq_to_sql_time() {
    string input_str = cmdseq.front();
    cmdseq.pop();
    string ans = "2022-00-00 08:00:00.000";
    bool is_time = false;
    string::size_type pos;

    pos = input_str.find("_");
    if (pos == input_str.npos) {
        is_time = false;
    }
    else {
        is_time = true;
    }

    string ddmmyyyy = "", hhmm = "";
    if (is_time) {
        ddmmyyyy = input_str.substr(0, pos);
        hhmm = input_str.substr(pos + 1);
    }
    else {
        ddmmyyyy = input_str;
    }
#ifdef DEBUG
    cout << input_str << endl;
    cout << ddmmyyyy << endl;
    cout << hhmm << endl;
#endif

    ans[8] = ddmmyyyy[0];
    ans[9] = ddmmyyyy[1];

    ans[5] = ddmmyyyy[3];
    ans[6] = ddmmyyyy[4];


    if (ddmmyyyy.length() > 5) {
        ans[0] = ddmmyyyy[6];
        ans[1] = ddmmyyyy[7];
        ans[2] = ddmmyyyy[8];
        ans[3] = ddmmyyyy[9];
    }

    if (is_time) {
        ans[11] = hhmm[0];
        ans[12] = hhmm[1];
        if (hhmm.length() > 3) {
            ans[14] = hhmm[3];
            ans[15] = hhmm[4];
        }
    }
    return ans;
}

void addtask(int uid, int interact) {
    if (interact == 0 && cmdseq.size() < 2) {
        fprintf(stderr, "ERROR: 参数数量不正确");
        sqlite3_close(db);
        exit(0);
    }

    int meid;
    string sentence;
    if (task_empty(uid)) {
        meid = 0;
    }
    else {
        sentence = "SELECT MAX(EID) FROM TB" + to_string(uid) + ";";
        sqlite3_exec(db, sentence.c_str(), value_callback, &meid, &errmsg);
    }

    string EID = to_string(meid + 1);
    string TASKNAME = "";
    
    if (cmdseq.size() == 0) {
        fprintf(stdout, "请输入事项名称：");
        input2cmdseq();
    }
    TASKNAME = cmdseq.front();
    cmdseq.pop();

    if (cmdseq.size() == 0) {
        fprintf(stdout, "请输入事项时间：");
        input2cmdseq();
    }
    string SETTIME = cmdseq_to_sql_time();
    
#ifdef DEBUG
    puts("时间搞定了");
    cout << SETTIME << endl;
#endif

    int prio = 2;
    if (cmdseq.size() > 0) {
        if (cmdseq.front() == "高") {
            prio = 0;
        }
        if (cmdseq.front() == "中") {
            prio = 1;
        }
        if (cmdseq.front() == "低") {
            prio = 2;
        }
    }

    int clas = 2;
    if (cmdseq.size() > 0) {
        if (cmdseq.front() == "学习") {
            clas = 0;
        }
        if (cmdseq.front() == "娱乐") {
            clas = 1;
        }
        if (cmdseq.front() == "生活") {
            clas = 2;
        }
        cmdseq.pop();
    }

    string ALTIME = SETTIME;
    if (cmdseq.size() > 0) {
        ALTIME = cmdseq_to_sql_time();
    }

    sentence = "INSERT INTO TB" + to_string(uid) + " VALUES ("
                    + EID + ",0 , '" + TASKNAME + "', '" + SETTIME + "', "
                    + to_string(prio) + ", " + to_string(clas) + ", '" + ALTIME + "');";
    sqlite3_exec(db, sentence.c_str(), callback, 0, &errmsg);
    printf("加入成功！\n");
    count_row = 0;
}




void donetask(int uid, int interact) {
    if (interact == 0 && cmdseq.size() == 0) {
        fprintf(stderr, "ERROR: 参数数量不正确\n");
        return ;
    }

    if (cmdseq.size() == 0) {
        fprintf(stdout, "请输入完成的事项id: ");
        input2cmdseq();
    }
    int id = stoi(cmdseq.front());
    cmdseq.pop();

    if (count_row == 0) {
        showtask(uid, 0);
    }

    string sentence = "UPDATE TB" + to_string(uid) + " SET DONE = 1 WHERE EID = " + to_string(id2eid[id - 1]) + ";";
    sqlite3_exec(db, sentence.c_str(), callback, 0, &errmsg);
    printf("登记成功！\n");
    count_row = 0;
}

void deltask(int uid, int interact) {
    if (interact == 0 && cmdseq.size() == 0) {
        fprintf(stderr, "ERROR: 参数数量不正确\n");
        return ;
    }

    if (cmdseq.size() == 0) {
        fprintf(stdout, "请输入完成的事项id: ");
        input2cmdseq();
    }

    if (count_row == 0) {
        showtask(uid, 0);
    }

    int id = stoi(cmdseq.front());
    cmdseq.pop();
    puts("del _ pooooop");
    string sentence = "DELETE FROM TB" + to_string(uid) + " WHERE EID = " + to_string(id2eid[id - 1]) + ";";
    
#ifdef DEBUG
    printf("del读取id完成\n");
    cout << sentence << endl;
#endif

    int respond = sqlite3_exec(db, sentence.c_str(), callback, 0, &errmsg);
    if (respond == 0) {
        printf("删除成功！\n");
    }
    else {
        fprintf(stderr, "ERROR: 删除事项失败 %s\n", errmsg);
    }
    count_row = 0;
}

int handle(int uid, int interact) {
    if (cmdseq.size() == 0) return 0;
    string cmd = cmdseq.front();
    cmdseq.pop();
    if (cmd == "exit") return 1;
    if (cmd == "showtask") showtask(uid, 1);
    if (cmd == "donetask") donetask(uid, interact);
    if (cmd == "deltask") deltask(uid, interact);
    if (cmd == "addtask") addtask(uid, interact);
    return 0;
}

void run(int interact) {
    int uid;
    uid = login(interact);
    if (uid == -1) {
        return ;
    }
    creat_user_table(uid);
    if (interact) {
        printf(".........登录成功，程序开始运行.........\n");
    }
    if (interact == 0) {
        handle(uid, interact);
    }
    while (interact) {
        printf("$ ");
        if (cmdseq.size() == 0) input2cmdseq();
        if (handle(uid, interact)) break;
    }
}



void usage() {
    string str = 
"使用方式: \n"
"   程序分为单次调用和前台多次使用两种模式: \n"
"\n"
"前台多次使用: \n"
"  *  登录软件\n"
"       $ ./todo run\n"
"       $ ./todo run [用户名]\n"
"       $ ./todo run [用户名] [用户密码]\n"
"       若未直接输入用户密码则后续会要求用户输入\n"
"  *  使用软件\n"
"       1. 查看计划事项: 以时间、优先级为序显示所有计划事项\n"
"            $ showtask\n"
"       2. 添加计划事项: \n"
"            $ addtask [任务名称] [任务时间] [优先级] [任务类型] [提醒时间]\n"
"            [任务名称]: 任意字符串(长度限制20), 必填\n"
"            [任务时间]: 格式为dd/mm[/yyyy]_[hh][:mm][:ss], [日期/月份/年份_小时:分钟:秒], 必填\n"
"                    例: 13/07/2022  表示 2022年 7月 13日\n"
"                        13/07       表示 2022年 7月 13日\n"
"                        14/08_12:00 表示 2022年 8月 14日 12时 00分\n"
"                        14/08_12    表示 2022年 8月 14日 12时 00分\n"
"                   其中 [日期/月份] 必填\n"
"                        [年份]      缺省为 2022年\n"
"                        [小时:分钟] 缺省为当天 08时 00分\n"
"                        [分钟]      缺省设定小时的 00分\n"
"                        [秒]        缺省为 00秒\n"
"            [优先级]:   低 | 中 | 高\n"
"                        缺省值为“低”\n"
"            [任务类型]: 学习 | 娱乐 | 生活\n"
"                        缺省值为“生活”\n"
"            [提醒时间]：选择在任务开始前提醒用户。时间格式同 [任务时间]\n"
"                        若不填，缺省值为“任务时间”，即在设定的任务时间提醒\n"
"       3.  完成计划事项: \n"
"             $ donetask [事项id]\n"
"           其中 [事项id] 会在“查看计划事项”中给出\n"
"       4.  删除计划事项: \n"
"             $ deltask [事项id]\n"
"           其中 [事项id] 会在“查看计划事项”中给出\n"
"           !!! 注意：此操作无法恢复\n"
"  *  退出软件\n"
"       $ exit\n"
"\n"
"单次调用: \n"
"    单次调用请严格遵循调用方式, 若参数不正确会直接由缺省值替代或抛出错误\n"
"  *  使用方式：\n"
"       $ ./todo [用户名] [密码] [操作] <参数>\n"
"  *  查看事项：\n"
"       [操作]: showtask\n"
"       <参数> 为空\n"
"  *  添加事项：\n"
"       [操作]: addtask\n"
"       <参数>: [任务名称] [任务时间] [优先级] [任务类型] [提醒时间]\n"
"       具体格式参照前文\n"
"  *  完成事项：\n"
"       [操作]: donetask\n"
"       <参数>: [事项id]\n"
"  *  删除事项：\n"
"       [操作]: deltask\n"
"       <参数>: [事项id]\n"
"\n";
    cout << str;
}

#endif
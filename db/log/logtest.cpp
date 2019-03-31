
#include "Log.h"
#include <iostream>
#include <string>
using lmmdb::log::Log;
using lmmdb::globalVar::OpLog;
using lmmdb::globalVar::Op;
using std::endl;
using std::cout;
using std::cin;
int main(void) {
    Log* log = new Log();

    if (log->createLog("./log/log.txt", true) ) {
        cout << "log create success" << endl;
        //get log
        log->writeLog(1, "ABC", "");
        log->writeLog(Op::GET, "222", "");
        //write log
        log->writeLog(2, "hhhh", "hhhhhh");
        log->writeLog(2, "write_log", "good");
        //del
        log->writeLog(3, "key", "NULL");
        log->writeLog(3, "key", "NULL");
        //SCAN
        log->writeLog(4, "scan", "scan");
    }
    else {

    }
    return 0;
}
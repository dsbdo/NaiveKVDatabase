// this class is used to create log

#ifndef DB_LOG_H_
#define DB_LOG_H_

#include <string>
#include "../global.h"
#include <fstream>
#include <iostream>

namespace lmmdb {
    namespace log {
    class Log{
        public:
        // 是否指定每一条log，sync一次，<性能，可靠性>的衡量
        ~Log();
        bool createLog(std::string file_path, bool is_sync = false); 
        //通过存储映射I/O，标准I/O, 文件I/O, 提供写操作
        bool writeLog(int, std::string, std::string);
        
        // 修改文件状态
        void changeFileStatus(bool change_current_file, int status, std::string new_log_file = "");

        // 设置一条log 刷新一次到磁盘
        void setSync(bool is_sync);
        static Log* getInstance();
        private:
        Log();
        Log(const Log&);
        Log& operator=(const Log&);
        bool emitPhysicalRecord();
        static Log* instance;
        // 是否写一条log 就fflush 或者 sync一次， 还是等到达到一定大小时sync一次
        bool one_log_one_sync_;
        
        // 如果memTable满就将current file -> 移动到当前状态
        std::fstream* old_log_file_;

        // 指着当前的log fiie
        std::fstream* current_log_file_;


        //用来标志当前log文件到达那一个状态了
        int current_file_status_;
        int old_file_status_;
        
    };
    }

}
#endif //DB_LOG_H_
// this class is used to create log

#ifndef DB_LOG_H_
#define DB_LOG_H_

#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
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
        bool emitPhysicalRecord();
        
        // 设置一条log 刷新一次到磁盘
        void setSync(bool is_sync);

        static Log* getInstance();
        private:
        Log();
        Log(const Log&);
        Log& operator=(const Log&);
        static Log* instance;
        //是否写一条log 就fflush 或者 sync一次， 还是等到达到一定大小时sync一次
        bool one_log_one_sync_;
        std::fstream log_file_;
        
    };
    }

}
#endif //DB_LOG_H_
//log 实现

#include "Log.h"

namespace lmmdb {
    namespace log {


        Log::Log() {
            one_log_one_sync_ = false;

        }
        Log::Log(const Log& temp) {}
        Log& Log::operator=(const Log& temp) {}
        
        //初始化
        Log* Log::instance = new Log();

        Log* Log::getInstance() {
            return instance;
        }

        void Log::setSync(bool is_sync) {
            one_log_one_sync_ = is_sync;
        }


        // Log::Log(std::string file_path, bool is_sync) {
        //     // 判断文件是否存在，假定到时候系统跑的文件夹Log文件夹
        //     // 三种类型的Log, 一种是opLog, 另外一种是dumpLog， 第三种是ssTable的log
        //     // if((fd_ = open(file_path.c_str(), O_RDWR | O_CREAT | O_TRUNC)) < 0) {
        //     //     //文件打开失败处理，后面看看怎么处理
        //     //     perror("create new log file fail");
        //     // }
        //     // if(fd_  > 0) {
        //     //     //先设置文件大小, 先简单设置为8M
        //     //     if(ftruncate(fd_, 8 * 1024 * 1024) < 0){
        //     //         // 错误处理
        //     //         perror("set log file size error");
        //     //     }
        //     //     //这里需要使用mmap 打开文件, 需要判断是否出错
        //     //     log_file_ = (lmmdb::globalVar::OpLog*)mmap(NULL, 8 * 1024 * 1024, PROT_WRITE, MAP_PRIVATE, fd_, 0);
        //     //     if(log_file_ == MAP_FAILED) {
        //     //         perror("mmap failed");
        //     //     }
        //     // }
        // }
        Log::~Log() {
            log_file_.flush();
            log_file_.close();
        }
        bool Log::createLog(std::string file_path, bool is_sync) {

            log_file_.open(file_path.c_str(), std::ios::in | std::ios::out | std::ios::trunc);
            if(log_file_.is_open()) {
                // 打开文件成功
                std::cout << "open log file success\n";
                one_log_one_sync_ = is_sync;
                return true;
            }
            else {
                // 打开文件失败
                // 此处需要处理
                std::cout << "open file error \n";
                return false;
            }
        }

        bool Log::writeLog(int op, std::string key, std::string value) {
            
            using lmmdb::globalVar::Op;

            if(op < Op::GET || op > Op::SCAN) {
                // operator error
            }
            if(key.empty() || key.size() > 8) {
                // key error
            }
            if(value.empty() || value.size() > 256 ){
                // value error
            }
            // 构造定长log, log 由 3部分组成 op, key， value, 如果key value中出现同样的分隔符如何处理
            std::string temp_log;
            temp_log += "<";
            temp_log += std::to_string(op);
            temp_log += ",";
            temp_log += key;
            temp_log += ",";
            temp_log += value;
            temp_log += ">\n";
            // 添加记录到内存上
            log_file_.write(temp_log.c_str(), temp_log.size());
            
            if(one_log_one_sync_) {
                if(log_file_.good()) {
                    return emitPhysicalRecord();
                }
                //write fail
                return false;
            }
            else {
                //不立即刷新,不做任何操作，等待缓冲区满自己刷新
                return true;
                
            }

        }
        bool Log::emitPhysicalRecord() {
            //提交到物理磁盘上
            //写入流缓冲成功, 刷新到内核缓冲区中去, 是否有将数据存储到磁盘中去，这是个问题，唯一可以保证的是
            //用户缓冲区已经刷新
            log_file_.flush();
            if(log_file_.good()) {
                return true;
            }
            else {
            //？？？
                return false;
            }
        } 
    }
}
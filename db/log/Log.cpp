//log 实现

#include "Log.h"

namespace lmmdb {
    namespace log {


        Log::Log() {
            one_log_one_sync_ = false;
            current_log_file_ = new std::fstream();
            old_log_file_ = NULL;

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

        Log::~Log() {
            using std::cout;
            cout << "deconstructor of Log\n";
            if(current_log_file_ != NULL) {
                current_log_file_->flush();
                current_log_file_->close();
            }
            if(old_log_file_ != NULL) {
                old_log_file_->flush();
                old_log_file_->close();
            }

        }

        bool Log::createLog(std::string file_path, bool is_sync) {
            using lmmdb::globalVar::FileStatus;

            current_log_file_->open(file_path.c_str(), std::ios::in | std::ios::out | std::ios::trunc);
            if(current_log_file_->is_open()) {
                // 打开文件成功
                std::cout << "open log file success\n";
                one_log_one_sync_ = is_sync;
                current_file_status_ = FileStatus::USING;
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
            current_log_file_->write(temp_log.c_str(), temp_log.size());
            
            if(one_log_one_sync_) {
                if(current_log_file_->good()) {
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

        void Log::changeFileStatus(bool change_current_file, int status, std::string new_log_file_name) {
            using lmmdb::globalVar::FileStatus;
            using std::ios;

            if(change_current_file) {
                // 创建新的文件
                if(old_file_status_ == FileStatus::DUMP) {
                    // 老文件已经直接dump到本地了
                }
                else {
                    // 关闭老文件
                    old_log_file_->close();
                    delete old_log_file_;
                    
                    // 将current log 文件移交到 old log 文件
                    old_log_file_ = current_log_file_;
                    old_file_status_ = status;
                    current_log_file_ = new std::fstream();
                    current_log_file_->open(new_log_file_name, ios::in | ios::out | ios::trunc);
                    if(current_log_file_->is_open()) {
                        // file open success
                        current_file_status_ = FileStatus::USING;
                    }
                    else {
                        // file open failed
                    }
                }
            }
            else {
                // 仅仅修改文件状态
                old_file_status_ = status;
            }
        }
        bool Log::emitPhysicalRecord() {

            //提交到物理磁盘上
            //写入流缓冲成功, 刷新到内核缓冲区中去, 是否有将数据存储到磁盘中去，这是个问题，唯一可以保证的是
            //用户缓冲区已经刷新
            current_log_file_->flush();
            if(current_log_file_->good()) {
                return true;
            }
            else {
            //？？？
                return false;
            }
        } 
    }
}
//log 实现

#include "Log.h"

namespace lmmdb {
    namespace log {
        Log::Log(std::string file_path, bool is_sync) {
            // 判断文件是否存在，假定到时候系统跑的文件夹Log文件夹
            // 三种类型的Log, 一种是opLog, 另外一种是dumpLog， 第三种是ssTable的log

        }
        bool Log::writeLog() {
            // 添加记录到内存上
        }
        
        bool Log::emitPhysicalRecord() {
            //
        } 
    }
}
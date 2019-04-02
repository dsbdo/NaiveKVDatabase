#include "MemTable.h"

namespace lmmdb {
    namespace memtable{
        MemTable::MemTable(/* args */)
        {
            // 处理new 可能出现的错误问题
            raw_map_data_ = new std::map<std::string, std::string>();
            // 现在memTable的大小
            current_table_size_ = 0;
            
            log_ = lmmdb::log::Log::getInstance();
            std::cout << "mem get log addr: " << (long)log_ << std::endl;
            immutable_table_ = lmmdb::immutableTable::ImmutableTable::getInstance();
            using std::cout;

            cout << "mem table get immutable: "<< (long) immutable_table_ << std::endl;
        }
        
        MemTable::~MemTable()
        {
            //清除所有数据
        }

        MemTable* MemTable::instance = new MemTable();

        MemTable* MemTable::getInstance() {
            return instance;
        }

        bool MemTable::checkPoint() {
            using std::map;
            using std::string;
            using std::cout;
            using std::endl;
            using lmmdb::globalVar::FileStatus;
            using lmmdb::globalVar::K_DISK_BLOCK_SIZE;
            // 这里应该写成配置文件参数会更好， 现在先设定为4M, 如果下一条就已经超过4M, 那现在就应该更新
            if(current_table_size_ + 8 + 256 > K_DISK_BLOCK_SIZE) {
                // 达到触发点，这个时候需要immutable 去dump到磁盘上，dump结束之后，把新的memTable移到 immutable
                // 这里不可以等，在第一个memTable 移交成功之后，immuTable立即dump一份副本到磁盘上，当新的memTable完成之后可以立即移交
                cout << "update immutable \n";
                cout << (long)immutable_table_ << endl;
                immutable_table_->updateImmutableTable(raw_map_data_);

                raw_map_data_ = new map<string, string>();

                //这里触发memTable满，进行移交，同时创建新的log文件，修改log文件状态
                current_table_size_= 0;
                cout << "create new log file " << endl;
                log_->changeFileStatus(true, FileStatus::PENDING, "new_log_file");
                // 移交成功return true， 创建新的MemTable, 否则return false
                return true;
            }

            return true;
        }
        
        std::string MemTable::getKeyValue(std::string key) {
            // 第一步，从memTable中寻找
            auto it = raw_map_data_->find(key);
            if(it != raw_map_data_->end()) {
                // MemTable中可以找到对应的数据
                return it->second;
            }
            return "";
        }

        bool MemTable::putKeyValue(std::string key, std::string value) {
            // 写log的时候已经保证是正确的了

            auto it = raw_map_data_->find(key);
            if(it != raw_map_data_->end()) {
                // 已经存在，这个时候是更新, 这里需要测一下
                it->second = value;
                return true;
            }
            raw_map_data_->insert(std::pair<std::string, std::string>(key, value));
            current_table_size_ += 8 + 256;
            
            // 检查是否可以转化为immutableTable
            checkPoint();
            return true;
        }
    }
}
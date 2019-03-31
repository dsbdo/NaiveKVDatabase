#include "MemTable.h"

namespace lmmdb {
    namespace memtable{
        MemTable::MemTable(/* args */)
        {
            // 处理new 可能出现的错误问题
            raw_map_data_ = new std::map<std::string, std::string>();
            // 现在memTable的大小
            current_table_size_ = 0;

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
            // 这里应该写成配置文件参数会更好， 现在先设定为4M
            if(current_table_size_ > 4 * 1024 * 1024) {
                // 达到触发点，这个时候需要immutable 去dump到磁盘上，dump结束之后，把新的memTable移到 immutable
                // 这里不可以等，在第一个memTable 移交成功之后，immuTable立即dump一份副本到磁盘上，当新的memTable完成之后可以立即移交
                

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

            // 第二步，从immutable中寻找

            // 第三步，磁盘中定位寻找，其中磁盘中有index_log， 从index_log中定位在那一层level, 再定位到在哪一个文件上
            
            return "";
        }
        bool MemTable::putKeyValue(std::string key, std::string value) {
            // 第一步， 先写log
        }
    }
}
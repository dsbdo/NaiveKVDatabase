#ifndef DB_MEMTABLE_H_
#define DB_MEMTABLE_H_

#include <map>
#include <string>
#include "../log/Log.h"
#include "../immutableTable/ImmutableTable.h"

namespace lmmdb {
    namespace memtable{
        //写成单例会更合适
        class MemTable
        {
        public:

            ~MemTable();
            // 检查memTable的大小是否已经达到检查点
            bool checkPoint();
            std::string getKeyValue(std::string key);
            //  delete 函数就在这个函数实现， 其中value 为空
            bool putKeyValue(std::string key, std::string value);

            // scan 接口 需要考虑返回的数据很大的时候如何处理
            // 暂留
            static MemTable* getInstance();
        private:
            /* data */
            // 关于数据长度问题，在插入时，写log的时候就进行限制
            MemTable(/* args */);
            static MemTable* instance;
            std::map<std::string, std::string>* raw_map_data_;
            // 记录memTable 的大小
            int current_table_size_;
            lmmdb::log::Log* log_;
            lmmdb::immutableTable::ImmutableTable* immutable_table_;

        };
    }
}
#endif //DB_MEMTABLE_H_
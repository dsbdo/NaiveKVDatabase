#ifndef DB_IMMUTABLETABLE_H_
#define DB_IMMUTABLETABLE_H_
#include <map>
#include <string>
#include <cstring>
namespace lmmdb {
    namespace immutableTable{
        class ImmutableTable
        { 
        public:

            ~ImmutableTable();
            static ImmutableTable* getInstance();
            
            // 检查看是否有对应的key的值
            std::string getKeyValue(std::string key);
            // 将数据dump到磁盘上。
            bool dumpFileToDisk();

            // 生成对应的disk block， 方便后面直接dump到磁盘上
            bool generateDiskBlock();

            // memTable向immutableTable转换时调用该函数
            bool updateImmutableTable(const std::map<std::string, std::string>* new_map_data);

        private:
            /* data */
            ImmutableTable(/* args */);
            // 防止复制
            ImmutableTable(const ImmutableTable&);
            ImmutableTable& operator=(const ImmutableTable&);
            static ImmutableTable* instance; 
            
            //数据不可修改，仅可访问
            const std::map<std::string, std::string>* raw_map_data_;
            //当有新的memTable转换为immutable时，遍历创建当前raw_map_data_同时将其dump到本机上
            char* disk_block_;

        };

    }
}
#endif //DB_IMMUTABLETABLE_H_
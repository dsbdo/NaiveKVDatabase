//如何实现，option log 需要写磁盘， 实现memtable， 还需要将memTable进行转化为const 类型dump到本地上
//本地文件进行合并


//还需要维护一个文件索引的log
#include "log/Log.h"
#include "memTable/MemTable.h"
#include "immutableTable/ImmutableTable.h"
#include <string>

namespace lmmdb {
    namespace db {

        class DB
        {
        private:
            /* data */
            log::Log* log_;
            memtable::MemTable* memtable_;
            immutableTable::ImmutableTable* immutable_table_;
        public:
            DB(/* args */);
            ~DB();
            std::string Get(std::string key);
            bool Put(std::string key, std::string value);
            bool Delete(std::string key);
            // 大数据量的返回，需要一边取，一边返回，假设为
            void Scan(std::string key_start, std::string key_end, char* &buffer);
        };
        
        DB::DB(/* args */)
        {
            log_ = log::Log::getInstance();
            memtable_ = memtable::MemTable::getInstance();
            immutable_table_ = immutableTable::ImmutableTable::getInstance();
        }
        
        DB::~DB()
        {
        }

        std::string DB::Get(std::string key) {
            std::string temp  = "";
            // 第一步， 从memTable中寻找
            
            // 第二步， 从immutableTable中寻找
            
            // 第三步， 从文件中寻找先从index_log中定位level，再定位文件

            return "";
        }

        bool DB::Put(std::string key, std::string value) {
            // 第一步，先写log

            // 第二步，写log成功之后，写memTable。

        }
        bool DB::Delete(std::string key) {
            // 第一步，先检查key存在否。 其实实际上可以不检查
            // 第二步，直接调用Put接口即可

        }
        void DB::Scan(std::string key_start, std::string key_end, char* &buf) {
            // 遍历整一个范围，如果找到把找到的内容填充buf中
        }

    }
}
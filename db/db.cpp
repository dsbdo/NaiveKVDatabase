//如何实现，option log 需要写磁盘， 实现memtable， 还需要将memTable进行转化为const 类型dump到本地上
//本地文件进行合并

//还需要维护一个文件索引的log
#include "log/Log.h"
#include "memTable/MemTable.h"
#include "immutableTable/ImmutableTable.h"
#include "global.h"
#include <string>

namespace lmmdb
{
namespace db
{

class DB
{
  private:
    /* data */
    log::Log *log_;
    memtable::MemTable *memtable_;
    immutableTable::ImmutableTable *immutable_table_;

  public:
    DB(/* args */);
    ~DB();
    std::string Get(std::string key);
    bool Put(std::string key, std::string value);
    bool Delete(std::string key);
    // 大数据量的返回，需要一边取，一边返回，假设为
    void Scan(std::string key_start, std::string key_end, char *&buffer);
};

std::string g_log_path_prefix = "./lmmdbLog/";
std::string g_data_path_prefix = "./lmmdbData/";

std::string getNewLogName()
{
    time_t time_value;
    time(&time_value);
    return std::to_string(time_value);
}
DB::DB(/* args */)
{
    // 获取对应组件的操作指针
    log_ = log::Log::getInstance();
    // 通过文件名判断当前日志是否需要回滚？？？？？,文件名命名机制，时间命名？

    log_->createLog(g_log_path_prefix + "USING" + getNewLogName(), false);

    memtable_ = memtable::MemTable::getInstance();
    immutable_table_ = immutableTable::ImmutableTable::getInstance();
}

DB::~DB()
{
    delete memtable_;
    delete immutable_table_;
    delete log_;

}

std::string DB::Get(std::string key)
{
    using lmmdb::globalVar::Op;
    //先写log
    log_->writeLog(Op::GET, key, "");
    
    std::string temp = "";
    // 第一步， 从memTable中寻找
    temp = memtable_->getKeyValue(key);
    if(temp.size() > 0) {
        return temp;
    }
    // 第二步， 从immutableTable中寻找
    temp  = immutable_table_->getKeyValue(key);
    if(temp.size() > 0) {
        return temp;
    }
    
    // 第三步， 从文件中寻找先从index_log中定位level，再定位文件
    
    return "";
}

bool DB::Put(std::string key, std::string value)
{
    using namespace std;
    using lmmdb::globalVar::Op;
    using lmmdb::memtable::MemTable;
    // 第一步，先写log, log文件应该与memTable对应，如果immutableMemtable dump成功的话，需要创建新的log
    bool write_result = log_->writeLog(Op::PUT, key, value);
    // 第二步，写log成功之后，写memTable。
    if (write_result)
    {
        //写log 成功，写memTable
        write_result = memtable_->putKeyValue(key, value);
    }
    else
    {
        cout << "write log failed" << endl;
    }
}
bool DB::Delete(std::string key)
{
    // 第一步，先检查key存在否。 其实实际上可以不检查
    // 第二步，直接调用Put接口即可
}
void DB::Scan(std::string key_start, std::string key_end, char *&buf)
{
    // 遍历整一个范围，如果找到把找到的内容填充buf中
}

} // namespace db
} // namespace lmmdb



int main(void) {
    // main 函数
    using lmmdb::db::DB;
    using std::cout;
    using std::endl;
    DB temp_db;
    // 因为是key and value 是固定的，memTable 与 log 插入时不修改大小，准备dump到磁盘时才修改大小
    temp_db.Put("hello", "world");
    temp_db.Put("hello", "world");
    temp_db.Put("hello2", "world2");
    cout << "key: hello, value: " << temp_db.Get("hello") << endl;
    cout << "key: hello2, value: " << temp_db.Get("hello2") << endl;
    cout << "key: NULL, value: " << temp_db.Get("NULL") << endl;
    return 0;
}
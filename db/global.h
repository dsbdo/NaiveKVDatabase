// 这个文件主要用来存储与全局共享声明。例如定义一条日志的格式
// option log format
#ifndef GLOBAL_H_
#define GLOBAL_H_
#include <string>
#include <ctime>
namespace lmmdb
{
namespace globalVar
{
    //  operator log 前缀
    extern std::string g_log_path_prefix;
    // 数据文件的前缀
    extern std::string g_data_path_prefix;

// 操作枚举量
enum Op
{
    GET = 1,
    PUT = 2,
    DELETE = 3,
    SCAN = 4
};
// 文件状态
enum FileStatus {
    USING = 1,
    PENDING = 2,
    COMMIT = 3,
    DUMP = 4
};

// 日志格式
struct OpLog
{
    //we define one more char for \0
    /* data */
    int op;
    char key[9];
    char value[257];
    OpLog()
    {
        op = -1;
        for (int i = 0; i < 9; ++i)
        {
            key[i] = '\0';
        }
        for (int i = 0; i < 257; ++i)
        {
            value[i] = '\0';
        }
    }
};

//生成日志名的函数,日志使用 状态 + 时间命名
//extern std::string getNewLogName();


} // namespace globalVar

} // namespace lmmdb
#endif //GLOBAL_H_
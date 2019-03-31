// 这个文件主要用来存储与全局共享声明。例如定义一条日志的格式
// option log format
#ifndef GLOBAL_H_
#define GLOBAL_H_
namespace lmmdb
{
namespace globalVar
{
enum Op
{
    GET = 1,
    PUT = 2,
    DELETE = 3,
    SCAN = 4
};
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

} // namespace globalVar

} // namespace lmmdb
#endif //GLOBAL_H_
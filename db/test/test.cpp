
#include <string>
#include <iostream>
#include <dirent.h>
#include <map>

using namespace std;
std::string getNextDataFileName()
{
    using std::cout;
    using std::endl;
    using std::string;
    // 扫描整个文件目录，统计计算文件目录下的文件数量，返回下一个文件名
    // 实际上这个功能应该由compaction类提供这个功能
    DIR *dp;
    dirent *drip;
    if ((dp = opendir("./../lmmdbData/"))== NULL)
        {
            cout << "open dir failed\n";
        }
    int counter = 0;
    while ((drip = readdir(dp)) != NULL)
    {
        counter++;
        cout << drip->d_name << endl;
    }
    closedir(dp);
    return string("level0-") + std::to_string(counter - 1);
}
class A {
    public:
    void func() {
        cout << "hello world" << endl;
    }
    int a;
};
int main(void)
{

    map<string, string>* result = NULL;
    if(result == NULL) {
        cout << "result is null" << endl;
        cout << reinterpret_cast<long>(result) << endl;
    }
    result = nullptr;
    if(result == nullptr) {
        cout << "result is nullptr" << endl;
        cout << reinterpret_cast<long>(result) << endl;
    }
    A* obj = NULL;
    if (obj == NULL) {
        cout << "obj is NULL" << endl;
        cout << reinterpret_cast<long>(obj) << endl;
    }
    obj == nullptr;
    if(obj == nullptr) {
         cout << "obj is nullptr" << endl;
         cout << reinterpret_cast<long>(obj)<<endl;
    }
   // cout << getNextDataFileName() << endl;
    return 0;
}
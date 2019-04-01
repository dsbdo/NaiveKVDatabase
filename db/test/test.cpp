
#include <string>
#include <iostream>
#include <dirent.h>
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

int main(void)
{
    cout << getNextDataFileName() << endl;
    return 0;
}
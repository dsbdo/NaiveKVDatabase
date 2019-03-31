#include <unistd.h>
#include <fcntl.h>
#include <iostream>

using namespace std;
int main(void) {
    int fd = open("./log/log.txt", O_CREAT | O_RDWR);

    int a = 65;
    cout << static_cast<char>(a) << endl;
}

#include "ImmutableTable.h"

namespace lmmdb {
    namespace immutableTable {
        
        std::string getNextDataFileName() {
            using std::string;
            using std::cout;
            using std::endl;
           
           
            // using lmmdb::globalVar::g_data_path_prefix;
            // 扫描整个文件目录，统计计算文件目录下的文件数量，返回下一个文件名
            // 实际上这个功能应该由compaction类提供这个功能
            DIR* dp;
            dirent* drip;
            if((dp = opendir(string("./lmmdbData/").c_str())) == NULL) {
                cout << "open dir failed\n";
            }
            int counter = 0;
            while((drip = readdir(dp)) != NULL) {
                counter++;
                cout << drip << endl;
            }
            closedir(dp);
            return string("./lmmdbData/level0-") + std::to_string(counter - 1);
        }

        ImmutableTable::ImmutableTable(/* args */)
        {
            disk_block_ = NULL;
            raw_map_data_ = NULL;
            using std::endl;
            using std::cout;
            cout << "init pointer " << endl;
            cout << "raw_map_data " << endl;
            if(raw_map_data_ == NULL) {
                 cout << reinterpret_cast<long>(raw_map_data_) << endl;
            }
           std::cout << (long)this << std::endl;
        } 

        ImmutableTable::ImmutableTable(const ImmutableTable&) {
        }

        ImmutableTable& ImmutableTable::operator=(const ImmutableTable&) {
        }

        ImmutableTable::~ImmutableTable()
        {
        }

        ImmutableTable* ImmutableTable::instance = new ImmutableTable();
        
        // 返回实例
        ImmutableTable* ImmutableTable::getInstance() {
           std::cout << "get instance" << std::endl;
            std::cout << (long) instance << std::endl;
            return instance;
        }

        std::string ImmutableTable::getKeyValue(std::string key) {
            if(raw_map_data_ == NULL) {
                //Immutable为空
                return "";
            }
            else {
                auto it = raw_map_data_->find(key);
                if(it == raw_map_data_->end()) {
                    // 这里不存在
                    return "";
                }
                else {
                    return it->second;
                }
            }
        }
        
        // 将数据dump到硬盘上
        bool ImmutableTable::dumpFileToDisk() {
            using std::fstream;
            using std::ios;
            using std::string;
            using std::cout;
            using std::cerr;
            using std::endl;
            using lmmdb::globalVar::K_DISK_BLOCK_SIZE;

            if(disk_block_ == NULL) {
                return false;
            }
            else {
                // 创建磁盘块，然后存到硬盘上
                // 创建文件
                fstream new_file( getNextDataFileName().c_str(), ios::out | ios::trunc);
                if( new_file.is_open() ) {
                    // 这里需要判断如果出错如何处理
                    new_file.write(disk_block_, K_DISK_BLOCK_SIZE);
                    new_file.flush();
                    new_file.close();
                    cout << "dump filse success\n";

                }
                else {
                    cerr << "open file to dump fail\n" << endl;
                }
                // 销毁预防内存泄露
                delete []disk_block_;
                disk_block_ = NULL;
            }
            return true;
        }

        // 生成新的磁盘块
        bool ImmutableTable::generateDiskBlock() {

            using lmmdb::globalVar::K_DISK_BLOCK_SIZE;
            using std::cout;
            if( raw_map_data_ == NULL) {
                return false;
            }

            if(raw_map_data_ == NULL || disk_block_ != NULL) {
                return false;
            }
            else {

                //创建一个4M大小的内存块，用来准备dump到磁盘上
                disk_block_ = new char[K_DISK_BLOCK_SIZE];
                int index = 0;
                //memTable上保证record大小不会超过 4 * 1024 * 1024
                for(auto it = raw_map_data_->begin(); it != raw_map_data_->end(); it++) {
                    // 插入时仅仅只是保证了其key 一定小于等于8byte， value 一定小于等于256byte
                    std::string record;
                    // 前八个字节是key
                    record += it->first;
                    int counter = 8 - it->first.size();
                    while(counter > 0 ) {
                        record += " ";
                        counter--;
                    }
                    record += it->second;
                    counter = 256 - it->second.size();
                    while(counter > 0) {
                        record += "#";
                        counter--;
                    }

                    // 如果这里异常呢？
                    memcpy(disk_block_ + index, record.c_str(), record.size());
                    
                    index += record.size();

                    std::cout << "record size is: " << record.size() << " " << record << std::endl;
                    
                    record.clear();
                }
                // dump 到磁盘上
                std::cout << "dump file \n" << std::endl;

                dumpFileToDisk();
            }
        }

        // 更新immutable
        bool ImmutableTable::updateImmutableTable(const std::map<std::string, std::string>* new_map_data) {
           // 此处this指针地址值为0x00 为什么呢
           std::cout << "test is: " << test << std::endl;
           if(disk_block_ == NULL) {
               std::cout << "disk block is null" << std::endl;
           }

           std::cout << "upate immutable function" << std::endl;
           //std::cout << (long)raw_map_data_ << std::endl;
           if(nullptr == raw_map_data_) {
               std::cout << "raw_map_data_ is NULL " << std::endl;
           }
            using std::cout;
            cout << "generate disk block \n";
            if(raw_map_data_ == NULL) {

            }
            else {
                delete raw_map_data_;
                raw_map_data_ = NULL;
            }

            raw_map_data_ = new_map_data;
            generateDiskBlock();
            // 这里是否可以判断raw_map_data_

            return true;
            // if(raw_map_data_ == NULL) {
            //     raw_map_data_ = new_map_data;
            //     return true;
            // }
            // else {
            //     // 这里需要处理如果delete失败的问题
            //     delete raw_map_data_;
            //     raw_map_data_ = new_map_data;
            //     // 生成新的disk_block, 最好改成多线程处理
            //     generateDiskBlock();
            //     return true;
            // }
        }
    }
}

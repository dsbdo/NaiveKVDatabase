
#include "ImmutableTable.h"

namespace lmmdb {
    namespace immutableTable {
        
        std::string getNextDataFileName() {
            using std::string;
            using std::cout;
            using std::endl;
            using lmmdb::globalVar::g_data_path_prefix;
            // 扫描整个文件目录，统计计算文件目录下的文件数量，返回下一个文件名
            // 实际上这个功能应该由compaction类提供这个功能
            DIR* dp;
            dirent* drip;
            if((dp = opendir(g_data_path_prefix.c_str())) == NULL) {
                cout << "open dir failed\n";
            }
            int counter = 0;
            while((drip = readdir(dp)) != NULL) {
                counter++;
                cout << drip << endl;
            }
            closedir(dp);
            return string("level0-") + std::to_string(counter - 1);
        }
        ImmutableTable::ImmutableTable(/* args */)
        {
            disk_block_ = NULL;
            raw_map_data_ = NULL;
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
            if(disk_block_ == NULL) {
                return false;
            }
            else {
                // 创建磁盘块，然后存到硬盘上
                // 创建文件

                // 销毁预防内存泄露
                delete []disk_block_;
                disk_block_ = NULL;
            }
            return true;
        }

        //生成新的磁盘块
        bool ImmutableTable::generateDiskBlock() {
            if(raw_map_data_ == NULL || disk_block_ != NULL) {
                return false;
            }
            else {
                //创建一个4M大小的内存块，用来准备dump到磁盘上
                disk_block_ = new char[4 * 1024 * 1024];
                int index = 0;
                //memTable上保证record大小不会超过 4 * 1024 * 1024
                for(auto it = raw_map_data_->begin(); it != raw_map_data_->end(); it++) {
                    // 插入时仅仅只是保证了其key 一定小于等于8byte， value 一定小于等于256byte
                    std::string record;
                    // 前八个字节是key
                    record += it->first;
                    int counter = 8 - it->first.size();
                    while(counter > 0 ) {
                        record += "\0";
                    }
                    record += it->second;
                    counter = 256 - it->second.size();
                    while(counter > 0) {
                        record += "\0";
                    }

                    // 如果这里异常呢？
                    memcpy(disk_block_ + index, record.c_str(), record.size());
                    
                    index += record.size();
                    record.clear();
                }
                // dump 到磁盘上
                dumpFileToDisk();
            }
            // 是否要在这个函数上直接dump到磁盘上???
        }

        //更新immutable
        bool ImmutableTable::updateImmutableTable(const std::map<std::string, std::string>* new_map_data) {

            generateDiskBlock();
            delete raw_map_data_;
            raw_map_data_ = new_map_data;
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

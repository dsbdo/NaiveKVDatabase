
#include "ImmutableTable.h"

namespace lmmdb {
    namespace immutableTable {
                
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
            if(raw_map_data_ == NULL) {
                raw_map_data_ = new_map_data;
                return true;
            }
            else {
                // 这里需要处理如果delete失败的问题
                delete raw_map_data_;
                raw_map_data_ = new_map_data;
                // 生成新的disk_block, 最好改成多线程处理
                generateDiskBlock();
                return true;
            }
        }
    }
}

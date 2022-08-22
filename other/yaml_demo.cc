/*
 * @Author: Ryu-59073
 * @Date: 2022-07-28 02:02:04
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-07-28 02:47:34
 * @FilePath: /Cpp_code/other/yaml_demo.cc
 * @Description: 
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#include <yaml-cpp/yaml.h> //yaml-cpp 头文件
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

template<class From,class To>
class LaxicalCast{
public:
    To operator()(const From& from){
        // return lexical_cast<To>(from);
        return To();
    }
};

template<class T>
class LaxicalCast<std::vector<T> ,std::string>{
public:
    std::string operator()(const std::vector<T>& v){
        YAML::Node node;
        for(auto& item:v){
            //node.push_back(item);
            node.push_back(YAML::Load(LaxicalCast<T,std::string>()(item)));
        }
        std::stringstream ss;
        ss<<node;
        return ss.str();
    }
};

int main(int argc, char** argv) {
    // try {
    //     YAML::Node node = YAML::LoadFile("test.yml");
    //     std::cout << node << std::endl; //输出yaml数据
    // } catch (...) { //文件为非yaml格式抛出异常
    //     std::cout << "error" << std::endl;
    // }
    std::vector<int> ve{1,2,3,5,6,7,5};
    std::string s = LaxicalCast<std::vector<int>, std::string>()(ve);
    std::vector<int> ve1 = LaxicalCast<std::string, std::vector<int>>()(s);
    for(auto v : ve1) {
        
    }
    return 0;
}

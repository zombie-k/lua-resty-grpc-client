#include <iostream>
#include "src/diaochan/diaochan.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::string target("i.diaochan.recom.weibo.com:5001");
    //std::string target("10.185.12.134:10471");
    CDiaochan cd(target, 50000);
    std::string reply = cd.Recall("hello");
    //std::cout << "reply:" << reply << std::endl;
    reply = cd.Recall("hello");
    //std::cout << "reply:" << reply << std::endl;
    return 0;
}
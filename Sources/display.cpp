#include "display.h"
#include <iostream>

Display::Display(int width, int height, const std::string& title){
    std::cout<<"CONSTRUCT"<<std::endl;
}
Display::~Display(){
    std::cout<<"DESTRUCT"<<std::endl;
}
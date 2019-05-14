//
// Created by Alexandre Carqueja on 02/05/2019.
//

#include "Option.h"

Option::Option(const std::string &optionName, std::function<void()> optionFunc)
        : optionName(optionName), optionFunc(optionFunc) {}

const std::string &Option::getOptionName() const {
    return optionName;
}

void Option::clickFunc() const {
    optionFunc();
}

Option::Option(const std::string &optionName) : optionName(optionName){
    optionFunc = nullptr;
}

StringOption::StringOption(const std::string &optionName, std::function<void(std::string)> function) : Option(optionName), stringOptionFunc(function){}

void StringOption::clickFunc() const {
    stringOptionFunc(optionName);
}
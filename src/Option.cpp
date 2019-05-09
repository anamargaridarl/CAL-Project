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

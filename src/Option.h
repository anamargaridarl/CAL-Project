//
// Created by Alexandre Carqueja on 02/05/2019.
//
#include <iostream>
#include <string>
#include <functional>


#ifndef PROJECT_OPTION_H
#define PROJECT_OPTION_H

class Option {
public:
    Option(const std::string &optionName, std::function<void()> optionFunc);

    const std::string &getOptionName() const;

    void clickFunc() const;

private:
    std::string optionName;
    std::function<void()> optionFunc;
};


#endif //PROJECT_OPTION_H

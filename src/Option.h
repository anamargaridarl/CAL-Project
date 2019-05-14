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
    Option(const std::string &optionName);

    const std::string &getOptionName() const;
    virtual void clickFunc() const;

protected:
    std::string optionName;

private:
    std::function<void()> optionFunc;
};

class StringOption: public Option{
public:
    StringOption(const std::string &optionName, std::function<void(std::string)>);
    void clickFunc() const override;

private:
    std::function<void(std::string)> stringOptionFunc;
};


#endif //PROJECT_OPTION_H

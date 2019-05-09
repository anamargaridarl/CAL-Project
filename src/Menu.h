#ifndef PROJECT_MENU_H
#define PROJECT_MENU_H

#include <string>
#include "Option.h"
#include <vector>
#include <functional>

class Menu {
public:
    Menu(const std::string &menuName, std::vector<Option> menuOptions);
    void run();
private:
    std::string menuName;
    std::vector<Option> menuOptions;
    bool running;

    void displayOptions();
    void navigateOptions();

    static void nullFunction();
};


#endif //PROJECT_MENU_H

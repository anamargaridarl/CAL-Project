#ifndef PROJECT_MENU_H
#define PROJECT_MENU_H

#include <string>
#include "Option.h"
#include "Vehicle.h"
#include <vector>
#include <functional>

class Menu {
public:
    Menu(const std::string &menuName, std::vector<Option> menuOptions);
    void run();
private:
    bool running;

    void displayOptions();
    void navigateOptions();

    static void nullFunction();

protected:
    Menu();

    std::string menuName;
    std::vector<Option> menuOptions;
};


#endif //PROJECT_MENU_H

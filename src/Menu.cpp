#include <limits>
#include "Menu.h"

using namespace std;

Menu::Menu(const std::string &menuName, std::vector<Option*> menuOptions)
        : menuName(menuName) {
    running = false;
    Option* backOption = new Option("Back", nullFunction);
    menuOptions.push_back(backOption);
    this->menuOptions = menuOptions;
}

void Menu::displayOptions()
{
    for(unsigned int i = 0; i < menuOptions.size(); i++)
    {
        Option* option = menuOptions.at(i);
        cout << "|" << i << ": " << option->getOptionName() << endl;
    }
    string s(menuName.length() + 8, '-');
    cout << "|" + s << endl;
}

void Menu::navigateOptions()
{
    unsigned int maxSize = menuOptions.size();
    const auto readOption = [maxSize](unsigned int &num) {
        if(!(cin >> num) || num < 0 || num > maxSize)
        {
            cout << "Invalid Option!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return false;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl;
        return true;
    };

    unsigned int optionNumber = menuOptions.size();
    do{
        cout << "Insert an Option: ";
    } while(!readOption(optionNumber));

    if(optionNumber == menuOptions.size() - 1)
    {
        running = false;
        for(Option* option: menuOptions)
        {
            delete option;
        }
        return;
    }
    menuOptions.at(optionNumber)->clickFunc();
}

void Menu::run() {
    running = true;
    while(running)
    {
        cout << "\n|----" << menuName << "----" << endl;
        displayOptions();
        navigateOptions();
    }
}

void Menu::nullFunction() {}
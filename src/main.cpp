#include <iostream>
#include "Graph.h"
#include "MutablePriorityQueue.h"
#include "Option.h"

using namespace std;

void DisplayOptions(vector<Option> options)
{
    for(unsigned int i = 0; i < options.size(); i++)
    {
        Option option = options.at(i);
        cout << i << ": " << option.getOptionName() << endl;
    }
}

void LoadMapMenu()
{

}

void MainMenu()
{
    cout << "----Main Menu----" << endl;
    vector<Option> options;
    options.push_back(Option("Load Map"));
    options.push_back(Option("Vehicles"));
    options.push_back(Option("Create Journey"));
    options.push_back(Option("Quit"));

    DisplayOptions(options);
}

int main() {
    MainMenu();
    return 0;
}
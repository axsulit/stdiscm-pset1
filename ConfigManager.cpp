#include "ConfigManager.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept> // for exception handling

using namespace std;

// Constructor
ConfigManager::ConfigManager() : numOfThreads(-1), upperLimit(-1), taskDivisionScheme(TaskDivisionScheme::STRAIGHT_DIVISION), printScheme(PrintScheme::IMMEDIATE) {}

// Read config file
void ConfigManager::readConfig() {
    ifstream file("config.txt");

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        size_t pos = line.find("=");

        // Skip invalid lines
        if (pos == string::npos) continue;

        // Get key and value
        string key = line.substr(0, pos);
        int value = stoi(line.substr(pos + 1));

        // Set values based on key
        if (key == "x") numOfThreads = value;
        else if (key == "y") upperLimit = value;
        else {
            cerr << "Invalid key: " << key << endl;
            return;
        }
    }

    file.close();
	cout << "Settings:" << endl; 
    cout << "   Number of Threads: " << numOfThreads << endl;
    cout << "   Upper Limit: " << upperLimit << endl;

    cout << endl;
}

// Helper function to handle variation input
int getOption(const string& prompt, const string& errorMessage) {
    int option;
    while (true) {
        try {
            cout << prompt;
            cin >> option;
            if (cin.fail()) {
                throw invalid_argument("Invalid input!");
            }

			if (option != 1 && option != 2) {
				throw invalid_argument("Invalid selection!");
			}
            return option;
        }
        catch (const exception& e) {
            cin.clear();  
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discards invalid input
            cerr << errorMessage << endl;
        }
    }
}

// Get variation option
void ConfigManager::getVariationOption() {
    // Set Task division scheme
    cout << "Available Task Division Schemes:\n   (1) STRAIGHT_DIVISION\n   (2) PARALLEL_DIVISIBILITY" << endl;
    int selectedTaskDivision = getOption("Input preferred task division scheme (1/2): ", "Invalid selection. Please try again.");
    taskDivisionScheme = static_cast<TaskDivisionScheme>(selectedTaskDivision); 

    cout << endl;

    // Set Print scheme
    cout << "Available Print Schemes:\n   (1) IMMEDIATE\n   (2) DEFERRED" << endl;
    int selectedPrintScheme = getOption("Input preferred print scheme (1/2): ", "Invalid selection. Please try again.");
    printScheme = static_cast<PrintScheme>(selectedPrintScheme); 

    cout << endl;
}

// Get Task Division Scheme
ConfigManager::TaskDivisionScheme ConfigManager::getTaskDivisionScheme() {
	return taskDivisionScheme;
}

// Get Print Scheme
ConfigManager::PrintScheme ConfigManager::getPrintScheme() {
	return printScheme;
}
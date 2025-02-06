#include "ConfigManager.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <thread>
#include <cstdlib>

using namespace std;

// Constructor
ConfigManager::ConfigManager() : numOfThreads(-1), upperLimit(-1), taskDivisionScheme(TaskDivisionScheme::STRAIGHT_DIVISION), printScheme(PrintScheme::IMMEDIATE) {}

// Read config file
void ConfigManager::readConfig() {
    ifstream file("config.txt");

    if (!file.is_open()) {
        cerr << "Error opening config file!" << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(file, line)) {
        size_t pos = line.find("=");

        // Skip invalid lines
        if (pos == string::npos) continue;

        // Get key and value
        string key = line.substr(0, pos);
        string valueStr = line.substr(pos + 1);

        // Check if the value is a valid integer
        try {
            int value = stoi(valueStr);

            // Validate the number of threads (x)
            if (key == "x") {
                int maxThreads = thread::hardware_concurrency();  // Get available cores
                if (value <= 0 || value > maxThreads) {
                    cerr << "FATAL ERROR: Invalid value for x (Number of Threads). Must be between 1 and " << maxThreads << "." << endl;
                    exit(EXIT_FAILURE);
                }
                numOfThreads = value;
            }
            // Validate the upper limit (y)
            else if (key == "y") {
                if (value < 2) {
                    cerr << "FATAL ERROR: Invalid value for y (Upper Limit). Must be greater than 1." << endl;
                    exit(EXIT_FAILURE);
                }
                upperLimit = value;
            }
            else {
                cerr << "WARNING: Invalid key found in config: " << key << endl << endl;
            }
        }
        catch (const invalid_argument&) {
            cerr << "FATAL ERROR: Invalid number format in config for key: " << key << endl;
            exit(EXIT_FAILURE);
        }
        catch (const out_of_range&) {
            cerr << "Value for key: " << key << " is too large!" << endl;
            exit(EXIT_FAILURE);
        }
    }

    file.close();
    cout << "Settings Loaded:" << endl;
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

// Get Number of Threads
int ConfigManager::getNumOfThreads() {
	return numOfThreads;
}

// Get Upper Limit
int ConfigManager::getUpperLimit() {
	return upperLimit;
}

// Get Task Division Scheme
ConfigManager::TaskDivisionScheme ConfigManager::getTaskDivisionScheme() {
	return taskDivisionScheme;
}

// Get Print Scheme
ConfigManager::PrintScheme ConfigManager::getPrintScheme() {
	return printScheme;
}
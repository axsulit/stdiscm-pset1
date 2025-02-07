#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <iomanip>  
#include <chrono>

#include "ConfigManager.h"
#include "PrimeChecker.h"

using namespace std;
using namespace chrono;

// Table lines for immediate print
const string tableLines[2] = {
    "+--------------------------++------------++---------++\n"
    "| time                     || thread_id  || number  ||\n"
    "+--------------------------++------------++---------++\n",

    "+--------------------------++------------++---------++\n"
};

// Function to capture and print the start time
static void printStartTime() {
	auto startTime = chrono::system_clock::to_time_t(chrono::system_clock::now()); 
    char startTimeBuffer[26];
    ctime_s(startTimeBuffer, sizeof(startTimeBuffer), &startTime);

    cout << "\nStart Time: " << startTimeBuffer;
}

// Function to capture and print the end time
static void printEndTime() {
    auto endTime = chrono::system_clock::to_time_t(chrono::system_clock::now());  
    char endTimeBuffer[26];
    ctime_s(endTimeBuffer, sizeof(endTimeBuffer), &endTime);

    cout << "End Time: " << endTimeBuffer;
}

int main()
{
    cout << "Time to spot prime numbers!\n\n";

    // Read config file 
	ConfigManager config;
	config.readConfig(); 
	int numOfThreads = config.getNumOfThreads();
	int upperLimit = config.getUpperLimit();
	int rangeSize = upperLimit / numOfThreads;

    // Get variation option from user
    config.getVariationOption(); 
	ConfigManager::TaskDivisionScheme selectedTaskDivisionScheme = config.getTaskDivisionScheme();
	ConfigManager::PrintScheme selectedPrintScheme = config.getPrintScheme();

    vector<thread> threads;

    printStartTime();

    if (selectedTaskDivisionScheme == ConfigManager::TaskDivisionScheme::STRAIGHT_DIVISION) {
		// Variation 1: STRAIGHT_DIVISION & IMMEDIATE
        if (selectedPrintScheme == ConfigManager::PrintScheme::IMMEDIATE) {
            cout << "DOING NOW: Straight Division and Immediate" << endl;
            cout << tableLines[0];

            for (int i = 0; i < numOfThreads; ++i) {
                int start = i * rangeSize + 1;
                int end = (i == numOfThreads - 1) ? upperLimit : (i + 1) * rangeSize;

                threads.emplace_back(PrimeChecker::checkPrimeRangeImmediate, start, end, i + 1);
            }

            for (auto& t : threads) t.join();
            
            cout << tableLines[1];
        }

		// Variation 2: STRAIGHT_DIVISION & DEFERRED
        else {
            cout << "DOING NOW: Straight Division and Deferred" << endl;

            for (int i = 0; i < numOfThreads; ++i) {
                int start = i * rangeSize + 1;
                int end = (i == numOfThreads - 1) ? upperLimit : (i + 1) * rangeSize;

                threads.emplace_back(PrimeChecker::checkPrimeRangeDeferred, start, end);
            }

            for (auto& t : threads) t.join();
            
            PrimeChecker::printDeferredResults();
        }
    }
    else {
		// Variation 3: PARALLEL_DIVISIBILITY & IMMEDIATE
        if (selectedPrintScheme == ConfigManager::PrintScheme::IMMEDIATE) {
            cout << "DOING NOW: Parallel Divisibility and Immediate" << endl;
			cout << tableLines[0];

            for (int i = 0; i < numOfThreads; ++i) threads.emplace_back(PrimeChecker::checkPrimeParallelImmediate, upperLimit, i + 1);
            for (auto& t : threads) t.join();

			cout << tableLines[1];
        }
		// Variation 4: PARALLEL_DIVISIBILITY & DEFERRED
        else {
            cout << "DOING NOW: Parallel Divisibility and Deferred" << endl;

            for (int i = 0; i < numOfThreads; ++i) threads.emplace_back(PrimeChecker::checkPrimeParallelDeferred, upperLimit);
            for (auto& t : threads) t.join();

			PrimeChecker::printDeferredResults();
        }
    }

    printEndTime();

	return 0;
}
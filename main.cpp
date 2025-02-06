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

// Function to capture and print the start time
void printStartTime() {
    auto startTime = system_clock::now();  // Capture start time
    time_t start_t = system_clock::to_time_t(startTime);
    char startTimeBuffer[26];
    ctime_s(startTimeBuffer, sizeof(startTimeBuffer), &start_t);

    cout << "\nStart Time: " << startTimeBuffer;
}

// Function to capture and print the end time
void printEndTime() {
    auto endTime = system_clock::now();  // Capture end time
    time_t end_t = system_clock::to_time_t(endTime);
    char endTimeBuffer[26];
    ctime_s(endTimeBuffer, sizeof(endTimeBuffer), &end_t);

    cout << "End Time: " << endTimeBuffer;
}

int main()
{
    cout << "Time to spot prime numbers!\n\n";

	ConfigManager config;
	config.readConfig(); // read config file & print values
	config.getVariationOption(); // get variation option

    if (config.getTaskDivisionScheme() == ConfigManager::TaskDivisionScheme::STRAIGHT_DIVISION) {
		// Variation 1: STRAIGHT_DIVISION & IMMEDIATE
        if (config.getPrintScheme() == ConfigManager::PrintScheme::IMMEDIATE) {
            cout << "DOING NOW: Straight Division and Immediate" << endl;

			printStartTime();

            cout << "+--------------------------++------------++---------++" << endl;
            cout << "| time                     || thread_id  || number  ||" << endl;
            cout << "+--------------------------++------------++---------++" << endl;

            vector<thread> threads;
            int rangeSize = config.getUpperLimit() / config.getNumOfThreads();

            for (int i = 0; i < config.getNumOfThreads(); ++i) {
                int start = i * rangeSize + 1;
                int end = (i == config.getNumOfThreads() - 1) ? config.getUpperLimit() : (i + 1) * rangeSize;

                threads.emplace_back(PrimeChecker::checkPrimeRangeImmediate, start, end, i + 1);
            }

            // Wait for all threads to finish
            for (auto& t : threads) {
                t.join();
            }
            cout << "+--------------------------++------------++---------++" << endl;

			printEndTime();
        }

		// Variation 2: STRAIGHT_DIVISION & DEFERRED
        else {
            cout << "DOING NOW: Straight Division and Deferred\n" << endl;

			printStartTime();

            vector<thread> threads;
            int rangeSize = config.getUpperLimit() / config.getNumOfThreads();

            for (int i = 0; i < config.getNumOfThreads(); ++i) {
                int start = i * rangeSize + 1;
                int end = (i == config.getNumOfThreads() - 1) ? config.getUpperLimit() : (i + 1) * rangeSize;

                threads.emplace_back(PrimeChecker::checkPrimeRangeDeferred, start, end);
            }

            // Wait for all threads to finish
            for (auto& t : threads) {
                t.join();
            }

            //  Print only the prime numbers in a grid format
            cout << "Prime Numbers Found:\n" << endl;
            int count = 0;
            for (int prime : PrimeChecker::getPrimeResults()) {
                cout << setw(5) << prime << " ";  // Print each prime with 5 spaces for alignment
                if (++count % 10 == 0) cout << endl;  // Newline every 10 numbers
            }
            cout << "\n" << endl;

			printEndTime();
        }
    }
    else {
		// Variation 3: PARALLEL_DIVISIBILITY & IMMEDIATE
        if (config.getPrintScheme() == ConfigManager::PrintScheme::IMMEDIATE) {
            cout << "DOING NOW: Parallel Divisibility and Immediate" << endl;

			printStartTime();

            cout << "+--------------------------++------------++---------++" << endl;
            cout << "| time                     || thread_id  || number  ||" << endl;
            cout << "+--------------------------++------------++---------++" << endl;

            vector<thread> threads;
            int numThreads = config.getNumOfThreads();
            int upperLimit = config.getUpperLimit();

            for (int i = 0; i < numThreads; ++i) {
                threads.emplace_back(PrimeChecker::checkPrimeParallelImmediate, upperLimit, i + 1);
            }

            for (auto& t : threads) {
                t.join();
            }
            cout << "+--------------------------++------------++---------++" << endl;

			printEndTime();
        }
		// Variation 4: PARALLEL_DIVISIBILITY & DEFERRED
        else {
            cout << "DOING NOW: Parallel Divisibility and Deferred" << endl;

			printStartTime();

            vector<thread> threads;
            int numThreads = config.getNumOfThreads();
            int upperLimit = config.getUpperLimit();

            for (int i = 0; i < numThreads; ++i) {
                threads.emplace_back(PrimeChecker::checkPrimeParallelDeferred, upperLimit);
            }

            for (auto& t : threads) {
                t.join();
            }

            // Print results
            cout << "Prime Numbers Found:\n" << endl;
            int count = 0;
            for (int prime : PrimeChecker::getPrimeResults()) {
                cout << setw(5) << prime << " ";
                if (++count % 10 == 0) cout << endl;  // Newline every 10 numbers
            }
            cout << "\n" << endl;

			printEndTime();
        }
    }

	return 0;
}
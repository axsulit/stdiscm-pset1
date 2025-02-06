#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <iomanip>  

#include "ConfigManager.h"
#include "PrimeChecker.h"

using namespace std;

// Function to get the number of digits in a number
static int numDigits(int num) {
    return (num == 0) ? 1 : static_cast<int>(log10(num) + 1);
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

            cout << "+--------------------------++------------++---------++" << endl;
            cout << "| time                     || thread_id  || number  ||" << endl;
            cout << "+--------------------------++------------++---------++" << endl;

            vector<thread> threads;
            int rangeSize = config.getUpperLimit() / config.getNumOfThreads();

            for (int i = 0; i < config.getNumOfThreads(); ++i) {
                int start = i * rangeSize + 1;
                int end = (i == config.getNumOfThreads() - 1) ? config.getUpperLimit() : (i + 1) * rangeSize;  // Ensure the last thread covers the full range

                threads.emplace_back(PrimeChecker::checkPrimeRangeImmediate, start, end, i + 1);
            }

            // Wait for all threads to finish
            for (auto& t : threads) {
                t.join();
            }
            cout << "+--------------------------++------------++---------++" << endl;
        }
		// Variation 2: STRAIGHT_DIVISION & DEFERRED
        else {
            cout << "DOING NOW: Straight Division and Deferred\n" << endl;

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
        }
    }
    else {
		// Variation 3: PARALLEL_DIVISIBILITY & IMMEDIATE
        if (config.getPrintScheme() == ConfigManager::PrintScheme::IMMEDIATE) {
            cout << "DOING NOW: Parallel Divisibility and Immediate" << endl;

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
        }
		// Variation 4: PARALLEL_DIVISIBILITY & DEFERRED
        else {
            cout << "DOING NOW: Parallel Divisibility and Deferred" << endl;

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
        }
    }

	return 0;
}
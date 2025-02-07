#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <iomanip>  
#include <future>

#include "ConfigManager.h"
#include "PrimeChecker.h"

using namespace std;
using namespace chrono;


// Table lines for immediate print
const string tableLines[2] = {
    "+-------------------------++------------++---------++\n"
    "| time                    || thread_id  || number  ||\n"
    "+-------------------------++------------++---------++\n",

    "+-------------------------++------------++---------++\n"
};


static string getCurrentTime() {
    auto now = system_clock::now();
    time_t now_c = system_clock::to_time_t(now);
    auto ms = duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    tm timeInfo;
    localtime_s(&timeInfo, &now_c);

    stringstream ss;
    ss << put_time(&timeInfo, "%Y-%m-%d %H:%M:%S")  
        << '.' << setfill('0') << setw(3) << ms.count(); 

    return ss.str();
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

    // Get Start time
    auto startTime = high_resolution_clock::now();
    string startStr = getCurrentTime();
    cout << "Start Time: " << startStr << endl << endl;

    if (selectedTaskDivisionScheme == ConfigManager::TaskDivisionScheme::STRAIGHT_DIVISION) {

        vector<thread> threads;

		// Variation 1: STRAIGHT_DIVISION & IMMEDIATE
        if (selectedPrintScheme == ConfigManager::PrintScheme::IMMEDIATE) {
            cout << "DOING NOW: Straight Division and Immediate Printing" << endl;
            cout << tableLines[0];

            for (int i = 0; i < numOfThreads; ++i) {
                int start = i * rangeSize + 1;
                int end = (i == numOfThreads - 1) ? upperLimit : (i + 1) * rangeSize;

                threads.emplace_back(PrimeChecker::checkPrimeRangeImmediate, start, end);
            }

            for (auto& t : threads) t.join();
            
            cout << tableLines[1];
        }

		// Variation 2: STRAIGHT_DIVISION & DEFERRED
        else {
            cout << "DOING NOW: Straight Division and Deferred Printing" << endl;

            for (int i = 0; i < numOfThreads; ++i) {
                int start = i * rangeSize + 1;
                int end = (i == numOfThreads - 1) ? upperLimit : (i + 1) * rangeSize;

                threads.emplace_back(PrimeChecker::checkPrimeRangeDeferred, start, end);
            }

            for (auto& t : threads) t.join();
            
            PrimeChecker::printPrimeRangeDeferredResults();
        }
    }
    else {

        vector <future<void>> threads;
        vector<bool> isNumPrime(upperLimit + 1, true);

		// Variation 3: PARALLEL_DIVISIBILITY & IMMEDIATE
        if (selectedPrintScheme == ConfigManager::PrintScheme::IMMEDIATE) {
            cout << "DOING NOW: Parallel Divisibility and Immediate Printing" << endl;
			cout << tableLines[0];

            for (int i = 2; i <= upperLimit; i++) {
                if (isNumPrime[i]) threads.push_back(async(launch::async, PrimeChecker::markImmediateNonPrimes, ref(isNumPrime), i, upperLimit)); 
                if (threads.size() >= numOfThreads) {
                    for (auto& thread : threads) thread.get();
                    threads.clear();
                }
            }

            for (auto& thread : threads) thread.get();

			cout << tableLines[1];
        }
		// Variation 4: PARALLEL_DIVISIBILITY & DEFERRED
        else {
            cout << "DOING NOW: Parallel Divisibility and Deferred Printing" << endl;

            for (int i = 2; i <= sqrt(upperLimit); i++) {
                if (isNumPrime[i]) threads.push_back(async(launch::async, PrimeChecker::markDeferredNonPrimes, ref(isNumPrime), i, upperLimit));
                if (threads.size() >= numOfThreads) {
                    for (auto& thread : threads) thread.get();
                    threads.clear();
                }
            }

            for (auto& future : threads) future.get();
            
			PrimeChecker::printParallelDeferredResults(isNumPrime);
        }
    }

    // Get end time
    auto endTime = high_resolution_clock::now();
    string endStr = getCurrentTime();
    cout << endl << "End Time:   " << endStr << endl;

    auto duration = duration_cast<milliseconds>(endTime - startTime);
    cout << "Execution Time: " << duration.count() << " ms" << endl;

	return 0;
}
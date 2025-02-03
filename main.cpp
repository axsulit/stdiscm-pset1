#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <iomanip>  
#include "ConfigManager.h"
using namespace std;

mutex printMutex;

// Check if a number is prime
static bool isPrime(int n) {
    if (n < 2) return false;  // 0 and 1 are not prime numbers
    if (n == 2 || n == 3) return true;  // 2 and 3 are prime numbers
    if (n % 2 == 0) return false;  // Exclude even numbers

    for (int i = 3; i * i <= n; i += 2) {  // Check odd numbers only
        if (n % i == 0) return false;
    }

    return true;
}

// Thread function to check primes in a given range
static void checkPrimeRange(int start, int end, int threadId) {
    for (int i = start; i <= end; ++i) {
        if (isPrime(i)) {
            auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());

            char timeBuffer[26];  // Buffer to hold formatted time
            ctime_s(timeBuffer, sizeof(timeBuffer), &now);  // Safe version of ctime()

            // Remove the newline character from timeBuffer
            timeBuffer[strcspn(timeBuffer, "\n")] = '\0';


                lock_guard<mutex> lock(printMutex);
                cout << left
                    << "| " << setw(22) << timeBuffer
                    << " || " << setw(10) << threadId  
                    << " || " << setw(7) << i  // Assuming max number is at most 10 digits
                    << " ||" << endl;

            // Simulate "loading" effect
            this_thread::sleep_for(chrono::milliseconds(100));  // 0.5-second delay per print
        }
    }
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

                threads.emplace_back(checkPrimeRange, start, end, i + 1);
            }

            // Wait for all threads to finish
            for (auto& t : threads) {
                t.join();
            }
            cout << "+--------------------------++------------++---------++" << endl;
        }
		// Variation 2: STRAIGHT_DIVISION & DEFERRED
        else {
            cout << "Straight Division and Deferred" << endl;
        }
    }
    else {
		// Variation 3: PARALLEL_DIVISIBILITY & IMMEDIATE
        if (config.getPrintScheme() == ConfigManager::PrintScheme::IMMEDIATE) {
            cout << "Parallel Divisibility and Immediate" << endl;
        }
		// Variation 4: PARALLEL_DIVISIBILITY & DEFERRED
        else {
            cout << "Parallel Divisibility and Deferred" << endl;
        }
    }

	return 0;
}
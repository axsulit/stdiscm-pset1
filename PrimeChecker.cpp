#include <ctime>

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <iomanip> 

#include "PrimeChecker.h"

using namespace std;

std::mutex PrimeChecker::printMutex;
std::vector<int> PrimeChecker::primeResults;

// Check if a number is prime
bool PrimeChecker::isPrime(int n) {
    if (n < 2) return false;  // 0 and 1 are not prime numbers
    if (n == 2 || n == 3) return true;  // 2 and 3 are prime numbers
    if (n % 2 == 0) return false;  // Exclude even numbers

    for (int i = 3; i * i <= n; i += 2) {  // Check odd numbers only
        if (n % i == 0) return false;
    }

    return true;
}

void PrimeChecker::checkPrimeRangeImmediate(int start, int end, int threadId) {
    for (int i = start; i <= end; ++i) {
        if (isPrime(i)) {
            auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());

            // Buffer to hold formatted time
            char timeBuffer[26];
            ctime_s(timeBuffer, sizeof(timeBuffer), &now);
            timeBuffer[strcspn(timeBuffer, "\n")] = '\0';


            lock_guard<mutex> lock(printMutex);
            cout << left
                << "| " << setw(22) << timeBuffer
                << " || " << setw(10) << threadId
                << " || " << setw(7) << i
                << " ||" << endl;

            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }
}


void PrimeChecker::checkPrimeRangeDeferred(int start, int end) {
    vector<int> localPrimes;

    for (int i = start; i <= end; ++i) {
        if (isPrime(i)) {
            localPrimes.push_back(i);
        }
    }

    // Lock mutex before updating shared results
    {
        lock_guard<mutex> lock(printMutex);
        primeResults.insert(primeResults.end(), localPrimes.begin(), localPrimes.end());
    }
}

vector<int> PrimeChecker::getPrimeResults() {
	return primeResults;
}
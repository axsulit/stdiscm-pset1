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
atomic<int> PrimeChecker::currentNumber(1); 
mutex PrimeChecker::resultMutex;  

bool PrimeChecker::isPrime(int n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false; 

    // Check from 5 to sqrt(n)
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }

    return true;
}

void PrimeChecker::checkPrimeRangeImmediate(int start, int end, int threadId) {
	// Loop through the range and check for prime numbers
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
        }
        this_thread::sleep_for(chrono::milliseconds(100));
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

void PrimeChecker::checkPrimeParallelImmediate(int y, int threadId) {
    while (true) {
        int num = currentNumber.fetch_add(1); 
        if (num > y) break;  // Stop when all numbers are processed

        if (isPrime(num)) {
            auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());

            char timeBuffer[26];
            ctime_s(timeBuffer, sizeof(timeBuffer), &now);
            timeBuffer[strcspn(timeBuffer, "\n")] = '\0';

            lock_guard<mutex> lock(printMutex);
            cout << left
                << "| " << setw(22) << timeBuffer
                << " || " << setw(10) << threadId
                << " || " << setw(7) << num
                << " ||" << endl;
        }

        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void PrimeChecker::checkPrimeParallelDeferred(int y) {
    vector<int> localPrimes; 

    while (true) {
        int num = currentNumber.fetch_add(1);  
        if (num > y) break;  // Stop when all numbers are processed
        if (isPrime(num)) localPrimes.push_back(num); 
    }

    // Merge results
    {
        lock_guard<mutex> lock(resultMutex);
        primeResults.insert(primeResults.end(), localPrimes.begin(), localPrimes.end());
    }
}

vector<int> PrimeChecker::getPrimeResults() {
	return primeResults;
}

void PrimeChecker::printDeferredResults() {
    cout << endl << "Prime Numbers Found:" << endl;
    int count = 0;
    for (int prime : PrimeChecker::getPrimeResults()) {
        cout << setw(5) << prime << " ";  
        if (++count % 10 == 0) cout << endl; 
    }
    cout << endl << endl;
}
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
using namespace chrono;

mutex PrimeChecker::printMutex;
vector<int> PrimeChecker::primeResults;
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

void PrimeChecker::checkPrimeRangeImmediate(int start, int end, int threadId) {
	// Loop through the range and check for prime numbers
    for (int i = start; i <= end; ++i) {
        if (isPrime(i)) {
            string currentTime = getCurrentTime();

            lock_guard<mutex> lock(printMutex);
            cout << left
                << "| " << setw(22) << currentTime
                << " || " << setw(10) << threadId
                << " || " << setw(7) << i
                << " ||" << endl;
        }
        //this_thread::sleep_for(chrono::milliseconds(50));
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
            string currentTime = getCurrentTime();

            lock_guard<mutex> lock(printMutex);
            cout << left
                << "| " << setw(22) << currentTime
                << " || " << setw(10) << threadId
                << " || " << setw(7) << num
                << " ||" << endl;
        }

        //this_thread::sleep_for(chrono::milliseconds(50));
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
    cout << endl;
}

void PrimeChecker::markNonPrimes(std::vector<bool>& isNumPrime, int div, int upperLimit) {
    for (int i = div * div; i <= upperLimit; i += div) {
        isNumPrime[i] = false;
    }
}

void PrimeChecker::printParallelDeferredResults(const vector<bool>& isNumPrime) {
    int count = 0; 
cout << "\nPrime Numbers Found:\n";
    for (size_t i = 2; i < isNumPrime.size(); i++) {
        if (isNumPrime[i]) {
            cout << setw(5) << i << " "; 
            if (++count % 10 == 0) {
                cout << endl; 
            }
        }
    }
    cout << endl; 
}
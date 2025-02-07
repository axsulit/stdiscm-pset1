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

void PrimeChecker::checkPrimeRangeImmediate(int start, int end) {
	// Loop through the range and check for prime numbers
    for (int i = start; i <= end; ++i) {
        if (isPrime(i)) {
            lock_guard<mutex> lock(printMutex);
            thread::id threadId = this_thread::get_id();
            string currentTime = getCurrentTime();
            cout << left
                << "| " << setw(22) << currentTime
                << " || " << setw(10) << threadId
                << " || " << setw(7) << i
                << " ||" << endl;
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

void PrimeChecker::printPrimeRangeDeferredResults() {
    cout << endl << "Prime Numbers Found:" << endl;
    int count = 0;
    for (int prime : PrimeChecker::getPrimeResults()) {
        cout << setw(5) << prime << " ";  
        if (++count % 10 == 0) cout << endl; 
    }
    cout << endl;
}

void PrimeChecker::markImmediateNonPrimes(std::vector<bool>& isNumPrime, int div, int upperLimit) {
    for (int i = div; i <= upperLimit; i += div) {
        isNumPrime[i] = false;
    }
	printParallelImmediateResults(div);
}

void PrimeChecker::markDeferredNonPrimes(std::vector<bool>& isNumPrime, int div, int upperLimit) {
    for (int i = div * div; i <= upperLimit; i += div) {
        isNumPrime[i] = false;
    }
}

void PrimeChecker::printParallelImmediateResults(int n) {
    lock_guard<mutex> lock(printMutex);
    thread::id threadId = this_thread::get_id();
    string currentTime = getCurrentTime();
    cout << left
        << "| " << setw(22) << currentTime
        << " || " << setw(10) << threadId
        << " || " << setw(7) << n
        << " ||" << endl;
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
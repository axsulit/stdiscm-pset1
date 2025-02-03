#include <iostream>
#include <fstream>
#include <string>

#include "ConfigManager.h"
using namespace std;

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

int main()
{
    cout << "Time to spot prime numbers!\n\n";

	ConfigManager config;
	config.readConfig(); // read config file & print values
	config.getVariationOption(); // get variation option

    if (config.getTaskDivisionScheme() == ConfigManager::TaskDivisionScheme::STRAIGHT_DIVISION) {
		// Variation 1: STRAIGHT_DIVISION & IMMEDIATE
        if (config.getPrintScheme() == ConfigManager::PrintScheme::IMMEDIATE) {
            cout << "Straight Division and Immediate" << endl;
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
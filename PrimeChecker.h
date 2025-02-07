#pragma once
#include <vector>
#include <mutex>
#include <atomic>

using namespace std;

class PrimeChecker
{
private:
	static mutex printMutex;
	static vector<int> primeResults;
	static atomic<int> currentNumber;  
	static mutex resultMutex; 

public:
	static vector<int> getPrimeResults();
	static bool isPrime(int n);
	static void checkPrimeRangeImmediate(int start, int end);
	static void checkPrimeRangeDeferred(int start, int end);
	static void printPrimeRangeDeferredResults();
	static void markImmediateNonPrimes(std::vector<bool>& isPrime, int divisor, int searchLimit);
	static void markDeferredNonPrimes(std::vector<bool>& isPrime, int divisor, int searchLimit);
	static void printParallelImmediateResults(int n);
	static void printParallelDeferredResults(const vector<bool>& isPrime);
};

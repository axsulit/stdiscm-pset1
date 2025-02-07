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
	static void markNonPrimes(std::vector<bool>& isPrime, int divisor, int searchLimit);
	static void checkPrimeRangeImmediate(int start, int end, int threadId);
	static void checkPrimeRangeDeferred(int start, int end);
	static void checkPrimeParallelImmediate(int y, int threadId);
	static void printDeferredResults();
	static void printParallelDeferredResults(const vector<bool>& isPrime);
};

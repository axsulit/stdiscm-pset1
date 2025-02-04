#pragma once
#include <vector>
#include <mutex>

using namespace std;

class PrimeChecker
{
private:
	static mutex printMutex;  
	static vector<int> primeResults;  


public:
	static vector<int> getPrimeResults();
	static bool isPrime(int n);
	static void checkPrimeRangeImmediate(int start, int end, int threadId);
	static void checkPrimeRangeDeferred(int start, int end);
};


#pragma once
class ConfigManager
{
public:
	enum class TaskDivisionScheme {
		STRAIGHT_DIVISION = 1,
		PARALLEL_DIVISIBILITY
	};

	enum class PrintScheme {
		IMMEDIATE = 1,
		DEFERRED
	};

	ConfigManager();
	void readConfig();
	void getVariationOption();
	int getNumOfThreads();
	int getUpperLimit();
	TaskDivisionScheme getTaskDivisionScheme();
	PrintScheme getPrintScheme();

private:
	int numOfThreads;
	int upperLimit;
	TaskDivisionScheme taskDivisionScheme;
	PrintScheme printScheme;
};
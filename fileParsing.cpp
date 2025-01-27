// fileParsing.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

int makeMapFromFiles(int argc, char* argv[], bool* map)
{
	ofstream resultFile("result.txt");
	ofstream resultBinFile("resultBin.txt");
	float prevVal = 0;
	float val = 0;
	char* str = new char[1024];
	char* tmpStr = new char[1024];
	char fieldTitle[]{ "<CLOSE>" };
	int columnNumber = 0;
	int tmp = 0;
	int resultCounter = 0;

	for (int fileNumber = 1; fileNumber < argc; fileNumber++) {
		ifstream inputFile(argv[fileNumber]);
		cout << argv[fileNumber] << endl;
		columnNumber = 0;
		while (true)
		{
			columnNumber++;
			inputFile >> str;
			if (strstr(str, fieldTitle) != 0) {
				cout << "Substring is found" << endl;
				break;
			}
			if (inputFile.eof()) {
				cout << "Error: end of file - there is no "
					<< fieldTitle << " field" << endl;
				return 1;
			}
		}
		inputFile.getline(tmpStr, 1024);
		while (!inputFile.eof())
		{
			tmp = columnNumber;
			inputFile.getline(str, 1024);
			stringstream string(str);
			while (tmp--)
				string >> val;
			map[resultCounter] = (val >= prevVal);
			resultBinFile << (val >= prevVal) << ' ';
			prevVal = val;
			resultCounter++;
			if (resultCounter % 25 == 0)
				resultBinFile << endl;
		}
		inputFile.close();
	}
	resultBinFile.close();
	resultFile.close();
	cout << "Sample size: " << resultCounter << endl;
	return resultCounter;
}

int readMapFromFile(bool* map)
{
	ifstream inputFile("resultBin.txt");
	long long i = 0;
	for (; !inputFile.eof(); i++)
	{
		inputFile >> map[i];
	}
	inputFile.close();
	cout << "Sample size: " << i << endl;
	return i;
}

typedef char(*FSM_t)(char state, char result);

char FSM1(char state, char result)	//68%
{
	if (result != 0 && result != 1) {
		cout << "ERROR: result is incorrect" << endl;
		return 1;
	}
	if (state < 0 && state > 3) {
		cout << "ERROR: result is incorrect" << endl;
		return 1;
	}

	return result << 1;
}

char FSM2(char state, char result)	//86%
{
	if (result != 0 && result != 1) {
		cout << "ERROR: result is incorrect" << endl;
		return 1;
	}
	if (state < 0 && state > 3) {
		cout << "ERROR: result is incorrect" << endl;
		return 1;
	}
	char fullState = (result << 2) + state;
	char newState = 0;
	switch (fullState) {
	case 0:
	case 2:
		newState = 0;
		break;
	case 4:
	case 6:
		newState = 1;
		break;
	case 1:
	case 3:
		newState = 2;
		break;
	case 5:
	case 7:
		newState = 3;
		break;
	default:
		cout << "ERROR: undefined option into the switch" << endl;
		break;
	}
	return newState;
}

char FSM3(char state, char result)	//82%
{
	if (result != 0 && result != 1) {
		cout << "ERROR: result is incorrect" << endl;
		return 1;
	}
	if (state < 0 && state > 3) {
		cout << "ERROR: result is incorrect" << endl;
		return 1;
	}
	char fullState = (result << 2) + state;
	char newState = 0;
	switch (fullState) {
	case 0:
	case 1:
		newState = 0;
		break;
	case 2:
	case 4:
		newState = 1;
		break;
	case 3:
	case 5:
		newState = 2;
		break;
	case 6:
	case 7:
		newState = 3;
		break;
	default:
		cout << "ERROR: undefined option into the switch" << endl;
		break;
	}
	return newState;
}

char FSM4(char state, char result)	//82%
{
	if (result != 0 && result != 1) {
		cout << "ERROR: result is incorrect" << endl;
		return 1;
	}
	if (state < 0 && state > 3) {
		cout << "ERROR: result is incorrect" << endl;
		return 1;
	}
	char fullState = (result << 2) + state;
	char newState = 0;
	switch (fullState) {
	case 0:
	case 1:
	case 2:
		newState = 0;
		break;
	case 4:
		newState = 1;
		break;
	case 3:
		newState = 2;
		break;
	case 7:
	case 6:
	case 5:
		newState = 3;
		break;
	default:
		cout << "ERROR: undefined option into the switch" << endl;
		break;
	}
	return newState;
}

char FSM5(char state, char result)	//82%
{
	if (result != 0 && result != 1) {
		cout << "ERROR: result is incorrect" << endl;
		return 1;
	}
	if (state < 0 && state > 3) {
		cout << "ERROR: result is incorrect" << endl;
		return 1;
	}
	char fullState = (result << 2) + state;
	char newState = 0;
	switch (fullState) {
	case 0:
	case 1:
		newState = 0;
		break;
	case 2:
	case 4:
		newState = 1;
		break;
	case 3:
		newState = 2;
		break;
	case 7:
	case 6:
	case 5:
		newState = 3;
		break;
	default:
		cout << "ERROR: undefined option into the switch" << endl;
		break;
	}
	return newState;
}

FSM_t FSMSet[] = {
	FSM1,
	FSM2,
	FSM3,
	FSM4,
	FSM5,
};

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
//OPTIMAL OPTIONS N = 17; stopCounter = 10 000 000

double fillMap(char* m, int mapSize, bool* sample, long long sampleSize, FSM_t FSM) {
	long long CNT = 0;		//command counter
	long long GHR = 0;		//shift register
	short result = 0;		//result of the prebvious operation
	short prediction = 0;	//current prediction
	long long ADDR = 0;		//address for map addressing
	char newState = 0;		//new state that FSM calculate
	long long numberOfTruePredictions = 0;
	long long numberOfPredictions = 0;
	long long sampleCounter = 0;
	double trueCounter = 0;
	double fullCounter = 0;

	char *map = (char*)calloc(mapSize, sizeof(char));

	do {
		GHR <<= 1;
		GHR += result;
		GHR %= mapSize;
		CNT++;
		CNT %= mapSize;

		ADDR = GHR;

		if (ADDR < 0) {
			cout << "ERROR: ADDR < 0\n";
			return 1;
		}

		prediction = (map[ADDR] >> 1) ? 0 : 1;

		if (sampleCounter++ >= sampleSize) {
			/*if (sampleCounter == sampleSize + 1) {
				cout << endl << endl
					<< "Now you can get a prediction(1 or 0), "
					<< "then you should type real operation result(1 or 0)."
					<< endl;
			}
			cout << "Statistics: "
				<< (double)numberOfTruePredictions * 100 / numberOfPredictions
				<< "\t";
			cout << "Prediction: " << prediction << "\t" << "Result: ";
			result = _getch() - '0';
			*/
			fullCounter++;
			if (prediction == result)
				trueCounter++;
			//cout << result << "\tManual stat: " << trueCounter * 100 / fullCounter << "%" << endl;
			
			//if (result == 9)
				break;
		}
		else {
			//HERE RESULT IS BORN1
			result = sample[sampleCounter] ? 1 : 0;
		}

		numberOfPredictions++;
		if (prediction == result)
			numberOfTruePredictions++;

		map[ADDR] = FSM(map[ADDR], result);
	} while (true);

	return (double)numberOfTruePredictions * 100 / numberOfPredictions;
}


int main()//(int argc, char* argv[])
{
	srand(time(NULL));
	char* fileList[30]{
	 (char*)"Free string",
/*	 (char*)"SBER_050101_060101.txt",
	 (char*)"SBER_060101_070101.txt",
	 (char*)"SBER_070101_080101.txt",
	 (char*)"SBER_080101_090101.txt",
	 (char*)"SBER_090101_100101.txt",
	 (char*)"SBER_100101_110101.txt",
	 (char*)"SBER_110101_120101.txt",
	 (char*)"SBER_120101_130101.txt",
*/	 (char*)"SBER_130101_140101.txt",
	 (char*)"SBER_140101_150101.txt",
	 (char*)"SBER_150101_160101.txt",
	 (char*)"SBER_160101_170101.txt",
	 (char*)"SBER_170101_180101.txt" };

	char N = 24;				//number bits into the variable
	/*
	24 ~ 55%
	23
	22
	21 ~ 53 - 55%
	20 ~ 52 - 54%
	19 ~ 51 - 54%
	18 ~ 50 - 52%
	17 ~ 50 - 51%
	16 ~ 49 - 50%
	15 ~ 48 - 49%
	14 ~ %
	13 ~ %
	12 ~ %
	11 ~ %
	10 ~ 47 - 49%
	*/
	int mapSize = pow(2, N);	//size of the map
	char *map = (char*)calloc(mapSize, sizeof(char));
	bool *sample = (bool*)calloc(1024 * 1024 * 128, sizeof(bool));
	long long sampleSize = 0;
	//sampleSize = readMapFromFile(sample);
	sampleSize = makeMapFromFiles(6, fileList, sample);

	FSM_t FSM;
	int tmpCnt = 5;
	while (tmpCnt--)
	{
		cout << "Result statistics:\t" << fillMap(map, mapSize, sample, sampleSize, FSMSet[tmpCnt]) << "%" << endl;
	}
	free(sample);
	free(map);
	system("pause");
	return 0;
}

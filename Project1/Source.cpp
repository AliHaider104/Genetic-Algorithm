#include<iostream>
#include<conio.h>
#include<windows.h>
#include<fstream>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include"Header.h"
using namespace std;

const int ROW = 110;
const int COL = 77;
const int SAMPLE_SIZE = 100;
unsigned long long int generation = 1;

void setColor(unsigned short color)
{
	// SET THE COLOR ON CONSLOLE FOR GOOD INTERFACE
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void MakeAnImageG(int** TargetImg)
{
	int x1 = 400, x2 = 402, y1 = 40, y2 = 42;
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			myRect(x1, y1, x2, y2, TargetImg[i][j], TargetImg[i][j], TargetImg[i][j], TargetImg[i][j], TargetImg[i][j], TargetImg[i][j]);
			x1 += 2;
			x2 += 2;
		}

		y1 += 2;
		y2 += 2;
		x1 = 400;
		x2 = 402;
	}
}

void MakeAnImage(int** TargetImg)
{

	for (unsigned long long int i = 0; i < ROW; i++)
	{
		for (unsigned long long int j = 0; j < COL; j++)
		{
			TargetImg[i][j] = rand() % 255;
		}
	}
}

void PrintImg(int** TargetImg)
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			cout << TargetImg[i][j] << "	";
		}
		cout << endl;
	}
}

int*** GenratePopulation()
{
	int*** Population = new int**[SAMPLE_SIZE]; // A RANDOM POPULATION

	for (unsigned long long int i = 0; i < SAMPLE_SIZE; i++) // MEMORRY ON HEAP
	{
		Population[i] = new int*[ROW];

		for (unsigned long long int j = 0; j < ROW; j++)
			Population[i][j] = new int[COL];
	}

	for ( unsigned long long int i = 0; i < SAMPLE_SIZE; i++)
	{
		MakeAnImage(Population[i]);
	}

	return Population;
}

unsigned long long int FitnessFunction(int** TargetImg, int** OurImg)
{
	
	unsigned long long int FitnessValue = 0;
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			FitnessValue = FitnessValue + abs(TargetImg[i][j] - OurImg[i][j]);

		}
	}

	return FitnessValue;
}

void Sorting(int*** Population,unsigned long long int FitnessValues[])
{
	for (unsigned long long int i = 0; i < SAMPLE_SIZE; i++)
	{
		for (unsigned long long int j = i+1; j < SAMPLE_SIZE; j++)
		{
			if (FitnessValues[i] > FitnessValues[j])
			{
				swap(FitnessValues[i], FitnessValues[j]);
				int** temp = Population[i];
				Population[i] = Population[j];
				Population[j] = temp;
			}
		}
	}
}

void Copy(int** source, int** destination)
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			destination[i][j] = source[i][j];
		}
	}
}

void Selection(int*** newPopulation,int*** Population)
{
	int sample_size = (SAMPLE_SIZE * 20) / 100; // Select top 20% from the Population and place in newPopulation

	for (int i = 0; i < sample_size; i++)
	{
		Copy(Population[i], newPopulation[i]);
	}
}

void CrossOver2(int** pop1, int ** pop2)
{
	int rPivot = rand() % ROW;
	int cPivot = rand() % COL;

	// temp memory
	int** temp = new int*[ROW];
	for (int i = 0; i < ROW; i++)
		temp[i] = new int[COL];

	for (int i = rPivot; i < ROW; i++)
	{
		for (int j = cPivot; j < COL; j++)
		{
			temp[i][j] = pop1[i][j];
		}
	}

	for (int i = rPivot; i < ROW; i++)
	{
		for (int j = cPivot; j < COL; j++)
		{
			pop1[i][j] = pop2[i][j];
		}
	}

	for (int i = rPivot; i < ROW; i++)
	{
		for (int j = cPivot; j < COL; j++)
		{
			pop2[i][j] = temp[i][j];
		}
	}

	for (int i = 0; i < ROW; i++)
		delete temp[i];
	
	delete[] temp;
	temp = NULL;
}

void CrossOver(int*** newPopulation,int*** Population)
{
	int StartIndex = (SAMPLE_SIZE * 20) / 100;
	int EndIndex = (SAMPLE_SIZE * 60) / 100;

	for (int i = 0; StartIndex<EndIndex; i++)
	{
		CrossOver2(Population[i], Population[i + 1]);
		Copy(Population[i], newPopulation[StartIndex]);
		StartIndex++;
		Copy(Population[i + 1], newPopulation[StartIndex]);
		StartIndex++;
		
	}

}

void Mutation(int*** newPopulation, int*** Population)
{
	int startIndex = (SAMPLE_SIZE * 60) / 100;
	int endIndex = (SAMPLE_SIZE * 100) / 100;
	
	while (startIndex < endIndex) {
		int rPivot = rand() % ROW;
		int cPivot = rand() % COL;
		int popNumber = rand() % (SAMPLE_SIZE-80);

		Population[popNumber][rPivot][cPivot] = rand() % 255; // toggle the value

		Copy(Population[popNumber], newPopulation[startIndex]);

		startIndex++;
	}
}

int** ReadingImage(char const filename[])
{
	int** Image = new int*[ROW];
	for (int i = 0; i < ROW; i++)
		Image[i] = new int[COL];

	ifstream fin;
	fin.open(filename);

	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			fin >> Image[i][j];
		}
	}

	return Image;
}

int main()
{

	 srand(time(0));

	// STEP #1 HAVE A TARGET IMAGE
	 setColor(4);
	cout << "						     TARGET IMAGE\n";
	setColor(15);
	cout << "PRESS ANY KEY TO START EVALUATING OUR GENERATIONS :)";
	int** TargetImg=new int*[ROW];
	for (int i = 0; i < ROW; i++)
	{
		TargetImg[i] = new int[COL];
	}
	
	TargetImg = ReadingImage("pixels.txt");

	MakeAnImageG(TargetImg);
	_getch();

	system("cls");

	// STEP #2 CREATE A RANDOM POPULATION FOR THE IMAGE
	int*** Population = GenratePopulation();



	// STEP #3 APPLY THE FITNESS FUNCTION AND SORT THE RESULT
	unsigned long long int FitnessValues[SAMPLE_SIZE];
	for (int i = 0; i < SAMPLE_SIZE; i++)
	{
		FitnessValues[i] = FitnessFunction(TargetImg, Population[i]);
	}

	Sorting(Population, FitnessValues);


	unsigned long long int BEST_OLD = 999999999; // A VALUE EQUAL TO INFINITY
	int TimeToShow = 1000;

	while (FitnessFunction(TargetImg, Population[0]) != 0) {

		TimeToShow--;

		if (TimeToShow == 0)
		{
			TimeToShow = 1000;
			system("cls");
			setColor(4);
			cout << "						  BEST GUESS SO FAR\n";
			setColor(15);
			cout << "PRESS ANY KEY TO CONTINUE IF YOU WANT MORE ACCURATE RESULT ;)";
			MakeAnImageG(Population[0]);
			_getch();
			system("cls");
		}
		for (int i = 0; i < SAMPLE_SIZE; i++)
		{
			FitnessValues[i] = FitnessFunction(TargetImg, Population[i]);

		}
		Sorting(Population, FitnessValues);

		if (BEST_OLD > FitnessFunction(TargetImg, Population[0]))
		{
			BEST_OLD = FitnessFunction(TargetImg, Population[0]);
			setColor(10);
			cout << "GENERATION : ";
			setColor(15);
			cout << generation;
			setColor(10);
			cout << " FITNESS VALUE : ";
			setColor(15);
			cout<< BEST_OLD << endl;
		}
		
		// STEP #4 CREATE A NEW POPULATION
		int*** newPopulation = new int**[SAMPLE_SIZE];

		for (int i = 0; i < SAMPLE_SIZE; i++)
		{
			newPopulation[i] = new int*[ROW];

			for (int j = 0; j < ROW; j++)
			{
				newPopulation[i][j] = new int[COL];
			}
		}

		// STEP #5 COPY TOP 20 TO THE NEW POPULATION

		Selection(newPopulation, Population);

		// STEP #6 CROSSOVER AND GET 40 MORE IN NEW POPULATION

		CrossOver(newPopulation, Population);

		// STEP #7 MUTATION AND GET 40 MORE IN NEW POPULATION

		Mutation(newPopulation, Population);

		// FREE SPACE

		for (int i = 0; i < SAMPLE_SIZE; i++)
		{
			for (int j = 0; j < ROW; j++)
			{
				delete Population[i][j];
			}

			delete[] Population[i];
		}
		delete[]Population;

		Population = newPopulation;
		newPopulation = NULL;
		generation++;
	}

	cout << "						     FINAL IMAGE\n";
	MakeAnImageG(Population[0]);


	
	_getch();
	
	

}
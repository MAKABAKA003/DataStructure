#include <random>
#include<cmath>

#ifndef RANDOM_H
#define RANDOM_H

//指数分布函数
int exprand(double lamda)
{	
	double pV = 0.0;
	while (true)
	{
		pV = (double)rand() / (double)RAND_MAX;
		if (pV != 1)
		{
			break;
		}
	}
	pV = (-1.0 / lamda) * log(1 - pV);
	pV = (int)pV;
	return pV;
}
//均匀分布函数,min为下界,max为上界
int unifrand(int min, int max)
{
	if (min > max)return 0;
	int a = rand();
	int num = a % (max - min + 1) + min;
	return num;
}
#endif // !RANDOM_H

#include <random>
#include<cmath>

#ifndef RANDOM_H
#define RANDOM_H

//ָ���ֲ�����,minΪ�½�
int exprand(double lamda,int min,int max)
{	
	double pV = 0.0;
	while (true)
	{
		srand(time(NULL));
		pV = (double)rand() / (double)RAND_MAX;
		if (pV != 1) break;
	}
	pV = (-1.0 / lamda) * log(1 - pV);
	pV = round(pV);
	if (pV > max) exit(-1);
	return pV;
}
//���ȷֲ�����,minΪ�½�,maxΪ�Ͻ�
int unifrand(int min, int max)
{
	if (min > max)return 0;
	int a = rand();
	int num = a % (max - min + 1) + min;
	return num;
}
#endif // !RANDOM_H

#pragma once

long long A = 39572931, B = 39581212, C = 9534028, M = 42424243;
int X = 17, Y = 19;

int rnd(long long a, long long b = 0)
{
	return int((A * a + B * b + C) % M);
}

template<typename T = int>
T rnd()
{
	int Z = rnd(X, Y);
	X = Y;
	Y = Z;
	return Y;
}

template<>
double rnd()
{
	int Z = rnd(X, Y);
	X = Y;
	Y = Z;
	return Y / 100.;
}

template<size_t N>
size_t rnd()
{
	return rnd<size_t>() % N;
}

void resetrand(int x = 17, int y = 19)
{
	X = x;
	Y = y;
}

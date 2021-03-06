// sieci.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include "FFNeuralNetwork.h"
#include "LinearActivation.h"
#include "../../matrix-library/include/matrix.h"
#include "FFNetworkLayer.h"


int main()
{	
	FFNeuralNetwork net;
	net << FFNetworkLayer(2, "Linear") << FFNetworkLayer(2, "Linear");

	matrix p=matrix::Add(matrix(2, 2), matrix(2, 2));
	std::cin.get();
}

#include "gtest/gtest.h"
#include "matrix.h"
#include "FFNetworkLayer.h"
#include "FFNeuralNetwork.h"
#include "NetworkTrainer.h"
#include "FFGradientDescend.h"
#include "LinearActivation.h"
#include "QuadraticCost.h"
#include <vector>
#include "Sigmoid.h"
#include "TanH.h"

/*void printNetworkError(FFGradientDescend &b, FFNeuralNetwork &net, bool layer1Bias = true)
{
	char x[1000];
	if (layer1Bias)
		sprintf(x, "network error: %.8f bias: %.8f", b.epochNetworkErr(), MX(1, 0, net.layers[1].biases));
	else
		sprintf(x, "network error: %.8f", b.epochNetworkErr());
	std::cout << x << std::endl;;
}

void printOutputWithExpected(FFNeuralNetwork &net, const matrix &expected)
{
	char x[1000];
	int l = 0;
	for (int i = 0; i < net.getOutput().rows_; i++) {
		l += sprintf(x + l, "\noutput %d: %.8f expected %d: %.8f\n", i, MX(i, 0, net.getOutput()), i, MX(i, 0, expected));
	}
	std::cout << x << std::endl;;
}

TEST(Test1000)
{
	matrix inp(2, 1);
	MX(0, 0, inp) = 1; MX(1, 0, inp) = 0.2;
	matrix exp(2, 1);
	MX(0, 0, exp) = 1.2; MX(1, 0, exp) = 0.5;

	FFNeuralNetwork net;
	net.AddLayer(2, LinearActivation());
	net.AddLayer(1000, Sigmoid());
	net.AddLayer(2, LinearActivation());
	FFGradientDescend b(net, QuadraticCost());
	b.learningRate = 0.01;
	b.momentum = 0;

	matrix t[1] = { inp };
	matrix e[1] = { exp };
	b.setTrainingSet(t, 1, e, 1);
	char x[60];
	for (int i = 0; i < 200; i++) {
		b.epoch();
		printNetworkError(b, net);
	}

	printOutputWithExpected(net, exp);
}

TEST(AddLayer)
{
	FFNeuralNetwork net;
	net.AddLayer(5, LinearActivation());
	EXPECT_EQ(5, net.getInputLayer().neuronsCount);
	EXPECT_EQ((size_t)1, net.layers.size());
	EXPECT_EQ(1, net.layers[0].neuronInputs);
}

TEST(AddLayers)
{
	FFNetworkLayer l;
	FFNeuralNetwork net;
	net.AddLayer(5, LinearActivation());
	net.AddLayer(2, LinearActivation());
	net.AddLayer(2, LinearActivation());
	EXPECT_EQ(5, net.getInputLayer().neuronsCount);
	EXPECT_EQ((size_t)3, net.layers.size());
	EXPECT_EQ(1, net.layers[0].neuronInputs);

	if (net.layers[net.layers.size() - 2].next == &net.layers[net.layers.size() - 1])
		EXPECT_EQ(1, 1);
}

TEST(RandomizeTrainingData)
{
	matrix inp(2, 1);
	MX(0, 0, inp) = 1; MX(1, 0, inp) = 0.2;
	matrix exp(2, 1);
	MX(0, 0, exp) = 1.2; MX(1, 0, exp) = 0.5;
	matrix inp2(2, 1);
	MX(0, 0, inp) = 0.9; MX(1, 0, inp) = 0.7;
	matrix exp2(2, 1);
	MX(0, 0, exp) = 1.0; MX(1, 0, exp) = 0.1;

	FFNeuralNetwork net;
	net.AddLayer(2, LinearActivation());
	net.AddLayer(2, LinearActivation());
	FFGradientDescend b(net, QuadraticCost());
	b.setRandomizeTrainingData(true);
	b.learningRate = 0.01;
	b.momentum = 0;

	matrix t[2] = { inp, inp2 };
	matrix e[2] = { exp, exp2 };
	b.setTrainingSet(t, 2, e, 2);
	b.epoch();

}

TEST(Evaluation2)
{
	FFNeuralNetwork net;
	net.AddLayer(5, LinearActivation());
	net.AddLayer(2, LinearActivation());
	net.layers[1].weights = matrix(2, 5, 1);
	matrix inp(5, 1, 5);
	matrix& y = net.evaluate(inp);
	EXPECT_EQ((double)25 + 1, MX(0, 0, y));
	EXPECT_EQ((double)25 + 1, MX(1, 0, y));
}

TEST(Evaluation4)
{
	FFNeuralNetwork net;
	net.AddLayer(5, LinearActivation());
	net.AddLayer(4, LinearActivation());
	net.AddLayer(4, LinearActivation());
	net.AddLayer(2, LinearActivation());
	net.layers[1].weights = matrix(4, 5, 1);
	net.layers[2].weights = matrix(4, 4, 2);
	net.layers[3].weights = matrix(2, 4, 1);
	matrix inp(5, 1, 5);
	matrix& y = net.evaluate(inp);
	//EXPECT_EQ((double)800 + 1, MX(0, 0, y));
	//EXPECT_EQ((double)800 + 1, MX(1, 0, y));
}



TEST(Backprop2)
{
	matrix inp(2, 1);
	MX(0, 0, inp) = 1; MX(1, 0, inp) = 0.2;
	matrix exp(2, 1);
	MX(0, 0, exp) = 1.2; MX(1, 0, exp) = 0.5;

	FFNeuralNetwork net;
	net.AddLayer(2, LinearActivation());
	net.AddLayer(2, LinearActivation());
	net.AddLayer(2, LinearActivation());
	FFGradientDescend b(net, QuadraticCost());
	b.learningRate = 0.01;
	b.momentum = 0;

	matrix t[1] = { inp };
	matrix e[1] = { exp };
	b.setTrainingSet(t, 1, e, 1);
	char x[60];
	for (int i = 0; i < 1000; i++) {
		b.epoch();
		printNetworkError(b, net);
	}

	printOutputWithExpected(net, exp);
}

TEST(Backprop2Classification)
{
	matrix inp(4, 1);
	MX(0, 0, inp) = 1; MX(1, 0, inp) = 0.2;
	MX(2, 0, inp) = 0.7; MX(3, 0, inp) = 0.1;
	matrix exp(2, 1);
	MX(0, 0, exp) = 1.2; MX(1, 0, exp) = 0.5;

	FFNeuralNetwork net;
	net.AddLayer(4, LinearActivation());
	net.AddLayer(2, LinearActivation());
	net.AddLayer(2, LinearActivation());
	NetworkTrainerBackpropagationClassification b(net, QuadraticCost());
	b.learningRate = 0.01;
	b.momentum = 0;

	matrix t[1] = { inp };
	matrix e[1] = { exp };
	b.SetTrainingSet(t, 1, e, 1);
	char x[60];
	for (int i = 0; i < 1000; i++) {
		b.Epoch();
		printNetworkError(b, net);
	}

	printOutputWithExpected(net, exp);
}

TEST(Backprop2_1000Lin)
{
	matrix inp(2, 1);
	MX(0, 0, inp) = 1; MX(1, 0, inp) = 0.2;
	matrix exp(2, 1);
	MX(0, 0, exp) = 1.2; MX(1, 0, exp) = 0.5;

	FFNeuralNetwork net;
	net.AddLayer(2, LinearActivation());
	net.AddLayer(1000, LinearActivation());
	net.AddLayer(2, LinearActivation());
	FFGradientDescend b(net, QuadraticCost());
	b.learningRate = 0.01;
	b.momentum = 0;

	matrix t[1] = { inp };
	matrix e[1] = { exp };
	b.setTrainingSet(t, 1, e, 1);
	char x[60];
	for (int i = 0; i < 1000; i++) {
		b.epoch();
		printNetworkError(b, net);
	}

	printOutputWithExpected(net, exp);
}

TEST(Backprop2_N)
{
	matrix inp(2, 1);
	MX(0, 0, inp) = 1; MX(1, 0, inp) = 0.2;
	matrix exp(2, 1);
	MX(0, 0, exp) = 1.2; MX(1, 0, exp) = 0.5;

	FFNeuralNetwork net;
	net.AddLayer(2, LinearActivation());
	net.AddLayer(2, LinearActivation(), FFNetworkLayer::RANDOM_N01);
	net.AddLayer(2, LinearActivation(), FFNetworkLayer::RANDOM_N01);
	FFGradientDescend b(net, QuadraticCost());
	b.learningRate = 0.01;
	b.momentum = 0;

	matrix t[1] = { inp };
	matrix e[1] = { exp };
	b.setTrainingSet(t, 1, e, 1);
	char x[60];
	for (int i = 0; i < 1000; i++) {
		b.epoch();
		printNetworkError(b, net);
	}

	printOutputWithExpected(net, exp);
}

TEST(Backprop2_Randomize)
{
	matrix inp(2, 1);
	MX(0, 0, inp) = 1; MX(1, 0, inp) = 0.2;
	matrix exp(2, 1);
	MX(0, 0, exp) = 1.2; MX(1, 0, exp) = 0.5;

	FFNeuralNetwork net;
	net.AddLayer(2, LinearActivation());
	net.AddLayer(2, LinearActivation());
	net.AddLayer(2, LinearActivation());
	FFGradientDescend b(net, QuadraticCost());
	b.setRandomizeTrainingData(true);
	b.learningRate = 0.01;
	b.momentum = 0;

	matrix t[1] = { inp };
	matrix e[1] = { exp };
	b.setTrainingSet(t, 1, e, 1);
	char x[60];
	for (int i = 0; i < 1000; i++) {
		b.epoch();
		printNetworkError(b, net);
	}

	printOutputWithExpected(net, exp);
}

TEST(Backprop4)
{
	char x[200];
	FFNeuralNetwork net;
	net.AddLayer(2, LinearActivation());
	net.AddLayer(2, LinearActivation());
	net.AddLayer(2, LinearActivation());
	net.AddLayer(2, LinearActivation());
	FFGradientDescend b(net, QuadraticCost());
	b.learningRate = 0.01;
	b.momentum = 0;
	b.setRandomizeTrainingData(true);
	sprintf(x, "init %f", MX(0, 0, net.getOutputLayer().output));
	std::cout << x << std::endl;;
	sprintf(x, "%f", MX(1, 0, net.getOutputLayer().output));
	std::cout << x << std::endl;;

	matrix inp(2, 1);
	MX(0, 0, inp) = 1; MX(1, 0, inp) = 0.2;
	matrix exp(2, 1);
	MX(0, 0, exp) = 2.1; MX(1, 0, exp) = 3;
	matrix inp2(2, 1);
	MX(0, 0, inp2) = 0.6; MX(1, 0, inp2) = 0.1;
	matrix exp2(2, 1);
	MX(0, 0, exp2) = 2.6; MX(1, 0, exp2) = 3.2;

	std::vector<matrix> t;
	t.push_back(inp); t.push_back(inp2);
	std::vector<matrix> e;
	e.push_back(exp); e.push_back(exp2);
	b.setTrainingSet(t, e);

	for (int i = 0; i < 10000; i++) {
		b.epoch();
		printNetworkError(b, net);
	}
	printOutputWithExpected(net, exp2);
}

TEST(Backprop4Sigmoid)
{
	char x[200];
	std::cout<<("SIGMOID");
	FFNeuralNetwork net;
	net.AddLayer(2, LinearActivation());
	net.AddLayer(2, Sigmoid());
	net.AddLayer(2, Sigmoid());
	net.AddLayer(2, Sigmoid());
	FFGradientDescend b(net, QuadraticCost());
	b.learningRate = 0.2;
	b.momentum = 0;
	sprintf(x, "init %f", MX(0, 0, net.getOutputLayer().output));
	std::cout << x << std::endl;;
	sprintf(x, "%f", MX(1, 0, net.getOutputLayer().output));
	std::cout << x << std::endl;;

	matrix inp(2, 1);
	MX(0, 0, inp) = 1; MX(1, 0, inp) = 0.2;
	matrix exp(2, 1);
	MX(0, 0, exp) = 0.2; MX(1, 0, exp) = 0.1;
	matrix inp2(2, 1);
	MX(0, 0, inp2) = 0.2; MX(1, 0, inp2) = 0.1;
	matrix exp2(2, 1);
	MX(0, 0, exp2) = 0.5; MX(1, 0, exp2) = 0.4;

	std::vector<matrix> t;
	t.push_back(inp); t.push_back(inp2);
	std::vector<matrix> e;
	e.push_back(exp); e.push_back(exp2);
	b.setTrainingSet(t, e);

	for (int i = 0; i < 10000; i++) {
		b.epoch();
		printNetworkError(b, net);
	}
	printOutputWithExpected(net, exp2);
}

TEST(Backprop4Tanh)
{
	char x[200];
	std::cout<<("TanH");
	FFNeuralNetwork net;
	net.AddLayer(2, LinearActivation());
	net.AddLayer(2, TanH());
	net.AddLayer(2, TanH());
	net.AddLayer(2, TanH());
	FFGradientDescend b(net, QuadraticCost());
	b.learningRate = 0.2;
	b.momentum = 0;
	sprintf(x, "init %f", MX(0, 0, net.getOutputLayer().output));
	std::cout << x << std::endl;;
	sprintf(x, "%f", MX(1, 0, net.getOutputLayer().output));
	std::cout << x << std::endl;;

	matrix inp(2, 1);
	MX(0, 0, inp) = 1; MX(1, 0, inp) = 0.2;
	matrix exp(2, 1);
	MX(0, 0, exp) = 0.2; MX(1, 0, exp) = 0.1;
	matrix inp2(2, 1);
	MX(0, 0, inp2) = 0.2; MX(1, 0, inp2) = 0.1;
	matrix exp2(2, 1);
	MX(0, 0, exp2) = 0.5; MX(1, 0, exp2) = 0.4;

	std::vector<matrix> t;
	t.push_back(inp); t.push_back(inp2);
	std::vector<matrix> e;
	e.push_back(exp); e.push_back(exp2);
	b.setTrainingSet(t, e);

	for (int i = 0; i < 10000; i++) {
		b.epoch();
		printNetworkError(b, net);
	}
	printOutputWithExpected(net, exp2);
}*/
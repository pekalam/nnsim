#include "gtest/gtest.h"
#include "FFNeuralNetwork.h"
#include "FFNetworkLayer.h"
#include "ActivationFunctionFactory.h"
#include "TanH.h"
#include <memory>
#include <iostream>
#include "LinearActivation.h"
#include "FFGradientDescend.h"
#include "QuadraticCost.h"
#include "NormalDistributionRVG.h"

TEST(RandomValueGeneratorFactoryTest, CreateObject)
{
	auto gen = std::make_unique<NormalDistributionRVG>();
	std::unique_ptr<RandomValueGenerator> obj(
		RandomValueGeneratorFactory::instance().createActivationFunction("N01")
		);
	ASSERT_EQ(typeid(*obj), typeid(NormalDistributionRVG));
}

TEST(ActivationFunctionFactoryTest, CreateObject)
{
	std::unique_ptr<TanH> tan(new TanH());
	std::unique_ptr<ActivationFunction> f(ActivationFunctionFactory::instance().createActivationFunction("TanH"));

	ASSERT_EQ(f->upperLimit(), tan->upperLimit());
	ASSERT_EQ(f->lowerLimit(), tan->lowerLimit());
}

TEST(ActivationFunctionFactoryTest, RegisterNewObject)
{
	std::unique_ptr<TanH> tan_h(new TanH());
	ActivationFunctionFactory::instance().registerActivationFunction("Test",tan_h.get());
	std::unique_ptr<ActivationFunction> t(ActivationFunctionFactory::instance().createActivationFunction("Test"));

	ASSERT_EQ(tan_h->upperLimit(), t->upperLimit());
	ASSERT_EQ(tan_h->lowerLimit(), t->lowerLimit());
}

TEST(FFNeuralNetworkTest, Create) {
	FFNeuralNetwork net;
	FFNetworkLayer linLayer(25, "Linear");

	net << FFNetworkLayer(10, "TanH") << FFNetworkLayer(10, "Sigmoid");
	net.addLayer(linLayer);

	ASSERT_EQ(net.getTotalLayersCount(), 3);
	ASSERT_EQ(net.getHiddenLayersCount(), 1);
	ASSERT_NE(&net.getLayer(2), &linLayer);
	ASSERT_EQ(net.getNeuronsCount(), 45);
	ASSERT_TRUE(net.getLayer(0).isInputLayer());
	ASSERT_TRUE(net.getLayer(2).isOutputLayer());
	ASSERT_TRUE(net.getLayer(1).isHiddenLayer());
	ASSERT_EQ(net.getLayer(1).getActivationFunctionName(), std::string("Sigmoid"));
}

TEST(FFNeuralNetworkTest, Evaluate)
{
	FFNeuralNetwork net;
	net << FFNetworkLayer(5, "Linear") << FFNetworkLayer(2, "Linear");
	net.getLayer(1).setWeights(matrix(2, 5, 1));
	matrix inp(5, 1, 5);
	matrix& y = net.evaluate(inp);

	ASSERT_EQ(25 + 1, MX(0, 0, y));
	ASSERT_EQ(25 + 1, MX(1, 0, y));
}

void printNetworkError(FFGradientDescend &b, FFNeuralNetwork &net, bool layer1Bias = true)
{
	char x[1000];
	if (layer1Bias)
		sprintf(x, "network error: %.8f bias: %.8f", b.lastEpochTrainingError(), MX(0, 0, net.getLayer(1).getBiases()));
	else
		sprintf(x, "network error: %.8f", b.lastEpochTrainingError());
	std::cout << x << std::endl;;
}

void printOutputWithExpected(FFNeuralNetwork &net, const matrix &expected)
{
	char x[1000];
	int l = 0;
	for (int i = 0; i < net.getOutput().rows(); i++) {
		l += sprintf(x + l, "\noutput %d: %.8f expected %d: %.8f\n", i, MX(i, 0, net.getOutput()), i, MX(i, 0, expected));
	}
	std::cout << x << std::endl;;
}

TEST(NetworkTrainer, Iteration)
{
	FFNeuralNetwork net;
	net << FFNetworkLayer(2, "Linear") << FFNetworkLayer(2, "Sigmoid") << FFNetworkLayer(2, "Linear");
	FFGradientDescend b(net, "Quadratic");
	b.learningRate = 0.07;
	b.momentum = 0;
	matrix t[2] = { matrix(2,1,1), matrix(2,1,2) };
	matrix e[2] = { matrix(2,1,1), matrix(2,1,2) };
	b.setTrainingSet(t, 2, e, 2);

	for (int i = 0; i < 2; i++) {
		b.iteration();
	}
	ASSERT_EQ(b.epochsCount(), 1);
	for (int i = 0; i < 2; i++) {
		b.iteration();
	}
	ASSERT_EQ(b.epochsCount(), 2);
}

TEST(FFTrainer, Backprop)
{
	matrix inp(2, 1);
	MX(0, 0, inp) = 1; MX(1, 0, inp) = 0.2;
	matrix exp(2, 1);
	MX(0, 0, exp) = 1.2; MX(1, 0, exp) = 0.5;

	FFNeuralNetwork net;
	net << FFNetworkLayer(2, "Linear") << FFNetworkLayer(2, "Sigmoid") << FFNetworkLayer(2, "Linear");
	FFGradientDescend b(net, "Quadratic");
	b.learningRate = 0.07;
	b.momentum = 0;
	matrix t[1] = { inp };
	matrix e[1] = { exp };
	b.setTrainingSet(t, 1, e, 1);

	for (int i = 0; i < 1000; i++) {
		b.epoch();
		printNetworkError(b, net);
	}

	printOutputWithExpected(net, exp);
}

TEST(FFTrainer, Backprop4Sigmoid)
{
	char x[200];
	std::cout << ("SIGMOID");
	FFNeuralNetwork net;
	net << FFNetworkLayer(2, "Linear") << FFNetworkLayer(27, "Sigmoid") << FFNetworkLayer(30, "Sigmoid") << FFNetworkLayer(2, "Sigmoid");
	FFGradientDescend b(net, "Quadratic");
	b.learningRate = 0.2;
	b.momentum = 0.6;

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
	std::cout << ("TanH");
	FFNeuralNetwork net;
	net << FFNetworkLayer(2, "Linear") << FFNetworkLayer(20, "TanH") << FFNetworkLayer(20, "TanH") << FFNetworkLayer(2, "TanH");
	FFGradientDescend b(net, "Quadratic");
	b.learningRate = 0.05;
	b.momentum = 0.1;

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
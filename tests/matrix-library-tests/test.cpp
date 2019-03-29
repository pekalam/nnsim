#include "gtest/gtest.h"
#include "matrix.h"


matrix::MATRIX_ACCEL accel = matrix::MATRIX_ACCEL::MULTI_CPU;

	TEST(Create)
	{
		matrix a(2, 2, 0, accel);
		MX(0, 0, a) = 1; MX(0, 1, a) = 2; MX(1, 0, a) = 3; MX(1, 1, a) = 4;

		matrix b( { {1,2},
					{3,4},
					{5,6}}, accel );

		EXPECT_EQ(4, MX(1, 1, a));
		EXPECT_EQ(1, MX(0, 0, a));
		EXPECT_EQ(2, a.rows());
		EXPECT_EQ(2, a.cols());
		EXPECT_EQ(1, MX(0, 0, b));
		EXPECT_EQ(2, MX(0, 1, b));
		EXPECT_EQ(3, b.rows());
		EXPECT_EQ(2, b.cols());
	}

	TEST(Copy)
	{
		matrix a(2, 2, 0, accel);
		MX(0, 0, a) = 1; MX(0, 1, a) = 2; MX(1, 0, a) = 3; MX(1, 1, a) = 4;
		matrix b(a);
		EXPECT_EQ(4, MX(1, 1, b));
	}

	TEST(Move)
	{
		matrix a(2, 2, 0, accel);
		MX(0, 0, a) = 1; MX(0, 1, a) = 2; MX(1, 0, a) = 3; MX(1, 1, a) = 4;
		matrix b = std::move(a);
		bool t = false;
		EXPECT_EQ(4, MX(1, 1, b));

	}

	TEST(ShallowCpy)
	{
		matrix a(2, 2, 0, accel);
		MX(0, 0, a) = 1; MX(0, 1, a) = 2; MX(1, 0, a) = 3; MX(1, 1, a) = 4;
		matrix b = matrix::shallow_copy(a);
		MX(0, 0, a) = 1; MX(0, 1, a) = 2; MX(1, 0, a) = 3; MX(1, 1, a) = 9;
		EXPECT_EQ(9, MX(1, 1, b));

		matrix c = b;
		EXPECT_EQ(9, MX(1, 1, c));
		MX(1, 1, c) = 0.5;
		EXPECT_EQ(0.5, MX(1, 1, c));
		EXPECT_NE(0.5, MX(1, 1, b));
	}

	TEST(DeepCpy)
	{
		matrix a(2, 2, 0, accel);
		MX(0, 0, a) = 1; MX(0, 1, a) = 2; MX(1, 0, a) = 3; MX(1, 1, a) = 4;
		matrix b = matrix::deep_copy(a);
		MX(0, 0, a) = 1; MX(0, 1, a) = 2; MX(1, 0, a) = 3; MX(1, 1, a) = 9;
		EXPECT_NE(9, MX(1, 1, b));
	}

	TEST(Add)
	{
		matrix a(2, 2, 0, accel);
		matrix b(2, 2, 0, accel);
		MX(0, 0, a) = 1; MX(0, 1, a) = 2; MX(1, 0, a) = 3; MX(1, 1, a) = 4;
		MX(0, 0, b) = 1; MX(0, 1, b) = 2; MX(1, 0, b) = 3; MX(1, 1, b) = 4;
		matrix x = a + b;
		EXPECT_EQ(2, MX(0, 0, x));
	}

	TEST(Subtract)
	{
		matrix a(2, 2, 0, accel);
		matrix b(2, 2, 0, accel);
		MX(0, 0, a) = 1; MX(0, 1, a) = 2; MX(1, 0, a) = 3; MX(1, 1, a) = 4;
		MX(0, 0, b) = 1; MX(0, 1, b) = 2; MX(1, 0, b) = 4; MX(1, 1, b) = 4;
		matrix x = a - b;
		EXPECT_EQ(0, MX(0, 0, x));
		EXPECT_EQ(-1, MX(1, 0, x));

		a -= b;
		EXPECT_EQ(0, MX(0, 0, a));
		EXPECT_EQ(-1, MX(1, 0, a));
	}

	TEST(Pow)
	{
		matrix a(2, 2, 0, accel);
		int p = 3;
		MX(0, 0, a) = 1; MX(0, 1, a) = 2; MX(1, 0, a) = 3; MX(1, 1, a) = 4;
		matrix::Pow_inp(a, p);
		EXPECT_EQ(1, MX(0, 0, a));
		EXPECT_EQ(27, MX(1, 0, a));
	}

	TEST(Multiply)
	{
		matrix a(1, 2, 0, accel);
		matrix b(2, 1, 0, accel);
		MX(0, 0, a) = 1; MX(0, 1, a) = 2;
		MX(0, 0, b) = 5; MX(1, 0, b) = 6;
		matrix x = matrix::Multiply(a, b);
		EXPECT_EQ(17, MX(0, 0, x));


		MX(0, 0, a) = 1; MX(0, 1, a) = 2;
		MX(0, 0, b) = 5; MX(1, 0, b) = 6;
		x = a * b;
		EXPECT_EQ(17, MX(0, 0, x));
		x *= 2;
		EXPECT_EQ(34, MX(0, 0, x));

		a = matrix(2, 3, accel);
		b = matrix(2, 3, accel);
		MX(0, 0, a) = 1; MX(0, 1, a) = 1; MX(0, 2, a) = 1;
		MX(1, 0, a) = 2; MX(1, 1, a) = 2; MX(1, 2, a) = 2;
		MX(0, 0, b) = 3; MX(0, 1, b) = 5; MX(0, 2, b) = 7;
		MX(1, 0, b) = 4; MX(1, 1, b) = 6; MX(1, 2, b) = 8;
		if (matrix::Multiply(a, b).isEmpty())
			EXPECT_EQ(1, 1);
		else
			EXPECT_EQ(0, 1);
		x = matrix::Multiply_BT(a, b);
		EXPECT_EQ(x.rows(), 2);
		EXPECT_EQ(x.cols(), 2);
		EXPECT_EQ(15, MX(0, 0, x));
		EXPECT_EQ(30, MX(1, 0, x));
		EXPECT_EQ(18, MX(0, 1, x));
		EXPECT_EQ(36, MX(1, 1, x));
	}

	TEST(Sigmoid)
	{
		matrix a(1, 2, 0, accel);
		MX(0, 0, a) = 20; MX(0, 1, a) = 0.3;
		matrix b(2, 2, 0, accel);
		MX(0, 0, b) = 0.5; MX(0, 1, b) = 0.3;
		MX(1, 0, b) = 0.1; MX(1, 1, b) = 1.5;
		matrix x = matrix::Sigmoid(a);
		matrix x2 = matrix::Sigmoid_D(b);
		EXPECT_EQ(2, x2.rows());
		EXPECT_EQ(2, x2.cols());
	}

	TEST(TanH)
	{
		matrix a(1, 2, 0, accel);
		MX(0, 0, a) = 20; MX(0, 1, a) = 0.3;
		matrix x = matrix::Tanh(a);
		EXPECT_EQ(1, x.rows());
		EXPECT_EQ(2, x.cols());
		EXPECT_TRUE(MX(0, 0, x) <= 1);
		matrix b(2, 2, 0, accel);
		MX(0, 0, b) = 0.5; MX(0, 1, b) = 0.3;
		MX(1, 0, b) = 0.1; MX(1, 1, b) = 1.5;
		matrix x2 = matrix::Tanh_D(b);
		EXPECT_EQ(2, x2.rows());
		EXPECT_EQ(2, x2.cols());
	}

	TEST(DotProd)
	{
		matrix a(1, 2, 0, accel);
		matrix b(1, 2, 0, accel);
		MX(0, 0, a) = 1; MX(0, 1, a) = 2;
		MX(0, 0, b) = 5; MX(0, 1, b) = 6;
		matrix x = matrix::Hadamard(a, b);

		EXPECT_EQ(x.rows(), 1);
		EXPECT_EQ(x.cols(), 2);
		EXPECT_EQ(5, MX(0, 0, x));
		EXPECT_EQ(12, MX(0, 1, x));
	}

	TEST(Transpose) {

		matrix a(2, 3, 0, accel);
		MX(0, 0, a) = 3; MX(0, 1, a) = 5; MX(0, 2, a) = 7;
		MX(1, 0, a) = 4; MX(1, 1, a) = 6; MX(1, 2, a) = 8;
		matrix x = matrix::Transpose(a);

		EXPECT_EQ(5, MX(1, 0, x));
		EXPECT_EQ(8, MX(2, 1, x));
	}

	TEST(Copying)
	{
		matrix b(2, 2, 0, matrix::DEFAULT);
		MX(0, 0, b) = 1; MX(0, 1, b) = 2; MX(1, 0, b) = 3; MX(1, 1, b) = 4;
		std::vector<matrix> v;
		v.push_back(b);
		MX(0, 0, b) = 99;
		EXPECT_NE(99, MX(0, 0, v.at(0)));
	}

	TEST(MemLeak)
	{
		matrix a(2, 3, 0, accel);
		matrix b = std::move(a);
		EXPECT_TRUE(a.isEmpty());
		matrix x(2, 3);
		EXPECT_FALSE(x.isEmpty());

	}

	TEST(CrossOperations)
	{
		matrix a(2, 2, 0, matrix::MULTI_CPU);
		matrix b(2, 2, 0, matrix::DEFAULT);
		MX(0, 0, a) = 1; MX(0, 1, a) = 2; MX(1, 0, a) = 3; MX(1, 1, a) = 4;
		MX(0, 0, b) = 2; MX(0, 1, b) = 2; MX(1, 0, b) = 3; MX(1, 1, b) = 4;
		matrix x = a + b;
		EXPECT_EQ(3, MX(0, 0, x));

		x = a * b;
		EXPECT_EQ(8, MX(0, 0, x));
		EXPECT_EQ(10, MX(0, 1, x));

		matrix a2(2, 2, 0, matrix::DEFAULT);
		matrix b2(2, 2, 0, matrix::MULTI_CPU);
		MX(0, 0, a2) = 1; MX(0, 1, a2) = 2; MX(1, 0, a2) = 3; MX(1, 1, a2) = 4;
		MX(0, 0, b2) = 2; MX(0, 1, b2) = 2; MX(1, 0, b2) = 3; MX(1, 1, b2) = 4;
		x = a2 + b2;
		EXPECT_EQ(3, MX(0, 0, x));
		x += b2;
		EXPECT_EQ(5, MX(0, 0, x));
		x = a2 * b2;
		EXPECT_EQ(8, MX(0, 0, x));
		EXPECT_EQ(10, MX(0, 1, x));

		x *= 2;
		EXPECT_EQ(16, MX(0, 0, x));

		x -= b2;
		EXPECT_EQ(14, MX(0, 0, x));
	}


	TEST(AccelerationChange)
	{
		matrix a(2, 2, 0, matrix::MULTI_CPU);
		matrix b(2, 2, 0, matrix::DEFAULT);
		MX(0, 0, a) = 1; MX(0, 1, a) = 2; MX(1, 0, a) = 3; MX(1, 1, a) = 4;
		MX(0, 0, b) = 2; MX(0, 1, b) = 2; MX(1, 0, b) = 3; MX(1, 1, b) = 4;
		a.changeAcceleration(matrix::DEFAULT);
		matrix x = a + b;
		EXPECT_EQ(3, MX(0, 0, x));

		b.changeAcceleration(matrix::MULTI_CPU);
		x = a * b;
		EXPECT_EQ(8, MX(0, 0, x));
		EXPECT_EQ(10, MX(0, 1, x));
	}
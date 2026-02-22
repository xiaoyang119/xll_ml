// fms_math.h - Some constexpr math functions
#pragma once
#include <limits>

namespace fms::math {

	template<class X = double>
	constexpr X epsilon = std::numeric_limits<X>::epsilon();

	template<class X = double>
	constexpr X infinity = std::numeric_limits<X>::infinity();

	template<class X = double>
	constexpr X NaN = std::numeric_limits<X>::quiet_NaN();
	static_assert(NaN<double> != NaN<double>);

	template<class X = double>
	constexpr bool isnan(X x) {
		return x != x;
	}

	template<class X>
	constexpr X abs(X x) {
		return x < 0 ? -x : x;
	}
	static_assert(abs(1) == 1);
	static_assert(abs(0) == 0);
	static_assert(abs(-1) == 1);

	template<class X>
	constexpr X pow(X x, int n)
	{
		return n == 0 ? 1 : n > 0 ? x * pow(x, n - 1) : 1 / pow(x, -n);
	}

	template<class X>
	constexpr bool samesign(X x, X y)
	{
		return (x > 0 && y > 0) || (x < 0 && y < 0);
	}

	// Constexpr square root using Newton-Raphson method
	template<class X>
	constexpr X sqrt(X x, X guess, int iterations = 10) 
	{
		return iterations == 0 ? guess : sqrt(x, (guess + x / guess) / 2, iterations - 1);
	}

	template<class X>
	constexpr X sqrt(X x) 
	{
		return x == 0 ? 0 : sqrt(x, x / 2, 20);
	}

	template<class X>
	constexpr X sqrt_epsilon = sqrt(epsilon<X>);	

	template<class X>
	constexpr X exp_approx(X x, int terms = 20) 
	{
		X sum = 1.0;
		X term = 1.0;
		for (int n = 1; n < terms; ++n) {
			term *= x / n;
			sum += term;
		}
		return sum;
	}

	// Abramowitz& Stegun approximation
	// Maximum error: ~1.5e-7
	template<class X>
	constexpr X erf_as(X x) 
	{
		if (x == 0) return 0;

		constexpr X a1 = 0.254829592;
		constexpr X a2 = -0.284496736;
		constexpr X a3 = 1.421413741;
		constexpr X a4 = -1.453152027;
		constexpr X a5 = 1.061405429;
		constexpr X p = 0.3275911;

		// Save the sign of x
		int sign = (x < 0) ? -1 : 1;
		x = fms::math::abs(x);

		// A&S formula 7.1.26
		X t = 1.0 / (1.0 + p * x);
		X t2 = t * t;
		X t3 = t2 * t;
		X t4 = t3 * t;
		X t5 = t4 * t;

		X y = 1.0 - (((((a5 * t5 + a4 * t4) + a3 * t3) + a2 * t2) + a1 * t) * exp_approx(-x * x));

		return sign * y;
	}

	static_assert(erf_as(0.0) == 0.0);
	static_assert(erf_as(1.0) > 0.842 && erf_as(1.0) < 0.843);

} // namespace fms::math

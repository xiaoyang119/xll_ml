// fms_option_normal.h - Black model with normal distribution
#pragma once
#include <cmath>
#include <numbers>
#include "fms_math.h"
#include "fms_option.h"

namespace fms::option {

	template<class X = double, class S = double>
	struct normal : base<X, S> {
		using base<X, S>::T;

	private:
		// Standard normal cumulative distribution function
		static X cdf(X x)
		{
			return 0.5 * (1 + math::erf_as(x / std::numbers::sqrt2));
		}
	public:
		// cumulative distribution function
		T _cdf(X x, S s) const override
		{
			return cdf(x - s);
		}
		// cumulant generating function
		S _cgf(S s) const override
		{
			return s * s /2;
		}
	};

} // namespace fms::option::black

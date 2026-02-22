// fms_option_discrete.h - Discrete distribution for option pricing	
#pragma once
#include <valarray>
#include "fms_option.h"

namespace fms::option::discrete {
	template<class X = double, class S = double>
	class model : option::base<X, S> {
		std::valarray<X> xi, pi; // x_i, p_i >= 0, sum p_i = 1
	
		void normalize()
		{
			pi /= pi.sum();
			X Ex = (xi * pi).sum();
			xi -= Ex; // mean 0
			xi /= std::sqrt((xi * xi * pi).sum()); // variance 1
		}
	public:
		using option::base<X, S>::T;
	
		// E[1(X <= x) exp(s X - kappa(s))] 
		//   = sum_{x_i <= x} p_i exp(s x_i - kappa(s))
		T _cdf(X x, S s) const override
		{
			return 0;
		}
	
		// kappa(s) = log E[exp(s X)] = log sum p_i exp(s x_i)
		S _cgf(S s) const override
		{
			return 0;
		}
	};
} // namespace fms::option::discrete
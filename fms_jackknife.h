// fms_jackknife.h - Jackknife resampling for statistical estimation
#pragma once
#include <numeric>

namespace fms {

	// x_[i] = (sum(x) - x[i]) / (n - 1)
	template<class X = double>
	inline X* jackknife(size_t n, X* x)
	{
		X sum = std::accumulate(x, x + n, 0.0);
		for (size_t i = 0; i < n; ++i) {
			x[i] = (sum  - x[i])/ (n - 1);
		}

		return x;
	}

	
	inline int jackknife_test()
	{
		{
			double x[] = { 1,2,3 };
			auto jk = jackknife(3, (double*)x);
			assert(jk[0] == 2.5);
			assert(jk[1] == 2.0);
			assert(jk[2] == 1.5);

			return 0;
		}
	}
} // namespace fms
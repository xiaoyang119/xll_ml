// fms_curve_bootstrap.h - Bootstrap a piecewise-flat curve forward curve.
#pragma once
#ifdef _DEBUG
#include <cassert>
#endif
#include <utility>
#include "fms_instrument.h"
#include "fms_curve_pwflat.h"
#include "fms_valuation.h"
#include "fms_math.h"

namespace fms::curve {

	// bootstrap1 - cash deposit
	// bootstrap2 - forward rate agreement

	// TODO: Adjust this to take instrument::base<U,C>
	// Bootstrap a single instrument given last time on curve and optional initial forward rate guess.
	// Return point on the curve repricing the instrument.
	template<class U, class C, class T = double, class F = double>
	inline std::pair<T, F> bootstrap0(instrument::base<U, C> i, const curve::base<T, F>& f,
		T _t, F _f = math::NaN<F>, F p = 0)
	{
		const auto uc = i.last(); // last instrument cash flow
		if (uc.u <= _t) {
			return { math::NaN<T>, math::NaN<F> };
		}

		// fix up initial guess
		if (std::isnan(_f)) {
			_f = f(_t); // last forward rate
		}
		if (std::isnan(_f)) {
			_f = 0.01;
		}

		const auto vp = [i, &f, _t, p](F f_) { return value::present(i, extrapolate(f, _t, f_)) - p; };

		auto [f_, tol, n] = root1d::secant(_f, _f + 0.01).solve(vp);
		_f = f_;

		return { uc.u, _f };
	}

	// Bootstrap a piecewise flat curve from instruments and prices.
	template<class I, class U = double, class C = double, class P = double>
	constexpr curve::pwflat<U, P> bootstrap(std::span<const I> is, std::span<const P> ps,
		double _t = 0, double _f = 0.03)
		//requires std::convertible_to<I,const instrument::base<U,C>&>
	{
		curve::pwflat<U, P> f;

		/*
		// TODO: Use a for loop like in the present value function.
		// Assume is[i] is a const reference to an instrument<U,C>
		while (is and ps) {
			std::tie(_t, _f) = bootstrap0(*is, f, _t, _f, *ps);
			f.push_back(_t, _f);
			// f.push_back(bootstrap0(...));
			++is;
			++ps;
		}
		*/

		return f;
	}
#ifdef _DEBUG
	inline int bootstrap_test()
	{
		// TODO: Delete code that is not working.
		/*
		using namespace fms::iterable;
		{
			curve::constant<> f;
			double r = 0.1;
			auto zcb = instrument::zero_coupon_bond(1, std::exp(r));
			auto D = f.discount(1, 0., r);
			assert(D < 1);
			auto p = value::present(zcb, extrapolate(f, 0., r));
			assert(p == 1);
			auto d = value::duration(zcb, extrapolate(f, 0., r));
			assert(d == -1);
			auto [_t, _f] = curve::bootstrap0(zcb, f, 0., 0.2, 1.);
			assert(_t == 0);
			assert(std::fabs(_f - r) <= math::sqrt_epsilon<double>);
		}
		*/

		return 0;
	}
#endif // _DEBUG

}
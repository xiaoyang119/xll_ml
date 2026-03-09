// xll_bootstrap.cpp - bootstrap functions
#include <vector>
#include "fms_bootstrap.h"
#include "xll_fi.h"
 
using namespace fms;
using namespace xll;

Auto<OpenAfter> xoa_bootstrap_test([](){ curve::bootstrap_test(); return 1; });

AddIn xai_curve_pwflat_bootstrap_(
	Function(XLL_HANDLEX, L"xll_curve_pwflat_bootstrap_", L"\\" CATEGORY L".CURVE.PWFLAT.BOOTSTRAP.")
	.Arguments({
		Arg(XLL_FP, L"i", L"is an array of instrument handles."),
		Arg(XLL_FP, L"p", L"is an array of prices."),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return a handle to a pwflat bootstrapped curve.")
);
HANDLEX WINAPI xll_curve_pwflat_bootstrap_(_FP12* pi, _FP12* pp)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		// Validate array sizes
		ensure(size(*pi) > 0 || !"bootstrap: instrument array cannot be empty");
		ensure(size(*pp) > 0 || !"bootstrap: price array cannot be empty");
		ensure(size(*pi) == size(*pp) || !"bootstrap: instrument and price arrays must have same size");

		int n = size(*pi);

		std::vector<instrument::instrument<>*> is(n);
		
		for (int i = 0; i < n; ++i) {
			// Pointers to instruments from the HANDELX in pi.
			handle<instrument::base<>> inst(pi->array[i]);
			ensure(inst || !__FUNCTION__ ": invalid instrument handle");
			is[i] = inst.as<instrument::instrument<>>();  // Get raw pointer from handle
		}
		// Bootstrap using the corrected function signature
		auto f = curve::bootstrap(std::span(is.data(), is.size()), span(*pp));
		// Create handle to the bootstrapped curve
		handle<curve::base<>> h_(new curve::pwflat(std::move(f)));
		ensure(h_);
		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
	}

	return h;
}

// xll_valuation.cpp - valuation routines
#include "fms_valuation.h"
#define CATEGORY L"FI"
#include "xll_ml.h"

using namespace fms;
using namespace xll;

AddIn xai_value_present(
	Function(XLL_DOUBLE, L"xll_valuation_present", CATEGORY L".VALUATION.PRESENT")
	.Arguments({
		Arg(XLL_HANDLEX, L"i", L"is a handle to an insturument."),
		Arg(XLL_HANDLEX, L"c", L"is a handle to a curve."),
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Return the present value of instrument given a curve.")
);
double WINAPI xll_valuation_present(HANDLEX i, HANDLEX c)
{
#pragma XLLEXPORT
	double pv = math::NaN<>;

	try {
		handle<instrument::base<>> i_(i);
		ensure(i_);
		handle<curve::base<>> c_(c);
		ensure(c_);

		pv = value::present(*i_, *c_);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
	}

	return pv;
}

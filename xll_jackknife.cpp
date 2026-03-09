// xll_jackknife.cpp - Jackknife resampling
#include "xll24/include/xll.h"
#include "fms_jackknife.h"	

using namespace xll;

Auto<Open> xao_jackknife([]() {
	// ensure fms_jackknife is linked
	fms::jackknife_test();
	return true;
	});

#define CATEGORY L"STAT"

AddIn xai_jackknife(
	Function(XLL_FP, L"xll_jackknife", CATEGORY L".JK")
	.Arguments({
		Arg(XLL_FP, L"x", L"is the array of observations."),
	})
	.Category(CATEGORY)
	.FunctionHelp(L"Return the jackknife resampling estimates for the mean of x.")
);
_FP12* WINAPI xll_jackknife(_FP12* x)
{
#pragma XLLEXPORT
	try {
		int n = size(*x);
		fms::jackknife(n, x->array);
	
		return x;
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return nullptr;
	}
	catch (...) {
		XLL_ERROR("xll_jackknife: unknown exception");
		return nullptr;
	}
}
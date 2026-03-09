//xll_option_discrete.cpp
#include "fms_option_discrete.h"
#include "xll_ml.h"

#undef CATEGORY
#define CATEGORY L"OPTION"

using namespace xll;
using namespace fms::option;

AddIn xai_option_discrete(
	Function(XLL_HANDLEX, L"xll_option_discrete", L"\\" CATEGORY L".DISCRETE")
	.Arguments({
		Arg(XLL_FP,L"x", L"is the xi values."),
		Arg(XLL_FP,L"p", L"is the pi values."),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return handle to discrete option pricing model.")
);
HANDLEX WINAPI xll_option_discrete(_FP12* px, _FP12* pp)
{
#pragma XLLEXPORT
		HANDLEX result = INVALID_HANDLEX;
		try {
			handle<base<>> m_(new discrete::model<>{(std::size_t)size(*px), px->array, pp->array});
			ensure(m_);
			result = m_.get();
		}
		catch (const std::exception& ex) {
			XLL_ERROR(ex.what());
		}
		catch (...) {
			XLL_ERROR(__FUNCTION__ ": unknown exception");
		}
	return result;
}

AddIn xai_option_discrete_xi(
	Function(XLL_FP, L"xll_option_discrete_xi", CATEGORY L".DISCRETE.XI")
	.Arguments({
		Arg(XLL_HANDLEX, L"m", L"is the handle to discrete option pricing model."),
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Return normalized xi values")
);
_FP12* WINAPI xll_option_discrete_xi(HANDLEX m)
{
#pragma XLLEXPORT
	static FPX result;
	try {
		result.resize(0, 0);
		handle<base<>> m_(m);
		ensure(m_);
		discrete::model<>* pm = m_.as<discrete::model<>>();
		const auto& xi = pm->xi;
		int n = (int)xi.size();
		result.resize(1, n);
		std::copy_n(&xi[0], n, result.array());
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
	}
	return result.get();
}
// xll_instrument.cpp - times and cash flows of an instrument
#include "xll_fi.h"
#include "fms_instrument.h"

using namespace xll;
using namespace fms;

AddIn xai_instrument_(
	Function(XLL_HANDLEX, L"xll_instrument_", L"\\" CATEGORY L".INSTRUMENT")
	.Arguments({
		Arg(XLL_FP, L"u", L"is the vector of cash flow times."),
		Arg(XLL_FP, L"c", L"is the vector of cash flow amounts."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return a handle to an instrument with cash flows c at times u.")
);
HANDLEX WINAPI xll_instrument_(_FP12* pu, _FP12* pc)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		handle<instrument::base<>> h_(new instrument::instrument(span(*pu), span(*pc)));
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

AddIn xai_instrument(
	Function(XLL_FP, L"xll_instrument", CATEGORY L".INSTRUMENT")
	.Arguments({
		Arg(XLL_HANDLEX, L"h", L"is a handle to an instrument."),
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Return two row array of cash flow times and amounts.")
);
_FP12* WINAPI xll_instrument(HANDLEX h)
{
#pragma XLLEXPORT
	static FPX uc;

	try {
		uc.resize(0, 0);
		handle<instrument::base<>> h_(h);
		ensure(h_);
		int n = static_cast<int>(h_->size());
		uc.resize(2, n);
		std::copy_n(h_->time(), n, uc.array());
		std::copy_n(h_->cash(), n, uc.array() + n);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return nullptr;
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
		return nullptr;
	}

	return uc.get();
}

AddIn xai_zero_coupon_bond(
	Function(XLL_HANDLEX, L"xll_zero_coupon_bond", L"\\" CATEGORY L".INSTRUMENT.ZERO_COUPON_BOND")
	.Arguments({
		Arg(XLL_DOUBLE, L"u", L"is the time of the payment."),
		Arg(XLL_DOUBLE, L"c", L"is the amount of the payment. Default is 1."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return a handle to a zero coupon bond with payment c at time u.")
);
HANDLEX WINAPI xll_zero_coupon_bond(double u, double c)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;
	try {
		if (c == 0) {
			c = 1;
		}
		handle<instrument::base<>> h_(new instrument::zero_coupon_bond<>(u, c));
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

AddIn xai_bond(
	Function(XLL_HANDLEX, L"xll_bond", L"\\" CATEGORY L".INSTRUMENT.BOND")
	.Arguments({
		Arg(XLL_DOUBLE, L"u", L"is the maturity in years."),
		Arg(XLL_DOUBLE, L"c", L"is the coupon."),
		Arg(XLL_UINT, L"f", L"is the frequency of coupon payments per year."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return a handle to a bond with payments c at times u.")
);
HANDLEX WINAPI xll_bond(double u, double c, UINT f)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;
	try {
		handle<instrument::base<>> h_(new instrument::bond(u, c, static_cast<instrument::frequency>(f)));
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
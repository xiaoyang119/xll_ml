// xll_curve.cpp - curve functions
#include "xll_fi.h"
#include "fms_curve_pwflat.h"

using namespace xll;
using namespace fms;

static AddIn xai_curve_pwflat_(
	Function(XLL_HANDLEX, L"xll_curve_pwflat_", L"\\" CATEGORY L".CURVE.PWFLAT")
	.Arguments({
		Arg(XLL_FP, L"t", L"is the vector of forward rate times."),
		Arg(XLL_FP, L"f", L"is the vector of forward rates."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return a handle to a curve with forward rates f at times t.")
);
HANDLEX WINAPI xll_curve_pwflat_(_FP12* pt, _FP12* pf)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		handle<curve::base<>> h_(new curve::pwflat(span(*pt), span(*pf)));
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

static AddIn xai_curve_pwflat(
	Function(XLL_FP, L"xll_curve_pwflat", CATEGORY L".CURVE.PWFLAT")
	.Arguments({
		Arg(XLL_HANDLEX, L"h", L"is a handle to a pwflat curve."),
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Return a two row array of times and rates.")
);
_FP12* WINAPI xll_curve_pwflat(HANDLEX h)
{
#pragma XLLEXPORT
	static FPX tf;

	try {
		tf.resize(0, 0);
		handle<curve::base<>> h_(h);
		ensure(h_);
		curve::pwflat<>* ptf = h_.as<curve::pwflat<>>();
		int n = (int)ptf->size();
		tf.resize(2, n);
		std::copy_n(ptf->time(), n, tf.array());
		std::copy_n(ptf->rate(), n, tf.array() + n);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
	}

	return tf.get();
}

// TODO: Implement CURVE.FORWARD, CURVE.DISCOUNT, CURVE.SPOT
// use `handle<curve::base<>> h_(h)

// ── CURVE.FORWARD ──────────────────────────────────────────────────

static AddIn xai_curve_forward(
    Function(XLL_FP, L"xll_curve_forward", CATEGORY L".CURVE.FORWARD")
    .Arguments({
        Arg(XLL_HANDLEX, L"h", L"is a handle to a curve."),
        Arg(XLL_FP,      L"u", L"is an array of times."),
        })
        .Category(CATEGORY)
    .FunctionHelp(L"Return the forward rate f(u) for each time u.")
);
_FP12* WINAPI xll_curve_forward(HANDLEX h, _FP12* pu)
{
#pragma XLLEXPORT
    try {
        handle<curve::base<>> h_(h);
        ensure(h_);

        for (int i = 0; i < size(*pu); ++i) {
            pu->array[i] = h_->forward(pu->array[i]);
        }
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
        return nullptr;
    }

    return pu;
}

// ── CURVE.DISCOUNT ─────────────────────────────────────────────────

static AddIn xai_curve_discount(
    Function(XLL_FP, L"xll_curve_discount", CATEGORY L".CURVE.DISCOUNT")
    .Arguments({
        Arg(XLL_HANDLEX, L"h", L"is a handle to a curve."),
        Arg(XLL_FP,      L"u", L"is an array of times."),
        })
        .Category(CATEGORY)
    .FunctionHelp(L"Return the discount factor D(u) = exp(-integral_0^u f) for each time u.")
);
_FP12* WINAPI xll_curve_discount(HANDLEX h, _FP12* pu)
{
#pragma XLLEXPORT
    try {
        handle<curve::base<>> h_(h);
        ensure(h_);

        for (int i = 0; i < size(*pu); ++i) {
            pu->array[i] = h_->discount(pu->array[i]);
        }
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
        return nullptr;
    }

    return pu;
}

// ── CURVE.SPOT ─────────────────────────────────────────────────────

static AddIn xai_curve_spot(
    Function(XLL_FP, L"xll_curve_spot", CATEGORY L".CURVE.SPOT")
    .Arguments({
        Arg(XLL_HANDLEX, L"h", L"is a handle to a curve."),
        Arg(XLL_FP,      L"u", L"is an array of times."),
        })
        .Category(CATEGORY)
    .FunctionHelp(L"Return the spot rate r(u) = (1/u) integral_0^u f for each time u.")
);
_FP12* WINAPI xll_curve_spot(HANDLEX h, _FP12* pu)
{
#pragma XLLEXPORT
    try {
        handle<curve::base<>> h_(h);
        ensure(h_);

        for (int i = 0; i < size(*pu); ++i) {
            pu->array[i] = h_->spot(pu->array[i]);
        }
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
        return nullptr;
    }

    return pu;
}
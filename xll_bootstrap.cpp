// xll_bootstrap.cpp - bootstrap functions
// TODO: Implement \FI.CURVE.PWFLAT.BOOTSTRAP(instruments, prices) 
#include "xll_fi.h"
#include "fms_bootstrap.h"
#include "fms_instrument.h"
#include "fms_curve_pwflat.h"

using namespace fms;
using namespace xll;

/* TODO: Hints
handle<curve::pwflat> WINAPI xll_fi_curve_bootstrap(_FP12* pi, _FP12* pp) { ...
	ensure(size(*pi) == size(*pp));
	<something> = curve::bootstrap(std::span<handle<instrument::base<>>(pi->array, n), span(*pp));
}
*/

static AddIn xai_fi_curve_bootstrap(
    Function(XLL_HANDLEX, L"xll_fi_curve_bootstrap", L"\\" CATEGORY L".CURVE.PWFLAT.BOOTSTRAP")
    .Arguments({
        Arg(XLL_FP, L"instruments", L"is a 1-D array of instrument handles."),
        Arg(XLL_FP, L"prices",      L"is a 1-D array of market prices."),
        })
        .Uncalced()
    .Category(CATEGORY)
    .FunctionHelp(L"Bootstrap a piecewise-flat forward curve from instruments and prices.")
);
HANDLEX WINAPI xll_fi_curve_bootstrap(_FP12* pi, _FP12* pp)
{
#pragma XLLEXPORT
    HANDLEX h = INVALID_HANDLEX;

    try {
        ensure(size(*pi) == size(*pp));
        int n = size(*pi);

        auto* f = new curve::pwflat<>();
        double _t = 0, _f = 0.03;

        for (int i = 0; i < n; ++i) {
            handle<instrument::base<>> instr(
                static_cast<HANDLEX>(pi->array[i])
            );
            ensure(instr);

            auto [t, fwd] = curve::bootstrap0(*instr, *f, _t, _f, pp->array[i]);
            ensure(!std::isnan(t) && !std::isnan(fwd));

            f->push_back(t, fwd);
            _t = t;
            _f = fwd; 
        }

        handle<curve::base<>> h_(f);
        ensure(h_);
        h = h_.get();
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
    }

    return h;
}
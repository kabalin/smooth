// This file was generated by Rcpp::compileAttributes
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <RcppArmadillo.h>
#include <Rcpp.h>

using namespace Rcpp;

// cesfitterwrap
RcppExport List cesfitterwrap(SEXP matxt, SEXP matF, SEXP matw, SEXP yt, SEXP vecg, SEXP Stype, SEXP seasfreq, SEXP matwex, SEXP matxtreg);
RcppExport SEXP smooth_cesfitterwrap(SEXP matxtSEXP, SEXP matFSEXP, SEXP matwSEXP, SEXP ytSEXP, SEXP vecgSEXP, SEXP StypeSEXP, SEXP seasfreqSEXP, SEXP matwexSEXP, SEXP matxtregSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type matxt(matxtSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matF(matFSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matw(matwSEXP);
    Rcpp::traits::input_parameter< SEXP >::type yt(ytSEXP);
    Rcpp::traits::input_parameter< SEXP >::type vecg(vecgSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Stype(StypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type seasfreq(seasfreqSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matwex(matwexSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matxtreg(matxtregSEXP);
    __result = Rcpp::wrap(cesfitterwrap(matxt, matF, matw, yt, vecg, Stype, seasfreq, matwex, matxtreg));
    return __result;
END_RCPP
}
// cesforecasterwrap
RcppExport arma::vec cesforecasterwrap(SEXP matxt, SEXP matF, SEXP matw, SEXP h, SEXP Stype, SEXP seasfreq, SEXP matwex, SEXP matxtreg);
RcppExport SEXP smooth_cesforecasterwrap(SEXP matxtSEXP, SEXP matFSEXP, SEXP matwSEXP, SEXP hSEXP, SEXP StypeSEXP, SEXP seasfreqSEXP, SEXP matwexSEXP, SEXP matxtregSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type matxt(matxtSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matF(matFSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matw(matwSEXP);
    Rcpp::traits::input_parameter< SEXP >::type h(hSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Stype(StypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type seasfreq(seasfreqSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matwex(matwexSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matxtreg(matxtregSEXP);
    __result = Rcpp::wrap(cesforecasterwrap(matxt, matF, matw, h, Stype, seasfreq, matwex, matxtreg));
    return __result;
END_RCPP
}
// cesforecastervar
RcppExport arma::vec cesforecastervar(SEXP matF, SEXP matw, SEXP vecg, SEXP h, SEXP errvar, SEXP Stype, SEXP seasfreq);
RcppExport SEXP smooth_cesforecastervar(SEXP matFSEXP, SEXP matwSEXP, SEXP vecgSEXP, SEXP hSEXP, SEXP errvarSEXP, SEXP StypeSEXP, SEXP seasfreqSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type matF(matFSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matw(matwSEXP);
    Rcpp::traits::input_parameter< SEXP >::type vecg(vecgSEXP);
    Rcpp::traits::input_parameter< SEXP >::type h(hSEXP);
    Rcpp::traits::input_parameter< SEXP >::type errvar(errvarSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Stype(StypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type seasfreq(seasfreqSEXP);
    __result = Rcpp::wrap(cesforecastervar(matF, matw, vecg, h, errvar, Stype, seasfreq));
    return __result;
END_RCPP
}
// ceserrorerwrap
RcppExport arma::mat ceserrorerwrap(SEXP matxt, SEXP matF, SEXP matw, SEXP yt, SEXP h, SEXP Stype, SEXP seasfreq, SEXP matwex, SEXP matxtreg);
RcppExport SEXP smooth_ceserrorerwrap(SEXP matxtSEXP, SEXP matFSEXP, SEXP matwSEXP, SEXP ytSEXP, SEXP hSEXP, SEXP StypeSEXP, SEXP seasfreqSEXP, SEXP matwexSEXP, SEXP matxtregSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type matxt(matxtSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matF(matFSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matw(matwSEXP);
    Rcpp::traits::input_parameter< SEXP >::type yt(ytSEXP);
    Rcpp::traits::input_parameter< SEXP >::type h(hSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Stype(StypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type seasfreq(seasfreqSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matwex(matwexSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matxtreg(matxtregSEXP);
    __result = Rcpp::wrap(ceserrorerwrap(matxt, matF, matw, yt, h, Stype, seasfreq, matwex, matxtreg));
    return __result;
END_RCPP
}
// cesoptimizerwrap
RcppExport double cesoptimizerwrap(SEXP matxt, SEXP matF, SEXP matw, SEXP yt, SEXP vecg, SEXP h, SEXP Stype, SEXP seasfreq, SEXP trace, SEXP CFt, SEXP normalizer, SEXP matwex, SEXP matxtreg);
RcppExport SEXP smooth_cesoptimizerwrap(SEXP matxtSEXP, SEXP matFSEXP, SEXP matwSEXP, SEXP ytSEXP, SEXP vecgSEXP, SEXP hSEXP, SEXP StypeSEXP, SEXP seasfreqSEXP, SEXP traceSEXP, SEXP CFtSEXP, SEXP normalizerSEXP, SEXP matwexSEXP, SEXP matxtregSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type matxt(matxtSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matF(matFSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matw(matwSEXP);
    Rcpp::traits::input_parameter< SEXP >::type yt(ytSEXP);
    Rcpp::traits::input_parameter< SEXP >::type vecg(vecgSEXP);
    Rcpp::traits::input_parameter< SEXP >::type h(hSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Stype(StypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type seasfreq(seasfreqSEXP);
    Rcpp::traits::input_parameter< SEXP >::type trace(traceSEXP);
    Rcpp::traits::input_parameter< SEXP >::type CFt(CFtSEXP);
    Rcpp::traits::input_parameter< SEXP >::type normalizer(normalizerSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matwex(matwexSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matxtreg(matxtregSEXP);
    __result = Rcpp::wrap(cesoptimizerwrap(matxt, matF, matw, yt, vecg, h, Stype, seasfreq, trace, CFt, normalizer, matwex, matxtreg));
    return __result;
END_RCPP
}
// initparams
RcppExport SEXP initparams(SEXP Ttype, SEXP Stype, SEXP datafreq, SEXP obsR, SEXP yt, SEXP damped, SEXP phi, SEXP smoothingparameters, SEXP initialstates, SEXP seasonalcoefs);
RcppExport SEXP smooth_initparams(SEXP TtypeSEXP, SEXP StypeSEXP, SEXP datafreqSEXP, SEXP obsRSEXP, SEXP ytSEXP, SEXP dampedSEXP, SEXP phiSEXP, SEXP smoothingparametersSEXP, SEXP initialstatesSEXP, SEXP seasonalcoefsSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type Ttype(TtypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Stype(StypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type datafreq(datafreqSEXP);
    Rcpp::traits::input_parameter< SEXP >::type obsR(obsRSEXP);
    Rcpp::traits::input_parameter< SEXP >::type yt(ytSEXP);
    Rcpp::traits::input_parameter< SEXP >::type damped(dampedSEXP);
    Rcpp::traits::input_parameter< SEXP >::type phi(phiSEXP);
    Rcpp::traits::input_parameter< SEXP >::type smoothingparameters(smoothingparametersSEXP);
    Rcpp::traits::input_parameter< SEXP >::type initialstates(initialstatesSEXP);
    Rcpp::traits::input_parameter< SEXP >::type seasonalcoefs(seasonalcoefsSEXP);
    __result = Rcpp::wrap(initparams(Ttype, Stype, datafreq, obsR, yt, damped, phi, smoothingparameters, initialstates, seasonalcoefs));
    return __result;
END_RCPP
}
// etsmatrices
RcppExport SEXP etsmatrices(SEXP matxt, SEXP vecg, SEXP phi, SEXP Cvalues, SEXP ncomponentsR, SEXP seasfreq, SEXP Ttype, SEXP Stype, SEXP nexovars, SEXP matxtreg, SEXP estimpersistence, SEXP estimphi, SEXP estiminit, SEXP estiminitseason, SEXP estimxreg);
RcppExport SEXP smooth_etsmatrices(SEXP matxtSEXP, SEXP vecgSEXP, SEXP phiSEXP, SEXP CvaluesSEXP, SEXP ncomponentsRSEXP, SEXP seasfreqSEXP, SEXP TtypeSEXP, SEXP StypeSEXP, SEXP nexovarsSEXP, SEXP matxtregSEXP, SEXP estimpersistenceSEXP, SEXP estimphiSEXP, SEXP estiminitSEXP, SEXP estiminitseasonSEXP, SEXP estimxregSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type matxt(matxtSEXP);
    Rcpp::traits::input_parameter< SEXP >::type vecg(vecgSEXP);
    Rcpp::traits::input_parameter< SEXP >::type phi(phiSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Cvalues(CvaluesSEXP);
    Rcpp::traits::input_parameter< SEXP >::type ncomponentsR(ncomponentsRSEXP);
    Rcpp::traits::input_parameter< SEXP >::type seasfreq(seasfreqSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Ttype(TtypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Stype(StypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type nexovars(nexovarsSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matxtreg(matxtregSEXP);
    Rcpp::traits::input_parameter< SEXP >::type estimpersistence(estimpersistenceSEXP);
    Rcpp::traits::input_parameter< SEXP >::type estimphi(estimphiSEXP);
    Rcpp::traits::input_parameter< SEXP >::type estiminit(estiminitSEXP);
    Rcpp::traits::input_parameter< SEXP >::type estiminitseason(estiminitseasonSEXP);
    Rcpp::traits::input_parameter< SEXP >::type estimxreg(estimxregSEXP);
    __result = Rcpp::wrap(etsmatrices(matxt, vecg, phi, Cvalues, ncomponentsR, seasfreq, Ttype, Stype, nexovars, matxtreg, estimpersistence, estimphi, estiminit, estiminitseason, estimxreg));
    return __result;
END_RCPP
}
// fitterwrap
RcppExport SEXP fitterwrap(SEXP matxt, SEXP matF, SEXP matw, SEXP yt, SEXP vecg, SEXP Etype, SEXP Ttype, SEXP Stype, SEXP seasfreq, SEXP matwex, SEXP matxtreg);
RcppExport SEXP smooth_fitterwrap(SEXP matxtSEXP, SEXP matFSEXP, SEXP matwSEXP, SEXP ytSEXP, SEXP vecgSEXP, SEXP EtypeSEXP, SEXP TtypeSEXP, SEXP StypeSEXP, SEXP seasfreqSEXP, SEXP matwexSEXP, SEXP matxtregSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type matxt(matxtSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matF(matFSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matw(matwSEXP);
    Rcpp::traits::input_parameter< SEXP >::type yt(ytSEXP);
    Rcpp::traits::input_parameter< SEXP >::type vecg(vecgSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Etype(EtypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Ttype(TtypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Stype(StypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type seasfreq(seasfreqSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matwex(matwexSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matxtreg(matxtregSEXP);
    __result = Rcpp::wrap(fitterwrap(matxt, matF, matw, yt, vecg, Etype, Ttype, Stype, seasfreq, matwex, matxtreg));
    return __result;
END_RCPP
}
// forecasterwrap
RcppExport SEXP forecasterwrap(SEXP matxt, SEXP matF, SEXP matw, SEXP h, SEXP Ttype, SEXP Stype, SEXP seasfreq, SEXP matwex, SEXP matxtreg);
RcppExport SEXP smooth_forecasterwrap(SEXP matxtSEXP, SEXP matFSEXP, SEXP matwSEXP, SEXP hSEXP, SEXP TtypeSEXP, SEXP StypeSEXP, SEXP seasfreqSEXP, SEXP matwexSEXP, SEXP matxtregSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type matxt(matxtSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matF(matFSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matw(matwSEXP);
    Rcpp::traits::input_parameter< SEXP >::type h(hSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Ttype(TtypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Stype(StypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type seasfreq(seasfreqSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matwex(matwexSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matxtreg(matxtregSEXP);
    __result = Rcpp::wrap(forecasterwrap(matxt, matF, matw, h, Ttype, Stype, seasfreq, matwex, matxtreg));
    return __result;
END_RCPP
}
// errorerwrap
RcppExport SEXP errorerwrap(SEXP matxt, SEXP matF, SEXP matw, SEXP yt, SEXP h, SEXP Etype, SEXP Ttype, SEXP Stype, SEXP seasfreq, SEXP trace, SEXP matwex, SEXP matxtreg);
RcppExport SEXP smooth_errorerwrap(SEXP matxtSEXP, SEXP matFSEXP, SEXP matwSEXP, SEXP ytSEXP, SEXP hSEXP, SEXP EtypeSEXP, SEXP TtypeSEXP, SEXP StypeSEXP, SEXP seasfreqSEXP, SEXP traceSEXP, SEXP matwexSEXP, SEXP matxtregSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type matxt(matxtSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matF(matFSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matw(matwSEXP);
    Rcpp::traits::input_parameter< SEXP >::type yt(ytSEXP);
    Rcpp::traits::input_parameter< SEXP >::type h(hSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Etype(EtypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Ttype(TtypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Stype(StypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type seasfreq(seasfreqSEXP);
    Rcpp::traits::input_parameter< SEXP >::type trace(traceSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matwex(matwexSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matxtreg(matxtregSEXP);
    __result = Rcpp::wrap(errorerwrap(matxt, matF, matw, yt, h, Etype, Ttype, Stype, seasfreq, trace, matwex, matxtreg));
    return __result;
END_RCPP
}
// optimizerwrap
RcppExport SEXP optimizerwrap(SEXP matxt, SEXP matF, SEXP matw, SEXP yt, SEXP vecg, SEXP h, SEXP Etype, SEXP Ttype, SEXP Stype, SEXP seasfreq, SEXP trace, SEXP CFt, SEXP normalizer, SEXP matwex, SEXP matxtreg);
RcppExport SEXP smooth_optimizerwrap(SEXP matxtSEXP, SEXP matFSEXP, SEXP matwSEXP, SEXP ytSEXP, SEXP vecgSEXP, SEXP hSEXP, SEXP EtypeSEXP, SEXP TtypeSEXP, SEXP StypeSEXP, SEXP seasfreqSEXP, SEXP traceSEXP, SEXP CFtSEXP, SEXP normalizerSEXP, SEXP matwexSEXP, SEXP matxtregSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type matxt(matxtSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matF(matFSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matw(matwSEXP);
    Rcpp::traits::input_parameter< SEXP >::type yt(ytSEXP);
    Rcpp::traits::input_parameter< SEXP >::type vecg(vecgSEXP);
    Rcpp::traits::input_parameter< SEXP >::type h(hSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Etype(EtypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Ttype(TtypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Stype(StypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type seasfreq(seasfreqSEXP);
    Rcpp::traits::input_parameter< SEXP >::type trace(traceSEXP);
    Rcpp::traits::input_parameter< SEXP >::type CFt(CFtSEXP);
    Rcpp::traits::input_parameter< SEXP >::type normalizer(normalizerSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matwex(matwexSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matxtreg(matxtregSEXP);
    __result = Rcpp::wrap(optimizerwrap(matxt, matF, matw, yt, vecg, h, Etype, Ttype, Stype, seasfreq, trace, CFt, normalizer, matwex, matxtreg));
    return __result;
END_RCPP
}
// costfunc
RcppExport SEXP costfunc(SEXP matxt, SEXP matF, SEXP matw, SEXP yt, SEXP vecg, SEXP h, SEXP Etype, SEXP Ttype, SEXP Stype, SEXP seasfreq, SEXP trace, SEXP CFt, SEXP normalizer, SEXP matwex, SEXP matxtreg, SEXP bounds, SEXP phi, SEXP Theta);
RcppExport SEXP smooth_costfunc(SEXP matxtSEXP, SEXP matFSEXP, SEXP matwSEXP, SEXP ytSEXP, SEXP vecgSEXP, SEXP hSEXP, SEXP EtypeSEXP, SEXP TtypeSEXP, SEXP StypeSEXP, SEXP seasfreqSEXP, SEXP traceSEXP, SEXP CFtSEXP, SEXP normalizerSEXP, SEXP matwexSEXP, SEXP matxtregSEXP, SEXP boundsSEXP, SEXP phiSEXP, SEXP ThetaSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type matxt(matxtSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matF(matFSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matw(matwSEXP);
    Rcpp::traits::input_parameter< SEXP >::type yt(ytSEXP);
    Rcpp::traits::input_parameter< SEXP >::type vecg(vecgSEXP);
    Rcpp::traits::input_parameter< SEXP >::type h(hSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Etype(EtypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Ttype(TtypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Stype(StypeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type seasfreq(seasfreqSEXP);
    Rcpp::traits::input_parameter< SEXP >::type trace(traceSEXP);
    Rcpp::traits::input_parameter< SEXP >::type CFt(CFtSEXP);
    Rcpp::traits::input_parameter< SEXP >::type normalizer(normalizerSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matwex(matwexSEXP);
    Rcpp::traits::input_parameter< SEXP >::type matxtreg(matxtregSEXP);
    Rcpp::traits::input_parameter< SEXP >::type bounds(boundsSEXP);
    Rcpp::traits::input_parameter< SEXP >::type phi(phiSEXP);
    Rcpp::traits::input_parameter< SEXP >::type Theta(ThetaSEXP);
    __result = Rcpp::wrap(costfunc(matxt, matF, matw, yt, vecg, h, Etype, Ttype, Stype, seasfreq, trace, CFt, normalizer, matwex, matxtreg, bounds, phi, Theta));
    return __result;
END_RCPP
}

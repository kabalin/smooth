#include <RcppArmadillo.h>
#include <iostream>
#include <cmath>
// [[Rcpp::depends(RcppArmadillo)]]

using namespace Rcpp;

/* # Function is needed to estimate the correct error for ETS when multisteps model selection with r(matvt) is sorted out. */
arma::mat matrixPower(arma::mat A, int power){
    arma::mat B(A.n_rows, A.n_rows, arma::fill::eye);

    if(power!=0){
        for(int i=0; i<power; i++){
            B = B * A;
        }
    }
    return B;
}

/* # Function allows to multiply polinomails */
arma::vec polyMult(arma::vec poly1, arma::vec poly2){

    int poly1Nonzero = arma::as_scalar(find(poly1,1,"last"));
    int poly2Nonzero = arma::as_scalar(find(poly2,1,"last"));

    arma::vec poly3(poly1Nonzero + poly2Nonzero + 1, arma::fill::zeros);

    for(int i = 0; i <= poly1Nonzero; i++){
        for(int j = 0; j <= poly2Nonzero; j++){
            poly3(i+j) += poly1(i) * poly2(j);
        }
    }

    return poly3;
}

/* # Function returns multiplicative or additive error for scalar */
double errorf(double yact, double yfit, char Etype){
    if(Etype=='A'){
        return yact - yfit;
    }
    else{
        if((yact==0) & (yfit==0)){
            return 0;
        }
        else if((yact!=0) & (yfit==0)){
            return R_PosInf;
        }
        else{
            return (yact - yfit) / yfit;
        }
    }
}

/* # Function is needed to estimate the correct error for ETS when multisteps model selection with r(matvt) is sorted out. */
arma::mat errorvf(arma::mat yact, arma::mat yfit, char Etype){
    if(Etype=='A'){
        return yact - yfit;
    }
    else{
        yfit.elem(find(yfit==0)).fill(1e-100);
        return (yact - yfit) / yfit;
    }
}

/* # Function returns value of w() -- y-fitted -- used in the measurement equation */
double wvalue(arma::vec matrixVt, arma::rowvec rowvecW, char T, char S){
// matrixVt is a vector here!
    double yfit = 0;

    switch(S){
// ZZN
    case 'N':
        switch(T){
        case 'N':
        case 'A':
            yfit = as_scalar(rowvecW * matrixVt);
        break;
        case 'M':
            yfit = as_scalar(exp(rowvecW * log(matrixVt)));
        break;
        }
    break;
// ZZA
    case 'A':
        switch(T){
        case 'N':
        case 'A':
            yfit = as_scalar(rowvecW * matrixVt);
        break;
        case 'M':
            yfit = as_scalar(exp(rowvecW.cols(0,1) * log(matrixVt.rows(0,1)))) + matrixVt(2);
        break;
        }
    break;
// ZZM
    case 'M':
        switch(T){
        case 'N':
        case 'M':
            yfit = as_scalar(exp(rowvecW * log(matrixVt)));
        break;
        case 'A':
            yfit = as_scalar(rowvecW.cols(0,1) * matrixVt.rows(0,1)) * matrixVt(2);
        break;
        }
    break;
    }

    return yfit;
}

/* # Function returns value of r() -- additive or multiplicative error -- used in the error term of measurement equation.
     This is mainly needed by sim.ets */
double rvalue(arma::vec matrixVt, arma::rowvec rowvecW, char E, char T, char S){

    switch(E){
// MZZ
    case 'M':
        return wvalue(matrixVt, rowvecW, T, S);
    break;
// AZZ
    case 'A':
    default:
        return 1.0;
    }
}

/* # Function returns value of f() -- new states without the update -- used in the transition equation */
arma::vec fvalue(arma::vec matrixVt, arma::mat matrixF, char T, char S){
    arma::vec matrixVtnew = matrixVt;

    switch(S){
// ZZN
    case 'N':
        switch(T){
        case 'N':
        case 'A':
            matrixVtnew = matrixF * matrixVt;
        break;
        case 'M':
            matrixVtnew = exp(matrixF * log(matrixVt));
        break;
        }
    break;
// ZZA
    case 'A':
        switch(T){
        case 'N':
        case 'A':
            matrixVtnew = matrixF * matrixVt;
        break;
        case 'M':
            matrixVtnew.rows(0,1) = exp(matrixF.submat(0,0,1,1) * log(matrixVt.rows(0,1)));
            matrixVtnew(2) = matrixVt(2);
        break;
        }
    break;
// ZZM
    case 'M':
        switch(T){
        case 'N':
        case 'M':
            matrixVtnew = exp(matrixF * log(matrixVt));
        break;
        case 'A':
            matrixVtnew = matrixF * matrixVt;
        break;
        }
    break;
    }

    return matrixVtnew;
}

/* # Function returns value of g() -- the update of states -- used in components estimation for the persistence */
arma::vec gvalue(arma::vec matrixVt, arma::mat matrixF, arma::mat rowvecW, char E, char T, char S){
    arma::vec g(matrixVt.n_rows, arma::fill::ones);

// AZZ
    switch(E){
    case 'A':
// ANZ
        switch(T){
        case 'N':
            switch(S){
            case 'M':
                g(0) = 1 / matrixVt(1);
                g(1) = 1 / matrixVt(0);
            break;
            }
        break;
// AAZ
        case 'A':
            switch(S){
            case 'M':
                g.rows(0,1) = g.rows(0,1) / matrixVt(2);
                g(2) = 1 / as_scalar(rowvecW.cols(0,1) * matrixVt.rows(0,1));
            break;
            }
        break;
// AMZ
        case 'M':
            switch(S){
            case 'N':
            case 'A':
                g(1) = g(1) / matrixVt(0);
            break;
            case 'M':
                g(0) = g(0) / matrixVt(2);
                g(1) = g(1) / (matrixVt(0) * matrixVt(2));
                g(2) = g(2) / as_scalar(exp(rowvecW.cols(0,1) * log(matrixVt.rows(0,1))));
            break;
            }
        break;
        }
    break;
// MZZ
    case 'M':
// MNZ
        switch(T){
        case 'N':
            switch(S){
            case 'N':
                g(0) = matrixVt(0);
            break;
            case 'A':
                g.rows(0,1).fill(matrixVt(0) + matrixVt(1));
            break;
            case 'M':
                g = matrixVt;
            break;
            }
        break;
// MAZ
        case 'A':
            switch(S){
            case 'N':
            case 'A':
                g.fill(as_scalar(rowvecW * matrixVt));
            break;
            case 'M':
                g.rows(0,1).fill(as_scalar(rowvecW.cols(0,1) * matrixVt.rows(0,1)));
                g(2) = matrixVt(2);
            break;
            }
        break;
// MMZ
        case 'M':
            switch(S){
            case 'N':
                g(0) = as_scalar(exp(rowvecW * log(matrixVt)));
                g(1) = pow(matrixVt(1),rowvecW(1));
            break;
            case 'A':
                g.rows(0,2).fill(as_scalar(exp(rowvecW.cols(0,1) * log(matrixVt.rows(0,1))) + matrixVt(2)));
                g(1) = g(0) / matrixVt(0);
            break;
            case 'M':
                g = exp(matrixF * log(matrixVt));
            break;
            }
        break;
        }
    break;
    }

    return g;
}

/* # Function is needed for the renormalisation of seasonal components. It should be done seasonal-wise.*/
arma::mat normaliser(arma::mat Vt, int obsall, unsigned int maxlag, char S, char T){

    unsigned int ncomponents = Vt.n_cols;
    arma::vec meanseason(maxlag, arma::fill::zeros);

    switch(S){
    case 'A':
        meanseason.fill(arma::as_scalar(mean(Vt.col(ncomponents-1))));
        Vt.col(ncomponents-1) = Vt.col(ncomponents-1) - meanseason;
        switch(T){
        case 'N':
        case 'A':
            Vt.col(0) = Vt.col(0) + meanseason;
        break;
        case 'M':
            Vt.col(0) = Vt.col(0) + meanseason / Vt.col(1);
        break;
        }
    break;
    case 'M':
        meanseason.fill(arma::as_scalar(exp(mean(log(Vt.col(ncomponents-1))))));
        Vt.col(ncomponents-1) = Vt.col(ncomponents-1) / meanseason;
        switch(T){
        case 'N':
        case 'M':
            Vt.col(0) = Vt.col(0) / meanseason;
        break;
        case 'A':
            Vt.col(0) = Vt.col(0) % meanseason;
            Vt.col(1) = Vt.col(1) % meanseason;
        break;
        }
    break;
    }

    return(Vt);
}

/* # initparams - function that initialises the basic parameters of ETS */
// [[Rcpp::export]]
RcppExport SEXP initparams(SEXP Ttype, SEXP Stype, SEXP datafreq, SEXP obsR, SEXP obsallR, SEXP yt,
                           SEXP damped, SEXP phi, SEXP smoothingparameters, SEXP initialstates, SEXP seasonalcoefs){

    char T = as<char>(Ttype);
    char S = as<char>(Stype);
    int freq = as<int>(datafreq);
    int obs = as<int>(obsR);
    int obsall = as<int>(obsallR);
    NumericMatrix yt_n(yt);
    arma::mat vecYt(yt_n.begin(), yt_n.nrow(), yt_n.ncol(), false);
    bool damping = as<bool>(damped);
    double phivalue;
    if(!Rf_isNull(phi)){
        phivalue = as<double>(phi);
    }

    NumericMatrix smoothingparam(smoothingparameters);
    arma::mat persistence(smoothingparam.begin(), smoothingparam.nrow(), smoothingparam.ncol(), false);
    NumericMatrix initials(initialstates);
    arma::mat initial(initials.begin(), initials.nrow(), initials.ncol(), false);
    NumericMatrix seasonalc(seasonalcoefs);
    arma::mat seascoef(seasonalc.begin(), seasonalc.nrow(), seasonalc.ncol(), false);

    unsigned int ncomponents = 1;
    int maxlag = 1;
    arma::vec modellags(3, arma::fill::ones);

/* # Define the number of components */
    if(T!='N'){
        ncomponents += 1;
        if(S!='N'){
            ncomponents += 1;
            maxlag = freq;
            modellags(2) = freq;
        }
        else{
            modellags.resize(2);
        }
    }
    else{
/* # Define the number of components and model frequency */
        if(S!='N'){
            ncomponents += 1;
            maxlag = freq;
            modellags(1) = freq;
            modellags.resize(2);
        }
        else{
            modellags.resize(1);
        }
    }

    arma::mat matrixVt(std::max(obs + 2*maxlag, obsall + maxlag), ncomponents, arma::fill::ones);
    arma::vec vecG(ncomponents, arma::fill::zeros);
    bool estimphi = TRUE;

// # Define the initial states for level and trend components
    switch(T){
    case 'N':
        matrixVt.submat(0,0,maxlag-1,0).each_row() = initial.submat(0,2,0,2);
    break;
    case 'A':
        matrixVt.submat(0,0,maxlag-1,1).each_row() = initial.submat(0,0,0,1);
    break;
// # The initial matrix is filled with ones, that is why we don't need to fill in initial trend
    case 'M':
        matrixVt.submat(0,0,maxlag-1,1).each_row() = initial.submat(0,2,0,3);
    break;
    }

/* # Define the initial states for seasonal component */
    switch(S){
    case 'A':
        matrixVt.submat(0,ncomponents-1,maxlag-1,ncomponents-1) = seascoef.col(0);
    break;
    case 'M':
        matrixVt.submat(0,ncomponents-1,maxlag-1,ncomponents-1) = seascoef.col(1);
    break;
    }

//    matrixVt.resize(obs+maxlag, ncomponents);

    if(persistence.n_rows < ncomponents){
        if((T=='M') | (S=='M')){
            vecG = persistence.submat(0,1,persistence.n_rows-1,1);
        }
        else{
            vecG = persistence.submat(0,0,persistence.n_rows-1,0);
        }
    }
    else{
        if((T=='M') | (S=='M')){
            vecG = persistence.submat(0,1,ncomponents-1,1);
        }
        else{
            vecG = persistence.submat(0,0,ncomponents-1,0);
        }
    }

    if(Rf_isNull(phi)){
        if(damping==TRUE){
            phivalue = 0.95;
        }
        else{
            phivalue = 1.0;
            estimphi = FALSE;
        }
    }
    else{
        if(damping==FALSE){
            phivalue = 1.0;
        }
        estimphi = FALSE;
    }

    return wrap(List::create(Named("n.components") = ncomponents, Named("maxlag") = maxlag, Named("modellags") = modellags,
                             Named("matvt") = matrixVt, Named("vecg") = vecG, Named("phiEstimate") = estimphi,
                             Named("phi") = phivalue));
}

/*
# etsmatrices - function that returns matF and matw.
# Needs to be stand alone to change the damping parameter during the estimation.
# Cvalues includes persistence, phi, initials, intials for seasons, matrixAt coeffs.
*/
// [[Rcpp::export]]
RcppExport SEXP etsmatrices(SEXP matvt, SEXP vecg, SEXP phi, SEXP Cvalues, SEXP ncomponentsR,
                            SEXP modellags, SEXP fittertype, SEXP Ttype, SEXP Stype, SEXP nexovars, SEXP matat,
                            SEXP estimpersistence, SEXP estimphi, SEXP estiminit, SEXP estiminitseason, SEXP estimxreg,
                            SEXP matFX, SEXP vecgX, SEXP gowild, SEXP estimFX, SEXP estimgX, SEXP estiminitX){

    NumericMatrix matvt_n(matvt);
    arma::mat matrixVt(matvt_n.begin(), matvt_n.nrow(), matvt_n.ncol());

    NumericMatrix vecg_n(vecg);
    arma::vec vecG(vecg_n.begin(), vecg_n.nrow(), false);

    double phivalue = as<double>(phi);

    NumericMatrix Cv(Cvalues);
    arma::rowvec C(Cv.begin(), Cv.ncol(), false);

    int ncomponents = as<int>(ncomponentsR);

    IntegerVector modellags_n(modellags);
    arma::uvec lags = as<arma::uvec>(modellags_n);
    int maxlag = max(lags);

    char fitterType = as<char>(fittertype);

    char T = as<char>(Ttype);
    char S = as<char>(Stype);

    int nexo = as<int>(nexovars);

    NumericMatrix matat_n(matat);
    arma::mat matrixAt(matat_n.begin(), matat_n.nrow(), matat_n.ncol());

    bool estimatepersistence = as<bool>(estimpersistence);
    bool estimatephi = as<bool>(estimphi);
    bool estimateinitial = as<bool>(estiminit);
    bool estimateinitialseason = as<bool>(estiminitseason);
    bool estimatexreg = as<bool>(estimxreg);

    NumericMatrix matFX_n(matFX);
    arma::mat matrixFX(matFX_n.begin(), matFX_n.nrow(), matFX_n.ncol());

    NumericMatrix vecgX_n(vecgX);
    arma::vec vecGX(vecgX_n.begin(), vecgX_n.nrow());

    bool wild = as<bool>(gowild);
    bool estimateFX = as<bool>(estimFX);
    bool estimategX = as<bool>(estimgX);
    bool estimateinitialX = as<bool>(estiminitX);

    arma::mat matrixF(1,1,arma::fill::ones);
    arma::mat rowvecW(1,1,arma::fill::ones);

    int currentelement = 0;

    if(estimatepersistence==TRUE){
        vecG = C.cols(currentelement,currentelement + ncomponents-1).t();
        currentelement = currentelement + ncomponents;
    }

    if(estimatephi==TRUE){
        phivalue = as_scalar(C.col(currentelement));
        currentelement = currentelement + 1;
    }

    if((fitterType=='o') | (fitterType=='p')){
        if(estimateinitial==TRUE){
            matrixVt.col(0).fill(as_scalar(C.col(currentelement).t()));
            currentelement = currentelement + 1;
            if(T!='N'){
                matrixVt.col(1).fill(as_scalar(C.col(currentelement).t()));
                currentelement = currentelement + 1;
            }
        }

        if(S!='N'){
            if(estimateinitialseason==TRUE){
                matrixVt.submat(0,ncomponents-1,maxlag-1,ncomponents-1) = C.cols(currentelement, currentelement + maxlag - 1).t();
                currentelement = currentelement + maxlag;
/* # Normalise the initial seasons */
                if(S=='A'){
                    matrixVt.submat(0,ncomponents-1,maxlag-1,ncomponents-1) = matrixVt.submat(0,ncomponents-1,maxlag-1,ncomponents-1) -
                                as_scalar(mean(matrixVt.submat(0,ncomponents-1,maxlag-1,ncomponents-1)));
                }
                else{
                    matrixVt.submat(0,ncomponents-1,maxlag-1,ncomponents-1) = exp(log(matrixVt.submat(0,ncomponents-1,maxlag-1,ncomponents-1)) -
                                as_scalar(mean(log(matrixVt.submat(0,ncomponents-1,maxlag-1,ncomponents-1)))));
                }
            }
        }
    }

    if(estimatexreg==TRUE){
        if(estimateinitialX==TRUE){
            matrixAt.each_row() = C.cols(currentelement,currentelement + nexo - 1);
            currentelement = currentelement + nexo;
        }

        if(wild==TRUE){
            if(estimateFX==TRUE){
                for(int i=0; i < nexo; i = i+1){
                    matrixFX.row(i) = C.cols(currentelement, currentelement + nexo - 1);
                    currentelement = currentelement + nexo;
                }
            }

            if(estimategX==TRUE){
                vecGX = C.cols(currentelement, currentelement + nexo - 1).t();
                currentelement = currentelement + nexo;
            }
        }
    }

/* # The default values of matrices are set for ZNN  models */
    switch(S){
    case 'N':
        switch(T){
        case 'A':
        case 'M':
            matrixF.set_size(2,2);
            matrixF(0,0) = 1.0;
            matrixF(1,0) = 0.0;
            matrixF(0,1) = phivalue;
            matrixF(1,1) = phivalue;

            rowvecW.set_size(1,2);
            rowvecW(0,0) = 1.0;
            rowvecW(0,1) = phivalue;
        break;
        }
    break;
    case 'A':
    case 'M':
        switch(T){
        case 'A':
        case 'M':
            matrixF.set_size(3,3);
            matrixF.fill(0.0);
            matrixF(0,0) = 1.0;
            matrixF(1,0) = 0.0;
            matrixF(0,1) = phivalue;
            matrixF(1,1) = phivalue;
            matrixF(2,2) = 1.0;

            rowvecW.set_size(1,3);
            rowvecW(0,0) = 1.0;
            rowvecW(0,1) = phivalue;
            rowvecW(0,2) = 1.0;
        break;
        case 'N':
            matrixF.set_size(2,2);
            matrixF.fill(0.0);
            matrixF.diag().fill(1.0);

            rowvecW.set_size(1,2);
            rowvecW.fill(1.0);
        break;
        }
    }

    return wrap(List::create(Named("matF") = matrixF, Named("matw") = rowvecW, Named("vecg") = vecG,
                             Named("phi") = phivalue, Named("matvt") = matrixVt, Named("matat") = matrixAt,
                             Named("matFX") = matrixFX, Named("vecgX") = vecGX));
}

/*
# polysos - function that transforms AR and MA parameters into polynomials
# and then in matF and other things.
# Cvalues includes AR, MA, initials, constant, matrixAt, transitionX and persistenceX.
*/
List polysos(arma::uvec arOrders, arma::uvec maOrders, arma::uvec iOrders, arma::uvec lags, int nComponents,
             arma::vec arValues, arma::vec maValues, double constValue, arma::vec C,
             arma::mat matrixVt, arma::vec vecG, arma::mat matrixF,
             char fitterType, int nexo, arma::mat matrixAt, arma::mat matrixFX, arma::vec vecGX,
             bool arEstimate, bool maEstimate, bool constRequired, bool constEstimate,
             bool xregEstimate, bool wild, bool fXEstimate, bool gXEstimate, bool initialXEstimate){

    bool initialEstimate = (fitterType=='o');

// Form matrices with parameters, that are then used for polynomial multiplication
    arma::mat arParameters(max(arOrders % lags)+1, arOrders.n_elem, arma::fill::zeros);
    arma::mat iParameters(max(iOrders % lags)+1, iOrders.n_elem, arma::fill::zeros);
    arma::mat maParameters(max(maOrders % lags)+1, maOrders.n_elem, arma::fill::zeros);

    arParameters.row(0).fill(1);
    iParameters.row(0).fill(1);
    maParameters.row(0).fill(1);

    int nParam = 0;
    int arnParam = 0;
    int manParam = 0;
    for(int i=0; i<lags.n_rows; i++){
        if(arOrders(i) * lags(i) != 0){
            for(int j=0; j<arOrders(i); j++){
                if(arEstimate){
                    arParameters((j+1)*lags(i),i) = -C(nParam);
                    nParam += 1;
                }
                else{
                    arParameters((j+1)*lags(i),i) = -arValues(arnParam);
                    arnParam += 1;
                }
            }
        }

        if(iOrders(i) * lags(i) != 0){
            iParameters(lags(i),i) = -1;
        }

        if(maOrders(i) * lags(i) != 0){
            for(int j=0; j<maOrders(i); j++){
                if(maEstimate){
                    maParameters((j+1)*lags(i),i) = C(nParam);
                    nParam += 1;
                }
                else{
                    maParameters((j+1)*lags(i),i) = maValues(manParam);
                    manParam += 1;
                }
            }
        }
    }

// Prepare vectors with coefficients for polynomials
    arma::vec arPolynomial(sum(arOrders % lags)+1, arma::fill::zeros);
    arma::vec iPolynomial(sum(iOrders % lags)+1, arma::fill::zeros);
    arma::vec maPolynomial(sum(maOrders % lags)+1, arma::fill::zeros);
    arma::vec ariPolynomial(sum(arOrders % lags)+sum(iOrders % lags)+1, arma::fill::zeros);

    arPolynomial.rows(0,arOrders(0)*lags(0)) = arParameters.submat(0,0,arOrders(0)*lags(0),0);
    iPolynomial.rows(0,iOrders(0)*lags(0)) = iParameters.submat(0,0,iOrders(0)*lags(0),0);
    maPolynomial.rows(0,maOrders(0)*lags(0)) = maParameters.submat(0,0,maOrders(0)*lags(0),0);

    for(int i=1; i<lags.n_rows; i++){
// Form polynomials
        arPolynomial = polyMult(arPolynomial, arParameters.col(i));
        maPolynomial = polyMult(maPolynomial, maParameters.col(i));
        if(iOrders(i)>1){
            for(int j=1; j<iOrders(i); j++){
                iParameters.col(i) = polyMult(iParameters.col(i), iParameters.col(i));
            }
        }
        iPolynomial = polyMult(iPolynomial, iParameters.col(i));
    }
    ariPolynomial = polyMult(arPolynomial, iPolynomial);

    if(maPolynomial.n_elem!=(nComponents+1)){
        maPolynomial.resize(nComponents+1);
    }
    if(ariPolynomial.n_elem!=(nComponents+1)){
        ariPolynomial.resize(nComponents+1);
    }

// Fill in transition matrix
    if(ariPolynomial.n_elem>1){
        matrixF.submat(0,0,ariPolynomial.n_elem-2,0) = -ariPolynomial.rows(1,ariPolynomial.n_elem-1);
    }

// Fill in persistence vector
    if(nComponents>0){
        vecG.rows(0,ariPolynomial.n_elem-2) = -ariPolynomial.rows(1,ariPolynomial.n_elem-1) + maPolynomial.rows(1,maPolynomial.n_elem-1);

// Fill in initials of state vector
        if(fitterType=='o'){
            matrixVt.submat(0,0,0,nComponents-1) = C.rows(nParam,nParam+nComponents-1).t();
            nParam += nComponents;
        }
    }

// Deal with constant if needed
    if(constRequired){
        if(constEstimate){
            matrixVt(0,matrixVt.n_cols-1) = C(nParam);
            nParam += 1;
        }
        else{
            matrixVt(0,matrixVt.n_cols-1) = constValue;
        }
    }

    if(xregEstimate){
        if(initialXEstimate){
            matrixAt.each_row() = C.rows(nParam,nParam + nexo - 1).t();
            nParam += nexo;
        }

        if(wild){
            if(fXEstimate){
                for(int i=0; i < nexo; i = i+1){
                    matrixFX.row(i) = C.rows(nParam, nParam + nexo - 1).t();
                    nParam += nexo;
                }
            }

            if(gXEstimate){
                vecGX = C.rows(nParam, nParam + nexo - 1);
                nParam += nexo;
            }
        }
    }

    return List::create(Named("matF") = matrixF, Named("vecg") = vecG,
                        Named("arPolynomial") = arPolynomial, Named("maPolynomial") = maPolynomial,
                        Named("matvt") = matrixVt, Named("matat") = matrixAt,
                        Named("matFX") = matrixFX, Named("vecgX") = vecGX);
}

// [[Rcpp::export]]
RcppExport SEXP polysoswrap(SEXP ARorders, SEXP MAorders, SEXP Iorders, SEXP ARIMAlags, SEXP nComp,
                            SEXP AR, SEXP MA, SEXP constant, SEXP Cvalues,
                            SEXP matvt, SEXP vecg, SEXP matF,
                            SEXP fittertype, SEXP nexovars, SEXP matat, SEXP matFX, SEXP vecgX,
                            SEXP estimAR, SEXP estimMA, SEXP requireConst, SEXP estimConst,
                            SEXP estimxreg, SEXP gowild, SEXP estimFX, SEXP estimgX, SEXP estiminitX){

    IntegerVector ARorders_n(ARorders);
    arma::uvec arOrders = as<arma::uvec>(ARorders_n);

    IntegerVector MAorders_n(MAorders);
    arma::uvec maOrders = as<arma::uvec>(MAorders_n);

    IntegerVector Iorders_n(Iorders);
    arma::uvec iOrders = as<arma::uvec>(Iorders_n);

    IntegerVector ARIMAlags_n(ARIMAlags);
    arma::uvec lags = as<arma::uvec>(ARIMAlags_n);

    int nComponents = as<int>(nComp);

    NumericVector AR_n;
    if(!Rf_isNull(AR)){
        AR_n = as<NumericVector>(AR);
    }
    arma::vec arValues(AR_n.begin(), AR_n.size(), false);

    NumericVector MA_n;
    if(!Rf_isNull(MA)){
        MA_n = as<NumericVector>(MA);
    }
    arma::vec maValues(MA_n.begin(), MA_n.size(), false);

    double constValue;
    if(!Rf_isNull(constant)){
        constValue = as<double>(constant);
    }

    NumericVector Cvalues_n;
    if(!Rf_isNull(Cvalues)){
        Cvalues_n = as<NumericVector>(Cvalues);
    }
    arma::vec C(Cvalues_n.begin(), Cvalues_n.size(), false);

    NumericMatrix matvt_n(matvt);
    arma::mat matrixVt(matvt_n.begin(), matvt_n.nrow(), matvt_n.ncol());

    NumericMatrix vecg_n(vecg);
    arma::vec vecG(vecg_n.begin(), vecg_n.nrow(), false);

    NumericMatrix matF_n(matF);
    arma::mat matrixF(matF_n.begin(), matF_n.nrow(), matF_n.ncol());

    char fitterType = as<char>(fittertype);

    int nexo = as<int>(nexovars);

    NumericMatrix matat_n(matat);
    arma::mat matrixAt(matat_n.begin(), matat_n.nrow(), matat_n.ncol());

    NumericMatrix matFX_n(matFX);
    arma::mat matrixFX(matFX_n.begin(), matFX_n.nrow(), matFX_n.ncol());

    NumericMatrix vecgX_n(vecgX);
    arma::vec vecGX(vecgX_n.begin(), vecgX_n.nrow());

    bool arEstimate = as<bool>(estimAR);
    bool maEstimate = as<bool>(estimMA);
    bool constRequired = as<bool>(requireConst);
    bool constEstimate = as<bool>(estimConst);
    bool initialEstimate = (fitterType=='o');
    bool xregEstimate = as<bool>(estimxreg);
    bool wild = as<bool>(gowild);
    bool fXEstimate = as<bool>(estimFX);
    bool gXEstimate = as<bool>(estimgX);
    bool initialXEstimate = as<bool>(estiminitX);


    return wrap(polysos(arOrders, maOrders, iOrders, lags, nComponents,
                        arValues, maValues, constValue, C,
                        matrixVt, vecG, matrixF,
                        fitterType, nexo, matrixAt, matrixFX, vecGX,
                        arEstimate, maEstimate, constRequired, constEstimate,
                        xregEstimate, wild, fXEstimate, gXEstimate, initialXEstimate));
}


List fitter(arma::mat matrixVt, arma::mat matrixF, arma::rowvec rowvecW, arma::vec vecYt, arma::vec vecG,
             arma::uvec lags, char E, char T, char S,
             arma::mat matrixXt, arma::mat matrixAt, arma::mat matrixFX, arma::vec vecGX, arma::vec vecOt) {
    /* # matrixVt should have a length of obs + maxlag.
    * # rowvecW should have 1 row.
    * # matgt should be a vector
    * # lags is a vector of lags
    * # matrixXt is the matrix with the exogenous variables
    * # matrixAt is the matrix with the parameters for the exogenous
    */

    int obs = vecYt.n_rows;
    int obsall = matrixVt.n_rows;
    int lagslength = lags.n_rows;
    unsigned int maxlag = max(lags);

    lags = maxlag - lags;

    for(int i=1; i<lagslength; i=i+1){
        lags(i) = lags(i) + obsall * i;
    }

    arma::uvec lagrows(lagslength, arma::fill::zeros);

    arma::vec matyfit(obs, arma::fill::zeros);
    arma::vec materrors(obs, arma::fill::zeros);
    arma::rowvec bufferforat(vecGX.n_rows);

    for (unsigned int i=maxlag; i<obs+maxlag; i=i+1) {
        lagrows = lags - maxlag + i;

/* # Measurement equation and the error term */
        matyfit.row(i-maxlag) = vecOt(i-maxlag) * (wvalue(matrixVt(lagrows), rowvecW, T, S) +
                                       matrixXt.row(i-maxlag) * arma::trans(matrixAt.row(i-1)));
        materrors(i-maxlag) = errorf(vecYt(i-maxlag), matyfit(i-maxlag), E);

/* # Transition equation */
        matrixVt.row(i) = arma::trans(fvalue(matrixVt(lagrows), matrixF, T, S) +
                                      gvalue(matrixVt(lagrows), matrixF, rowvecW, E, T, S) % vecG * materrors(i-maxlag));

/* Failsafe for cases when unreasonable value for state vector was produced */
        if(!matrixVt.row(i).is_finite()){
            matrixVt.row(i) = trans(matrixVt(lagrows));
        }
        if((S=='M') & (matrixVt(i,matrixVt.n_cols-1) <= 0)){
            matrixVt(i,matrixVt.n_cols-1) = arma::as_scalar(trans(matrixVt(lagrows.row(matrixVt.n_cols-1))));
        }
        if(T=='M'){
            if((matrixVt(i,0) <= 0) | (matrixVt(i,1) <= 0)){
                matrixVt(i,0) = arma::as_scalar(trans(matrixVt(lagrows.row(0))));
                matrixVt(i,1) = arma::as_scalar(trans(matrixVt(lagrows.row(1))));
            }
        }

/* Renormalise components if the seasonal model is chosen */
        if(S!='N'){
            if(double(i+1) / double(maxlag) == double((i+1) / maxlag)){
                matrixVt.rows(i-maxlag+1,i) = normaliser(matrixVt.rows(i-maxlag+1,i), obsall, maxlag, S, T);
            }
        }

/* # Transition equation for xreg */
        bufferforat = arma::trans(vecGX / arma::trans(matrixXt.row(i-maxlag)) * materrors(i-maxlag));
        bufferforat.elem(find_nonfinite(bufferforat)).fill(0);
        matrixAt.row(i) = matrixAt.row(i-1) * matrixFX + bufferforat;
    }

    for (int i=obs+maxlag; i<obsall; i=i+1) {
        lagrows = lags - maxlag + i;
        matrixVt.row(i) = arma::trans(fvalue(matrixVt(lagrows), matrixF, T, S));
        matrixAt.row(i) = matrixAt.row(i-1) * matrixFX;
    }

    return List::create(Named("matvt") = matrixVt, Named("yfit") = matyfit,
                        Named("errors") = materrors, Named("matat") = matrixAt);
}

List backfitter(arma::mat matrixVt, arma::mat matrixF, arma::rowvec rowvecW, arma::vec vecYt, arma::vec vecG,
             arma::uvec lags, char E, char T, char S,
             arma::mat matrixXt, arma::mat matrixAt, arma::mat matrixFX, arma::vec vecGX, arma::vec vecOt) {
    /* # matrixVt should have a length of obs + maxlag.
    * # rowvecW should have 1 row.
    * # matgt should be a vector
    * # lags is a vector of lags
    * # matrixXt is the matrix with the exogenous variables
    * # matrixAt is the matrix with the parameters for the exogenous
    */

    int nloops = 5;

    int obs = vecYt.n_rows;
    int obsall = matrixVt.n_rows;
    int lagslength = lags.n_rows;
    unsigned int maxlag = max(lags);
    unsigned int minlag = min(lags);
    arma::uvec backlags = lags - minlag;

    lags = maxlag - lags;

    for(int i=1; i<lagslength; i=i+1){
        lags(i) = lags(i) + obsall * i;
        backlags(i) = backlags(i) + obsall * i;
    }
    if(maxlag == minlag){
        backlags = backlags + maxlag - 1;
    }

    arma::uvec lagrows(lagslength, arma::fill::zeros);

    arma::vec matyfit(obs, arma::fill::zeros);
    arma::vec materrors(obs, arma::fill::zeros);
    arma::rowvec bufferforat(vecGX.n_rows);

    for(int j=0; j<=nloops; j=j+1){
/* ### Go forward ### */
        for (unsigned int i=maxlag; i<obs+maxlag; i=i+1) {
            lagrows = lags - maxlag + i;

/* # Measurement equation and the error term */
            matyfit.row(i-maxlag) = vecOt(i-maxlag) * (wvalue(matrixVt(lagrows), rowvecW, T, S) +
                                           matrixXt.row(i-maxlag) * arma::trans(matrixAt.row(i-1)));
            materrors(i-maxlag) = errorf(vecYt(i-maxlag), matyfit(i-maxlag), E);

/* # Transition equation */
            matrixVt.row(i) = arma::trans(fvalue(matrixVt(lagrows), matrixF, T, S) +
                                          gvalue(matrixVt(lagrows), matrixF, rowvecW, E, T, S) % vecG * materrors(i-maxlag));

/* Failsafe for cases when unreasonable value for state vector was produced */
            if(!matrixVt.row(i).is_finite()){
                matrixVt.row(i) = trans(matrixVt(lagrows));
            }
            if((S=='M') & (matrixVt(i,matrixVt.n_cols-1) <= 0)){
                matrixVt(i,matrixVt.n_cols-1) = arma::as_scalar(trans(matrixVt(lagrows.row(matrixVt.n_cols-1))));
            }
            if(T=='M'){
                if((matrixVt(i,0) <= 0) | (matrixVt(i,1) <= 0)){
                    matrixVt(i,0) = arma::as_scalar(trans(matrixVt(lagrows.row(0))));
                    matrixVt(i,1) = arma::as_scalar(trans(matrixVt(lagrows.row(1))));
                }
            }

/* Renormalise components if the seasonal model is chosen */
            if(S!='N'){
                if(double(i+1) / double(maxlag) == double((i+1) / maxlag)){
                    matrixVt.rows(i-maxlag+1,i) = normaliser(matrixVt.rows(i-maxlag+1,i), obsall, maxlag, S, T);
                }
            }

/* # Transition equation for xreg */
            bufferforat = arma::trans(vecGX / arma::trans(matrixXt.row(i-maxlag)) * materrors(i-maxlag));
            bufferforat.elem(find_nonfinite(bufferforat)).fill(0);
            matrixAt.row(i) = matrixAt.row(i-1) * matrixFX + bufferforat;
        }
/* # Fill in the tail of the matrices */
        for (int i=obs+maxlag; i<obsall; i=i+1) {
            lagrows = lags - maxlag + i;
            matrixVt.row(i) = arma::trans(fvalue(matrixVt(lagrows), matrixF, T, S));
            matrixAt.row(i) = matrixAt.row(i-1) * matrixFX;
        }

/* # If this is the last loop, stop here and don't do backcast */
        if(j==nloops){
            break;
        }

/* ### Now go back ### */
        for (unsigned int i=obs+maxlag-1; i>=maxlag; i=i-1) {
            lagrows = backlags + i + 1;

/* # Measurement equation and the error term */
            matyfit.row(i-maxlag) = vecOt(i-maxlag) * (wvalue(matrixVt(lagrows), rowvecW, T, S) +
                                           matrixXt.row(i-maxlag) * arma::trans(matrixAt.row(i+1)));
            materrors(i-maxlag) = errorf(vecYt(i-maxlag), matyfit(i-maxlag), E);

/* # Transition equation */
            matrixVt.row(i) = arma::trans(fvalue(matrixVt(lagrows), matrixF, T, S) +
                                          gvalue(matrixVt(lagrows), matrixF, rowvecW, E, T, S) % vecG * materrors(i-maxlag));

/* Failsafe for cases when unreasonable value for state vector was produced */
            if(!matrixVt.row(i).is_finite()){
                matrixVt.row(i) = trans(matrixVt(lagrows));
            }
            if((S=='M') & (matrixVt(i,matrixVt.n_cols-1) <= 0)){
                matrixVt(i,matrixVt.n_cols-1) = arma::as_scalar(trans(matrixVt(lagrows.row(matrixVt.n_cols-1))));
            }
            if(T=='M'){
                if((matrixVt(i,0) <= 0) | (matrixVt(i,1) <= 0)){
                    matrixVt(i,0) = arma::as_scalar(trans(matrixVt(lagrows.row(0))));
                    matrixVt(i,1) = arma::as_scalar(trans(matrixVt(lagrows.row(1))));
                }
            }

/* Skipping renormalisation of components in backcasting */

/* # Transition equation for xreg */
            bufferforat = arma::trans(vecGX / arma::trans(matrixXt.row(i-maxlag)) * materrors(i-maxlag));
            bufferforat.elem(find_nonfinite(bufferforat)).fill(0);
            matrixAt.row(i) = matrixAt.row(i+1) * matrixFX + bufferforat;
        }
/* # Fill in the head of the matrices */
        for (int i=maxlag-1; i>=0; i=i-1) {
            lagrows = backlags + i + 1;
            matrixVt.row(i) = arma::trans(fvalue(matrixVt(lagrows), matrixF, T, S));
            matrixAt.row(i) = matrixAt.row(i+1) * matrixFX;
        }
    }

    return List::create(Named("matvt") = matrixVt, Named("yfit") = matyfit,
                        Named("errors") = materrors, Named("matat") = matrixAt);
}

/* # Wrapper for fitter */
// [[Rcpp::export]]
RcppExport SEXP fitterwrap(SEXP matvt, SEXP matF, SEXP matw, SEXP yt, SEXP vecg,
                            SEXP modellags, SEXP Etype, SEXP Ttype, SEXP Stype, SEXP fittertype,
                            SEXP matxt, SEXP matat, SEXP matFX, SEXP vecgX, SEXP ot) {
    NumericMatrix matvt_n(matvt);
    arma::mat matrixVt(matvt_n.begin(), matvt_n.nrow(), matvt_n.ncol());

    NumericMatrix matF_n(matF);
    arma::mat matrixF(matF_n.begin(), matF_n.nrow(), matF_n.ncol(), false);

    NumericMatrix matw_n(matw);
    arma::rowvec rowvecW(matw_n.begin(), matw_n.ncol(), false);

    NumericMatrix yt_n(yt);
    arma::vec vecYt(yt_n.begin(), yt_n.nrow(), false);

    NumericMatrix vecg_n(vecg);
    arma::vec vecG(vecg_n.begin(), vecg_n.nrow(), false);

    IntegerVector modellags_n(modellags);
    arma::uvec lags = as<arma::uvec>(modellags_n);

    char E = as<char>(Etype);
    char T = as<char>(Ttype);
    char S = as<char>(Stype);

    char fitterType = as<char>(fittertype);

    NumericMatrix matxt_n(matxt);
    arma::mat matrixXt(matxt_n.begin(), matxt_n.nrow(), matxt_n.ncol(), false);

    NumericMatrix matat_n(matat);
    arma::mat matrixAt(matat_n.begin(), matat_n.nrow(), matat_n.ncol());

    NumericMatrix matFX_n(matFX);
    arma::mat matrixFX(matFX_n.begin(), matFX_n.nrow(), matFX_n.ncol(), false);

    NumericMatrix vecgX_n(vecgX);
    arma::vec vecGX(vecgX_n.begin(), vecgX_n.nrow(), false);

    NumericVector ot_n(ot);
    arma::vec vecOt(ot_n.begin(), ot_n.size(), false);

    switch(fitterType){
        case 'b':
            return wrap(backfitter(matrixVt, matrixF, rowvecW, vecYt, vecG, lags, E, T, S,
                                   matrixXt, matrixAt, matrixFX, vecGX, vecOt));
        break;
        case 'o':
        default:
            return wrap(fitter(matrixVt, matrixF, rowvecW, vecYt, vecG, lags, E, T, S,
                               matrixXt, matrixAt, matrixFX, vecGX, vecOt));
    }
}

/* # Function produces the point forecasts for the specified model */
arma::mat forecaster(arma::mat matrixVt, arma::mat matrixF, arma::rowvec rowvecW,
                     unsigned int hor, char T, char S, arma::uvec lags,
                     arma::mat matrixXt, arma::mat matrixAt, arma::mat matrixFX) {
    int lagslength = lags.n_rows;
    unsigned int maxlag = max(lags);
    unsigned int hh = hor + maxlag;

    arma::uvec lagrows(lagslength, arma::fill::zeros);
    arma::vec matyfor(hor, arma::fill::zeros);
    arma::mat matrixVtnew(hh, matrixVt.n_cols, arma::fill::zeros);
    arma::mat matrixAtnew(hh, matrixAt.n_cols, arma::fill::zeros);

    lags = maxlag - lags;
    for(int i=1; i<lagslength; i=i+1){
        lags(i) = lags(i) + hh * i;
    }

    matrixVtnew.submat(0,0,maxlag-1,matrixVtnew.n_cols-1) = matrixVt.submat(0,0,maxlag-1,matrixVtnew.n_cols-1);
    matrixAtnew.submat(0,0,maxlag-1,matrixAtnew.n_cols-1) = matrixAtnew.submat(0,0,maxlag-1,matrixAtnew.n_cols-1);

/* # Fill in the new xt matrix using F. Do the forecasts. */
    for (unsigned int i=maxlag; i<(hor+maxlag); i=i+1) {
        lagrows = lags - maxlag + i;
        matrixVtnew.row(i) = arma::trans(fvalue(matrixVtnew(lagrows), matrixF, T, S));
        matrixAtnew.row(i) = matrixAtnew.row(i-1) * matrixFX;

        matyfor.row(i-maxlag) = (wvalue(matrixVtnew(lagrows), rowvecW, T, S) + matrixXt.row(i-maxlag) * arma::trans(matrixAt.row(i-maxlag)));
    }

    return matyfor;
}

/* # Wrapper for forecaster */
// [[Rcpp::export]]
RcppExport SEXP forecasterwrap(SEXP matvt, SEXP matF, SEXP matw,
                               SEXP h, SEXP Ttype, SEXP Stype, SEXP modellags,
                               SEXP matxt, SEXP matat, SEXP matFX){
    NumericMatrix matvt_n(matvt);
    arma::mat matrixVt(matvt_n.begin(), matvt_n.nrow(), matvt_n.ncol(), false);

    NumericMatrix matF_n(matF);
    arma::mat matrixF(matF_n.begin(), matF_n.nrow(), matF_n.ncol(), false);

    NumericMatrix matw_n(matw);
    arma::mat rowvecW(matw_n.begin(), matw_n.nrow(), matw_n.ncol(), false);

    unsigned int hor = as<int>(h);
    char T = as<char>(Ttype);
    char S = as<char>(Stype);

    IntegerVector modellags_n(modellags);
    arma::uvec lags = as<arma::uvec>(modellags_n);

    NumericMatrix matxt_n(matxt);
    arma::mat matrixXt(matxt_n.begin(), matxt_n.nrow(), matxt_n.ncol(), false);

    NumericMatrix matat_n(matat);
    arma::mat matrixAt(matat_n.begin(), matat_n.nrow(), matat_n.ncol());

    NumericMatrix matFX_n(matFX);
    arma::mat matrixFX(matFX_n.begin(), matFX_n.nrow(), matFX_n.ncol(), false);

    return wrap(forecaster(matrixVt, matrixF, rowvecW, hor, T, S, lags, matrixXt, matrixAt, matrixFX));
}

/* # Function produces matrix of errors based on multisteps forecast */
arma::mat errorer(arma::mat matrixVt, arma::mat matrixF, arma::mat rowvecW, arma::mat vecYt,
                  int hor, char E, char T, char S, arma::uvec lags,
                  arma::mat matrixXt, arma::mat matrixAt, arma::mat matrixFX, arma::vec vecOt){
    int obs = vecYt.n_rows;
    int hh = 0;
    arma::mat materrors(obs+hor-1, hor, arma::fill::zeros);
    unsigned int maxlag = max(lags);

    for(int i = 0; i < obs; i=i+1){
        hh = std::min(hor, obs-i);
        materrors.submat(hor-1+i, 0, hor-1+i, hh-1) = trans(vecOt.rows(i, i+hh-1) % errorvf(vecYt.rows(i, i+hh-1),
            forecaster(matrixVt.rows(i,i+maxlag-1), matrixF, rowvecW, hh, T, S, lags, matrixXt.rows(i, i+hh-1),
                matrixAt.rows(i, i+hh-1), matrixFX), E));
    }

// Fix for GV in order to perform better in the sides of the series
    for(int i=0; i<(hor-1); i=i+1){
        materrors.submat((hor-2)-(i),i+1,(hor-2)-(i),hor-1) = materrors.submat(hor-1,0,hor-1,hor-i-2) * sqrt(1.0+i);
    }

    return materrors;
}

/* # Wrapper for errorer */
// [[Rcpp::export]]
RcppExport SEXP errorerwrap(SEXP matvt, SEXP matF, SEXP matw, SEXP yt,
                            SEXP h, SEXP Etype, SEXP Ttype, SEXP Stype, SEXP modellags,
                            SEXP matxt, SEXP matat, SEXP matFX, SEXP ot){
    NumericMatrix matvt_n(matvt);
    arma::mat matrixVt(matvt_n.begin(), matvt_n.nrow(), matvt_n.ncol(), false);

    NumericMatrix matF_n(matF);
    arma::mat matrixF(matF_n.begin(), matF_n.nrow(), matF_n.ncol(), false);

    NumericMatrix matw_n(matw);
    arma::rowvec rowvecW(matw_n.begin(), matw_n.ncol(), false);

    NumericMatrix yt_n(yt);
    arma::vec vecYt(yt_n.begin(), yt_n.nrow(), false);

    int hor = as<int>(h);
    char E = as<char>(Etype);
    char T = as<char>(Ttype);
    char S = as<char>(Stype);

    IntegerVector modellags_n(modellags);
    arma::uvec lags = as<arma::uvec>(modellags_n);

    NumericMatrix matxt_n(matxt);
    arma::mat matrixXt(matxt_n.begin(), matxt_n.nrow(), matxt_n.ncol(), false);

    NumericMatrix matat_n(matat);
    arma::mat matrixAt(matat_n.begin(), matat_n.nrow(), matat_n.ncol());

    NumericMatrix matFX_n(matFX);
    arma::mat matrixFX(matFX_n.begin(), matFX_n.nrow(), matFX_n.ncol(), false);

    NumericVector ot_n(ot);
    arma::vec vecOt(ot_n.begin(), ot_n.size(), false);

    return wrap(errorer(matrixVt, matrixF, rowvecW, vecYt,
                        hor, E, T, S, lags,
                        matrixXt, matrixAt, matrixFX, vecOt));
}

int CFtypeswitch (std::string const& CFtype) {
    if (CFtype == "TFL") return 1;
    if (CFtype == "MLSTFE") return 2;
    if (CFtype == "MSTFE") return 3;
    if (CFtype == "MSEh") return 4;
    if (CFtype == "MAE") return 5;
    if (CFtype == "HAM") return 6;
    if (CFtype == "MSE") return 7;
    if (CFtype == "aTFL") return 8;
    if (CFtype == "aMLSTFE") return 9;
    if (CFtype == "aMSTFE") return 10;
    if (CFtype == "aMSEh") return 11;
    else return 7;
}

/* # Function returns the chosen Cost Function based on the chosen model and produced errors */
double optimizer(arma::mat matrixVt, arma::mat matrixF, arma::rowvec rowvecW, arma::vec vecYt, arma::vec vecG,
                 unsigned int hor, arma::uvec lags, char E, char T, char S,
                 bool multi, std::string CFtype, double normalize, char fitterType,
                 arma::mat  matrixXt, arma::mat matrixAt, arma::mat matrixFX, arma::vec vecGX, arma::vec vecOt){
// # Make decomposition functions shut up!
    std::ostream nullstream(0);
    arma::set_stream_err2(nullstream);

    arma::uvec nonzeroes = find(vecOt>0);
    int obs = nonzeroes.n_rows;
    double CFres = 0;
    int matobs = obs + hor - 1;

// yactsum is needed for multiplicative error models
    double yactsum = arma::as_scalar(sum(log(vecYt.elem(nonzeroes))));

    List fitting;

    switch(fitterType){
        case 'b':
        fitting = backfitter(matrixVt, matrixF, rowvecW, vecYt, vecG, lags, E, T, S,
                             matrixXt, matrixAt, matrixFX, vecGX, vecOt);
        break;
        case 'o':
        default:
        fitting = fitter(matrixVt, matrixF, rowvecW, vecYt, vecG, lags, E, T, S,
                         matrixXt, matrixAt, matrixFX, vecGX, vecOt);
    }

    NumericMatrix mvtfromfit = as<NumericMatrix>(fitting["matvt"]);
    matrixVt = as<arma::mat>(mvtfromfit);
    NumericMatrix errorsfromfit = as<NumericMatrix>(fitting["errors"]);
    NumericMatrix matrixAtfromfit = as<NumericMatrix>(fitting["matat"]);
    matrixAt = as<arma::mat>(matrixAtfromfit);

    arma::mat materrors;

    arma::vec veccij(hor, arma::fill::ones);
    arma::mat matrixSigma(hor, hor, arma::fill::eye);

    if((multi==true) & (CFtypeswitch(CFtype)<=7)){
        materrors = errorer(matrixVt, matrixF, rowvecW, vecYt, hor, E, T, S, lags, matrixXt, matrixAt, matrixFX, vecOt);
        if(E=='M'){
            materrors = log(1 + materrors);
            materrors.elem(arma::find_nonfinite(materrors)).fill(1e10);

// This correction is needed in order to take the correct number of observations in the error matrix
            yactsum = yactsum / obs * matobs;
        }
    }
    else{
        arma::mat materrorsfromfit(errorsfromfit.begin(), errorsfromfit.nrow(), errorsfromfit.ncol(), false);
        materrors = materrorsfromfit;
        materrors = materrors.elem(nonzeroes);
        if(E=='M'){
            materrors = log(1 + materrors);
        }
    }

    if(CFtypeswitch(CFtype)>7){
// Form vector for basic values and matrix Mu
        for(unsigned int i=1; i<hor; i++){
            veccij(i) = as_scalar(rowvecW * matrixPower(matrixF,i) * vecG);
        }

// Fill in the diagonal of Sigma matrix
        for(unsigned int i=1; i<hor; i++){
            matrixSigma(i,i) = matrixSigma(i-1,i-1) + pow(veccij(i),2);
        }

        if(CFtype=="aTFL"){
            for(unsigned int i=0; i<hor; i++){
                for(unsigned int j=0; j<hor; j++){
                    if(i>=j){
                        continue;
                    }
                    if(i==0){
                        matrixSigma(i,j) = veccij(j);
                    }
                    else{
                        matrixSigma(i,j) = veccij(j-i) + sum(veccij.rows(j-i+1,j) % veccij.rows(1,i));
                    }
                }
            }
            matrixSigma = symmatu(matrixSigma);
        }
    }

    switch(E){
    case 'M':
        switch(CFtypeswitch(CFtype)){
        case 1:
            try{
                CFres = double(log(arma::prod(eig_sym(trans(materrors) * (materrors) / matobs))));
            }
            catch(const std::runtime_error){
                CFres = double(log(arma::det(arma::trans(materrors) * materrors / double(matobs))));
            }
            CFres = CFres + (2 / double(matobs)) * double(hor) * yactsum;
        break;
        case 2:
            CFres = arma::as_scalar(sum(log(sum(pow(materrors,2)) / double(matobs)), 1))
                    + (2 / double(obs)) * double(hor) * yactsum;
        break;
// no exp is the temporary fix for very strange behaviour of MAM type models
        case 3:
            CFres = arma::as_scalar(log(sum(sum(pow(materrors,2)) / double(matobs), 1))
                        + (2 / double(obs)) * double(hor) * yactsum);
        break;
        case 4:
            CFres = arma::as_scalar(exp(log(sum(pow(materrors.col(hor-1),2)) / double(matobs))
                                        + (2 / double(obs)) * yactsum));
        break;
        case 5:
            CFres = arma::as_scalar(exp(log(mean(abs(materrors))) + (2 / double(obs)) * yactsum));
        break;
        case 6:
            CFres = arma::as_scalar(exp(log(mean(sqrt(abs(materrors)))) + (2 / double(obs)) * yactsum));
        break;
        case 7:
            CFres = arma::as_scalar(exp(log(mean(pow(materrors,2))) + (2 / double(obs)) * yactsum));
        break;
        case 8:
        case 9:
            try{
                CFres = double(log(arma::prod(eig_sym(as_scalar(mean(pow(materrors / normalize,2))) * matrixSigma
                                                          ))) + hor*log(pow(normalize,2)));
            }
            catch(const std::runtime_error){
                CFres = log(arma::det(as_scalar(mean(pow(materrors / normalize,2))) * matrixSigma
                                          )) + hor*log(pow(normalize,2));
            }
            CFres = CFres + (2 / double(matobs)) * double(hor) * yactsum;
        break;
        case 10:
            CFres = arma::trace(as_scalar(mean(pow(materrors,2))) * matrixSigma
                                    );
            CFres = CFres + (2 / double(matobs)) * double(hor) * yactsum;
        break;
        case 11:
            CFres = (as_scalar(mean(pow(materrors,2))) * matrixSigma(hor-1,hor-1)
                                        );
            CFres = CFres + (2 / double(matobs)) * double(hor) * yactsum;
        }
    break;
    case 'A':
        switch(CFtypeswitch(CFtype)){
        case 1:
            try{
                CFres = double(log(arma::prod(eig_sym(trans(materrors / normalize) * (materrors / normalize) / matobs))) +
                    hor * log(pow(normalize,2)));
            }
            catch(const std::runtime_error){
                CFres = double(log(arma::det(arma::trans(materrors / normalize) * (materrors / normalize) / matobs)) +
                    hor * log(pow(normalize,2)));
            }
        break;
        case 2:
            CFres = arma::as_scalar(sum(log(sum(pow(materrors,2)) / double(matobs)), 1));
        break;
        case 3:
            CFres = arma::as_scalar(sum(sum(pow(materrors,2)) / double(matobs), 1));
        break;
        case 4:
            CFres = arma::as_scalar(sum(pow(materrors.col(hor-1),2)) / double(matobs));
        break;
        case 5:
            CFres = arma::as_scalar(mean(abs(materrors)));
        break;
        case 6:
            CFres = arma::as_scalar(mean(sqrt(abs(materrors))));
        break;
        case 7:
            CFres = arma::as_scalar(mean(pow(materrors,2)));
        break;
        case 8:
        case 9:
            try{
                CFres = double(log(arma::prod(eig_sym(as_scalar(mean(pow(materrors / normalize,2))) * matrixSigma
                                                          ))) + hor*log(pow(normalize,2)));
            }
            catch(const std::runtime_error){
                CFres = log(arma::det(as_scalar(mean(pow(materrors / normalize,2))) * matrixSigma
                                          )) + hor*log(pow(normalize,2));
            }
        break;
        case 10:
            CFres = arma::trace(as_scalar(mean(pow(materrors,2))) * matrixSigma
                                    );
        break;
        case 11:
            CFres = (as_scalar(mean(pow(materrors,2))) * matrixSigma(hor-1,hor-1)
                                        );
        }
    }
    return CFres;
}

/* # Wrapper for optimiser */
// [[Rcpp::export]]
RcppExport SEXP optimizerwrap(SEXP matvt, SEXP matF, SEXP matw, SEXP yt, SEXP vecg,
                              SEXP h, SEXP modellags, SEXP Etype, SEXP Ttype, SEXP Stype,
                              SEXP multisteps, SEXP CFt, SEXP normalizer, SEXP fittertype,
                              SEXP matxt, SEXP matat, SEXP matFX, SEXP vecgX, SEXP ot) {
    NumericMatrix matvt_n(matvt);
    arma::mat matrixVt(matvt_n.begin(), matvt_n.nrow(), matvt_n.ncol());

    NumericMatrix matF_n(matF);
    arma::mat matrixF(matF_n.begin(), matF_n.nrow(), matF_n.ncol(), false);

    NumericMatrix matw_n(matw);
    arma::rowvec rowvecW(matw_n.begin(), matw_n.ncol(), false);

    NumericMatrix yt_n(yt);
    arma::vec vecYt(yt_n.begin(), yt_n.nrow(), false);

    NumericMatrix vecg_n(vecg);
    arma::vec vecG(vecg_n.begin(), vecg_n.nrow(), false);

    unsigned int hor = as<unsigned int>(h);

    IntegerVector modellags_n(modellags);
    arma::uvec lags = as<arma::uvec>(modellags_n);

    char E = as<char>(Etype);
    char T = as<char>(Ttype);
    char S = as<char>(Stype);

    bool multi = as<bool>(multisteps);

    std::string CFtype = as<std::string>(CFt);

    double normalize = as<double>(normalizer);

    char fitterType = as<char>(fittertype);

    NumericMatrix matxt_n(matxt);
    arma::mat matrixXt(matxt_n.begin(), matxt_n.nrow(), matxt_n.ncol(), false);

    NumericMatrix matat_n(matat);
    arma::mat matrixAt(matat_n.begin(), matat_n.nrow(), matat_n.ncol());

    NumericMatrix matFX_n(matFX);
    arma::mat matrixFX(matFX_n.begin(), matFX_n.nrow(), matFX_n.ncol(), false);

    NumericMatrix vecgX_n(vecgX);
    arma::vec vecGX(vecgX_n.begin(), vecgX_n.nrow(), false);

    NumericVector ot_n(ot);
    arma::vec vecOt(ot_n.begin(), ot_n.size(), false);

    return wrap(optimizer(matrixVt, matrixF, rowvecW, vecYt, vecG,
                          hor, lags, E, T, S,
                          multi, CFtype, normalize, fitterType,
                          matrixXt, matrixAt, matrixFX, vecGX, vecOt));
}

/* # Function is used in cases when the persistence vector needs to be estimated.
# If bounds are violated, it returns variance of yt. */
// [[Rcpp::export]]
RcppExport SEXP costfunc(SEXP matvt, SEXP matF, SEXP matw, SEXP yt, SEXP vecg,
                              SEXP h, SEXP modellags, SEXP Etype, SEXP Ttype, SEXP Stype,
                              SEXP multisteps, SEXP CFt, SEXP normalizer, SEXP fittertype,
                              SEXP matxt, SEXP matat, SEXP matFX, SEXP vecgX, SEXP ot,
                              SEXP bounds) {
/* Function is needed to implement admissible constrains on smoothing parameters */
    NumericMatrix matvt_n(matvt);
    arma::mat matrixVt(matvt_n.begin(), matvt_n.nrow(), matvt_n.ncol());

    NumericMatrix matF_n(matF);
    arma::mat matrixF(matF_n.begin(), matF_n.nrow(), matF_n.ncol(), false);

    NumericMatrix matw_n(matw);
    arma::rowvec rowvecW(matw_n.begin(), matw_n.ncol(), false);

    NumericMatrix yt_n(yt);
    arma::vec vecYt(yt_n.begin(), yt_n.nrow(), false);

    NumericMatrix vecg_n(vecg);
    arma::vec vecG(vecg_n.begin(), vecg_n.nrow(), false);

    int hor = as<int>(h);

    IntegerVector modellags_n(modellags);
    arma::uvec lags = as<arma::uvec>(modellags_n);

    char E = as<char>(Etype);
    char T = as<char>(Ttype);
    char S = as<char>(Stype);

    bool multi = as<bool>(multisteps);

    std::string CFtype = as<std::string>(CFt);

    double normalize = as<double>(normalizer);

    char fitterType = as<char>(fittertype);

    NumericMatrix matxt_n(matxt);
    arma::mat matrixXt(matxt_n.begin(), matxt_n.nrow(), matxt_n.ncol(), false);

    NumericMatrix matat_n(matat);
    arma::mat matrixAt(matat_n.begin(), matat_n.nrow(), matat_n.ncol());

    NumericMatrix matFX_n(matFX);
    arma::mat matrixFX(matFX_n.begin(), matFX_n.nrow(), matFX_n.ncol(), false);

    NumericMatrix vecgX_n(vecgX);
    arma::vec vecGX(vecgX_n.begin(), vecgX_n.nrow(), false);

    NumericVector ot_n(ot);
    arma::vec vecOt(ot_n.begin(), ot_n.size(), false);

    char boundtype = as<char>(bounds);

// Values needed for eigenvalues calculation
    arma::cx_vec eigval;
    arma::mat matrixD = matrixF;

    if(boundtype=='u'){
// alpha in (0,1)
        if((vecG(0)>1) || (vecG(0)<0)){
            vecG.zeros();
            matrixVt.zeros();
        }
        if(T!='N'){
// beta in (0,alpha)
            if((vecG(1)>vecG(0)) || (vecG(1)<0)){
                vecG.zeros();
                matrixVt.zeros();
            }
            if(S!='N'){
// gamma in (0,1-alpha)
                if((vecG(2)>(1-vecG(0))) || (vecG(2)<0)){
                    vecG.zeros();
                    matrixVt.zeros();
                }
            }
        }
        if(S!='N'){
// gamma in (0,1-alpha)
            if((vecG(1)>(1-vecG(0))) || (vecG(1)<0)){
                vecG.zeros();
                matrixVt.zeros();
            }
        }
    }
    else if(boundtype=='a'){
        if(arma::eig_gen(eigval, matrixF - vecG * rowvecW)){
            if(max(abs(eigval))> (1 + 1E-50)){
                return wrap(max(abs(eigval))*1E+100);
            }
        }
        else{
            return wrap(1E+300);
        }
    }

    return wrap(optimizer(matrixVt, matrixF, rowvecW, vecYt, vecG,
                          hor, lags, E, T, S,
                          multi, CFtype, normalize, fitterType,
                          matrixXt, matrixAt, matrixFX, vecGX, vecOt));
}


/* # This is a costfunction for SSARIMA. It initialises ARIMA, checks conditions and then fits the model */
// [[Rcpp::export]]
RcppExport SEXP costfuncARIMA(SEXP ARorders, SEXP MAorders, SEXP Iorders, SEXP ARIMAlags, SEXP nComp,
                              SEXP AR, SEXP MA, SEXP constant, SEXP Cvalues,
                              SEXP matvt, SEXP matF, SEXP matw, SEXP yt, SEXP vecg,
                              SEXP h, SEXP modellags, SEXP Etype, SEXP Ttype, SEXP Stype,
                              SEXP multisteps, SEXP CFt, SEXP normalizer, SEXP fittertype,
                              SEXP nexovars, SEXP matxt, SEXP matat, SEXP matFX, SEXP vecgX, SEXP ot,
                              SEXP estimAR, SEXP estimMA, SEXP requireConst, SEXP estimConst,
                              SEXP estimxreg, SEXP gowild, SEXP estimFX, SEXP estimgX, SEXP estiminitX,
                              SEXP bounds) {

    IntegerVector ARorders_n(ARorders);
    arma::uvec arOrders = as<arma::uvec>(ARorders_n);

    IntegerVector MAorders_n(MAorders);
    arma::uvec maOrders = as<arma::uvec>(MAorders_n);

    IntegerVector Iorders_n(Iorders);
    arma::uvec iOrders = as<arma::uvec>(Iorders_n);

    IntegerVector ARIMAlags_n(ARIMAlags);
    arma::uvec lagsARIMA = as<arma::uvec>(ARIMAlags_n);

    int nComponents = as<int>(nComp);

    NumericVector AR_n;
    if(!Rf_isNull(AR)){
        AR_n = as<NumericVector>(AR);
    }
    arma::vec arValues(AR_n.begin(), AR_n.size(), false);

    NumericVector MA_n;
    if(!Rf_isNull(MA)){
        MA_n = as<NumericVector>(MA);
    }
    arma::vec maValues(MA_n.begin(), MA_n.size(), false);

    double constValue;
    if(!Rf_isNull(constant)){
        constValue = as<double>(constant);
    }

    NumericVector Cvalues_n;
    if(!Rf_isNull(Cvalues)){
        Cvalues_n = as<NumericVector>(Cvalues);
    }
    arma::vec C(Cvalues_n.begin(), Cvalues_n.size(), false);

    NumericMatrix matvt_n(matvt);
    arma::mat matrixVt(matvt_n.begin(), matvt_n.nrow(), matvt_n.ncol());

    NumericMatrix vecg_n(vecg);
    arma::vec vecG(vecg_n.begin(), vecg_n.nrow());

    NumericMatrix matF_n(matF);
    arma::mat matrixF(matF_n.begin(), matF_n.nrow(), matF_n.ncol());

    char fitterType = as<char>(fittertype);

    int nexo = as<int>(nexovars);

    NumericMatrix matat_n(matat);
    arma::mat matrixAt(matat_n.begin(), matat_n.nrow(), matat_n.ncol());

    NumericMatrix matFX_n(matFX);
    arma::mat matrixFX(matFX_n.begin(), matFX_n.nrow(), matFX_n.ncol());

    NumericMatrix vecgX_n(vecgX);
    arma::vec vecGX(vecgX_n.begin(), vecgX_n.nrow());

    bool arEstimate = as<bool>(estimAR);
    bool maEstimate = as<bool>(estimMA);
    bool constRequired = as<bool>(requireConst);
    bool constEstimate = as<bool>(estimConst);
    bool initialEstimate = (fitterType=='o');
    bool xregEstimate = as<bool>(estimxreg);
    bool wild = as<bool>(gowild);
    bool fXEstimate = as<bool>(estimFX);
    bool gXEstimate = as<bool>(estimgX);
    bool initialXEstimate = as<bool>(estiminitX);

// Initialise ARIMA
    List polynomials = polysos(arOrders, maOrders, iOrders, lagsARIMA, nComponents,
                               arValues, maValues, constValue, C,
                               matrixVt, vecG, matrixF,
                               fitterType, nexo, matrixAt, matrixFX, vecGX,
                               arEstimate, maEstimate, constRequired, constEstimate,
                               xregEstimate, wild, fXEstimate, gXEstimate, initialXEstimate);

    matvt_n = as<NumericMatrix>(polynomials["matvt"]);
    matrixVt = as<arma::mat>(matvt_n);

    matF_n = as<NumericMatrix>(polynomials["matF"]);
    matrixF = as<arma::mat>(matF_n);

    NumericMatrix matw_n(matw);
    arma::rowvec rowvecW(matw_n.begin(), matw_n.ncol(), false);

    NumericMatrix yt_n(yt);
    arma::vec vecYt(yt_n.begin(), yt_n.nrow(), false);

    vecg_n = as<NumericMatrix>(polynomials["vecg"]);
    vecG = as<arma::mat>(vecg_n);

    int hor = as<int>(h);

    IntegerVector modellags_n(modellags);
    arma::uvec lags = as<arma::uvec>(modellags_n);

    char E = as<char>(Etype);
    char T = as<char>(Ttype);
    char S = as<char>(Stype);

    bool multi = as<bool>(multisteps);

    std::string CFtype = as<std::string>(CFt);

    double normalize = as<double>(normalizer);

    NumericMatrix matxt_n(matxt);
    arma::mat matrixXt(matxt_n.begin(), matxt_n.nrow(), matxt_n.ncol(), false);

    matat_n = as<NumericMatrix>(polynomials["matat"]);
    matrixAt = as<arma::mat>(matat_n);

    matFX_n = as<NumericMatrix>(polynomials["matFX"]);
    matrixFX = as<arma::mat>(matFX_n);

    vecgX_n = as<NumericMatrix>(polynomials["vecgX"]);
    vecGX = as<arma::mat>(vecgX_n);

    NumericVector ot_n(ot);
    arma::vec vecOt(ot_n.begin(), ot_n.size(), false);

    char boundtype = as<char>(bounds);

    if((nComponents>0) & (boundtype=='a')){
        arma::cx_vec eigval;

// Check stability condition
        if(arma::eig_gen(eigval, matrixF - vecG * rowvecW)){
            if(max(abs(eigval))> (1 + 1E-50)){
                return wrap(max(abs(eigval))*1E+100);
            }
        }
        else{
            return wrap(1E+300);
        }

// Check stationarity condition
        if(as_scalar(arOrders.t() * lagsARIMA) > 0){
            NumericMatrix arPolynom = as<NumericMatrix>(polynomials["arPolynomial"]);
            arma::mat arPolynomial = as<arma::mat>(arPolynom);

            arma::mat arMatrixF = matrixF.submat(0,0,arPolynomial.n_elem-2,arPolynomial.n_elem-2);
            arMatrixF.submat(0,0,arMatrixF.n_rows-1,0) = arPolynomial.rows(1,arPolynomial.n_elem-1);

            if(arma::eig_gen(eigval, arMatrixF)){
                if(max(abs(eigval))> 1){
                    return wrap(max(abs(eigval))*1E+100);
                }
            }
            else{
                return wrap(1E+300);
            }
        }
    }

    return wrap(optimizer(matrixVt, matrixF, rowvecW, vecYt, vecG,
                          hor, lags, E, T, S,
                          multi, CFtype, normalize, fitterType,
                          matrixXt, matrixAt, matrixFX, vecGX, vecOt));
}

/*
# autoets - function estimates all the necessary ETS models and returns the one with the smallest chosen IC.
*/

// ##### Script for sim.ets function
List simulateETS(arma::cube arrayVt, arma::mat matrixerrors, arma::mat matrixot,
                 arma::mat matrixF, arma::rowvec rowvecW, arma::mat matrixG,
                 unsigned int obs, unsigned int nseries,
                 char E, char T, char S, arma::uvec lags) {

    arma::mat matY(obs, nseries);

    int lagslength = lags.n_rows;
    unsigned int maxlag = max(lags);
    int obsall = obs + maxlag;

    lags = maxlag - lags;

    for(int i=1; i<lagslength; i=i+1){
        lags(i) = lags(i) + obsall * i;
    }

    arma::uvec lagrows(lagslength, arma::fill::zeros);
    arma::mat matrixVt(obsall, lagslength, arma::fill::zeros);

    for(unsigned int i=0; i<nseries; i=i+1){
        matrixVt = arrayVt.slice(i);
        for (int j=maxlag; j<obsall; j=j+1) {

            lagrows = lags - maxlag + j;
/* # Measurement equation and the error term */
            matY(j-maxlag,i) = matrixot(j-maxlag,i) * (wvalue(matrixVt(lagrows), rowvecW, T, S) +
                                 rvalue(matrixVt(lagrows), rowvecW, E, T, S) * matrixerrors(j-maxlag,i));
/* # Transition equation */
            matrixVt.row(j) = arma::trans(fvalue(matrixVt(lagrows), matrixF, T, S) +
                                          gvalue(matrixVt(lagrows), matrixF, rowvecW, E, T, S) % matrixG.col(i) * matrixerrors(j-maxlag,i));
/* Failsafe for cases when unreasonable value for state vector was produced */
            if(!matrixVt.row(j).is_finite()){
                matrixVt.row(j) = trans(matrixVt(lagrows));
            }
            if((S=='M') & (matrixVt(j,matrixVt.n_cols-1) <= 0)){
                matrixVt(j,matrixVt.n_cols-1) = arma::as_scalar(trans(matrixVt(lagrows.row(matrixVt.n_cols-1))));
            }
            if(T=='M'){
                if(matrixVt(j,1) <= 0){
                    matrixVt(j,1) = arma::as_scalar(trans(matrixVt(lagrows.row(1))));
                }
            }
        }
        arrayVt.slice(i) = matrixVt;
    }

    return List::create(Named("arrvt") = arrayVt, Named("matyt") = matY);
}

/* # Wrapper for simulateETS */
// [[Rcpp::export]]
RcppExport SEXP simulateETSwrap(SEXP arrvt, SEXP materrors, SEXP matot, SEXP matF, SEXP matw, SEXP matg,
                                SEXP Etype, SEXP Ttype, SEXP Stype, SEXP modellags) {

// ### arrvt should contain array of obs x ncomponents x nseries elements.
    NumericVector arrvt_n(arrvt);
    IntegerVector arrvt_dim = arrvt_n.attr("dim");
    arma::cube arrayVt(arrvt_n.begin(),arrvt_dim[0], arrvt_dim[1], arrvt_dim[2], false);

    NumericMatrix materrors_n(materrors);
    arma::mat matrixerrors(materrors_n.begin(), materrors_n.nrow(), materrors_n.ncol(), false);

    NumericMatrix matot_n(matot);
    arma::mat matrixot(matot_n.begin(), matot_n.nrow(), matot_n.ncol(), false);

    NumericMatrix matF_n(matF);
    arma::mat matrixF(matF_n.begin(), matF_n.nrow(), matF_n.ncol(), false);

    NumericMatrix matw_n(matw);
    arma::rowvec rowvecW(matw_n.begin(), matw_n.ncol(), false);

// ### matg should contain persistence vectors in each column
    NumericMatrix matg_n(matg);
    arma::mat matrixG(matg_n.begin(), matg_n.nrow(), matg_n.ncol(), false);

    unsigned int obs = materrors_n.nrow();
    unsigned int nseries = materrors_n.ncol();
    char E = as<char>(Etype);
    char T = as<char>(Ttype);
    char S = as<char>(Stype);

    IntegerVector modellags_n(modellags);
    arma::uvec lags = as<arma::uvec>(modellags_n);

    return wrap(simulateETS(arrayVt, matrixerrors, matrixot, matrixF, rowvecW, matrixG,
                            obs, nseries, E, T, S, lags));
}

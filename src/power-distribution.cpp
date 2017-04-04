#include <Rcpp.h>
#include "shared.h"
// [[Rcpp::plugins(cpp11)]]

using std::pow;
using std::sqrt;
using std::abs;
using std::exp;
using std::log;
using std::floor;
using std::ceil;
using Rcpp::NumericVector;


/*
*  Power distribution
*
*  Values:
*  0 < x < alpha
*
*  Parameters:
*  alpha
*  beta
*
*  f(x)    = (beta*x^(beta-1)) / (alpha^beta)
*  F(x)    = x^beta / alpha^beta
*  F^-1(p) = alpha * p^(1/beta)
*
*/

/*
 inline double pdf_power(double x, double alpha, double beta,
                         bool& throw_warning) {
  if (ISNAN(x) || ISNAN(alpha) || ISNAN(beta))
    return x+alpha+beta;
  if (x <= 0.0 || x >= alpha)
    return 0.0;
  return beta * pow(x, beta-1.0) / pow(alpha, beta);
}

 inline double cdf_power(double x, double alpha, double beta,
                 bool& throw_warning) {
  if (ISNAN(x) || ISNAN(alpha) || ISNAN(beta))
    return x+alpha+beta;
  if (x <= 0.0)
    return 0.0;
  if (x >= alpha)
    return 1.0;
  return pow(x, beta) / pow(alpha, beta);
}
*/

inline double invcdf_power(double p, double alpha, double beta,
                           bool& throw_warning) {
  if (ISNAN(p) || ISNAN(alpha) || ISNAN(beta))
    return p+alpha+beta;
  if (!VALID_PROB(p)) {
    throw_warning = true;
    return NAN;
  }
  return alpha * pow(p, 1.0/beta);
}

inline double rng_power(double alpha, double beta,
                        bool& throw_warning) {
  if (ISNAN(alpha) || ISNAN(beta)) {
    throw_warning = true;
    return NA_REAL;
  }
  double u = rng_unif();
  return alpha * pow(u, 1.0/beta);
}

inline double logpdf_power(double x, double alpha, double beta,
                           bool& throw_warning) {
  if (ISNAN(x) || ISNAN(alpha) || ISNAN(beta))
    return x+alpha+beta;
  if (x <= 0.0 || x >= alpha)
    return R_NegInf;
  return log(beta) + log(x)*(beta-1.0) - log(alpha)*beta;
}

inline double logcdf_power(double x, double alpha, double beta,
                           bool& throw_warning) {
  if (ISNAN(x) || ISNAN(alpha) || ISNAN(beta))
    return x+alpha+beta;
  if (x <= 0.0)
    return R_NegInf;
  if (x >= alpha)
    return 0.0;
  return log(x)*beta - log(alpha)*beta;
}


// [[Rcpp::export]]
NumericVector cpp_dpower(
    const NumericVector& x,
    const NumericVector& alpha,
    const NumericVector& beta,
    const bool& log_prob = false
  ) {
  
  if (std::min({x.length(), alpha.length(), beta.length()}) < 1) {
    return NumericVector(0);
  }

  int Nmax = std::max({
    x.length(),
    alpha.length(),
    beta.length()
  });
  NumericVector p(Nmax);
  
  bool throw_warning = false;

  for (int i = 0; i < Nmax; i++)
    p[i] = logpdf_power(GETV(x, i), GETV(alpha, i),
                        GETV(beta, i), throw_warning);

  if (!log_prob)
    p = Rcpp::exp(p);
  
  if (throw_warning)
    Rcpp::warning("NaNs produced");

  return p;
}


// [[Rcpp::export]]
NumericVector cpp_ppower(
    const NumericVector& x,
    const NumericVector& alpha,
    const NumericVector& beta,
    const bool& lower_tail = true,
    const bool& log_prob = false
  ) {
  
  if (std::min({x.length(), alpha.length(), beta.length()}) < 1) {
    return NumericVector(0);
  }

  int Nmax = std::max({
    x.length(),
    alpha.length(),
    beta.length()
  });
  NumericVector p(Nmax);
  
  bool throw_warning = false;

  for (int i = 0; i < Nmax; i++)
    p[i] = logcdf_power(GETV(x, i), GETV(alpha, i),
                        GETV(beta, i), throw_warning);

  if (!lower_tail)
    p = 1.0 - p;

  if (!log_prob)
    p = Rcpp::exp(p);
  
  if (throw_warning)
    Rcpp::warning("NaNs produced");

  return p;
}


// [[Rcpp::export]]
NumericVector cpp_qpower(
    const NumericVector& p,
    const NumericVector& alpha,
    const NumericVector& beta,
    const bool& lower_tail = true,
    const bool& log_prob = false
  ) {
  
  if (std::min({p.length(), alpha.length(), beta.length()}) < 1) {
    return NumericVector(0);
  }

  int Nmax = std::max({
    p.length(),
    alpha.length(),
    beta.length()
  });
  NumericVector x(Nmax);
  NumericVector pp = Rcpp::clone(p);
  
  bool throw_warning = false;

  if (log_prob)
    pp = Rcpp::exp(pp);

  if (!lower_tail)
    pp = 1.0 - pp;

  for (int i = 0; i < Nmax; i++)
    x[i] = invcdf_power(GETV(pp, i), GETV(alpha, i),
                        GETV(beta, i), throw_warning);
  
  if (throw_warning)
    Rcpp::warning("NaNs produced");

  return x;
}


// [[Rcpp::export]]
NumericVector cpp_rpower(
    const int& n,
    const NumericVector& alpha,
    const NumericVector& beta
  ) {
  
  if (std::min({alpha.length(), beta.length()}) < 1) {
    Rcpp::warning("NAs produced");
    return NumericVector(n, NA_REAL);
  }

  NumericVector x(n);
  
  bool throw_warning = false;

  for (int i = 0; i < n; i++)
    x[i] = rng_power(GETV(alpha, i), GETV(beta, i),
                     throw_warning);
  
  if (throw_warning)
    Rcpp::warning("NAs produced");

  return x;
}


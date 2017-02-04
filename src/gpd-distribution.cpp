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
*  Generalized Pareto distribution
*
*  Values:
*  x
*
*  Parameters:
*  mu
*  sigma > 0
*  xi
*
*  z = (x-mu)/sigma
*  where 1+xi*z > 0
*
*  f(x)    = { (1+xi*z)^{-(xi+1)/xi}/sigma       if xi != 0
*            { exp(-z)/sigma                     otherwise
*  F(x)    = { 1-(1+xi*z)^{-1/xi}                if xi != 0
*            { 1-exp(-z)                         otherwise
*  F^-1(p) = { mu + sigma * ((1-p)^{-xi}-1)/xi   if xi != 0
*            { mu - sigma * log(1-p)             otherwise
*
*/

inline double pdf_gpd(double x, double mu, double sigma, double xi,
                      bool& throw_warning) {
  if (ISNAN(x) || ISNAN(mu) || ISNAN(sigma) || ISNAN(xi))
    return x+mu+sigma+xi;
  if (sigma <= 0.0) {
    throw_warning = true;
    return NAN;
  }
  double z = (x-mu)/sigma;
  if (xi != 0.0) {
    if (x >= mu)
      return pow(1.0+xi*z, -(xi+1.0)/xi)/sigma;
    else
      return 0.0;
  } else {
    if (x >= mu && x <= (mu - sigma/xi))
      return exp(-z)/sigma;
    else
      return 0.0;
  }
}

inline double cdf_gpd(double x, double mu, double sigma, double xi,
                      bool& throw_warning) {
  if (ISNAN(x) || ISNAN(mu) || ISNAN(sigma) || ISNAN(xi))
    return x+mu+sigma+xi;
  if (sigma <= 0.0) {
    throw_warning = true;
    return NAN;
  }
  double z = (x-mu)/sigma;
  if (xi != 0.0) {
    if (x >= mu)
      return 1.0 - pow(1.0+xi*z, -1.0/xi);
    else
      return 0.0;
  } else {
    if (x >= mu && x <= (mu - sigma/xi))
      return 1.0 - exp(-z);
    else
      return 0.0;
  }
}

inline double invcdf_gpd(double p, double mu, double sigma, double xi,
                         bool& throw_warning) {
  if (ISNAN(p) || ISNAN(mu) || ISNAN(sigma) || ISNAN(xi))
    return p+mu+sigma+xi;
  if (sigma <= 0.0 || !VALID_PROB(p)) {
    throw_warning = true;
    return NAN;
  }
  if (xi != 0.0)
    return mu + sigma * (pow(1.0-p, -xi)-1.0)/xi;
  else
    return mu - sigma * log(1.0-p);
}

inline double rng_gpd(double mu, double sigma, double xi,
                      bool& throw_warning) {
  if (ISNAN(mu) || ISNAN(sigma) || ISNAN(xi) || sigma <= 0.0) {
    throw_warning = true;
    return NA_REAL;
  }
  double u = rng_unif();
  if (xi != 0.0)
    return mu + sigma * (pow(u, -xi)-1.0)/xi;
  else
    return mu - sigma * log(u);
}


// [[Rcpp::export]]
NumericVector cpp_dgpd(
    const NumericVector& x,
    const NumericVector& mu,
    const NumericVector& sigma,
    const NumericVector& xi,
    const bool& log_prob = false
  ) {

  int Nmax = std::max({
    x.length(),
    mu.length(),
    sigma.length(),
    xi.length()
  });
  NumericVector p(Nmax);
  
  bool throw_warning = false;

  for (int i = 0; i < Nmax; i++)
    p[i] = pdf_gpd(GETV(x, i), GETV(mu, i),
                   GETV(sigma, i), GETV(xi, i),
                   throw_warning);

  if (log_prob)
    p = Rcpp::log(p);
  
  if (throw_warning)
    Rcpp::warning("NaNs produced");

  return p;
}


// [[Rcpp::export]]
NumericVector cpp_pgpd(
    const NumericVector& x,
    const NumericVector& mu,
    const NumericVector& sigma,
    const NumericVector& xi,
    const bool& lower_tail = true,
    const bool& log_prob = false
  ) {

  int Nmax = std::max({
    x.length(),
    mu.length(),
    sigma.length(),
    xi.length()
  });
  NumericVector p(Nmax);
  
  bool throw_warning = false;

  for (int i = 0; i < Nmax; i++)
    p[i] = cdf_gpd(GETV(x, i), GETV(mu, i),
                   GETV(sigma, i), GETV(xi, i),
                   throw_warning);

  if (!lower_tail)
    p = 1.0 - p;
  
  if (log_prob)
    p = Rcpp::log(p);
  
  if (throw_warning)
    Rcpp::warning("NaNs produced");

  return p;
}


// [[Rcpp::export]]
NumericVector cpp_qgpd(
    const NumericVector& p,
    const NumericVector& mu,
    const NumericVector& sigma,
    const NumericVector& xi,
    const bool& lower_tail = true,
    const bool& log_prob = false
  ) {

  int Nmax = std::max({
    p.length(),
    mu.length(),
    sigma.length(),
    xi.length()
  });
  NumericVector q(Nmax);
  NumericVector pp = Rcpp::clone(p);
  
  bool throw_warning = false;

  if (log_prob)
    pp = Rcpp::exp(pp);
  
  if (!lower_tail)
    pp = 1.0 - pp;

  for (int i = 0; i < Nmax; i++)
    q[i] = invcdf_gpd(GETV(pp, i), GETV(mu, i),
                      GETV(sigma, i), GETV(xi, i),
                      throw_warning);
  
  if (throw_warning)
    Rcpp::warning("NaNs produced");

  return q;
}


// [[Rcpp::export]]
NumericVector cpp_rgpd(
    const int& n,
    const NumericVector& mu,
    const NumericVector& sigma,
    const NumericVector& xi
  ) {

  NumericVector x(n);
  
  bool throw_warning = false;

  for (int i = 0; i < n; i++)
    x[i] = rng_gpd(GETV(mu, i), GETV(sigma, i),
                   GETV(xi, i), throw_warning);
  
  if (throw_warning)
    Rcpp::warning("NAs produced");

  return x;
}


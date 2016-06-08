#include <Rcpp.h>
#include "shared.h"
using namespace Rcpp;

/*
 * Discrete uniform distribution
 * 
 * Values:
 * a <= x <= b
 * 
 * f(x) = 1/(b-a+1)
 * F(x) = (floor(x)-a+1)/b-a+1
 *  
 */


double pmf_dunif(double x, double min, double max) {
  if (min > max || std::isinf(min) || std::isinf(max)) {
    Rcpp::warning("NaNs produced");
    return NAN;
  }
  if (x < min || x > max || !isInteger(x))
    return 0;
  return 1/(max-min+1);
}


double cdf_dunif(double x, double min, double max) {
  if (min > max || std::isinf(min) || std::isinf(max)) {
    Rcpp::warning("NaNs produced");
    return NAN;
  }
  if (x < min)
    return 0;
  else if (x >= max)
    return 1;
  return (floor(x)-min+1)/(max-min+1);
}

double invcdf_dunif(double p, double min, double max) {
  if (min > max || std::isinf(min) || std::isinf(max)) {
    Rcpp::warning("NaNs produced");
    return NAN;
  }
  if (p <= 0 || p > 1) {
    Rcpp::warning("NaNs produced");
    return NAN;
  }
  return ceil( p*(max-min+1)+min-1 );
}

double rng_dunif(double min, double max) {
  if (min > max || std::isinf(min) || std::isinf(max)) {
    Rcpp::warning("NaNs produced");
    return NAN;
  }
  
  if (min > 0)
    min = floor(min) - 1;
  else
    min = ceil(min) - 1;
  
  if (max > 0)
    max = ceil(max);
  else
    max = floor(max);
  
  return ceil(R::runif(min, max));
}


// [[Rcpp::export]]
NumericVector cpp_ddunif(
    const NumericVector& x,
    const NumericVector& min,
    const NumericVector& max,
    bool log_prob = false
  ) {
  
  int n  = x.length();
  int na = min.length();
  int nb = max.length();
  int Nmax = Rcpp::max(IntegerVector::create(n, na, nb));
  NumericVector p(Nmax);
  
  for (int i = 0; i < Nmax; i++)
    p[i] = pmf_dunif(x[i % n], min[i % na], max[i % nb]);
  
  if (log_prob)
    for (int i = 0; i < Nmax; i++)
      p[i] = log(p[i]);
  
  return p;
}


// [[Rcpp::export]]
NumericVector cpp_pdunif(
    const NumericVector& x,
    const NumericVector& min,
    const NumericVector& max,
    bool lower_tail = true, bool log_prob = false
  ) {
  
  int n  = x.length();
  int na = min.length();
  int nb = max.length();
  int Nmax = Rcpp::max(IntegerVector::create(n, na, nb));
  NumericVector p(Nmax);
  
  for (int i = 0; i < Nmax; i++)
    p[i] = cdf_dunif(x[i % n], min[i % na], max[i % nb]);
  
  if (!lower_tail)
    for (int i = 0; i < Nmax; i++)
      p[i] = 1-p[i];
  
  if (log_prob)
    for (int i = 0; i < Nmax; i++)
      p[i] = log(p[i]);
  
  return p;
}


// [[Rcpp::export]]
NumericVector cpp_qdunif(
    const NumericVector& p,
    const NumericVector& min,
    const NumericVector& max,
    bool lower_tail = true, bool log_prob = false
  ) {
  
  int n  = p.length();
  int na = min.length();
  int nb = max.length();
  int Nmax = Rcpp::max(IntegerVector::create(n, na, nb));
  NumericVector q(Nmax);
  NumericVector pp = Rcpp::clone(p);
  
  if (log_prob)
    for (int i = 0; i < n; i++)
      pp[i] = exp(pp[i]);
  
  if (!lower_tail)
    for (int i = 0; i < n; i++)
      pp[i] = 1-pp[i];
  
  for (int i = 0; i < Nmax; i++)
    q[i] = invcdf_dunif(pp[i % n], min[i % na], max[i % nb]);
  
  return q;
}


// [[Rcpp::export]]
NumericVector cpp_rdunif(
    const int n,
    const NumericVector& min,
    const NumericVector& max
  ) {
  
  int na = min.length();
  int nb = max.length();
  NumericVector x(n);
  
  for (int i = 0; i < n; i++)
    x[i] = rng_dunif(min[i % na], max[i % nb]);
  
  return x;
}


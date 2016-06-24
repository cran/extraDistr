#include <Rcpp.h>
#include "const.h"
#include "shared.h"

using std::pow;
using std::sqrt;
using std::abs;
using std::exp;
using std::log;
using std::floor;
using std::ceil;
using std::sin;
using std::cos;
using std::tan;
using std::atan;
using Rcpp::IntegerVector;
using Rcpp::NumericVector;
using Rcpp::NumericMatrix;


/*
*  Beta-binomial distribution
*
*  Values:
*  x
*
*  Parameters:
*  k > 0
*  alpha > 0
*  beta > 0
*
*  f(k) = choose(n, k) * (beta(k+alpha, n-k+beta)) / (beta(alpha, beta))
*
*/

double pmf_bbinom(double k, double n, double alpha, double beta) {
  if (alpha < 0.0 || beta < 0.0 || n < 0.0 || floor(n) != n) {
    Rcpp::warning("NaNs produced");
    return NAN;
  }
  if (!isInteger(k) || k < 0.0 || k > n)
    return 0.0;
  return R::choose(n, k) * R::beta(k+alpha, n-k+beta) / R::beta(alpha, beta);
}

double logpmf_bbinom(double k, double n, double alpha, double beta) {
  if (alpha < 0.0 || beta < 0.0 || n < 0.0 || floor(n) != n) {
    Rcpp::warning("NaNs produced");
    return NAN;
  }
  if (!isInteger(k) || k < 0.0 || k > n)
    return -INFINITY;
  return R::lchoose(n, k) + R::lbeta(k+alpha, n-k+beta) - R::lbeta(alpha, beta);
}

double cdf_bbinom(double k, double n, double alpha, double beta) {
  if (alpha < 0.0 || beta < 0.0 || n < 0.0 || floor(n) != n) {
    Rcpp::warning("NaNs produced");
    return NAN;
  }
  if (!isInteger(k) || k < 0.0)
    return 0.0;
  if (k > n)
    return 1.0;
  double p_tmp = 0.0;
  for (int j = 0; j < static_cast<int>(k)+1; j++)
    p_tmp += exp(logpmf_bbinom(static_cast<double>(j), n, alpha, beta));
  return p_tmp;
}

double rng_bbinom(double n, double alpha, double beta) {
  if (alpha < 0.0 || beta < 0.0 || n < 0.0 || floor(n) != n) {
    Rcpp::warning("NaNs produced");
    return NAN;
  }
  double prob = R::rbeta(alpha, beta);
  return R::rbinom(n, prob);
}


// [[Rcpp::export]]
NumericVector cpp_dbbinom(
    const NumericVector& x,
    const NumericVector& size,
    const NumericVector& alpha,
    const NumericVector& beta,
    bool log_prob = false
  ) {

  int n = x.length();
  int nn = size.length();
  int na = alpha.length();
  int nb = beta.length();
  int Nmax = Rcpp::max(IntegerVector::create(n, nn, na, nb));
  NumericVector p(Nmax);

  for (int i = 0; i < Nmax; i++)
    p[i] = logpmf_bbinom(x[i % n], size[i % nn], alpha[i % na], beta[i % nb]);

  if (!log_prob)
    for (int i = 0; i < Nmax; i++)
      p[i] = exp(p[i]);

  return p;
}


// [[Rcpp::export]]
NumericVector cpp_pbbinom(
    const NumericVector& x,
    const NumericVector& size,
    const NumericVector& alpha,
    const NumericVector& beta,
    bool lower_tail = true, bool log_prob = false
  ) {

  int n = x.length();
  int nn = size.length();
  int na = alpha.length();
  int nb = beta.length();
  int Nmax = Rcpp::max(IntegerVector::create(n, nn, na, nb));
  NumericVector p(Nmax);
  
  if (nn == 1 && na == 1 && nb == 1) {
    
    if (alpha[0] < 0.0 || beta[0] < 0.0 || size[0] < 0.0 || floor(size[0]) != size[0]) {
      Rcpp::warning("NaNs produced");
      for (int i = 0; i < n; i++)
        p[i] = NAN;
      return p;
    }
    
    double mx = finite_max(x);
    mx = static_cast<int>(std::max(mx, size[0]));
    NumericVector p_tab(mx+1);
    
    p_tab[0] = exp(logpmf_bbinom(0.0, size[0], alpha[0], beta[0]));
    for (int j = 1; j < mx+1; j++)
      p_tab[j] = p_tab[j-1] + exp(logpmf_bbinom(static_cast<double>(j),
                                                size[0], alpha[0], beta[0]));
    
    for (int i = 0; i < n; i++) {
      if (x[i] > size[0]) {
        p[i] = 1.0;
      } else if (isInteger(x[i]) && x[i] >= 0.0) {
        p[i] = p_tab[static_cast<int>(x[i])];
      } else {
        p[i] = 0.0;
      }
    }
    
  } else {
    
    for (int i = 0; i < Nmax; i++) {
      if (i % 1000 == 0)
        Rcpp::checkUserInterrupt();
      p[i] = cdf_bbinom(x[i % n], size[i % nn], alpha[i % na], beta[i % nb]);
    }
  
  }

  if (!lower_tail)
    for (int i = 0; i < Nmax; i++)
      p[i] = 1.0 - p[i];

  if (log_prob)
    for (int i = 0; i < Nmax; i++)
      p[i] = log(p[i]);

  return p;
}


// [[Rcpp::export]]
NumericVector cpp_rbbinom(
    const int n,
    const NumericVector& size,
    const NumericVector& alpha,
    const NumericVector& beta
  ) {

  int nn = size.length();
  int na = alpha.length();
  int nb = beta.length();
  NumericVector x(n);

  for (int i = 0; i < n; i++)
    x[i] = rng_bbinom(size[i % nn], alpha[i % na], beta[i % nb]);

  return x;
}


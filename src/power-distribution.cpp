#include <Rcpp.h>
using namespace Rcpp;


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

double pdf_power(double x, double alpha, double beta) {
  if (x < 0)
    return 0;
  if (x > 0 && x < alpha) {
    return beta * pow(x, beta-1) / pow(alpha, beta);
  } else {
    return 0;
  }
}

double cdf_power(double x, double alpha, double beta) {
  if (x < 0)
    return 0;
  if (x > 0 && x < alpha) {
    return pow(x, beta) / pow(alpha, beta);
  } else if (x >= alpha) {
    return 1;
  } else {
    return 0;
  }
}

double invcdf_power(double p, double alpha, double beta) {
  if (p < 0 || p > 1) {
    Rcpp::warning("NaNs produced");
    return NAN;
  }
  return alpha * pow(p, 1/beta);
}

double logpdf_power(double x, double alpha, double beta) {
  if (x < 0)
    return -INFINITY;
  if (x > 0 && x < alpha) {
    return log(beta) + log(x)*(beta-1) - log(alpha)*beta;
  } else {
    return -INFINITY;
  }
}

double logcdf_power(double x, double alpha, double beta) {
  if (x < 0)
    return -INFINITY;
  if (x > 0 && x < alpha) {
    return log(x)*beta - log(alpha)*beta;
  } else if (x >= alpha) {
    return 0;
  } else {
    return -INFINITY;
  }
}


// [[Rcpp::export]]
NumericVector cpp_dpower(
    const NumericVector& x,
    const NumericVector& alpha,
    const NumericVector& beta,
    bool log_prob = false
  ) {

  int n  = x.length();
  int na = alpha.length();
  int nb = beta.length();
  int Nmax = Rcpp::max(IntegerVector::create(n, na, nb));
  NumericVector p(Nmax);

  for (int i = 0; i < Nmax; i++)
    p[i] = logpdf_power(x[i % n], alpha[i % na], beta[i % nb]);

  if (!log_prob)
    for (int i = 0; i < Nmax; i++)
      p[i] = exp(p[i]);

  return p;
}


// [[Rcpp::export]]
NumericVector cpp_ppower(
    const NumericVector& x,
    const NumericVector& alpha,
    const NumericVector& beta,
    bool lower_tail = true, bool log_prob = false
  ) {

  int n  = x.length();
  int na = alpha.length();
  int nb = beta.length();
  int Nmax = Rcpp::max(IntegerVector::create(n, na, nb));
  NumericVector p(Nmax);

  for (int i = 0; i < Nmax; i++)
    p[i] = logcdf_power(x[i % n], alpha[i % na], beta[i % nb]);

  if (!lower_tail)
    for (int i = 0; i < Nmax; i++)
      p[i] = 1-p[i];

  if (!log_prob)
    for (int i = 0; i < Nmax; i++)
      p[i] = exp(p[i]);

  return p;
}


// [[Rcpp::export]]
NumericVector cpp_qpower(
    const NumericVector& p,
    const NumericVector& alpha,
    const NumericVector& beta,
    bool lower_tail = true, bool log_prob = false
  ) {

  int n  = p.length();
  int na = alpha.length();
  int nb = beta.length();
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
    q[i] = invcdf_power(pp[i % n], alpha[i % na], beta[i % nb]);

  return q;
}


// [[Rcpp::export]]
NumericVector cpp_rpower(
    const int n,
    const NumericVector& alpha,
    const NumericVector& beta
  ) {

  double u;
  int na = alpha.length();
  int nb = beta.length();
  NumericVector x(n);

  for (int i = 0; i < n; i++) {
    u = R::runif(0, 1);
    x[i] = invcdf_power(u, alpha[i % na], beta[i % nb]);
  }

  return x;
}

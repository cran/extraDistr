#include <Rcpp.h>
using namespace Rcpp;


/*
 *  Pareto distribution
 *
 *  Values:
 *  x
 *
 *  Parameters:
 *  a, b > 0
 *
 *  f(x)    = (a*b^a) / x^{a+1}
 *  F(x)    = 1 - (b/x)^a
 *  F^-1(p) = b/(1-p)^{1-a}
 *
 */

double pdf_pareto(double x, double a, double b) {
  if (a <= 0 || b <= 0) {
    Rcpp::warning("NaNs produced");
    return NAN;
  }
  if (x < b)
    return 0;
  return a * pow(b, a) / pow(x, a+1);
}

double logpdf_pareto(double x, double a, double b) {
  if (a <= 0 || b <= 0) {
    Rcpp::warning("NaNs produced");
    return NAN;
  }
  if (x < b)
    return -INFINITY;
  return log(a) + log(b)*a - log(x)*(a+1);
}

double cdf_pareto(double x, double a, double b) {
  if (a <= 0 || b <= 0) {
    Rcpp::warning("NaNs produced");
    return NAN;
  }
  if (x < b)
    return 0;
  return 1 - pow(b/x, a);
}

double invcdf_pareto(double p, double a, double b) {
  if (a <= 0 || b <= 0 || p < 0 || p > 1) {
    Rcpp::warning("NaNs produced");
    return NAN;
  }
  return b / pow(1-p, 1/a);
}

double invcdf_pareto2(double p, double a, double b) {
  if (a <= 0 || b <= 0) {
    Rcpp::warning("NaNs produced");
    return NAN;
  }
  return exp(log(b) - log(1-p)*(1/a));
}


// [[Rcpp::export]]
NumericVector cpp_dpareto(
    const NumericVector& x,
    const NumericVector& a,
    const NumericVector& b,
    bool log_prob = false
  ) {

  int n = x.length();
  int na = a.length();
  int nb = b.length();
  int Nmax = Rcpp::max(IntegerVector::create(n, na, nb));
  NumericVector p(Nmax);

  for (int i = 0; i < Nmax; i++)
    p[i] = logpdf_pareto(x[i % n], a[i % na], b[i % nb]);

  if (!log_prob)
    for (int i = 0; i < Nmax; i++)
      p[i] = exp(p[i]);

  return p;
}


// [[Rcpp::export]]
NumericVector cpp_ppareto(
    const NumericVector& x,
    const NumericVector& a,
    const NumericVector& b,
    bool lower_tail = true, bool log_prob = false
  ) {

  int n  = x.length();
  int na = a.length();
  int nb = b.length();
  int Nmax = Rcpp::max(IntegerVector::create(n, na, nb));
  NumericVector p(Nmax);

  for (int i = 0; i < Nmax; i++)
    p[i] = cdf_pareto(x[i % n], a[i % na], b[i % nb]);

  if (!lower_tail)
    for (int i = 0; i < Nmax; i++)
      p[i] = 1-p[i];

  if (log_prob)
    for (int i = 0; i < Nmax; i++)
      p[i] = log(p[i]);

  return p;
}


// [[Rcpp::export]]
NumericVector cpp_qpareto(
    const NumericVector& p,
    const NumericVector& a,
    const NumericVector& b,
    bool lower_tail = true, bool log_prob = false
  ) {

  int n  = p.length();
  int na = a.length();
  int nb = b.length();
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
    q[i] = invcdf_pareto(pp[i % n], a[i % na], b[i % nb]);

  return q;
}


// [[Rcpp::export]]
NumericVector cpp_rpareto(
    const int n,
    const NumericVector& a,
    const NumericVector& b
  ) {

  double u;
  int na = a.length();
  int nb = b.length();
  NumericVector x(n);

  for (int i = 0; i < n; i++) {
    u = R::runif(0, 1);
    x[i] = invcdf_pareto(u, a[i % na], b[i % nb]);
  }

  return x;
}

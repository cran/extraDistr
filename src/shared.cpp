#include <Rcpp.h>
#include "const.h"


// Basic functions

bool tol_equal(double x, double y) {
  return std::abs(x - y) <= MIN_DIFF_EPS;
}

bool isInteger(double x) {
  if (std::floor(x) != x) {
    char msg[55];
    std::snprintf(msg, sizeof(msg), "non-integer x = %f", x);
    Rcpp::warning(msg);
    return false;
  }
  return true;
}

// Dealing with Inf

bool anyFinite(Rcpp::NumericVector x) {
  int n = x.length();
  for (int i = 0; i < n; i++)
    if (!std::isinf(x[i]))
      return true;
  return false;
}

double finite_max(Rcpp::NumericVector x) {
  double max_x = -INFINITY;
  int n = x.length();
  for (int i = 0; i < n; i++) {
    if (!std::isinf(x[i]) && x[i] > max_x)
      max_x = x[i];
  }
  return max_x;
}

// Standard normal

double phi(double x) {
  return R::dnorm(x, 0.0, 1.0, false);
}

double Phi(double x) {
  return R::pnorm(x, 0.0, 1.0, true, false);
}

double InvPhi(double x) {
  return R::qnorm(x, 0.0, 1.0, true, false);
}

// Factorial

double factorial(double x) {
  return R::gammafn(x + 1.0);
}

double lfactorial(double x) {
  return R::lgammafn(x + 1.0);
}

// Random generation for Bernoulli

double rng_bernoulli(double p = 0.5) {
  if (p < 0.0 || p > 1.0) {
    Rcpp::warning("NaNs produced");
    return NAN;
  }
  double u = R::runif(0.0, 1.0);
  if (u <= 1.0 - p)
    return 0.0;
  else
    return 1.0;
}
 
 

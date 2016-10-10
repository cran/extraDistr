#ifndef EDCPP_SHARED_H
#define EDCPP_SHARED_H


// Basic functions

bool tol_equal(double x, double y);
bool isInteger(double x);

// Dealing with Inf and NAs

bool anyFinite(Rcpp::NumericVector x);
double finite_max(Rcpp::NumericVector x);
bool allNA(Rcpp::NumericVector x);

// Standard normal

double phi(double x);
double Phi(double x);
double InvPhi(double x);

// Factorial

double factorial(double x);
double lfactorial(double x);

// Random generation

double rng_unif();         // standard uniform
double rng_bern(double p); // Bernoulli distribution
double rng_sign();         // Rademacher distribution

// Checking parameters

Rcpp::NumericMatrix normalize_prob(const Rcpp::NumericMatrix& prob);

#endif


### 1.7.0

* Cleaning namespace - now mathematical functions are explicitely
  called from std library
* "using namespace Rcpp" was removed from all the files
* All the numerical values are now explicitely `<double>`'s,
  or casted to `<double>`; or `<int>`'s (for indexing)
* Improvements in algorithms for discrete uniform, categorical,
  mixture of normal, mixture of Poisson distributions
* Improvements in discrete uniform; now it accepts only integer
  valued parameters.
* Major C++ code clean-up

### 1.6.14

* Removed `erf`, `erfc`, `inv_erf` that are not used at this moment and
  caused problems when compiling on Fedora and Solaris

### 1.6.13

* cran-comments.Rd is now in .Rbuildignore

### 1.6.12

* Minor improvements in documentation

### 1.6.11

* Added mixtures of normal and of Poisson distributions

### 1.6.10

* Added truncated Poisson distribution

### 1.6.9

* Minor bug fixes
* Minor improvements in documention
* Additional tests
* Truncated normal returns normal for infinite truncation points

### 1.6.8

* Added half-t, half-normal and half-Cauchy distributions
* Minor changes in documentation
* Changed naming of data-variable from `x` to `q` for CDF's
* Added inverse-CDF for discrete normal distribution, fixed 
  random generation

### 1.6.7

* Improvements in the C++ code

### 1.6.6

* Improvements in documentation
* Added tests
* Multiple minor bug fixes (e.g. functions returning `NaN`
  instead of `0` for `Inf` values)

### 1.6.5

* Added Birnbaum-Saunders (fatigue life) distribution

### 1.6.4

* New algorithm for `rtriang`
* Minor changes in documentation

### 1.6.3

* Added quantile functions for: zero-inflated Poisson, zero-inflated binomial
  zero-inflated negative binomial, inverse gamma, inverse chi-squared
  distributions
* Added Huber density

### 1.6.2

* Minor improvements in the code

### 1.6.1

* Exception handling in discrete uniform distribution functions
* Bug fixes in `rdunif`
* Clean-up in documentation and examples
* Now `dbvnorm` and `dbvpois` work with matrixes
* Renaming of parameters in bivariate normal to be consistent with
  base R normal distribution

### 1.6.0

* Warning messages for non-integer values in discrete distributions
* Warning when returning NANs
* Code clean-up in `pbbinom` and `pbnbinom`
* Clean-up in documentation

### 1.5.19

* Ranamed zero-inflated distributions to *`zip` and *`zinb`
* Added zero-inflated binomial *`zib`

### 1.5.18

* Code clean-up
* Added `pzipois` and `pzibinom`

### 1.5.17

* Added `qlgser`; changes in `plgser` and `rlgser`
* Fixed bug in `rlgser`

### 1.5.16

* Examples for most of the functions

### 1.5.15

* Exception handling for `dmvhyper` and `rmvhyper`: values of `x`, `n`,
  and `k` are checked against being non-integers

### 1.5.14

* Added README and NEWS files


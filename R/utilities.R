#' Poset incidence matrix
#' 
#' @description The function returns the incidence matrix for objects of class \code{\link[POSetR:poset]{Rcpp_POSet}}.
#'
#' @param x an object of class \code{\link[POSetR:poset]{Rcpp_POSet}}.
#'
#' @return a logical square matrix.
#' 
#' @aliases incidenceMatrix incidenceMatrix.Rcpp_POSet
#'
#' @examples
#' dom <- matrix(c(
#'   "a", "b",
#'   "c", "b",
#'   "b", "d"
#' ), ncol = 2, byrow = TRUE)
#' p <- poset(x = dom)
#' incidenceMatrix(p)

#' @export
incidenceMatrix <- function(x) {
  UseMethod("incidenceMatrix")
}

#' @method incidenceMatrix Rcpp_POSet
#' @export
incidenceMatrix.Rcpp_POSet <- function(x) {
  return(x$incidenceMatrix())
}

########################

#' Poset cover matrix
#' 
#' @description The function returns the cover matrix for objects of class \code{\link[POSetR:poset]{Rcpp_POSet}}.
#'
#' @param x an object of class \code{\link[POSetR:poset]{Rcpp_POSet}}.
#'
#' @return a logical square matrix.
#' 
#' @aliases coverMatrix coverMatrix.Rcpp_POSet
#'
#' @examples
#' dom <- matrix(c(
#'   "a", "b",
#'   "c", "b",
#'   "b", "d"
#' ), ncol = 2, byrow = TRUE)
#' p <- poset(x = dom)
#' coverMatrix(p)

#' @export
coverMatrix <- function(x) {
  UseMethod("coverMatrix")
}

#' @method coverMatrix Rcpp_POSet
#' @export
coverMatrix.Rcpp_POSet <- function(x) {
  return(x$coverMatrix())
}
#' Product order between two posets
#' 
#' @description The function returns the product poset of two posets \code{X} and \code{Y}.
#'
#' @param x an S4 object of class \code{Rcpp_POSet}, see \code{\link[POSetR]{poset}} for details.
#' @param y an S4 object of class \code{Rcpp_POSet}, see \code{\link[POSetR]{poset}} for details.
#' @param sep a character object indicating the separator to be used to paste profiles names.
#'
#' @usage productOrder(x, y, sep = "-")
#' @usage x \%po\% y
#' @aliases productOrder %po%
#' @return an S4 object of class \code{Rcpp_POSet}, see \code{\link[POSetR]{poset}} for details
#' @seealso \code{\link[POSetR]{poset}}
#' @export
#'
#' @examples
#' dom <- matrix(c(
#'   "a", "b",
#'   "c", "b",
#'   "b", "d"
#' ), ncol = 2, byrow = TRUE)
#' p <- poset(x = dom)
#' q <- chain(1:3)
#' plot(p %po% q)
#' 
#' @references
#' \insertRef{davey2002introduction}{POSetR}


productOrder <- function(x, y, sep = "-") {
  stopifnot(class(x) == "Rcpp_POSet")
  stopifnot(class(y) == "Rcpp_POSet")
  
  res <- new(POSet, x$elements(), x$comparabilities(), y$elements(), y$comparabilities(), sep)
  
  return(res)
}

#' @export
"%po%" <- function(x, y) productOrder(x, y)

#' Intersection of two posets
#' 
#' @description Given two posets \code{X} and \code{Y} on the same set, \code{intersection} returns the poset Z defined by \code{a < b} in \code{Z} if and only if \code{a < b} in \code{X} and \code{a < b} in \code{Y}.
#'
#' @param x an S4 object of class \code{Rcpp_POSet}, see \code{\link[POSetR]{poset}} for details.
#' @param y an S4 object of class \code{Rcpp_POSet}, see \code{\link[POSetR]{poset}} for details.
#'
#' @usage intersection(x, y)
#' @usage x \%it\% y
#' @aliases intersection %it%
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
#' plot(p)
#' dom <- matrix(c(
#'   "a", "b",
#'   "c", "b",
#'   "d", "b"
#' ), ncol = 2, byrow = TRUE)
#' q <- poset(x = dom)
#' plot(q)
#' plot(p %it% q)
#' 
#' @references
#' \insertRef{davey2002introduction}{POSetR}

intersection <- function(x, y) {
  stopifnot(all(x$pointer$elements() %in% y$pointer$elements()))
  stopifnot(all(y$pointer$elements() %in% x$pointer$elements()))
  
  cmp <- rbind(x$pointer$comparabilities(), y$pointer$comparabilities())
  cmp <- cmp[duplicated(cmp), , drop = FALSE]
  
  poset(cmp, x$pointer$elements())
}

#' @export
"%it%" <- intersection
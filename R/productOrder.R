#' Product order between two posets
#' 
#' @description The function returns the product poset of two posets \code{X} and \code{Y}.
#'
#' @param x an S4 object of class \code{poset}, see \code{\link[POSetR]{poset}} for details.
#' @param y an S4 object of class \code{poset}, see \code{\link[POSetR]{poset}} for details.
#' @param sep a character object indicating the separator to be used to paste profiles names.
#'
#' @usage productOrder(x, y, sep = "-")
#' @usage x \%po\% y
#' @aliases productOrder %po%
#' @return an environment of class \code{poset}, see \code{\link[POSetR]{poset}} for details
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
  stopifnot(class(x) == "poset")
  stopifnot(class(y) == "poset")
  
  res <- new.env()
  res$pointer <- new(POSet, x$pointer$elements(), x$pointer$comparabilities(), y$pointer$elements(), y$pointer$comparabilities(), sep)
  
  res$elements <- res$pointer$elements()
  res$comparabilities <- res$pointer$comparabilities()
  
  class(res) <- "poset"
  
  return(res)
}

#' @export
"%po%" <- function(x, y) productOrder(x, y)

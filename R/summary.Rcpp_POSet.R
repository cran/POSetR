#' Poset summary
#' 
#' @description Method of the \code{\link[base]{summary}} function for objects of class \code{\link[POSetR:poset]{Rcpp_POSet}}.
#'
#' @param object an object of class \code{\link[POSetR:poset]{Rcpp_POSet}}.
#' @param ... further arguments passed to or from other methods.
#'
#' @return An S3 object of class \code{summary_poset} listing and counting the poset elements, their strict dominances and their incomparabilities.
#' 
#' @aliases summary.Rcpp_POSet
#' 
#' @export
#'
#' @examples
#' dom <- matrix(c(
#'   "a", "b",
#'   "c", "b",
#'   "b", "d"
#' ), ncol = 2, byrow = TRUE)
#' p <- poset(x = dom)
#' summary(p)
#' 
#' summary(chain(1:4))
#' summary(antiChain(LETTERS[1:5]))

summary.Rcpp_POSet <- function(object, ...) {
  
  res <- list(
    elements = object$elements(),
    comparabilities = object$comparabilities(),
    incomparabilities = object$incomparabilities()
  )
  
  res$elements_number <- length(res$elements)
  res$comparabilities_number <- nrow(res$comparabilities)
  res$incomparabilities_number <- nrow(res$incomparabilities)
  
  class(res) <- "summary_poset"
  
  return(res)
}
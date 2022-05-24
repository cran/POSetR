#' Poset summary
#' 
#' @description Method of the \code{\link[base]{summary}} function for objects of class \code{\link[POSetR:poset]{poset}}.
#'
#' @param object an object of class \code{\link[POSetR:poset]{poset}}.
#' @param ... further arguments passed to or from other methods.
#'
#' @return An S3 object of class \code{summary_poset} listing and counting the poset elements, their strict dominances and their incomparabilities.
#' 
#' @aliases summary.poset
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

summary.poset <- function(object, ...) {
  
  pointerRebuild(object)
  
  res <- list(
    elements = object$pointer$elements(),
    comparabilities = object$pointer$comparabilities(),
    incomparabilities = object$pointer$incomparabilities()
  )
  
  res$elements_number <- length(res$elements)
  res$comparabilities_number <- nrow(res$comparabilities)
  # res$comparabilities_number <- 2 * res$comparabilities_number + res$elements_number
  res$incomparabilities_number <- nrow(res$incomparabilities)
  
  class(res) <- "summary_poset"
  
  return(res)
}
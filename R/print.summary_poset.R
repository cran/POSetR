#' Method for the print function that shows the poset summary
#' @description \code{print} prints the poset summary.
#' @param x an object of class \code{summary_poset}.
#' @param ... further arguments passed to or from other methods.
#' @return nothing
#' @export
#' @aliases print.summary_poset
#' @seealso \code{\link[POSetR]{summary.poset}}
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

#' @export
print.summary_poset <- function(x, ...) {
  if (x$comparabilities_number == 0) {
    if (x$elements_number > 1) cat("anti-chain of", x$elements_number, "elements\n")
  } else if (x$comparabilities_number == x$elements_number^2) # * (x$elements_number - 1)/2)
    cat("chain of", x$elements_number, "elements\n")
  else cat(
    x$elements_number, " elements\n",
    x$comparabilities_number, " comparabilities\n",
    nrow(x$incomparabilities), " incomparabilities\n",
    sep = ""
  )
}
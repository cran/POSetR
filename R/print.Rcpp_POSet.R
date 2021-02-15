#' Method for the print function that shows the poset elements and comparabilities
#' @description \code{print} prints the list of poset elements and all of the strict dominances in it.
#' @param x an object of class \code{Rcpp_POSet}.
#' @param max a non-null value for max specifies the approximate maximum number of entries to be printed. The default, NULL, uses \code{\link{getOption}("max.print")}: see that help page for more details.
#' @param ... further arguments passed to or from other methods.
#' @return nothing
#' @export
#' @examples
#' dom <- matrix(c(
#'   "a", "b",
#'   "c", "b",
#'   "b", "d"
#' ), ncol = 2, byrow = TRUE)
#' p <- poset(x = dom)
#' print(p)

print.Rcpp_POSet <- function(x, max = NULL, ...) {
  e <- x$elements()
  n <- length(e)
  if (is.null(max))
    max <- getOption("max.print")
  m <- min(n, max)
  cat("elements:\n")
  cat(e[1:m])
  if (n > m)
    cat(" ...")
  cat("\n")
  rm(e)
  x <- x$comparabilities()
  n <- nrow(x)
  if (n > 0) {
    m <- min(n, max)
    cat("\nstrict comparabilities:\n")
    apply(x[1:m, , drop = FALSE], 1, function(y) cat(y[1], "<", y[2], "\n"))
    if (n > m)
      cat("...\n")
  } else {
    if (length(e) > 1) cat("\nthe poset is an anti-chain")
  }
}
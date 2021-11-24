#' Generates a Partially Ordered SET from a data frame through the component-wise comparability relation
#' @description This function generates a poset from an object \code{x} of class \code{data.frame} that lists by rows all the observations in a statistical population and by columns the values of different orderable variables (i.e. vectors of class \code{ordered} or \code{numeric}).
#' 
#' @param x an object of class \code{data.frame}.
#' @param collapse an object of class \code{character} that defines the separator between observations with the same profile, by default \code{"-"}.  For more information see the details section.
#' @return an S4 object of class \code{Rcpp_POSet}; this class contains different C++ methods used by other functions of the package.
#' 
#' @details
#' The poset is generated through the component-wise comparability relation \insertCite{davey2002introduction}{POSetR}. Note that some observations may have the same profile (i.e. the same values for all the variables) therefore they have to be included into the same element of the poset, than the label of such element correspond to the labels of the statistical units separated by the \code{collapse} character.
#'
#' @aliases poset_from_dataframe
#' 
#' @export
#'
#' @references
#' \insertRef{davey2002introduction}{POSetR}
#' 
#' @seealso \code{\link[POSetR]{poset}}.
#' @examples
#' set <- ordered(letters[1:5])
#' set.seed(0)
#' n <- 30
#' x <- data.frame(
#'   A = sample(set, n, replace = TRUE),
#'   B = sample(set, n, replace = TRUE),
#'   C = sample(set, n, replace = TRUE)
#' )
#' 
#' p <- poset_from_dataframe(x, collapse = ",")
#' 
#' plot(p, equispaced = TRUE, vertex.frame.color = "lightgray", vertex.color = "lightgray")

poset_from_dataframe <- function(x, collapse = "-") {
  if (!is.matrix(x) & !is.data.frame(x))
    stop("x is neither a dataframe or a matrix")
  if (is.null(rownames(x)))
    rownames(x) <- paste0("E", 1:nrow(x))
  x <- aggregate(rownames(x), by = x, FUN = paste, collapse = collapse)
  k <- ncol(x)
  e <- x[, k]
  x <- x[, -k]
  rownames(x) <- e
  cmp <- matrix(NA, 0, 2)
  n <- length(e)
  for (i in 1:(n - 1)) for (j in (i + 1):n) {
    if (all(x[i, ] <= x[j, ]))
      cmp <- rbind(cmp, c(e[i], e[j]))
    else if (all(x[j, ] <= x[i, ]))
        cmp <- rbind(cmp, c(e[j], e[i]))
  }
  return(poset(cmp))
}
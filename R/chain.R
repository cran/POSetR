#' Generate a complete order from a vector of elements' labels
#' @description The function produces a complete order from the vector of elements' labels \code{elements}.
#' If \code{elements} is a numeric vector, it is first coerced to a character vector.
#' 
#' A complete order is a poset where all of the elements are comparable; the order of the labels in \code{elements} defines their position in the chain, from bottom to top.
#'
#' @param elements a vector of characters listing the elements' labels.
#'
#' @return an S4 object of class \code{Rcpp_POSet}.
#' 
#' @export
#'
#' @seealso function \code{\link[POSetR]{poset}} for more details and to create a generic poset and function \code{\link[POSetR]{antiChain}} to create an anti-chain.
#' 
#' @examples
#' chain(LETTERS[1:5])

chain <- function(elements) {
  stopifnot(is.vector(elements))
  stopifnot(length(elements) > 1)
  n <- length(elements)
  return(poset(cbind(elements[1:(n-1)], elements[2:n])))
}
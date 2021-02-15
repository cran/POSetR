#' Generate an anti-chain from a vector of elements' labels
#' @description The function produces an antichain from the vector of elements' labels \code{elements}.
#' If \code{elements} is a numeric vector, it is first coerced to a character vector.
#' 
#' An antichain is a poset with no comparabilities, therefore the order of the labels in
#' \code{elements} does not matter.
#'
#' @param elements a vector of characters with elements' labels.
#'
#' @return an S4 object of class \code{Rcpp_POSet}.
#' 
#' @export
#'
#' @seealso function \code{\link[POSetR]{poset}} for more details and to create a generic poset and function \code{\link[POSetR]{chain}} to create a complete order.
#' 
#' @examples
#' antiChain(LETTERS[1:5])

antiChain <- function(elements) {
  return(poset(elements = elements))
}
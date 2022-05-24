#' Generates a Partially Ordered SET from the list of dominances
#' @description Function \code{poset} creates a poset from a dominance list. \code{x} argument is a two-column matrix, each row defines a pair of comparable elements, where the element in the first column is dominated by or coincide with the element in the second column. If the elements of \code{x} are numeric, they are first coerced to character and used as elements labels. 
#' @param x an object of class \code{matrix} with two columns, listing the dominances, by rows.
#' @param elements a vector of characters listing all the labels of the elements.
#' @return an environment of class \code{poset}; this class contains a pointer with different C++ methods used by other functions of the package.
#' @details A partial order relation is reflexive, transitive and anti-symmetric. Given the dominance list provided by the user, the function produces the smallest poset comprising them (reflexive and transitive closure); in case the dominances provided by the user imply non-trivial cicles, violating anti-simmetry, the function returns an error.
#' 
#' By default \code{elements} is equal to all the different labels available in \code{x}.
#' If some elements are incomparable, list all of the elements in \code{elements} or include self-comparabilities in \code{x}. Notice that antichains can be created in a simpler way, by function
#' \code{\link[POSetR]{antiChain}}.
#'
#' @aliases POSet poset POSetR Rcpp_POSet-class C++Object-class poset-class pointerRebuild
#' 
#' @export
#' 
#' @seealso in the package are available functions that simply the creation of particular posets: \code{\link[POSetR]{antiChain}} to create a poset without comparabilities,
#' \code{\link[POSetR]{chain}} to create a complete order,
#' \code{\link[POSetR]{poset_from_dataframe}} to create a poset from a statistical population.
#' @examples
#' dom <- matrix(c(
#'   "a", "b",
#'   "c", "b",
#'   "b", "d"
#' ), ncol = 2, byrow = TRUE)
#' poset(x = dom)
#' poset(x = dom, elements = letters[1:5])

poset <- function(x = NULL, elements = unique(as.character(x))) {
  
  stopifnot(!is.null(elements))
  
  res <- new.env()
  res$elements <- as.character(elements)
  res$comparabilities <- matrix(as.character(x), ncol = 2)
  
  res$pointer <- new(POSet, res$elements, res$comparabilities)
  
  class(res) <- "poset"
  
  return(res)
}

#' @keywords internal
#' @export
pointerRebuild <- function(x) {
  
  stopifnot(class(x) == "poset")
  tryCatch(
    x$pointer$testPointer(),
    error = function(e) {
      x$pointer <- new(POSet, x$elements, x$comparabilities)
    }
  )
  
}
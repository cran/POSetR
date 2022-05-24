#' Generates a Partially Ordered SET from an incidence matrix
#' @description This function generates a poset from an incidence matrix \code{x}. Such matrix is a named and \code{logical} that is \code{TRUE} if the row element is dominated by the column one, \code{FALSE} otherwise. Such incidence relation should be reflexive, anti-symmetric, and transitive..
#' 
#' @param x a \code{logical} matrix.
#' @return an environment of class \code{poset}.
#' 
#' @aliases poset_from_incidence
#' 
#' @export
#'
#' @references
#' \insertRef{davey2002introduction}{POSetR}
#' 
#' @seealso \code{\link[POSetR]{poset}}.
#' @examples
#' x <- c(
#'   TRUE, FALSE, FALSE, FALSE, FALSE, FALSE,
#'   TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE,
#'   TRUE,  TRUE,  TRUE, FALSE, FALSE, FALSE,
#'   TRUE, FALSE, FALSE,  TRUE, FALSE, FALSE,
#'   TRUE,  TRUE, FALSE,  TRUE,  TRUE, FALSE,
#'   TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE
#' )
#' x <- matrix(x, nrow = 6, ncol = 6)
#' rownames(x) <- colnames(x) <- LETTERS[1:6]
#' p <- poset_from_incidence(x)
#' plot(p)

poset_from_incidence <- function(x) {
  g <- igraph::graph_from_incidence_matrix(x, directed = TRUE, mode = "out")
  if (!igraph::is.dag(g))
    stop("the list of comparabilities does not return a poset")
  p <- igraph::as_edgelist(g)
  return(poset(p))
}
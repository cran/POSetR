#' Generates a Partially Ordered SET from a Directed Acyclic Graph (DAG)
#' @description This function generates a poset from an object\code{g}. The function checks if the graph is directed and cyclic in oreder it represent the cover-relation.
#' 
#' @param g an \code{igraph} object
#' @return an S4 object of class \code{Rcpp_POSet}; this class contains different C++ methods used by other functions of the package.
#' 
#' @details Note that the Hasse diagram represents the cover relation from the top to the bottom, therefore its representation is the opposite of the plot of the graph with Sugiyama layout.
#' 
#' @aliases poset_from_igraph
#' 
#' @export
#'
#' @references
#' \insertRef{davey2002introduction}{POSetR}
#' 
#' @seealso \code{\link[POSetR]{poset}}.
#' @examples
#' library(igraph)
#' g <- make_tree(10)
#' ly <- igraph::layout_with_sugiyama(g)$layout
#' g$layout <- ly
#' plot(g)
#' p <- poset_from_igraph(g)
#' plot(p)

poset_from_igraph <- function(g) {
  if (class(g) != "igraph")
    stop("g is not an igraph object")
  if (!igraph::is.dag(g))
    stop("g is not a directed aciclic graph")
  p <- igraph::as_edgelist(g)
  return(poset(p))
}
#' Plotting the Hasse diagram of a poset
#' 
#' @description \code{plot} produces an \code{\link[igraph:make_graph]{igraph}} object and shows the Hasse diagram.
#' @details \code{plot.Rcpp_POSet} computes the cover relation and produces the corresponding Directed Acyclic Graph (DAG), as an \code{\link[igraph:make_graph]{igraph}} object, returned as invisible output.
#' Function \code{\link[igraph]{layout_with_sugiyama}} generates the DAG layout with edges oriented from top to bottom. When \code{equispaced=TRUE}, nodes on the same Hasse diagram level are horizontally equispaced.
#' 
#' The Hasse diagram is displayed by a call to \code{\link[igraph]{plot.igraph}} (some default argument values are set to get a cleaner plot, by exploiting Hasse diagram properties.
#' 
#' Setting \code{show = FALSE} produces the \code{\link[igraph:make_graph]{igraph}} object, without showing the Hasse diagram.
#' 
#' Note that
#'
#' @param x an S4 object of class \code{Rcpp_POSet}, see \code{\link[POSetR]{poset}} for details.
#' @param vertex.color argument of the \code{plot.igraph} function, see \code{\link[igraph:plot.common]{igraph.plotting}} for details.
#' @param vertex.label argument of the \code{plot.igraph} function, see \code{\link[igraph:plot.common]{igraph.plotting}} for details.
#' @param vertex.label.color argument of the \code{plot.igraph} function, see \code{\link[igraph:plot.common]{igraph.plotting}} for details.
#' @param vertex.label.family argument of the \code{plot.igraph} function, see \code{\link[igraph:plot.common]{igraph.plotting}} for details.
#' @param edge.color argument of the \code{plot.igraph} function, see \code{\link[igraph:plot.common]{igraph.plotting}} for details.
#' @param edge.label argument of the \code{plot.igraph} function, see \code{\link[igraph:plot.common]{igraph.plotting}} for details.
#' @param edge.arrow.mode argument of the \code{plot.igraph} function, see \code{\link[igraph:plot.common]{igraph.plotting}} for details.
#' @param asp argument of the \code{plot.igraph} function, see \code{\link[igraph:plot.common]{igraph.plotting}} for details.
#' @param ... additional plotting parameters, see \code{\link[igraph:plot.common]{igraph.plotting}} for details.
#' @param equispaced logical, if \code{TRUE} the nodes on the same level of the Hasse diagram are horizontally equispaced.
#' @param show logical, if \code{TRUE} (default) the Hasse diagram is plotted.
#'
#' @return an \code{\link[igraph:make_graph]{igraph}} object.
#' 
#' @seealso \code{\link[POSetR]{poset}}, \code{\link[igraph:make_graph]{igraph}}, \code{\link[igraph:plot.common]{igraph.plotting}}
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
#' hasse <- plot(p)
#' class(hasse)

plot.Rcpp_POSet <- function(x,
  vertex.color = rgb(1, 1, 1, 1),
  vertex.label = x$elements(),
  vertex.label.color = rgb(0, 0, 0, 1),
  vertex.label.family = "sans",
  edge.color = rgb(0, 0, 0, 1),
  edge.label = NA,
  edge.arrow.mode = "-",
  asp = 0,
  ...,
  equispaced = FALSE, 
  show = TRUE) {
  
  g <- igraph::graph_from_adjacency_matrix(t(x$coverMatrix()))
  
  n <- length(x$elements())
  
  if (n > 1) {
    ly <- igraph::layout_with_sugiyama(g)$layout
  
    if (equispaced) {
      rg <- range(ly[, 1])
      fr <- table(ly[, 2])
      w <- max(fr)
      d <- diff(rg) / (w - (w > 1))
      ctr <- diff(rg) / 2
      xax <- tapply(ly[, 1], ly[, 2], function(x) {
        res <- order(x) - 1
        (res - median(res)) * d + ctr
      })
      
      hgt <- max(ly[, 2])
      
      for (i in 1:hgt)
        ly[ly[, 2] == i, 1] <- xax[[as.character(i)]]
    }
    
    g$layout <- ly
  }
  
  if (show) igraph::plot.igraph(g,
    vertex.color = vertex.color,
    vertex.label = vertex.label,
    vertex.label.color = vertex.label.color,
    vertex.label.family = vertex.label.family,
    edge.color = edge.color,
    edge.arrow.mode = edge.arrow.mode,
    edge.label = edge.label,
    asp = asp,
  ...)
  
  invisible(g)
}
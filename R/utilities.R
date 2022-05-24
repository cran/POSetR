#' Poset incidence matrix
#' 
#' @description The function returns the incidence matrix for objects of class \code{\link[POSetR:poset]{poset}}.
#'
#' @param x an object of class \code{\link[POSetR:poset]{poset}}.
#'
#' @return a logical square matrix.
#' 
#' @aliases incidenceMatrix incidenceMatrix.poset
#'
#' @examples
#' dom <- matrix(c(
#'   "a", "b",
#'   "c", "b",
#'   "b", "d"
#' ), ncol = 2, byrow = TRUE)
#' p <- poset(x = dom)
#' incidenceMatrix(p)

#' @export
incidenceMatrix <- function(x) {
  UseMethod("incidenceMatrix")
}

#' @method incidenceMatrix poset
#' @export
incidenceMatrix.poset <- function(x) {
  pointerRebuild(x)
  return(x$pointer$incidenceMatrix())
}

########################

#' Poset cover matrix
#' 
#' @description The function returns the cover matrix for objects of class \code{\link[POSetR:poset]{poset}}.
#'
#' @param x an object of class \code{\link[POSetR:poset]{poset}}.
#'
#' @return a logical square matrix.
#' 
#' @aliases coverMatrix coverMatrix.poset
#'
#' @examples
#' dom <- matrix(c(
#'   "a", "b",
#'   "c", "b",
#'   "b", "d"
#' ), ncol = 2, byrow = TRUE)
#' p <- poset(x = dom)
#' coverMatrix(p)

#' @export
coverMatrix <- function(x) {
  UseMethod("coverMatrix")
}

#' @method coverMatrix poset
#' @export
coverMatrix.poset <- function(x) {
  pointerRebuild(x)
  return(x$pointer$coverMatrix())
}
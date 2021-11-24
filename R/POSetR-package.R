#' \packageTitle{POSetR}
#' 
#' \packageDescription{POSetR}
#' @docType package
#' @name POSetR-package
#' @aliases POSetR-package
#' @author \packageAuthor{POSetR}
#' @importFrom Rcpp evalCpp
#' @importFrom grDevices rgb
#' @importFrom methods new
#' @importFrom stats aggregate
#' @importFrom stats median runif
#' @importFrom utils setTxtProgressBar txtProgressBar
#' @useDynLib POSetR, .registration=TRUE
#' @exportPattern "^[[:alpha:]]+"
#' 
# package module
Rcpp::loadModule("poset_module", "POSet")
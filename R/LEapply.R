#' Applies scalar functions over the set of linear extensions of a poset and returns the corresponding average values
#' 
#' @description \code{"LEapply"} is composed of three main elements: (i) the linear extensions generator, (ii) the application of the argument functions to the linear extensions and (iii) the computation of the averages of the results, for each function separately; see \insertRef{fattore2016partially}{POSetR}.
#'
#' @param x an S4 object of class \code{Rcpp_POSet}, see \code{\link[POSetR]{poset}} for details.
#' @param FUN the function, or a list of functions, to be applied to each linear extension: see 'Details'.
#' @param ... optional arguments to \code{FUN}.
#' @param generator a string specifying the method used to generate the linear extensions. The default value is \code{"AllLE"}. See section 'Details' below.
#' @param bubleydyer.precision considered only if \code{"BubleyDyer"} generator is selected. It corresponds to the number of digit precision of the frequencies in the sampling distributions of linear extensions.
#' @param bubleydyer.nit considered only if \code{"BubleyDyer"} generator is selected. Number of iterations in the Bubley-Dyer algorithm, if NULL (default) it is set as indicated in Bubley and Dyer (1999) depending on the value of \code{bubleydyer.precision} and the number of elements of the poset.
#' @param bubleydyer.progressBar logical that indicates whether to show a text progress bar or not 
#' @param degrees to generate the lexicographic linear extensions of a product order, the poset \code{x} describes the dominance (e.g. relative importance) between ordinal variables and \code{degrees} is a numerical vector specifying the number of degrees of each variable, represented by in the poset.
#'
#' @usage LEapply(x, ...)
#' @usage \method{LEapply}{Rcpp_POSet}(
#'   x,
#'   FUN = "MutualRankingProbability",
#'   ...,
#'   generator = c("AllLE", "BubleyDyer"),
#'   bubleydyer.precision = 10,
#'   bubleydyer.nit = NULL,
#'   bubleydyer.progressBar = TRUE,
#'   degrees = NULL
#' )
#' 
#' @aliases LEapply LEapply.Rcpp_POSet
#' 
#' @return The average values of the argument functions \code{FUN} over the set of linear extensions (or lexicographic ones if \code{degrees} argument is not \code{NULL}).
#' 
#' @details Argument \code{FUN} must be either a function or a list of functions, each one depending on a vector of characters representing the names of the elements of the poset.
#' 
#' If \code{degrees} is not \code{NULL} but a numerical vector as long as the number of elements in the poset, the poset elements are considered as ordinal variables. Therefore \code{degrees} represents their number of degrees that are represented as integer numbers starting from 0. In this case, \code{LEapply} generates the lexicographical linear extensions of the product order of the ordeinal variables. Its elements are called profiles and they are obtained by the combination of the degrees of variables separated by a dash. For details about lexicographical linear extensions and profiles see \insertRef{fattore2018reduced}{POSetR}.
#' 
#' Some functions are already implemented in the \code{C++} library and they can be called by their names. Currently, such functions are \code{"MutualRankingProbability"}, \code{"Separation"}, and \code{"AverageHeight"}.
#' 
#' Each function in \code{FUN} must return a numerical or logical matrix.
#' Each function can depend on additional arguments that can be passed through \code{...}; such additinal arguments must be the same for all the functions in the list.
#' 
#' Argument \code{generator} specifies the linear extension generation algorithm. The available generators are \code{"AllLE"}, that produces all of the linear extensions of the input poset, and \code{"BubleyDyer"}, which samples uniformly from the set of linear extensions, through an MCMC algorithm \insertCite{bubley1999faster}{POSetR}.
#'
#' @references
#' \insertRef{bubley1999faster}{POSetR}
#' 
#' \insertRef{fattore2016partially}{POSetR}
#' 
#' \insertRef{fattore2018reduced}{POSetR}
#' 
#' \insertRef{habib2001efficient}{POSetR}
#' 
#' @seealso \code{\link[POSetR]{poset}}
#' @examples
#' dom <- matrix(c(
#' "a", "b",
#' "c", "b",
#' "b", "d"
#' ), ncol = 2, byrow = TRUE)
#' p <- poset(x = dom)
#' 
#' \dontrun{#' LEapply(
#'   x = p,
#'   FUN = "MutualRankingProbability",
#'   generator = "AllLE",
#'   degrees = c(3, 2, 3, 2)
#' )
#' 
#' a_rank_dist <- function(le) {
#'   return(matrix(le == "a"))
#' }
#' LEapply(x = p, FUN = a_rank_dist)}

#' @export
LEapply <- function(x, ...) {
  UseMethod("LEapply")
}

#' @method LEapply Rcpp_POSet
#' @export
LEapply.Rcpp_POSet <- function(
  # lapply-sapply functions arguments
  x,
  FUN = "MutualRankingProbability", # it is possible to use also a list of functions but all of them shoud depend from the same set of arguments ...
  ...,
  
  # generator choiche
  generator = c("AllLE", "BubleyDyer"),
  
  # BubleyDyer
  bubleydyer.precision = 10, # number of digit precision of LE frequencies
  bubleydyer.nit = NULL, # number of iterations,
  
  bubleydyer.progressBar = TRUE,
  
  # Product order and lexicographic linear extensions
  degrees = NULL
  
) {
  
  if (is.null(bubleydyer.nit)) {
    n <- length(x$elements())
    bubleydyer.nit <- trunc((n^4 * log(n)^2) + (n^3 * log(n) * bubleydyer.precision))
  }
  
  generator <- generator[1]
  
  stopifnot(generator %in% c("AllLE", "BubleyDyer"))#, "oldAllLE"))
  
  if (generator == "AllLE")
    generator <- "hmnsAllLE"
  
  if (generator == "oldAllLE")
    generator <- "AllLE"
  
  if (class(FUN) == "character")
    FUN <- as.list(FUN)
  
  if (!is.list(FUN))
    FUN <- list(FUN)
  
  FUN_no_args <- lapply(FUN, function(fun) {
    if (class(fun) != "function") {
      stopifnot(fun %in% c("MutualRankingProbability", "Separation", "AverageHeight"))
      return(fun)
    }
    
    return(function(x) fun(x, ...)) # !!! all functions must depend on the same arguments
  })
  generator <- generator[1]
  
  args <- list(
    LinearExtensionGenerator = generator #,
    # LinearExtensionGeneratorOutputFile = "" # just for tests
  )
  
  fle <- x$firstLE()
  
  if (is.null(degrees)) {
    if (any(FUN_no_args == "MutualRankingProbability")) { #, "Separation"))) {
      args$FLEMRPSelection <- x$incomparabilities()
    }
    # if (any(FUN_no_args == "Separation")) {
      # args$FLESepSelection <- x$incomparabilities()
    # }
  } else {
    # FLESelection not defined yet for Lexicographical linear extensions
    
    stopifnot(length(degrees) == length(x$elements()))
    names(degrees) <- x$elements()
    args$TranformExtension <- "Lexicographical"
    args$TEModalities <- degrees
    
    M <- expand.grid(lapply(degrees, function(y) 0:(y - 1)))
    ord <- do.call(order, M[, rev(fle)])
    M <- apply(M, 1, paste, collapse = "-")
    fle <- M[ord]
    rm(M, ord)
    
    apply(
      expand.grid(lapply(degrees, function(y) 0:(y - 1)))
    , 1, paste, collapse  = "-")
  }
  
  init <- NULL
  for (j in 1:length(FUN_no_args)) {
    test <- NULL
    if (class(FUN_no_args[[j]]) == "function") {
      test <- FUN_no_args[[j]](fle)
      if (class(test)[1] != "matrix")
        stop(paste("the output of function number", j, "is not a matrix"))
      if (!is.numeric(test) && !is.logical(test))
        stop(paste("the output of function number", j, "is neither numerical nor logical"))
    }
    
    init <- c(init, list(test))
  }
  
  args$FunctionsLinearExtension <- FUN_no_args
  args$FunctionsLinearExtensionSize <- lapply(init, dim)
  
  pb <- NULL
  if (generator == "BubleyDyer") {
    if (bubleydyer.progressBar) {
      cat("Number of iterations:", bubleydyer.nit, "\n")
      pb <- txtProgressBar(style = 3, max = bubleydyer.nit)
    }
    args$BubleyDyerSeed <- as.integer(runif(1, 0, .Machine$integer.max))
    args$BubleyDyerMaxIterations = bubleydyer.nit
  }
  
  if (!is.null(pb))
    args$ProgressBarUpdate = function(x) setTxtProgressBar(pb, value = x)
  
  # evaluation
  res <- x$evaluation(args)
  
  if (!is.null(pb))
    close(pb)
  
  if (is.null(degrees)) {
    MRPidx <- which(sapply(FUN, function(fun) class(fun) == "character" && fun == "MutualRankingProbability"))
    if (length(MRPidx) > 0) {
      I <- x$incidenceMatrix()
      for (j in MRPidx)
        res[[j]] <- I + res[[j]] + t(1 - res[[j]]) * t(res[[j]] > 0)
    }
  }
  
  for (j in 1:length(res)) {
    if (class(FUN[[j]]) == "function")
      dimnames(res[[j]]) <- dimnames(init[[j]])
  }
  
  names(res) <- names(FUN)
  
  if (length(res) == 1)
    res <- res[[1]]
  
  return(res)
}
% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/print.summary_poset.R
\name{print.summary_poset}
\alias{print.summary_poset}
\title{Method for the print function that shows the poset summary}
\usage{
\method{print}{summary_poset}(x, ...)
}
\arguments{
\item{x}{an object of class \code{summary_poset}.}

\item{...}{further arguments passed to or from other methods.}
}
\value{
nothing
}
\description{
\code{print} prints the poset summary.
}
\examples{
dom <- matrix(c(
  "a", "b",
  "c", "b",
  "b", "d"
), ncol = 2, byrow = TRUE)
p <- poset(x = dom)
summary(p)

summary(chain(1:4))
summary(antiChain(LETTERS[1:5]))
}
\seealso{
\code{\link[POSetR]{summary.poset}}
}

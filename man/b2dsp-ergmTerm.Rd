% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/InitErgmTerm.R
\name{b2dsp-ergmTerm}
\alias{b2dsp-ergmTerm}
\alias{InitErgmTerm.b2dsp}
\title{Dyadwise shared partners for dyads in the second bipartition}
\usage{
# binary: b2dsp(d)
}
\arguments{
\item{d}{a vector of distinct integers}
}
\description{
Dyadwise shared partners for dyads in the second bipartition
}
\details{
This term adds one network statistic to the model for each element in \code{d} ; the \eqn{i} th
such statistic equals the number of dyads in the second bipartition with exactly
\code{d[i]} shared partners. (Those shared partners, of course, must be members
of the first bipartition.) This term can only be used with bipartite networks.
}
\seealso{
\code{\link{ergmTerm}} for index of model terms currently visible to the package.
}
\concept{bipartite}
\concept{undirected}

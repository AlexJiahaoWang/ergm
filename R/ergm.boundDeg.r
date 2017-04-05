ergm.boundDeg <- function(boundDeg,nnodes=network.size(g)){    
#  Resolve conditioning in ERGM call, as expressed in the
#  argument boundDeg (a list, with item names as seen below)
 if(is.null(boundDeg) ||
    all(sapply(boundDeg,function(x){length(x)=1 && x==0}))
   ) {
    condAllDegExact <- 0
    attribs <- 0
    maxout <- 0
    maxin <- 0
    minout <- 0
    minin <- 0
 } else {
    condAllDegExact <- boundDeg$condAllDegExact
    attribs <- boundDeg$attribs
    maxout <- boundDeg$maxout
    maxin <- boundDeg$maxin
    minout <- boundDeg$minout
    minin <- boundDeg$minin
    if (is.null(attribs)){ 
      if(any(!is.null(c(minin,minout,maxout,maxin,condAllDegExact)))){ 
       attribs <- matrix(1,ncol=1,nrow=nnodes)
      }else{
       attribs <- 0
      }
    }
    if(is.null(minin )) minin <- matrix(0,ncol=ncol(attribs),nrow=nnodes)
    if(is.null(minout)) minout <- matrix(0,ncol=ncol(attribs),nrow=nnodes)
    if(is.null(maxin )) maxin <- matrix(nnodes-1,ncol=ncol(attribs),nrow=nnodes)
    if(is.null(maxout)) maxout <- matrix(nnodes-1,ncol=ncol(attribs),nrow=nnodes)
    if(length(minin )==1) minin  <- matrix(minin ,ncol=ncol(attribs),nrow=nnodes)
    if(length(minout)==1) minout <- matrix(minout,ncol=ncol(attribs),nrow=nnodes)
    if(length(maxin )==1) maxin  <- matrix(maxin ,ncol=ncol(attribs),nrow=nnodes)
    if(length(maxout)==1) maxout <- matrix(maxout,ncol=ncol(attribs),nrow=nnodes)
     minin[is.na( minin)| minin<0] <- 0
    minout[is.na(minout)|minout<0] <- 0
     maxin[is.na( maxin)] <- nnodes-1
    maxout[is.na(maxout)] <- nnodes-1
    attribs[is.na(attribs)] <- 0
    if (is.null(condAllDegExact)) condAllDegExact <- 0
    if (condAllDegExact!=1) condAllDegExact <- 0
 }
 list(condAllDegExact=condAllDegExact,
      attribs=attribs,
      maxout=maxout,
      maxin=maxin,
      minout=minout,
      minin=minin)
}

/*  This is a collection of functions used to calculate diagnostic 
    statistics for dynamic networks. */

#include "infection.h"

/* These #defines are not really necessary but may make the code a bit
   easier to read.  They come at a price (the possibility of hard-to-track
   errors).   */
#define DMATRIX(a,b) (dmatrix[(a)+(offset)*(b)])
#define EDGE(a,b) (edge[(a)+(*nedge)*(b)])
#define CHANGE(a,b) (change[(a)+(*nchange)*(b)])
#define DISSOLVE(a,b) (dissolve[(a)+(*ndissolve)*(b)])
#define OMATRIX(a,b) (omatrix[(a)+(maxo)*(b)])
#define DEGMIXMAT(a,b) (degmixmat[(a)+(*nnodes)*(b)])

void Prevalence (int *nnodes,
      int *nedge, int *edge, int *ntimestep, int *nfem, int *nseeds,
      int *ntotal, int *nchange, int *change, int *ndissolve, int *dissolve,
      int *randomseeds, double *betarate, int *infected, int *totinfected,
      int *nsim, int *prev) {
  Vertex alter=0;
  Edge e;
  Vertex *id, *od;
  Edge i, j, ne = *nedge, nwedge;
  int k, time, ndyads, rane;
  int bipartite = *nfem;
  double *heads, *tails;
  int *sinfected;
  double beta=*betarate;
  TreeNode *ie, *oe;
  Network nw;
  
  sinfected = (int *) malloc(sizeof(int) * (*nnodes));
  /* Set up a statnet Network using existing edgeTree code 
     Must coerce heads and tails to double, */
  heads = (double *) malloc(sizeof(double) * ne);
  tails = (double *) malloc(sizeof(double) * ne);
  for (i=0; i < ne; i++) {
   heads[i] = (double) EDGE(i, 0);
   tails[i] = (double) EDGE(i, 1);
  }
//    Rprintf("initial bipartite %d edges %d heads[i] %f tails[i] %f\n", bipartite,ne,
//		           heads[i-1],tails[i-1]);

  for (k=0; k < *nsim; k++) {
   nw = NetworkInitialize(heads, tails, ne, *nnodes, 0, bipartite);
   ie=nw.inedges;
   oe=nw.outedges;
   id=nw.indegree;
   od=nw.outdegree;
   ndyads = bipartite*(*nnodes-bipartite);
   if(*randomseeds){
      // Sample numdissolved edges without replacement
      ndyads = *nnodes;
      for (i = 0; i < ndyads; i++){sinfected[i] = i;}
      for (i = 0; i < (*nseeds); i++) {
	rane = ndyads * unif_rand();
	infected[i] = sinfected[rane] + 1;
	sinfected[rane] = sinfected[--ndyads];
      }
      for (i = 0; i < *nnodes; i++){sinfected[i] = 0;}
//  Rprintf("\n", infected[i]);
      for (i=0; i < (*nseeds); i++) {
//  Rprintf(" %d", infected[i]);
	sinfected[infected[i]] = 1;
      }
   }else{
    for (i=0; i < *nnodes; i++) {
      sinfected[i] = infected[i];
    }
   }
  
   /* Step through time one click at a time */
   for (time=j=0; time <= *ntimestep; time++) {
//Rprintf("time %d \n",time);
    /* Update the infection vector */
    for (i=0; i < *nfem; i++) {
     /* step through outedges of i  */
     if(sinfected[i]){
      for(e = EdgetreeMinimum(oe, i+1);
	(alter = oe[e].value) != 0;
	e = EdgetreeSuccessor(oe, e)){
	     if(!sinfected[alter-1]){
 	       if(unif_rand() < beta){
//  Rprintf("f time %d i %d sinfected %d alter %d sinfected %d beta %f\n",time,i,sinfected[i],alter,sinfected[alter],beta);
		       sinfected[alter-1]=1;}
//       if(unif_rand() < beta/od[i+1]){sinfected[alter-1]=1;}
 	     }
          }
      }
//    Rprintf("time %d i %d sinfected %d beta %f\n",time,i,sinfected[i]);
    }
    for (; i < *nnodes; i++) {
//    Rprintf("males time %d i %d sinfected %d\n",time,i,sinfected[i]);
    /* step through outedges of i  */
    if(sinfected[i]){
     for(e = EdgetreeMinimum(ie, i+1);
       (alter = ie[e].value) != 0;
       e = EdgetreeSuccessor(ie, e)){
          if(!sinfected[alter-1]){
//          if(unif_rand() < beta/id[i]){sinfected[alter-1]=1;}
            if(unif_rand() < beta){
//    Rprintf("m time %d i %d sinfected %d alter %d sinfected %d beta %f\n",time,i,sinfected[i],alter,sinfected[alter],beta);
		    sinfected[alter-1]=1;}
          }
        }
    }
   }
//Rprintf("time %d i %d sinfected %d alter %d sinfected %d beta %f\n",i,sinfected[i],alter,sinfected[alter],beta);
    /* Toggle the edges at this timestep */
    if (time < *ntimestep) {
     for(; CHANGE(j,0) == time; j++) {
        ToggleEdge(CHANGE(j, 1), CHANGE(j, 2), &nw); 
     }
     // End time step toggle
    }
   // End of time step 
   }
   NetworkDestroy (&nw);
   // Next k
   for (i=0; i < *nnodes; i++) {
     prev[k]=prev[k]+sinfected[i];
   }
   for (i=0; i < *nnodes; i++) {
     totinfected[i] = totinfected[i] + sinfected[i];
   }
//   Rprintf("k %d edges %d prev %d \n",k,nw.nedges,prev[k]);
// if (k < *nsim) {
//  NetworkDestroy (&nw);
//  nw = NetworkInitialize(heads, tails, ne, *nnodes, 0, bipartite);
//  ie=nw.inedges;
//  oe=nw.outedges;
//  id=nw.indegree;
//  od=nw.outdegree;
// }
  }
  for (i=0; i < *nnodes; i++) {
      infected[i] = sinfected[i];
  }
  /* Free memory used by network object before returning */  
  free (sinfected);
  free (heads);
  free (tails);
//NetworkDestroy (&nw);
}
void PrevalenceWithBernoulliOption(int *nnodes,
      int *nedge, int *edge, int *ntimestep, int *nfem,
      int *ntotal, int *nchange, int *change, int *ndissolve, int *dissolve,
      int *bernoulli, double *betarate, int *infected, int *nsim, int *prev) {
  Vertex alter=0;
  Edge e;
  Vertex *id, *od;
  Edge i, j, ne = *nedge, nwedge;
  int k, time, ndyads, rane;
  int bipartite = *nfem;
  double *heads, *tails;
  double *bheads, *btails;
  int *sinfected, *bsort;
  double beta=*betarate;
  TreeNode *ie, *oe;
  Network nw, nws, nwt;
  
  sinfected = (int *) malloc(sizeof(int) * (*nnodes));
  /* Set up a statnet Network using existing edgeTree code 
     Must coerce heads and tails to double, */
  heads = (double *) malloc(sizeof(double) * ne);
  tails = (double *) malloc(sizeof(double) * ne);
  for (i=0; i < ne; i++) {
   heads[i] = (double) EDGE(i, 0);
   tails[i] = (double) EDGE(i, 1);
  }
//    Rprintf("initial bipartite %d edges %d heads[i] %f tails[i] %f\n", bipartite,ne,
//		           heads[i-1],tails[i-1]);
  nws = NetworkInitialize(heads, tails, ne, *nnodes, 0, bipartite);
  if(*bernoulli){
    nw = NetworkInitialize(heads, tails, ne, *nnodes, 0, bipartite);
  }else{
    nw = nws;
  }
  ie=nw.inedges;
  oe=nw.outedges;
  id=nw.indegree;
  od=nw.outdegree;
  ndyads = bipartite*(nws.nnodes-bipartite);
  bsort = (int *) malloc(sizeof(int) * ndyads);

  for (k=0; k < *nsim; k++) {
   for (i=0; i < *nnodes; i++) {
     sinfected[i] = infected[i];
   }
  
   /* Step through time one click at a time */
   for (time=j=0; time <= *ntimestep; time++) {
//Rprintf("time %d \n",time);
    /* Update the infection vector */
    for (i=0; i < *nfem; i++) {
     /* step through outedges of i  */
     if(sinfected[i]){
      for(e = EdgetreeMinimum(oe, i+1);
	(alter = oe[e].value) != 0;
	e = EdgetreeSuccessor(oe, e)){
	     if(!sinfected[alter-1]){
//  Rprintf("time %d i %d sinfected %d alter %d sinfected %d beta %f\n",time,i,sinfected[i],alter,sinfected[alter],beta);
 	       if(unif_rand() < beta/od[i+1]){sinfected[alter-1]=1;}
 	     }
          }
      }
//    Rprintf("time %d i %d sinfected %d beta %f\n",time,i,sinfected[i]);
    }
    for (; i < *nnodes; i++) {
//    Rprintf("males time %d i %d sinfected %d\n",time,i,sinfected[i]);
    /* step through outedges of i  */
    if(sinfected[i]){
     for(e = EdgetreeMinimum(ie, i+1);
       (alter = ie[e].value) != 0;
       e = EdgetreeSuccessor(ie, e)){
//    Rprintf("time %d i %d sinfected %d alter %d sinfected %d beta %f\n",time,i,sinfected[i],alter,sinfected[alter],beta);
          if(!sinfected[alter-1]){
//    Rprintf("time %d i %d sinfected %d alter %d sinfected %d beta %f\n",time,i,sinfected[i],alter,sinfected[alter],beta);
            if(unif_rand() < beta/id[i]){sinfected[alter-1]=1;}
          }
        }
    }
   }
//Rprintf("time %d i %d sinfected %d alter %d sinfected %d beta %f\n",i,sinfected[i],alter,sinfected[alter],beta);
    /* Toggle the edges at this timestep */
    if (time < *ntimestep) {
     for(; CHANGE(j,0) == time; j++) {
        ToggleEdge(CHANGE(j, 1), CHANGE(j, 2), &nws); 
     }
     if(*bernoulli){
      // Sample numdissolved edges without replacement
      ndyads = bipartite*(nws.nnodes-bipartite);
      bheads = (double *) malloc(sizeof(double) * nws.nedges);
      btails = (double *) malloc(sizeof(double) * nws.nedges);
      for (i = 0; i < ndyads; i++){bsort[i] = i;}
      for (i = 0; i < nws.nedges; i++) {
	rane = ndyads * unif_rand();
	btails[i] = bsort[rane] + 1;
	bsort[rane] = bsort[--ndyads];
      }
      for (i=0; i < nws.nedges; i++) {
//    Rprintf("i %d sort[i] %f ",i, btails[i]);
	rane = (double)(((Vertex)(btails[i]/bipartite)));
	bheads[i] = btails[i] - bipartite*rane;
	btails[i] = rane + bipartite;
//    Rprintf("i %d bheads[i] %f btails[i] %f\n",i, bheads[i],btails[i]);
      }
//    Rprintf("final k %d time %d bipartite %d edges %d bheads[i] %f btails[i] %f\n",k, time, bipartite,nws.nedges,
//		           bheads[i-1],btails[i-1]);
      NetworkDestroy (&nw);
      nwedge=nws.nedges;
      Network nw;
      nw = NetworkInitialize(bheads, btails, nwedge, *nnodes, 0, bipartite);
//    Rprintf("network reinitalized for Bernoulli bipartite %d edges %d\n", bipartite,nw.nedges);
      ie=nw.inedges;
      oe=nw.outedges;
      id=nw.indegree;
      od=nw.outdegree;
      free (bheads);
      free (btails);
     }
     // End time step toggle
    }
   // End of time step 
   }
   // Next k
   for (i=0; i < *nnodes; i++) {
     prev[k]=prev[k]+sinfected[i];
   }
//   Rprintf("k %d edges %d prev %d \n",k,nw.nedges,prev[k]);
   if (k < *nsim) {
    NetworkDestroy (&nw);
    nw = NetworkInitialize(heads, tails, ne, *nnodes, 0, bipartite);
    ie=nw.inedges;
    oe=nw.outedges;
    id=nw.indegree;
    od=nw.outdegree;
   }
  }
  /* Free memory used by network object before returning */  
  free (sinfected);
  free (heads);
  free (tails);
  NetworkDestroy (&nw);
}

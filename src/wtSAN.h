/*  File src/wtSAN.h in package ergm, part of the Statnet suite
 *  of packages for network analysis, https://statnet.org .
 *
 *  This software is distributed under the GPL-3 license.  It is free,
 *  open source, and has the attribution requirements (GPL Section 7) at
 *  https://statnet.org/attribution
 *
 *  Copyright 2003-2019 Statnet Commons
 */
#ifndef WTSAN_H
#define WTSAN_H

#include "ergm_wtedgetree.h"
#include "ergm_wtchangestat.h"
#include "ergm_wtMHproposal.h"
#include "ergm_wtmodel.h"
#include "wtMCMC.h"

MCMCStatus WtSANSample(WtErgmState *s,
                       double *invcov, double *tau, double *networkstatistics, double *prop_networkstatistics,
                       int samplesize, int nsteps,
                       int nstats,
                       int *statindices,
                       int noffsets,
                       int *offsetindices,
                       double *offsets,
                       int verbose);
MCMCStatus WtSANMetropolisHastings(WtErgmState *s,
                                   double *invcov, double *tau, double *statistics, double *prop_statistics,
                                   int nsteps, int *staken,
                                   int nstats,
                                   int *statindices,
                                   int noffsets,
                                   int *offsetindices,
                                   double *offsets,
                                   int verbose);
#endif

		  char **MHproposaltype, char **MHproposalpackage,
		  double *inputs, double *theta0, double *tau, int *samplesize, 
		  double *sample, int *burnin, int *interval,  
		  int *newnetworkheads, 
		  int *newnetworktails, 
		  double *invcov,
		  int *fVerbose, 
		  int *attribs, int *maxout, int *maxin, int *minout,
		  int *minin, int *condAllDegExact, int *attriblength, 
		  int *maxedges,
		  int *mheads, int *mtails, int *mdnedges);

void SANSample (char *MHproposaltype, char *MHproposalpackage,
		double *theta, double *invcov, double *tau, double *networkstatistics, 
		long int samplesize, long int burnin, 
		long int interval, int hammingterm, int fVerbose,
		Network *nwp, Model *m, DegreeBound *bd);
void SANMetropolisHastings (MHproposal *MHp,
			 double *theta, double *invcov, double *tau, double *statistics, 
			 long int nsteps, long int *staken,
			 int hammingterm, int fVerbose,
			 Network *nwp, Model *m, DegreeBound *bd);
#endif

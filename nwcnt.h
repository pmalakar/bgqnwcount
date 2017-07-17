#include <stdio.h>
#include <bgpm/include/bgpm.h>

int hNWSet;

void bgpminit() {

	Bgpm_Init(BGPM_MODE_SWDISTRIB);

	unsigned nwEvtList[] = {	
		PEVT_NW_SYS_PP_SENT,			
		PEVT_NW_SYS_PP_RECV		
	};

	// Network events
	hNWSet = Bgpm_CreateEventSet();
	Bgpm_AddEventList(hNWSet, nwEvtList, sizeof(nwEvtList)/sizeof(unsigned));
	if (Bgpm_Attach(hNWSet, UPC_NW_ALL_LINKS, 0) == BGPM_WALREADY_ATTACHED) {
		printf("Another sw thread on node owns network link counters\n");
	}
	Bgpm_Start(hNWSet);

}

void bgpmfinalize() {

	Bgpm_Stop(hNWSet);

}

// Print counter results from evt set
void PrintCounts(char *name, int hEvtSet, int myrank) {

    int i;
    int numEvts = Bgpm_NumEvents(hEvtSet);
    uint64_t cnt;
    for (i=0; i<numEvts; i++) {
        Bgpm_ReadEvent(hEvtSet, i, &cnt);
        printf("Rank %d %s => %llu\n", myrank, Bgpm_GetEventLabel(hEvtSet, i), cnt);
    }

    Bgpm_Disable();
}


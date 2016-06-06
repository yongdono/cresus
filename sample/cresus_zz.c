#include "sim/sim.h"
#include "input/yahoo.h"
#include "engine/cluster.h"
#include "engine/timeline.h"
#include "indicator/zigzag.h"
#include "framework/verbose.h"

#include <getopt.h>
#include <string.h>

#define ZIGZAG 1
#define START_TIME VAL_YEAR(2012)|VAL_MONTH(1)|VAL_DAY(1)

#define ZIGZAG_THRES 0.02 /* 1/100th */

static struct timeline *
timeline_create(const char *filename, const char *name, time_info_t min,
		double threshold) {

  struct yahoo *yahoo;
  struct zigzag *zigzag;
  struct timeline *timeline;
  
  /* TODO : Check return values */
  yahoo_alloc(yahoo, filename, START_TIME, TIME_MAX); /* load everything */
  timeline_alloc(timeline, name, __input__(yahoo));
  /* Indicators alloc */
  zigzag_alloc(zigzag, ZIGZAG, threshold, CANDLE_CLOSE);
  /* And insert */
  timeline_add_indicator(timeline, __indicator__(zigzag));
  
  return timeline;
}

/* more final functions */

static void add_timeline_to_cluster(struct cluster *c, const char *path,
				    const char *name, time_info_t time,
				    double threshold) {
  
  struct timeline *sub;
  sub = timeline_create(path, name, time, threshold);
  cluster_add_timeline(c, sub);
}

/* Global */

static zigzag_dir_t __trend__;

static int trend_set(zigzag_dir_t trend) {
  
  zigzag_dir_t old = __trend__;
  __trend__ = trend;
  /* Choose what's best here */
  return (__trend__ != old);
}

/* sim interface */

static int sim_feed(struct sim *s, struct cluster *c) {

  char buf[256];
  int status = 0;
  
  struct timeline *t;
  struct timeline_entry *entry;
  struct indicator_entry *ientry;
  struct zigzag_entry *zref = NULL;
  
  /* TODO : better management of this ? */
  if(timeline_entry_current(__timeline__(c), &entry) != -1){
    struct candle *candle = __timeline_entry_self__(entry);
    if((ientry = candle_find_indicator_entry(candle, ZIGZAG))){
      zref = __indicator_entry_self__(ientry);
      PR_WARN("%s ZIGZAG is %.2f\n", __timeline__(c)->name, zref->value);
      /* Manage cluster's status here */
      if(trend_set(zref->dir)){
	/* We got a change here */
	PR_INFO("General trend switched to %s\n",
		__trend__ == ZIGZAG_UP ? "up" : "down");
	/* Close all positions */
	sim_close_all_positions(s);
      }
    }
  }
  
  __slist_for_each__(&c->slist_timeline, t){
    /* TODO : this is facultative */
    //timeline_display_info(t);
    /* This is mandatory */
    if(t->status == TIMELINE_STATUS_RESET){
      /* Timeline's been reset. We gotta leave our positions */
      sim_ignore_position(s, t);
      continue;
    }
    
    if(timeline_entry_current(t, &entry) != -1){
      struct position *p;
      struct candle *candle = __timeline_entry_self__(entry);
      if((ientry = candle_find_indicator_entry(candle, ZIGZAG))){
	struct zigzag_entry *zentry = __indicator_entry_self__(ientry);
	PR_WARN("%s ZZ is %.2f\n", t->name, zentry->value);
	
	/* Always check if something's open */
	sim_find_opened_position(s, t, &p);

	/* LONG positions */
	if(__trend__ == ZIGZAG_UP){
	  if(zentry->value > zref->value){
	    if(p == NULL){
	      sim_open_position(s, t, POSITION_LONG, 1);
	      PR_ERR("Taking LONG position on %s at %s\n",
		     t->name, candle_str(candle, buf));
	    }
	  }else{
	    if(p != NULL){
	      sim_close_position(s, t);
	      PR_ERR("Leaving LONG position on %s at %s\n",
		     t->name, candle_str(candle, buf));
	    }
	  }
	}

	/* SHORT positions */
	if(__trend__ == ZIGZAG_DOWN){
	  if(zentry->value < zref->value){
	    if(p == NULL){
	      sim_open_position(s, t, POSITION_SHORT, 1);
	      PR_ERR("Taking SHORT position on %s at %s\n",
		     t->name, candle_str(candle, buf));
	    }
	  }else{
	    if(p != NULL){
	      sim_close_position(s, t);
	      PR_ERR("Leaving SHORT position on %s at %s\n",
		     t->name, candle_str(candle, buf));
	    }
	  }
	}
      }
    }
  }
}

int main(int argc, char **argv) {

  int c;
  struct sim sim;
  struct zigzag zigzag;
  struct cluster cluster;
  double threshold = ZIGZAG_THRES;
  
  /* VERBOSE_LEVEL(WARN); */
  
  while((c = getopt(argc, argv, "vp:")) != -1){
    switch(c){
    case 'p' :
      sscanf(optarg, "%lf", &threshold);
      printf("Thresshold is %.1lf%%\n", threshold * 100.0);
      break;
      
    case 'v' : VERBOSE_LEVEL(DBG); break;
    }
  }
  
  /* 01/01/2000 */
  struct yahoo *yahoo;
  time_info_t time = START_TIME;
  /* Load ref data */
  yahoo_alloc(yahoo, "data/%5EFCHI.yahoo", time, TIME_MAX);
  cluster_init(&cluster, "my cluster", __input__(yahoo), time, TIME_MAX);
  /* Init general roc indicator */
  zigzag_init(&zigzag, ZIGZAG, threshold, CANDLE_CLOSE);
  timeline_add_indicator(__timeline__(&cluster), __indicator__(&zigzag));
  
  /* Sub-timelines */
  add_timeline_to_cluster(&cluster, "data/AC.yahoo",    "AC", time, threshold);
  add_timeline_to_cluster(&cluster, "data/ACA.yahoo",   "ACA", time, threshold);
  add_timeline_to_cluster(&cluster, "data/AI.yahoo",    "AI", time, threshold);
  add_timeline_to_cluster(&cluster, "data/AIR.yahoo",   "AIR", time, threshold);
  add_timeline_to_cluster(&cluster, "data/BN.yahoo",    "BN", time, threshold);
  add_timeline_to_cluster(&cluster, "data/BNP.yahoo",   "BNP", time, threshold);
  add_timeline_to_cluster(&cluster, "data/CA.yahoo",    "CA", time, threshold);
  add_timeline_to_cluster(&cluster, "data/CAP.yahoo",   "CAP", time, threshold);
  add_timeline_to_cluster(&cluster, "data/CS.yahoo",    "CS", time, threshold);
  add_timeline_to_cluster(&cluster, "data/DG.yahoo",    "DG", time, threshold);
  add_timeline_to_cluster(&cluster, "data/EI.yahoo",    "EI", time, threshold);
  add_timeline_to_cluster(&cluster, "data/EN.yahoo",    "EN", time, threshold);
  add_timeline_to_cluster(&cluster, "data/ENGI.yahoo",  "ENGI", time, threshold);
  add_timeline_to_cluster(&cluster, "data/FP.yahoo",    "FP", time, threshold);
  add_timeline_to_cluster(&cluster, "data/FR.yahoo",    "FR", time, threshold);
  add_timeline_to_cluster(&cluster, "data/GLE.yahoo",   "GLE", time, threshold);
  add_timeline_to_cluster(&cluster, "data/KER.yahoo",   "KER", time, threshold);
  add_timeline_to_cluster(&cluster, "data/LHN.yahoo",   "LHN", time, threshold);
  add_timeline_to_cluster(&cluster, "data/LI.yahoo",    "LI", time, threshold);
  add_timeline_to_cluster(&cluster, "data/LR.yahoo",    "LR", time, threshold);
  add_timeline_to_cluster(&cluster, "data/MC.yahoo",    "MC", time, threshold);
  add_timeline_to_cluster(&cluster, "data/ML.yahoo",    "ML", time, threshold);
  add_timeline_to_cluster(&cluster, "data/MT.yahoo",    "MT", time, threshold);
  add_timeline_to_cluster(&cluster, "data/NOKIA.yahoo", "NOKIA", time, threshold);
  add_timeline_to_cluster(&cluster, "data/OR.yahoo",    "OR", time, threshold);
  add_timeline_to_cluster(&cluster, "data/ORA.yahoo",   "ORA", time, threshold);
  add_timeline_to_cluster(&cluster, "data/PUB.yahoo",   "PUB", time, threshold);
  add_timeline_to_cluster(&cluster, "data/RI.yahoo",    "RI", time, threshold);
  add_timeline_to_cluster(&cluster, "data/RNO.yahoo",   "RNO", time, threshold);
  add_timeline_to_cluster(&cluster, "data/SAF.yahoo",   "SAF", time, threshold);
  add_timeline_to_cluster(&cluster, "data/SAN.yahoo",   "SAN", time, threshold);
  add_timeline_to_cluster(&cluster, "data/SGO.yahoo",   "SGO", time, threshold);
  add_timeline_to_cluster(&cluster, "data/SOLB.yahoo",  "SOLB", time, threshold);
  add_timeline_to_cluster(&cluster, "data/SU.yahoo",    "SU", time, threshold);
  add_timeline_to_cluster(&cluster, "data/SW.yahoo",    "SW", time, threshold);
  add_timeline_to_cluster(&cluster, "data/TEC.yahoo",   "TEC", time, threshold);
  add_timeline_to_cluster(&cluster, "data/UG.yahoo",    "UG", time, threshold);
  add_timeline_to_cluster(&cluster, "data/UL.yahoo",    "UL", time, threshold);
  add_timeline_to_cluster(&cluster, "data/VIE.yahoo",   "VIE", time, threshold);
  add_timeline_to_cluster(&cluster, "data/VIV.yahoo",   "VIV", time, threshold);
  
  /* Now create sim */
  sim_init(&sim, &cluster);
  sim_run(&sim, sim_feed);
  sim_display_report(&sim);
  
  return 0;
}

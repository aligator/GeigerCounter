#ifndef GEIGER_STATS_H_
#define GEIGER_STATS_H_

struct GeigerStats {
    bool isFullAccuracy;
    unsigned long lastImpulses;
    unsigned long cpm;
    double uSv;
};

#endif /* GEIGER_STATS_H_ */
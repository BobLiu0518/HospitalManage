#ifndef BED
#define BED

#include "timeTools.h"
#define MAX_ZONES 50
#define BED_STATUS_EMPTY 0
#define BED_STATUS_OCCUPIED 1

typedef struct {
    int bedNumber;
    int status;
} Bed;

typedef struct {
    char zoneName[10];
    char department[50];
    int bedsCount;
    Bed* beds;
} Zone;

typedef struct {
    long long patientId;
    Datetime admissionDate;
    char zoneName[10];
    int bedNumber;
} OccupancyRecord;

void saveZones();
void loadZones();
void recordOccupancy(long long patientId, Datetime admissionDate, char* zoneName, int bedNumber);
int selectBed(int* zoneIndex, int* bedNumber);
int setOccupy(long long patientId);
int deleteOccupancy(int zoneIndex, int bedNumber);
int bedMain();

#endif
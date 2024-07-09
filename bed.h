#ifndef BED
#define BED

#define MAX_ZONES 5
#define MAX_BEDS_PER_ZONE 10
#define BED_STATUS_EMPTY 0
#define BED_STATUS_OCCUPIED 1

typedef struct {
    int bedNumber;
    int status; // BED_STATUS_EMPTY or BED_STATUS_OCCUPIED
} Bed;

typedef struct {
    char zoneName[50];
    Bed beds[MAX_BEDS_PER_ZONE];
} Zone;

typedef struct {
    char patientName[100];
    char admissionDate[50];
    int zoneIndex;
    int bedNumber;
} OccupancyRecord;

void initializeZones();
void saveZonesToFile(const char* filename);
void loadZonesFromFile(const char* filename);
void recordOccupancy(const char* patientName, const char* admissionDate, int zoneIndex, int bedNumber);
int deleteOccupancy(int zoneIndex, int bedNumber);
int bedMain();

#endif
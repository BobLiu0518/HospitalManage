#ifndef MEDICINE
#define MEDICINE

typedef struct Medicine {
    char fullName[100];
    char abbreviation[20];
    int stock;
    struct Medicine* next;
} Medicine;

void addMedicine();
void viewMedicine();
void updateMedicine();
int ModifyStock(char abbr[ ], int quantity);
Medicine* getMedicine(char abbr[ ]);
void deleteMedicine();
void exportMedicine();
void importMedicine();
int medicineMain();

#endif
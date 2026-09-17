
#include <stdio.h>
#include <string.h>

#define maxPatients 80
#define numberOfSpecialties 4
#define numberOfWards 4
#define maxBeds 20

// specialties data

int specialtyId[4] = {1,2,3,4};
char specialtyName[4][30] = {"General Practice", "Paediatrics",
                             "Cardiology", "Neurology"};
float consultationFee[4] = {1500.00, 2500.00,
                            4500.00, 5000.00};
int consultationTime[4] = {15, 20, 30, 30};
int dailyPatientCapacity[4] = {30, 20, 12, 10};

// wards data

int wardId[4] = {1, 2, 3, 4};
char wardName[4][30] = {"General Ward", "Paediatric Ward",
                        "Surgical Ward", "ICU"};
float wardDailyRate[4] = {3000.00, 6000.00, 12000.00,
                          25000.00};
int wardCapacity[4] = {20, 10, 10, 5};
int bedOccupancy[4][20];

int patientId[maxPatients];
char patientName[maxPatients][50];
int patientAge[maxPatients];
int patientUrgency[maxPatients];
int patientSpecialty[maxPatients];
int patientAdmitted[maxPatients];
int patientWard[maxPatients];
int patientDays[maxPatients];
int patientBed[maxPatients];
int registrationOrder[maxPatients];
float baseFee[maxPatients];
float emergencySurcharge[maxPatients];
float wardCost[maxPatients];
float grossTotal[maxPatients];
float discount[maxPatients];
float finalPayable[maxPatients];
float waitingTime[maxPatients];

int specialtyQueue[4] = {0, 0, 0, 0};
int patientCount = 0;
int nextPatientId = 1001;

void initializeBeds()
{
    int i,j;
    for (i=0; i<numberOfWards; i++)
    {
        for(j=0; j<maxBeds; j++)
        {
            bedOccupancy[i][j] = 0;
        }
    }
}

void displaySpecialties() {
    int i;
    printf("\n=================================================================================\n");
    printf("                                DOCTOR SPECIALTIES\n");
    printf("=================================================================================\n");
    printf("%-5s | %-20s | %-15s | %-12s | %-10s\n", "ID", "Specialty", "Fee (LKR)",
           "Time (mins)", "Daily Cap");
    printf("---------------------------------------------------------------------------------\n");
    for (i = 0; i < numberOfSpecialties; i++) {
        printf("%-5d | %-20s | %-15.2f | %-12d | %-10d\n",
               specialtyId[i], specialtyName[i], consultationFee[i], consultationTime[i],
               dailyPatientCapacity[i]);
    }
    printf("=================================================================================\n");
}


int main()
{
    printf("Smart Hospital System Loaded.\n");
    return 0;
}

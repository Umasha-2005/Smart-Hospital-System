
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

void displayWards() {
    int i;
    printf("\n=================================================================================\n");
    printf("                                 HOSPITAL WARDS\n");
    printf("=================================================================================\n");
    printf("%-5s | %-20s | %-15s | %-10s\n", "ID", "Ward Name", "Daily Rate (LKR)", "Capacity");
    printf("---------------------------------------------------------------------------------\n");
    for (i = 0; i < numberOfWards; i++) {
        printf("%-5d | %-20s | %-15.2f | %-10d\n",
               wardId[i], wardName[i], wardDailyRate[i], wardCapacity[i]);
    }
    printf("=================================================================================\n");
}

int getSpecialtyIndex (int id)
{
    for (int i=0; i<numberOfSpecialties; i++)
    {
        if (specialtyId[i] == id)
            return i;
    }
    return -1;
}

int getWardIndex (int id)
{
    for (int i=0; i<numberOfWards; i++)
    {
        if (wardId[i] == id)
            return i;
    }
    return -1;
}

int findAvailableBed(int wardIdx)
{
    int capacity = wardCapacity[wardIdx];
    for (int b=0; b<capacity; b++)
    {
        if (bedOccupancy[wardIdx] == 0)
        {
            return b;
        }
    }
    return -1;
}

float calculateWaitingTime (int specialtyIdx, int currentQueuePosition)
{
    int avgTime = consultationTime[specialtyIdx];
    return (float)(currentQueuePosition * avgTime);
}

void registerPatient()
 {
    if (patientCount >= maxPatients) {
        printf("\nError: Maximum patient limit (%d) reached.\n", maxPatients);
        return;
    }

    int specId, specIdx;
    printf("\n--- Patient Registration ---\n");
    displaySpecialties();

    while (1) {
        printf("Enter Specialty ID (1-4): ");
        if (scanf("%d", &specId) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Enter a number.\n");
            continue;
        }
        specIdx = getSpecialtyIndex(specId);
        if (specIdx == -1) {
            printf("Invalid Specialty ID. Try again.\n");
            continue;
        }
        if (specialtyQueue[specIdx] >= dailyPatientCapacity[specIdx]) {
            printf("Daily patient capacity reached for this specialty. Choose another.\n");
            continue;
        }
        break;
    }

    while (getchar() != '\n');

    int idx = patientCount;
    patientId[idx] = nextPatientId++;

    printf("Enter Patient Name: ");
    fgets(patientName[idx], 50, stdin);

    int len = strlen(patientName[idx]);
    if (len > 0 && patientName[idx][len - 1] == '\n') {
        patientName[idx][len - 1] = '\0';
    }

    while (1) {
        printf("Enter Patient Age: ");
        if (scanf("%d", &patientAge[idx]) == 1 && patientAge[idx] >= 0) {
            break;
        }
        while (getchar() != '\n');
        printf("Invalid age! Age cannot be negative.\n");
    }

    while (1) {
        printf("Enter Urgency Level (1-Emergency, 2-Urgent, 3-Standard): ");
        scanf("%d", &patientUrgency[idx]);
        if (patientUrgency[idx] >= 1 && patientUrgency[idx] <= 3) break;
        printf("Invalid urgency level. Choose 1, 2, or 3.\n");
    }

    patientSpecialty[idx] = specId;
    specialtyQueue[specIdx]++;
    registrationOrder[idx] = idx;

    printf("Is patient being admitted to Ward? (1-Yes, 0-No): ");
    scanf("%d", &patientAdmitted[idx]);

    if (patientAdmitted[idx] == 1) {
        int wId, wIdx;
        displayWards();
        while (1) {
            printf("Enter Ward ID (1-4): ");
            scanf("%d", &wId);
            wIdx = getWardIndex(wId);
            if (wIdx == -1) {
                printf("Invalid Ward ID. Try again.\n");
                continue;
            }
            int bed = findAvailableBed(wIdx);
            if (bed == -1) {
                printf("Ward is full! Choose another ward.\n");
                continue;
            }
            patientWard[idx] = wId;
            patientBed[idx] = bed + 1;
            bedOccupancy[wIdx][bed] = 1;
            break;
        }

        while (1) {
            printf("Enter Expected Days of Stay: ");
            if (scanf("%d", &patientDays[idx]) == 1 && patientDays[idx] > 0) {
                break;
            }
            while (getchar() != '\n');
            printf("Invalid days! Please enter a number greater than 0.\n");
        }
    } else {
        patientWard[idx] = 0;
        patientBed[idx] = 0;
        patientDays[idx] = 0;
    }

    patientCount++;
    printf("\nPatient registered successfully! Assigned Patient ID: %d\n", patientId[idx]);
}

void calculateBilling(int idx)
{
    int specIdx = getSpecialtyIndex(patientSpecialty[idx]);

    baseFee[idx] = consultationFee[specIdx];

    if (patientUrgency[idx] == 1)
    {
        emergencySurcharge[idx] = baseFee[idx] * 0.20f;
    }
    else
    {
        emergencySurcharge[idx] = 0.0f;
    }

    if (patientAdmitted[idx] == 1)
    {
        int wIdx = getWardIndex(patientWard[idx]);
        wardCost[idx] = wardDailyRate[wIdx] * patientDays[idx];
    }
    else
    {
        wardCost[idx] = 0.0f;
    }

    grossTotal[idx] = baseFee[idx] + emergencySurcharge[idx] + wardCost[idx];

    if (patientAge[idx]>=60)
    {
        discount[idx] = grossTotal[idx] * 0.10f;
    }
    else
    {
        discount[idx] = 0.0f;
    }

    finalPayable[idx] = grossTotal[idx] - discount[idx];
}

int main()
{
    printf("Smart Hospital System Loaded.\n");
    return 0;
}

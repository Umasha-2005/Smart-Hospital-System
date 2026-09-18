
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
        if (bedOccupancy[wardIdx][b] == 0)
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
        printf("Please Enter Specialty ID (1-4)\t\t: ");
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

    printf("Please Enter Patient Name\t\t: ");
    fgets(patientName[idx], 50, stdin);

    int len = strlen(patientName[idx]);
    if (len > 0 && patientName[idx][len - 1] == '\n') {
        patientName[idx][len - 1] = '\0';
    }

    while (1) {
        printf("Please Enter Patient Age\t\t: ");
        if (scanf("%d", &patientAge[idx]) == 1 && patientAge[idx] >= 0) {
            break;
        }
        while (getchar() != '\n');
        printf("Invalid age! Age cannot be negative.\n");
    }

    while (1) {
        printf("Please Enter Urgency Level (1-Emergency, 2-Urgent, 3-Standard): ");
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
            printf("Please Enter Ward ID (1-4)\t\t: ");
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
            printf("Please Enter Expected Days of Stay\t\t: ");
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

void displayBillingReceipt(int idx) {
    calculateBilling(idx);

    int specIdx = getSpecialtyIndex(patientSpecialty[idx]);
    char urgencyStr[15];
    if (patientUrgency[idx] == 1)
    {
        strcpy(urgencyStr, "Emergency");
    }
    else if (patientUrgency[idx] == 2)
    {
        strcpy(urgencyStr, "Urgent");
    }
    else
    {
        strcpy(urgencyStr, "Standard");
    }
    printf("\n==================================================\n");
    printf("              PATIENT BILLING RECEIPT             \n");
    printf("==================================================\n");
    printf("Patient ID     : %d\n", patientId[idx]);
    printf("Patient Name   : %s\n", patientName[idx]);
    printf("Age            : %d\n", patientAge[idx]);
    printf("Specialty      : %s\n", specialtyName[specIdx]);
    printf("Urgency Level  : %s\n", urgencyStr);

    if (patientAdmitted[idx] == 1) {
        int wIdx = getWardIndex(patientWard[idx]);
        printf("Ward           : %s (Bed #%d)\n", wardName[wIdx], patientBed[idx]);
        printf("Days of Stay   : %d\n", patientDays[idx]);
    } else {
        printf("Ward Admission : No\n");
    }
    printf("--------------------------------------------------\n");
    printf("Base Consultation Fee   : LKR %10.2f\n", baseFee[idx]);
    printf("Emergency Surcharge     : LKR %10.2f\n", emergencySurcharge[idx]);
    printf("Ward Charges            : LKR %10.2f\n", wardCost[idx]);
    printf("--------------------------------------------------\n");
    printf("Gross Total             : LKR %10.2f\n", grossTotal[idx]);
    printf("Discount                : LKR %10.2f\n", discount[idx]);
    printf("--------------------------------------------------\n");
    printf("FINAL PAYABLE AMOUNT    : LKR %10.2f\n", finalPayable[idx]);
    printf("==================================================\n");
}

int findPatientIndexById(int targetId)
{
    for(int i=0; i<patientCount; i++)
    {
        if(patientId[i] == targetId)
        {
            return i;
        }
    }
    return -1;
}

void searchPatient()
{
    int searchId;
    printf("\nPlease Enter Patient ID to search\t:");

    if (scanf("%d", &searchId) !=1)
    {
        while (getchar() != '\n');
        printf("Invalid ID format.\n");
        return;
    }

    int idx = findPatientIndexById(searchId);
    if (idx == -1)
    {
        printf("Patient with ID %d not found.\n", searchId);
        return;
    }

    displayBillingReceipt(idx);
}

void displayQueue()
{
    if (patientCount == 0)
    {
        printf("\nNo patients currently registered in the system.\n");
        return;
    }

    printf("\n===================================================================================================\n");
    printf("                                  PATIENT CONSULTATION QUEUE\n");
    printf("===================================================================================================\n");
    printf("%-5s | %-8s | %-20s | %-12s | %-18s | %-12s\n",
           "Pos", "ID", "Name", "Urgency", "Specialty", "Est. Wait (m)");
    printf("---------------------------------------------------------------------------------------------------\n");

    int pos = 1;
    for (int level = 1; level <= 3; level++)
    {
        for (int i=0; i<patientCount; i++)
        {
            if (patientUrgency[i] == level)
            {
                int specIdx = getSpecialtyIndex(patientSpecialty[i]);
                float wait = calculateWaitingTime(specIdx, pos - 1);

                char urgStr[12];
                if(level == 1)
                {
                     strcpy(urgStr, "Emergency");
                }
                else if (level == 2)
                {
                    strcpy(urgStr, "Urgent");
                }
                else
                {
                    strcpy(urgStr, "Standard");
                }

                printf("%-5d | %-8d | %-20s | %-12s | %-18s | %-12.1f\n",
                       pos, patientId[i], patientName[i], urgStr, specialtyName[specIdx], wait);
                pos++;

            }
        }
    }

    printf("===================================================================================================\n");

}

void updatePtient()
{
    int targetId;
    printf("\nPlease Enter Patient ID to update\t:");
    if (scanf("%d", &targetId) != 1)
    {
        while (getchar() != '\n');
        printf("Invalid ID.\n");
        return;
    }

    int idx = findPatientIndexById(targetId);
    if (idx == -1)
    {
        printf("Patient with ID %d not found.\n", targetId);
        return;
    }

    printf("\nUpdating records for Patient ID\t: %d (%s)\n", patientId[idx],
            patientName[idx]);

    while (1)
    {
        printf("Please Enter New Urgency Level (1-Emergency, 2-Urgency, 3-Standard): ");
        if (scanf("%d", &patientUrgency[idx]) == 1 && patientUrgency[idx] >= 1 &&
                  patientUrgency[idx] <= 3)
        {
            break;
        }
        while (getchar() != '\n');
        printf("Invalid choice. Enter 1, 2, or 3.\n");
    }
    if (patientAdmitted[idx] == 1)
    {
        while (1)
        {
            printf("Please Enter New Expected Days of stay\t: ");
            if (scanf("%d", &patientDays[idx]) == 1 && patientDays[idx] > 0)
            {
                break;
            }
            while (getchar() != '\n');
            printf ("Invalid days! Please Enter a number greater than 0.\n");
        }
    }

    printf("Patient record update successfully!\n");
}

void deletePatient()
{
    int targetId;
    printf("\nPlease Enter Patient ID to discharge\t: ");
    if (scanf("%d", &targetId) != 1)
    {
        while(getchar() != '\n');
        printf("Invalid ID.\n");
        return;
    }

    int idx = findPatientIndexById(targetId);
    if (idx == -1)
    {
        printf("Patient with ID %d not found.\n", targetId);
        return;
    }

    int specIdx = getSpecialtyIndex(patientSpecialty[idx]);
    if (specIdx != -1 && specialtyQueue[specIdx] > 0)
    {
        specialtyQueue[specIdx]--;
    }

    if (patientAdmitted[idx] == 1)
    {
        int wIdx = getWardIndex(patientWard[idx]);
        int bedNum = patientBed[idx] - 1;
        if (wIdx != -1 && bedNum >= 0 && bedNum < maxBeds)
        {
            bedOccupancy[wIdx][bedNum] = 0;
        }
    }

    for (int i = idx; i < patientCount-1; i++)
    {
        patientId[i] = patientId[i+1];
        strcpy(patientName[i], patientName[i+1]);
        patientAge[i] = patientAge[i + 1];
        patientUrgency[i] = patientUrgency[i + 1];
        patientSpecialty[i] = patientSpecialty[i + 1];
        patientAdmitted[i] = patientAdmitted[i + 1];
        patientWard[i] = patientWard[i + 1];
        patientDays[i] = patientDays[i + 1];
        patientBed[i] = patientBed[i + 1];
        registrationOrder[i] = registrationOrder[i + 1];
    }

    patientCount--;
    printf("Patient ID %d discharged/deleted successfully!\n", targetId);
}

void generateDailyAnalytics()
{
   printf("\n=================================================================================\n");
   printf("                           DAILY HOSPITAL ANALYTICS & REPORTS\n");
   printf("=================================================================================\n");
   printf("Total Patients Registered Today : %d\n", patientCount);

   int emergencyCount = 0, urgentCount = 0, standardCount =0;
   int totalAdmitted = 0;
   float totalRevenue = 0.0f;
   float totalDiscounts = 0.0f;

   for (int i=0; i<patientCount; i++)
   {
       calculateBilling(i);
       totalRevenue += finalPayable[i];
       totalDiscounts += discount[i];

       if (patientUrgency[i] == 1)
       {
           emergencyCount++;
       }
       else if (patientUrgency[i] == 2)
       {
           urgentCount++;
       }
       else
       {
           standardCount++;
       }

       if (patientAdmitted[i] == 1)
       {
           totalAdmitted++;
       }
   }

    printf("Total In-Patient Admissions     : %d\n", totalAdmitted);
    printf("Total Out-Patients              : %d\n", patientCount - totalAdmitted);
    printf("---------------------------------------------------------------------------------\n");
    printf("Urgency Breakdown               : Emergency: %d | Urgent: %d | Standard: %d\n",
           emergencyCount, urgentCount, standardCount);
    printf("---------------------------------------------------------------------------------\n");
    printf("Total Gross Revenue Generated   : LKR %.2f\n", totalRevenue + totalDiscounts);
    printf("Total Discounts Awarded         : LKR %.2f\n", totalDiscounts);
    printf("Total Net Revenue Collected     : LKR %.2f\n", totalRevenue);
    printf("=================================================================================\n");

    printf("\n--- Ward Occupancy Rates ---\n");
    for (int w = 0; w < numberOfWards; w++) {
        int occupied = 0;
        for (int b = 0; b < wardCapacity[w]; b++) {
            if (bedOccupancy[w][b] == 1) occupied++;
        }
        float rate = ((float)occupied / wardCapacity[w]) * 100.0f;
        printf("%-20s : %d / %d Beds Occupied (%.1f%%)\n", wardName[w], occupied, wardCapacity[w], rate);
    }
    printf("=================================================================================\n");
}



int main()
{
    initializeBeds();
    int choice;

    while(1)
    {
        printf("\n==========================================================\n");
        printf("               SMART HOSPITAL MANAGEMENT SYSTEM           \n");
        printf("==========================================================\n");
        printf(" 1. Display Doctor Specialties\n");
        printf(" 2. Display Wards & Bed Status\n");
        printf(" 3. Register New Patient\n");
        printf(" 4. Search Patient & Print Receipt\n");
        printf(" 5. Display Consultation Queue (Priority Order)\n");
        printf(" 6. Update Patient Details\n");
        printf(" 7. Discharge / Delete Patient\n");
        printf(" 8. Generate Daily Analytics & Summary Report\n");
        printf(" 9. Exit System\n");
        printf("==========================================================\n");
        printf("Enter your choice (1-9)\t\t: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n');
            printf("Invalid input! Please enter a number between 1 and 9.\n");
            continue;
        }

        switch (choice)
        {
        case 1:
            displaySpecialties();
            break;
        case 2:
            displayWards();
            break;
        case 3:
            registerPatient();
            break;
        case 4:
            searchPatient();
            break;
        case 5:
            displayQueue();
            break;
        case 6:
            updatePtient();
            break;
        case 7:
            deletePatient();
            break;
        case 8:
            generateDailyAnalytics();
            break;
        case 9:
            printf("\nExiting Smart Hospital system. Thank you!\n");
            return 0;
        default:
            printf("Invalid choice! Please select an option between 1 and 9.\n");
        }
    }
    return 0;
}

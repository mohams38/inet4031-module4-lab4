#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SEATS 24
#define NAME_LEN 30
#define DATA_FILE "reservations.dat"

typedef struct
{
    int seat_id;
    int assigned;
    char last[NAME_LEN];
    char first[NAME_LEN];
} Seat;

void initializeSeats(Seat flight[], int size);
void mainMenu(Seat outbound[], Seat inbound[]);
void flightMenu(Seat flight[], const char flightName[], Seat outbound[], Seat inbound[]);
void showEmptyCount(Seat flight[], int size);
void showEmptySeats(Seat flight[], int size);
void showAlphabeticalSeats(Seat flight[], int size);
void assignSeat(Seat flight[], int size, Seat outbound[], Seat inbound[]);
void deleteSeat(Seat flight[], int size, Seat outbound[], Seat inbound[]);
void clearInputBuffer(void);
char getMenuChoice(void);
void saveData(Seat outbound[], Seat inbound[]);
void loadData(Seat outbound[], Seat inbound[]);

int main(void)
{
    Seat outbound[SEATS];
    Seat inbound[SEATS];

    initializeSeats(outbound, SEATS);
    initializeSeats(inbound, SEATS);

    loadData(outbound, inbound);
    mainMenu(outbound, inbound);

    printf("Program ended.\n");
    return 0;
}

void initializeSeats(Seat flight[], int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        flight[i].seat_id = i + 1;
        flight[i].assigned = 0;
        strcpy(flight[i].last, "");
        strcpy(flight[i].first, "");
    }
}

void loadData(Seat outbound[], Seat inbound[])
{
    FILE *fp = fopen(DATA_FILE, "rb");

    if (fp == NULL)
    {
        printf("No saved reservation data found. Starting with empty seats.\n");
        return;
    }

    fread(outbound, sizeof(Seat), SEATS, fp);
    fread(inbound, sizeof(Seat), SEATS, fp);
    fclose(fp);

    printf("Reservation data loaded from file.\n");
}

void saveData(Seat outbound[], Seat inbound[])
{
    FILE *fp = fopen(DATA_FILE, "wb");

    if (fp == NULL)
    {
        printf("Error: could not save reservation data.\n");
        return;
    }

    fwrite(outbound, sizeof(Seat), SEATS, fp);
    fwrite(inbound, sizeof(Seat), SEATS, fp);
    fclose(fp);

    printf("Reservation data saved.\n");
}

void mainMenu(Seat outbound[], Seat inbound[])
{
    char choice;

    do
    {
        printf("\n===== Colossus Airlines Reservation System =====\n");
        printf("a) Outbound Flight\n");
        printf("b) Inbound Flight\n");
        printf("c) Quit\n");
        printf("Enter your choice: ");

        choice = getMenuChoice();

        switch (choice)
        {
        case 'a':
            flightMenu(outbound, "Outbound Flight", outbound, inbound);
            break;
        case 'b':
            flightMenu(inbound, "Inbound Flight", outbound, inbound);
            break;
        case 'c':
            saveData(outbound, inbound);
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 'c');
}

void flightMenu(Seat flight[], const char flightName[], Seat outbound[], Seat inbound[])
{
    char choice;

    do
    {
        printf("\n===== %s =====\n", flightName);
        printf("a) Show number of empty seats\n");
        printf("b) Show list of empty seats\n");
        printf("c) Show alphabetical list of seats\n");
        printf("d) Assign a customer to a seat\n");
        printf("e) Delete a seat assignment\n");
        printf("f) Return to main menu\n");
        printf("Enter your choice: ");

        choice = getMenuChoice();

        switch (choice)
        {
        case 'a':
            showEmptyCount(flight, SEATS);
            break;
        case 'b':
            showEmptySeats(flight, SEATS);
            break;
        case 'c':
            showAlphabeticalSeats(flight, SEATS);
            break;
        case 'd':
            assignSeat(flight, SEATS, outbound, inbound);
            break;
        case 'e':
            deleteSeat(flight, SEATS, outbound, inbound);
            break;
        case 'f':
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 'f');
}

void showEmptyCount(Seat flight[], int size)
{
    int i, count = 0;

    for (i = 0; i < size; i++)
    {
        if (flight[i].assigned == 0)
        {
            count++;
        }
    }

    printf("Number of empty seats: %d\n", count);
}

void showEmptySeats(Seat flight[], int size)
{
    int i;
    int found = 0;

    printf("Empty seats: ");
    for (i = 0; i < size; i++)
    {
        if (flight[i].assigned == 0)
        {
            printf("%d ", flight[i].seat_id);
            found = 1;
        }
    }

    if (!found)
    {
        printf("None");
    }

    printf("\n");
}

void showAlphabeticalSeats(Seat flight[], int size)
{
    Seat temp[SEATS];
    Seat swap;
    int i, j, count = 0;

    for (i = 0; i < size; i++)
    {
        if (flight[i].assigned == 1)
        {
            temp[count] = flight[i];
            count++;
        }
    }

    if (count == 0)
    {
        printf("No seats are currently assigned.\n");
        return;
    }

    for (i = 0; i < count - 1; i++)
    {
        for (j = i + 1; j < count; j++)
        {
            if (strcmp(temp[i].last, temp[j].last) > 0)
            {
                swap = temp[i];
                temp[i] = temp[j];
                temp[j] = swap;
            }
        }
    }

    printf("\nAssigned seats in alphabetical order:\n");
    printf("%-10s %-10s %-10s\n", "Seat", "Last Name", "First Name");

    for (i = 0; i < count; i++)
    {
        printf("%-10d %-10s %-10s\n",
               temp[i].seat_id,
               temp[i].last,
               temp[i].first);
    }
}

void assignSeat(Seat flight[], int size, Seat outbound[], Seat inbound[])
{
    int seatNum;
    char first[NAME_LEN];
    char last[NAME_LEN];

    showEmptySeats(flight, size);

    printf("Enter seat number to assign (0 to abort): ");
    if (scanf("%d", &seatNum) != 1)
    {
        printf("Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (seatNum == 0)
    {
        printf("Assignment canceled.\n");
        return;
    }

    if (seatNum < 1 || seatNum > size)
    {
        printf("Invalid seat number.\n");
        return;
    }

    if (flight[seatNum - 1].assigned == 1)
    {
        printf("That seat is already assigned.\n");
        return;
    }

    printf("Enter first name (or q to abort): ");
    scanf("%29s", first);
    clearInputBuffer();

    if (strcmp(first, "q") == 0 || strcmp(first, "Q") == 0)
    {
        printf("Assignment canceled.\n");
        return;
    }

    printf("Enter last name (or q to abort): ");
    scanf("%29s", last);
    clearInputBuffer();

    if (strcmp(last, "q") == 0 || strcmp(last, "Q") == 0)
    {
        printf("Assignment canceled.\n");
        return;
    }

    strcpy(flight[seatNum - 1].first, first);
    strcpy(flight[seatNum - 1].last, last);
    flight[seatNum - 1].assigned = 1;

    printf("Seat %d assigned to %s %s.\n", seatNum, first, last);
    saveData(outbound, inbound);
}

void deleteSeat(Seat flight[], int size, Seat outbound[], Seat inbound[])
{
    int seatNum;
    int i;

    printf("\nAssigned seats:\n");
    for (i = 0; i < size; i++)
    {
        if (flight[i].assigned == 1)
        {
            printf("Seat %d: %s %s\n",
                   flight[i].seat_id,
                   flight[i].first,
                   flight[i].last);
        }
    }

    printf("Enter seat number to delete (0 to abort): ");
    if (scanf("%d", &seatNum) != 1)
    {
        printf("Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (seatNum == 0)
    {
        printf("Deletion canceled.\n");
        return;
    }

    if (seatNum < 1 || seatNum > size)
    {
        printf("Invalid seat number.\n");
        return;
    }

    if (flight[seatNum - 1].assigned == 0)
    {
        printf("That seat is already empty.\n");
        return;
    }

    flight[seatNum - 1].assigned = 0;
    strcpy(flight[seatNum - 1].first, "");
    strcpy(flight[seatNum - 1].last, "");

    printf("Seat %d assignment deleted.\n", seatNum);
    saveData(outbound, inbound);
}

void clearInputBuffer(void)
{
    while (getchar() != '\n')
    {
        continue;
    }
}

char getMenuChoice(void)
{
    char choice;
    scanf(" %c", &choice);
    clearInputBuffer();
    return tolower(choice);
}
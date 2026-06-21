#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEATS 40
#define MAX_STR 50

// ==========================================
// DATA STRUCTURES
// ==========================================

typedef struct {
    int customerID;
    char name[MAX_STR];
    char cnic[MAX_STR];
    int age;
    char phone[MAX_STR];
    char password[MAX_STR];
} Customer;

typedef struct {
    int busID;
    char busName[MAX_STR];
    char route[MAX_STR];
    char date[MAX_STR];
    char time[MAX_STR];
    float fare;
    int totalSeats;
    int availableSeats;
    int seats[MAX_SEATS]; // 0: available, 1: reserved
} Bus;

typedef struct {
    int ticketID;
    int customerID;
    int busID;
    int seatNumber;
    char status[MAX_STR]; // Pending, Approved, Rejected, Cancelled
} Booking;

typedef struct {
    int paymentID;
    int ticketID;
    float amount;
    char method[MAX_STR]; // EasyPaisa, JazzCash, Credit Card
    char status[MAX_STR]; // Pending, Approved, Rejected
} Payment;

// ==========================================
// GLOBAL VARIABLES & FORWARD DECLARATIONS
// ==========================================

Customer currentCustomer;
int isAdmin = 0;
int isOperator = 0;

void mainMenu();
void adminMenu();
void adminLogin();
void operatorLogin();
void operatorMenu();
void customerSignup();
void customerLogin();
void customerMenu();
void addBus();
void viewAllBuses();
void manageAdminBookings();
void manageAdminPayments();
void searchAndBook();
void viewBookingStatus();
void cancelTicket();
void viewCustomerHistory();
void clearScreen();
void header(const char* title);
int getNextID(const char* filename, size_t structSize);

// ==========================================
// UTILITY FUNCTIONS
// ==========================================

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void header(const char* title) {
    printf("\n==================================================\n");
    printf("           BUS SEAT RESERVATION SYSTEM            \n");
    printf("                 %s\n", title);
    printf("==================================================\n\n");
}

int getNextID(const char* filename, size_t structSize) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return 1;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return (int)(size / structSize) + 1;
}

// ==========================================
// MAIN FUNCTION
// ==========================================

int main() {
    mainMenu();
    return 0;
}

void mainMenu() {
    int choice;
    while(1) {
        clearScreen();
        header("MAIN MENU");
        printf("1. Admin Login\n");
        printf("2. Bus Operator Login\n");
        printf("3. Customer Signup\n");
        printf("4. Customer Login\n");
        printf("5. Exit\n\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            continue;
        }
        getchar(); // consume newline

        switch(choice) {
            case 1: adminLogin(); break;
            case 2: operatorLogin(); break;
            case 3: customerSignup(); break;
            case 4: customerLogin(); break;
            case 5:
                printf("Exiting... Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice! Press enter to continue.");
                getchar();
        }
    }
}

// ==========================================
// ADMIN MODULE
// ==========================================

void adminLogin() {
    char user[MAX_STR], pass[MAX_STR];
    clearScreen();
    header("ADMIN LOGIN");
    printf("Username: ");
    fgets(user, MAX_STR, stdin); user[strcspn(user, "\n")] = 0;
    printf("Password: ");
    fgets(pass, MAX_STR, stdin); pass[strcspn(pass, "\n")] = 0;

    if (strcmp(user, "admin") == 0 && strcmp(pass, "admin123") == 0) {
        isAdmin = 1;
        adminMenu();
    } else {
        printf("Invalid credentials!\n");
        getchar();
    }
}

void adminMenu() {
    int choice;
    while(isAdmin) {
        clearScreen();
        header("ADMIN DASHBOARD");
        printf("1. Add Bus\n");
        printf("2. View All Buses\n");
        printf("3. Manage Bookings\n");
        printf("4. Manage Payments\n");
        printf("5. Logout\n\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch(choice) {
            case 1: addBus(); break;
            case 2: viewAllBuses(); break;
            case 3: manageAdminBookings(); break;
            case 4: manageAdminPayments(); break;
            case 5: isAdmin = 0; break;
            default: printf("Invalid choice!\n"); getchar();
        }
    }
}

void addBus() {
    Bus b;
    FILE *fp = fopen("buses.dat", "ab");
    if (!fp) return;

    b.busID = getNextID("buses.dat", sizeof(Bus));
    printf("Enter Bus Name: ");
    fgets(b.busName, MAX_STR, stdin); b.busName[strcspn(b.busName, "\n")] = 0;
    printf("Enter Route: ");
    fgets(b.route, MAX_STR, stdin); b.route[strcspn(b.route, "\n")] = 0;
    printf("Enter Date (DD/MM/YYYY): ");
    fgets(b.date, MAX_STR, stdin); b.date[strcspn(b.date, "\n")] = 0;
    printf("Enter Time (HH:MM): ");
    fgets(b.time, MAX_STR, stdin); b.time[strcspn(b.time, "\n")] = 0;
    printf("Enter Fare: ");
    scanf("%f", &b.fare);
    b.totalSeats = MAX_SEATS;
    b.availableSeats = MAX_SEATS;
    for(int i=0; i<MAX_SEATS; i++) b.seats[i] = 0;

    fwrite(&b, sizeof(Bus), 1, fp);
    fclose(fp);
    printf("Bus added successfully! ID: %d\n", b.busID);
    getchar(); getchar();
}

void viewAllBuses() {
    Bus b;
    FILE *fp = fopen("buses.dat", "rb");
    if (!fp) {
        printf("No buses found.\n");
        getchar();
        return;
    }
    printf("%-5s %-15s %-15s %-12s %-8s %-8s %-5s\n", "ID", "Name", "Route", "Date", "Time", "Fare", "Avail");
    while(fread(&b, sizeof(Bus), 1, fp)) {
        printf("%-5d %-15s %-15s %-12s %-8s %-8.2f %-5d\n", b.busID, b.busName, b.route, b.date, b.time, b.fare, b.availableSeats);
    }
    fclose(fp);
    printf("\nPress Enter to continue...");
    getchar();
}

void manageAdminBookings() {
    Booking bk;
    FILE *fp = fopen("bookings.dat", "rb+");
    if (!fp) {
        printf("No bookings found.\n");
        getchar();
        return;
    }

    printf("%-10s %-10s %-10s %-10s %-10s\n", "TicketID", "CustID", "BusID", "Seat", "Status");
    while(fread(&bk, sizeof(Booking), 1, fp)) {
        printf("%-10d %-10d %-10d %-10d %-10s\n", bk.ticketID, bk.customerID, bk.busID, bk.seatNumber, bk.status);
    }

    int tid;
    printf("\nEnter Ticket ID to Approve/Reject (0 to cancel): ");
    scanf("%d", &tid);
    if (tid == 0) { fclose(fp); return; }

    rewind(fp);
    int found = 0;
    long pos;
    while((pos = ftell(fp)), fread(&bk, sizeof(Booking), 1, fp)) {
        if (bk.ticketID == tid) {
            found = 1;
            printf("1. Approve\n2. Reject\nChoice: ");
            int c;
            scanf("%d", &c);
            if (c == 1) strcpy(bk.status, "Approved");
            else strcpy(bk.status, "Rejected");
            
            fseek(fp, pos, SEEK_SET);
            fwrite(&bk, sizeof(Booking), 1, fp);
            printf("Status updated!\n");
            break;
        }
    }
    if(!found) printf("Ticket not found!\n");
    fclose(fp);
    getchar(); getchar();
}

void manageAdminPayments() {
    Payment p;
    FILE *fp = fopen("payments.dat", "rb+");
    if (!fp) {
        printf("No payments found.\n");
        getchar();
        return;
    }

    printf("%-10s %-10s %-10s %-15s %-10s\n", "PayID", "TicketID", "Amount", "Method", "Status");
    while(fread(&p, sizeof(Payment), 1, fp)) {
        printf("%-10d %-10d %-10.2f %-15s %-10s\n", p.paymentID, p.ticketID, p.amount, p.method, p.status);
    }

    int pid;
    printf("\nEnter Payment ID to Approve/Reject (0 to cancel): ");
    scanf("%d", &pid);
    if (pid == 0) { fclose(fp); return; }

    rewind(fp);
    int found = 0;
    long pos;
    while((pos = ftell(fp)), fread(&p, sizeof(Payment), 1, fp)) {
        if (p.paymentID == pid) {
            found = 1;
            printf("1. Approve\n2. Reject\nChoice: ");
            int c;
            scanf("%d", &c);
            if (c == 1) strcpy(p.status, "Approved");
            else strcpy(p.status, "Rejected");
            
            fseek(fp, pos, SEEK_SET);
            fwrite(&p, sizeof(Payment), 1, fp);
            printf("Payment updated!\n");
            break;
        }
    }
    if(!found) printf("Payment not found!\n");
    fclose(fp);
    getchar(); getchar();
}

// ==========================================
// OPERATOR MODULE
// ==========================================

void operatorLogin() {
    char user[MAX_STR], pass[MAX_STR];
    clearScreen();
    header("OPERATOR LOGIN");
    printf("Username: ");
    fgets(user, MAX_STR, stdin); user[strcspn(user, "\n")] = 0;
    printf("Password: ");
    fgets(pass, MAX_STR, stdin); pass[strcspn(pass, "\n")] = 0;

    if (strcmp(user, "operator") == 0 && strcmp(pass, "op123") == 0) {
        isOperator = 1;
        operatorMenu();
    } else {
        printf("Invalid credentials!\n");
        getchar();
    }
}

void operatorMenu() {
    int choice;
    while(isOperator) {
        clearScreen();
        header("OPERATOR DASHBOARD");
        printf("1. Add Bus\n");
        printf("2. View All Buses\n");
        printf("3. View Booking Requests\n");
        printf("4. Logout\n\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch(choice) {
            case 1: addBus(); break;
            case 2: viewAllBuses(); break;
            case 3: manageAdminBookings(); break;
            case 4: isOperator = 0; break;
            default: printf("Invalid choice!\n"); getchar();
        }
    }
}

// ==========================================
// CUSTOMER MODULE
// ==========================================

void customerSignup() {
    Customer c;
    FILE *fp = fopen("customers.dat", "ab");
    if (!fp) return;

    c.customerID = getNextID("customers.dat", sizeof(Customer));
    clearScreen();
    header("CUSTOMER SIGNUP");
    printf("Full Name: ");
    fgets(c.name, MAX_STR, stdin); c.name[strcspn(c.name, "\n")] = 0;
    printf("CNIC: ");
    fgets(c.cnic, MAX_STR, stdin); c.cnic[strcspn(c.cnic, "\n")] = 0;
    printf("Age: ");
    scanf("%d", &c.age); getchar();
    printf("Phone: ");
    fgets(c.phone, MAX_STR, stdin); c.phone[strcspn(c.phone, "\n")] = 0;
    printf("Password: ");
    fgets(c.password, MAX_STR, stdin); c.password[strcspn(c.password, "\n")] = 0;

    fwrite(&c, sizeof(Customer), 1, fp);
    fclose(fp);
    printf("Signup successful! ID: %d\n", c.customerID);
    getchar();
}

void customerLogin() {
    char cnic[MAX_STR], pass[MAX_STR];
    Customer c;
    FILE *fp = fopen("customers.dat", "rb");
    clearScreen();
    header("CUSTOMER LOGIN");
    printf("CNIC: ");
    fgets(cnic, MAX_STR, stdin); cnic[strcspn(cnic, "\n")] = 0;
    printf("Password: ");
    fgets(pass, MAX_STR, stdin); pass[strcspn(pass, "\n")] = 0;

    if (!fp) {
        printf("No customers registered yet!\n");
        getchar();
        return;
    }

    int found = 0;
    while(fread(&c, sizeof(Customer), 1, fp)) {
        if (strcmp(c.cnic, cnic) == 0 && strcmp(c.password, pass) == 0) {
            currentCustomer = c;
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (found) {
        customerMenu();
    } else {
        printf("Invalid CNIC or password!\n");
        getchar();
    }
}

void customerMenu() {
    int choice;
    while(1) {
        clearScreen();
        header("CUSTOMER DASHBOARD");
        printf("Welcome, %s\n\n", currentCustomer.name);
        printf("1. Search Bus & Book Ticket\n");
        printf("2. View Booking Status\n");
        printf("3. Cancel Ticket\n");
        printf("4. View History\n");
        printf("5. Logout\n\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch(choice) {
            case 1: searchAndBook(); break;
            case 2: viewBookingStatus(); break;
            case 3: cancelTicket(); break;
            case 4: viewCustomerHistory(); break;
            case 5: return;
            default: printf("Invalid choice!\n"); getchar();
        }
    }
}

void searchAndBook() {
    Bus b;
    char route[MAX_STR];
    FILE *fp = fopen("buses.dat", "rb+");
    if (!fp) {
        printf("No buses available.\n");
        getchar();
        return;
    }

    printf("Enter Route to search: ");
    fgets(route, MAX_STR, stdin); route[strcspn(route, "\n")] = 0;

    int found = 0;
    printf("\n%-5s %-15s %-12s %-8s %-8s %-5s\n", "ID", "Name", "Date", "Time", "Fare", "Avail");
    while(fread(&b, sizeof(Bus), 1, fp)) {
        if (strstr(b.route, route)) {
            printf("%-5d %-15s %-12s %-8s %-8.2f %-5d\n", b.busID, b.busName, b.date, b.time, b.fare, b.availableSeats);
            found = 1;
        }
    }

    if (!found) {
        printf("No buses found for this route.\n");
        fclose(fp);
        getchar();
        return;
    }

    int busID, seatNum;
    printf("\nEnter Bus ID to book: ");
    scanf("%d", &busID);
    
    rewind(fp);
    found = 0;
    long pos;
    while((pos = ftell(fp)), fread(&b, sizeof(Bus), 1, fp)) {
        if (b.busID == busID) {
            found = 1;
            if (b.availableSeats <= 0) {
                printf("Sorry, no seats available!\n");
                break;
            }
            printf("Available Seats: ");
            for(int i=0; i<MAX_SEATS; i++) if(b.seats[i] == 0) printf("%d ", i+1);
            printf("\nEnter Seat Number: ");
            scanf("%d", &seatNum);

            if (seatNum < 1 || seatNum > MAX_SEATS || b.seats[seatNum-1] == 1) {
                printf("Invalid or already reserved seat!\n");
            } else {
                b.seats[seatNum-1] = 1;
                b.availableSeats--;
                fseek(fp, pos, SEEK_SET);
                fwrite(&b, sizeof(Bus), 1, fp);

                Booking bk;
                bk.ticketID = getNextID("bookings.dat", sizeof(Booking));
                bk.customerID = currentCustomer.customerID;
                bk.busID = b.busID;
                bk.seatNumber = seatNum;
                strcpy(bk.status, "Pending");
                
                FILE *bfp = fopen("bookings.dat", "ab");
                fwrite(&bk, sizeof(Booking), 1, bfp);
                fclose(bfp);

                Payment p;
                p.paymentID = getNextID("payments.dat", sizeof(Payment));
                p.ticketID = bk.ticketID;
                p.amount = b.fare;
                printf("\nSelect Payment Method:\n1. EasyPaisa\n2. JazzCash\n3. Credit Card\nChoice: ");
                int pchoice;
                scanf("%d", &pchoice);
                if(pchoice == 1) strcpy(p.method, "EasyPaisa");
                else if(pchoice == 2) strcpy(p.method, "JazzCash");
                else strcpy(p.method, "Credit Card");
                
                strcpy(p.status, "Pending");
                FILE *pfp = fopen("payments.dat", "ab");
                fwrite(&p, sizeof(Payment), 1, pfp);
                fclose(pfp);

                printf("\nBooking successful! Ticket ID: %d. Status: Pending approval.\n", bk.ticketID);
            }
            break;
        }
    }
    fclose(fp);
    getchar(); getchar();
}

void viewBookingStatus() {
    Booking bk;
    FILE *bfp = fopen("bookings.dat", "rb");
    if (!bfp) {
        printf("No bookings found.\n");
        getchar();
        return;
    }

    printf("%-10s %-10s %-10s %-10s\n", "TicketID", "BusID", "Seat", "Status");
    while(fread(&bk, sizeof(Booking), 1, bfp)) {
        if (bk.customerID == currentCustomer.customerID) {
            printf("%-10d %-10d %-10d %-10s\n", bk.ticketID, bk.busID, bk.seatNumber, bk.status);
        }
    }
    fclose(bfp);
    printf("\nPress Enter to continue...");
    getchar();
}

void cancelTicket() {
    Booking bk;
    Bus b;
    FILE *bfp = fopen("bookings.dat", "rb+");
    if (!bfp) {
        printf("No bookings found.\n");
        getchar();
        return;
    }

    printf("%-10s %-10s %-10s %-10s\n", "TicketID", "BusID", "Seat", "Status");
    int count = 0;
    while(fread(&bk, sizeof(Booking), 1, bfp)) {
        if (bk.customerID == currentCustomer.customerID && strcmp(bk.status, "Cancelled") != 0) {
            printf("%-10d %-10d %-10d %-10s\n", bk.ticketID, bk.busID, bk.seatNumber, bk.status);
            count++;
        }
    }

    if (count == 0) {
        printf("No active bookings to cancel.\n");
        fclose(bfp);
        getchar();
        return;
    }

    int tid;
    printf("\nEnter Ticket ID to Cancel: ");
    scanf("%d", &tid);

    rewind(bfp);
    int found = 0;
    long bpos;
    while((bpos = ftell(bfp)), fread(&bk, sizeof(Booking), 1, bfp)) {
        if (bk.ticketID == tid && bk.customerID == currentCustomer.customerID) {
            found = 1;
            strcpy(bk.status, "Cancelled");
            fseek(bfp, bpos, SEEK_SET);
            fwrite(&bk, sizeof(Booking), 1, bfp);

            FILE *busfp = fopen("buses.dat", "rb+");
            while(fread(&b, sizeof(Bus), 1, busfp)) {
                if (b.busID == bk.busID) {
                    b.seats[bk.seatNumber-1] = 0;
                    b.availableSeats++;
                    fseek(busfp, -(long)sizeof(Bus), SEEK_CUR);
                    fwrite(&b, sizeof(Bus), 1, busfp);
                    break;
                }
            }
            fclose(busfp);

            printf("Ticket cancelled successfully!\n");
            printf("Refund Policy: 70%% refund processed (30%% deduction).\n");
            break;
        }
    }
    if(!found) printf("Ticket not found or not yours!\n");
    fclose(bfp);
    getchar(); getchar();
}

void viewCustomerHistory() {
    Booking bk;
    Payment p;
    FILE *bfp = fopen("bookings.dat", "rb");
    FILE *pfp = fopen("payments.dat", "rb");
    
    printf("\n--- BOOKING HISTORY ---\n");
    if (bfp) {
        printf("%-10s %-10s %-10s %-10s\n", "TicketID", "BusID", "Seat", "Status");
        while(fread(&bk, sizeof(Booking), 1, bfp)) {
            if (bk.customerID == currentCustomer.customerID)
                printf("%-10d %-10d %-10d %-10s\n", bk.ticketID, bk.busID, bk.seatNumber, bk.status);
        }
        fclose(bfp);
    }

    printf("\n--- PAYMENT HISTORY ---\n");
    if (pfp) {
        printf("%-10s %-10s %-10s %-15s %-10s\n", "PayID", "TicketID", "Amount", "Method", "Status");
        while(fread(&p, sizeof(Payment), 1, pfp)) {
            printf("%-10d %-10d %-10.2f %-15s %-10s\n", p.paymentID, p.ticketID, p.amount, p.method, p.status);
        }
        fclose(pfp);
    }
    printf("\nPress Enter to continue...");
    getchar();
}

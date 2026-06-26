#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEATS 40
#define MAX_STR 50

typedef struct {
    int operatorID;
    char username[MAX_STR];
    char password[MAX_STR];
    char name[MAX_STR];
} Operator;

typedef struct {
    int customerID;
    char name[MAX_STR];
    char cnic[MAX_STR];
    int age;
    char phone[MAX_STR];
    char email[MAX_STR]; 
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
    int seats[MAX_SEATS]; 
} Bus;

typedef struct {
    int ticketID;
    int customerID;
    int busID;
    int seatNumber;
    char status[MAX_STR]; 
} Booking;

typedef struct {
    int paymentID;
    int ticketID;
    float amount;
    char method[MAX_STR]; 
    char status[MAX_STR]; 
} Payment;

Customer currentCustomer;
int isAdmin = 0;
int isOperator = 0;

void mainMenu();
void clearScreen();
void header(const char* title);
int getNextID(const char* filename, size_t structSize);

void adminLogin();
void adminMenu();
void adminAddOperator();
void adminViewOperators();
void adminSearchOperator();
void adminEditOperator();
void adminDeleteOperator();

void operatorLogin();
void operatorMenu();
void operatorAddBus();
void operatorViewBuses();
void operatorSearchBus();
void operatorEditBus();
void operatorDeleteBus();
void operatorManageBookings();
void operatorManagePayments();
void operatorViewHistory();

void customerSignup();
void customerLogin();
void customerResetPassword();
void customerMenu();
void viewFacilities();
void customerSearchAndBook();
void customerCheckStatus();
void customerCancelTicket();
void customerViewHistory();

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
        printf("5. Forgot Password (Reset)\n");
        printf("6. Exit\n\n");
        printf("Enter your choice (1-6): ");
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            continue;
        }
        getchar(); 
        switch(choice) {
            case 1: adminLogin(); break;
            case 2: operatorLogin(); break;
            case 3: customerSignup(); break;
            case 4: customerLogin(); break;
            case 5: customerResetPassword(); break;
            case 6:
                printf("\nExiting system. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice! Press Enter to retry.");
                getchar();
        }
    }
}

void adminLogin() {
    char user[MAX_STR], pass[MAX_STR];
    clearScreen();
    header("ADMIN LOGIN");
    printf("Enter Username: ");
    fgets(user, MAX_STR, stdin); user[strcspn(user, "\n")] = 0;
    printf("Enter Password: ");
    fgets(pass, MAX_STR, stdin); pass[strcspn(pass, "\n")] = 0;
    
    if (strcmp(user, "admin") == 0 && strcmp(pass, "admin123") == 0) {
        isAdmin = 1;
        adminMenu();
    } else {
        printf("\nInvalid Admin credentials! Press Enter.");
        getchar();
    }
}

void adminMenu() {
    int choice;
    while(isAdmin) {
        clearScreen();
        header("ADMIN MENU");
        printf("1. Add Bus Operator Account\n");
        printf("2. View Registered Operators\n");
        printf("3. Search Operator Profile\n");
        printf("4. Edit Operator Details\n");
        printf("5. Delete Operator Account\n");
        printf("6. Logout\n\n");
        printf("Enter choice (1-6): ");
        scanf("%d", &choice); getchar();
        
        switch(choice) {
            case 1: adminAddOperator(); break;
            case 2: adminViewOperators(); break;
            case 3: adminSearchOperator(); break;
            case 4: adminEditOperator(); break;
            case 5: adminDeleteOperator(); break;
            case 6: isAdmin = 0; break;
            default: printf("Invalid choice! Press Enter."); getchar();
        }
    }
}

void adminAddOperator() {
    Operator op;
    FILE *fp = fopen("operators.dat", "ab");
    if (!fp) return;
    
    op.operatorID = getNextID("operators.dat", sizeof(Operator));
    printf("Enter Operator Name: ");
    fgets(op.name, MAX_STR, stdin); op.name[strcspn(op.name, "\n")] = 0;
    printf("Create Username: ");
    fgets(op.username, MAX_STR, stdin); op.username[strcspn(op.username, "\n")] = 0;
    printf("Create Password: ");
    fgets(op.password, MAX_STR, stdin); op.password[strcspn(op.password, "\n")] = 0;
    
    fwrite(&op, sizeof(Operator), 1, fp);
    fclose(fp);
    printf("\nOperator added successfully! Operator ID: %d\n", op.operatorID);
    printf("Press Enter to continue..."); getchar();
}

void adminViewOperators() {
    Operator op;
    FILE *fp = fopen("operators.dat", "rb");
    clearScreen();
    header("VIEW OPERATORS");
    if (!fp) {
        printf("No operators registered yet.\n");
        printf("Press Enter..."); getchar();
        return;
    }
    printf("%-10s %-20s %-20s\n", "ID", "Operator Name", "Username");
    printf("------------------------------------------------------------------\n");
    while(fread(&op, sizeof(Operator), 1, fp)) {
        printf("%-10d %-20s %-20s\n", op.operatorID, op.name, op.username);
    }
    fclose(fp);
    printf("\nPress Enter to return..."); getchar();
}

void adminSearchOperator() {
    Operator op;
    char target[MAX_STR];
    clearScreen();
    header("SEARCH OPERATOR");
    FILE *fp = fopen("operators.dat", "rb");
    if (!fp) {
        printf("No operators registered yet.\n");
        printf("Press Enter..."); getchar();
        return;
    }
    printf("Enter Operator Name or Username to search: ");
    fgets(target, MAX_STR, stdin); target[strcspn(target, "\n")] = 0;
    
    int match = 0;
    printf("\n%-10s %-20s %-20s\n", "ID", "Operator Name", "Username");
    printf("------------------------------------------------------------------\n");
    while(fread(&op, sizeof(Operator), 1, fp)) {
        if (strstr(op.name, target) || strstr(op.username, target)) {
            printf("%-10d %-20s %-20s\n", op.operatorID, op.name, op.username);
            match = 1;
        }
    }
    if(!match) printf("\nNo matching operator found.\n");
    fclose(fp);
    printf("\nPress Enter to continue..."); getchar();
}

void adminEditOperator() {
    Operator op;
    int targetID, found = 0;
    clearScreen();
    header("EDIT OPERATOR");
    FILE *fp = fopen("operators.dat", "rb+");
    if (!fp) return;
    
    printf("Enter Operator ID to update: ");
    scanf("%d", &targetID); getchar();
    
    long pos;
    while((pos = ftell(fp)), fread(&op, sizeof(Operator), 1, fp)) {
        if (op.operatorID == targetID) {
            found = 1;
            printf("Enter New Name: ");
            fgets(op.name, MAX_STR, stdin); op.name[strcspn(op.name, "\n")] = 0;
            printf("Enter New Username: ");
            fgets(op.username, MAX_STR, stdin); op.username[strcspn(op.username, "\n")] = 0;
            printf("Enter New Password: ");
            fgets(op.password, MAX_STR, stdin); op.password[strcspn(op.password, "\n")] = 0;
            
            fseek(fp, pos, SEEK_SET);
            fwrite(&op, sizeof(Operator), 1, fp);
            printf("\nOperator updated successfully!\n");
            break;
        }
    }
    if(!found) printf("Operator ID not found.\n");
    fclose(fp);
    printf("Press Enter..."); getchar();
}

void adminDeleteOperator() {
    Operator op;
    int targetID, found = 0;
    clearScreen();
    header("DELETE OPERATOR");
    FILE *fp = fopen("operators.dat", "rb");
    FILE *temp = fopen("temp_op.dat", "wb");
    if (!fp || !temp) return;
    
    printf("Enter Operator ID to delete: ");
    scanf("%d", &targetID); getchar();
    
    while(fread(&op, sizeof(Operator), 1, fp)) {
        if (op.operatorID == targetID) {
            found = 1;
        } else {
            fwrite(&op, sizeof(Operator), 1, temp);
        }
    }
    fclose(fp);
    fclose(temp);
    remove("operators.dat");
    rename("temp_op.dat", "operators.dat");
    
    if (found) printf("Operator account deleted successfully.\n");
    else printf("Operator ID not found.\n");
    printf("Press Enter..."); getchar();
}

void operatorLogin() {
    char user[MAX_STR], pass[MAX_STR];
    Operator op;
    clearScreen();
    header("OPERATOR LOGIN");
    printf("Enter Username: ");
    fgets(user, MAX_STR, stdin); user[strcspn(user, "\n")] = 0;
    printf("Enter Password: ");
    fgets(pass, MAX_STR, stdin); pass[strcspn(pass, "\n")] = 0;
    
    FILE *fp = fopen("operators.dat", "rb");
    if (!fp) {
        if (strcmp(user, "operator") == 0 && strcmp(pass, "op123") == 0) {
            isOperator = 1;
            operatorMenu();
            return;
        }
        printf("No operators registered. Please contact Admin.\n");
        printf("Press Enter..."); getchar();
        return;
    }
    
    int found = 0;
    while(fread(&op, sizeof(Operator), 1, fp)) {
        if (strcmp(op.username, user) == 0 && strcmp(op.password, pass) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp);
    
    if (found) {
        isOperator = 1;
        operatorMenu();
    } else {
        printf("\nInvalid credentials! Press Enter.");
        getchar();
    }
}

void operatorMenu() {
    int choice;
    while(isOperator) {
        clearScreen();
        header("OPERATOR MENU");
        printf("1. Add New Bus\n");
        printf("2. View All Buses\n");
        printf("3. Search Bus Route\n");
        printf("4. Edit Bus Details\n");
        printf("5. Delete Bus Route\n");
        printf("6. Manage Booking Requests\n");
        printf("7. Verify Payment Receipts\n");
        printf("8. View Complete System History\n");
        printf("9. Logout\n\n");
        printf("Enter choice (1-9): ");
        scanf("%d", &choice); getchar();
        
        switch(choice) {
            case 1: operatorAddBus(); break;
            case 2: operatorViewBuses(); break;
            case 3: operatorSearchBus(); break;
            case 4: operatorEditBus(); break;
            case 5: operatorDeleteBus(); break;
            case 6: operatorManageBookings(); break;
            case 7: operatorManagePayments(); break;
            case 8: operatorViewHistory(); break;
            case 9: isOperator = 0; break;
            default: printf("Invalid choice! Press Enter."); getchar();
        }
    }
}

void operatorAddBus() {
    Bus b;
    FILE *fp = fopen("buses.dat", "ab");
    if (!fp) return;
    b.busID = getNextID("buses.dat", sizeof(Bus));
    printf("Enter Bus Name: ");
    fgets(b.busName, MAX_STR, stdin); b.busName[strcspn(b.busName, "\n")] = 0;
    printf("Enter Route: ");
    fgets(b.route, MAX_STR, stdin); b.route[strcspn(b.route, "\n")] = 0;
    printf("Enter Departure Date (DD/MM/YYYY): ");
    fgets(b.date, MAX_STR, stdin); b.date[strcspn(b.date, "\n")] = 0;
    printf("Enter Departure Time (HH:MM): ");
    fgets(b.time, MAX_STR, stdin); b.time[strcspn(b.time, "\n")] = 0;
    printf("Enter Ticket Fare (PKR): ");
    scanf("%f", &b.fare); getchar();
    
    b.totalSeats = MAX_SEATS;
    b.availableSeats = MAX_SEATS;
    for(int i=0; i<MAX_SEATS; i++) b.seats[i] = 0; 
    
    fwrite(&b, sizeof(Bus), 1, fp);
    fclose(fp);
    printf("\nBus added successfully! Bus ID: %d\n", b.busID);
    printf("Press Enter to continue..."); getchar();
}

void operatorViewBuses() {
    Bus b;
    FILE *fp = fopen("buses.dat", "rb");
    clearScreen();
    header("VIEW FLEET LOGS");
    if (!fp) {
        printf("No buses found in records.\n");
        printf("Press Enter..."); getchar();
        return;
    }
    printf("%-5s %-15s %-15s %-12s %-8s %-10s %-5s\n", "ID", "Bus Name", "Route", "Date", "Time", "Fare", "Available Seats");
    printf("----------------------------------------------------------------------------------------\n");
    while(fread(&b, sizeof(Bus), 1, fp)) {
        printf("%-5d %-15s %-15s %-12s %-8s %-10.2f %-5d\n", b.busID, b.busName, b.route, b.date, b.time, b.fare, b.availableSeats);
    }
    fclose(fp);
    printf("\nPress Enter to return..."); getchar();
}

void operatorSearchBus() {
    Bus b;
    char targetRoute[MAX_STR];
    clearScreen();
    header("SEARCH BUS ROUTE");
    FILE *fp = fopen("buses.dat", "rb");
    if (!fp) {
        printf("No buses registered yet.\n");
        printf("Press Enter..."); getchar();
        return;
    }
    printf("Enter Route to search: ");
    fgets(targetRoute, MAX_STR, stdin); targetRoute[strcspn(targetRoute, "\n")] = 0;
    
    int found = 0;
    printf("\n%-5s %-15s %-15s %-12s %-8s %-10s\n", "ID", "Bus Name", "Route", "Date", "Time", "Fare");
    printf("--------------------------------------------------------------------------------\n");
    while(fread(&b, sizeof(Bus), 1, fp)) {
        if (strstr(b.route, targetRoute)) {
            printf("%-5d %-15s %-15s %-12s %-8s %-10.2f\n", b.busID, b.busName, b.route, b.date, b.time, b.fare);
            found = 1;
        }
    }
    if(!found) printf("No buses found for this route.\n");
    fclose(fp);
    printf("\nPress Enter to continue..."); getchar();
}

void operatorEditBus() {
    Bus b;
    int targetID, found = 0;
    clearScreen();
    header("EDIT BUS DETAILS");
    FILE *fp = fopen("buses.dat", "rb+");
    if (!fp) return;
    
    printf("Enter Bus ID to edit: ");
    scanf("%d", &targetID); getchar();
    
    long pos;
    while((pos = ftell(fp)), fread(&b, sizeof(Bus), 1, fp)) {
        if (b.busID == targetID) {
            found = 1;
            printf("Enter New Bus Name: ");
            fgets(b.busName, MAX_STR, stdin); b.busName[strcspn(b.busName, "\n")] = 0;
            printf("Enter New Route: ");
            fgets(b.route, MAX_STR, stdin); b.route[strcspn(b.route, "\n")] = 0;
            printf("Enter New Date: ");
            fgets(b.date, MAX_STR, stdin); b.date[strcspn(b.date, "\n")] = 0;
            printf("Enter New Time: ");
            fgets(b.time, MAX_STR, stdin); b.time[strcspn(b.time, "\n")] = 0;
            printf("Enter New Ticket Fare: ");
            scanf("%f", &b.fare); getchar();
            
            fseek(fp, pos, SEEK_SET);
            fwrite(&b, sizeof(Bus), 1, fp);
            printf("\nBus details updated successfully!\n");
            break;
        }
    }
    if(!found) printf("Bus ID not found.\n");
    fclose(fp);
    printf("Press Enter..."); getchar();
}

void operatorDeleteBus() {
    Bus b;
    int targetID, found = 0;
    clearScreen();
    header("DELETE BUS");
    FILE *fp = fopen("buses.dat", "rb");
    FILE *temp = fopen("temp_bus.dat", "wb");
    if (!fp || !temp) return;
    
    printf("Enter Bus ID to delete: ");
    scanf("%d", &targetID); getchar();
    
    while(fread(&b, sizeof(Bus), 1, fp)) {
        if (b.busID == targetID) {
            found = 1;
        } else {
            fwrite(&b, sizeof(Bus), 1, temp);
        }
    }
    fclose(fp);
    fclose(temp);
    remove("buses.dat");
    rename("temp_bus.dat", "buses.dat");
    
    if (found) printf("Bus deleted successfully.\n");
    else printf("Bus ID not found.\n");
    printf("Press Enter..."); getchar();
}

void operatorManageBookings() {
    Booking bk;
    clearScreen();
    header("MANAGE BOOKINGS");
    FILE *fp = fopen("bookings.dat", "rb+");
    if (!fp) {
        printf("No bookings found in system.\n");
        printf("Press Enter..."); getchar();
        return;
    }
    printf("%-10s %-10s %-10s %-10s %-10s\n", "TicketID", "CustID", "BusID", "SeatNum", "Status");
    printf("------------------------------------------------------------\n");
    while(fread(&bk, sizeof(Booking), 1, fp)) {
        printf("%-10d %-10d %-10d %-10d %-10s\n", bk.ticketID, bk.customerID, bk.busID, bk.seatNumber, bk.status);
    }
    
    int targetID;
    printf("\nEnter Ticket ID to Accept/Reject (0 to go back): ");
    scanf("%d", &targetID); getchar();
    if (targetID == 0) { fclose(fp); return; }
    
    rewind(fp);
    int found = 0;
    long pos;
    while((pos = ftell(fp)), fread(&bk, sizeof(Booking), 1, fp)) {
        if (bk.ticketID == targetID) {
            found = 1;
            printf("\nChoose Status:\n1. Accept / Approve Booking\n2. Reject Booking (Issues 100%% Refund)\nYour choice: ");
            int decision;
            scanf("%d", &decision); getchar();
            
            if (decision == 1) {
                strcpy(bk.status, "Approved");
                printf("Booking approved successfully!\n");
            } else {
                strcpy(bk.status, "Rejected");
                
                FILE *pfp = fopen("payments.dat", "rb+");
                if (pfp) {
                    Payment p;
                    long pPos;
                    while((pPos = ftell(pfp)), fread(&p, sizeof(Payment), 1, pfp)) {
                        if (p.ticketID == bk.ticketID) {
                            strcpy(p.status, "Refunded 100%");
                            fseek(pfp, pPos, SEEK_SET);
                            fwrite(&p, sizeof(Payment), 1, pfp);
                            break;
                        }
                    }
                    fclose(pfp);
                }
                
                FILE *bfp = fopen("buses.dat", "rb+");
                if (bfp) {
                    Bus b;
                    long bPos;
                    while((bPos = ftell(bfp)), fread(&b, sizeof(Bus), 1, bfp)) {
                        if (b.busID == bk.busID) {
                            b.seats[bk.seatNumber - 1] = 0;
                            b.availableSeats++;
                            fseek(bfp, bPos, SEEK_SET);
                            fwrite(&b, sizeof(Bus), 1, bfp);
                            break;
                        }
                    }
                    fclose(bfp);
                }
                printf("Booking rejected and seat released. Full refund issued.\n");
            }
            fseek(fp, pos, SEEK_SET);
            fwrite(&bk, sizeof(Booking), 1, fp);
            break;
        }
    }
    if(!found) printf("Ticket ID not found.\n");
    fclose(fp);
    printf("Press Enter..."); getchar();
}

void operatorManagePayments() {
    Payment p;
    clearScreen();
    header("VERIFY RECEIPTS");
    FILE *fp = fopen("payments.dat", "rb+");
    if (!fp) {
        printf("No payment records found.\n");
        printf("Press Enter..."); getchar();
        return;
    }
    printf("%-10s %-10s %-12s %-15s %-15s\n", "PayID", "TicketID", "Amount", "Method", "Status");
    printf("----------------------------------------------------------------------------\n");
    while(fread(&p, sizeof(Payment), 1, fp)) {
        printf("%-10d %-10d %-12.2f %-15s %-15s\n", p.paymentID, p.ticketID, p.amount, p.method, p.status);
    }
    
    int targetID;
    printf("\nEnter Payment ID to verify (0 to go back): ");
    scanf("%d", &targetID); getchar();
    if (targetID == 0) { fclose(fp); return; }
    
    rewind(fp);
    int found = 0;
    long pos;
    while((pos = ftell(fp)), fread(&p, sizeof(Payment), 1, fp)) {
        if (p.paymentID == targetID) {
            found = 1;
            printf("\nSet Status:\n1. Verify & Approve Receipt\n2. Reject Receipt\nEnter choice: ");
            int decision;
            scanf("%d", &decision); getchar();
            if (decision == 1) strcpy(p.status, "Approved");
            else strcpy(p.status, "Rejected");
            
            fseek(fp, pos, SEEK_SET);
            fwrite(&p, sizeof(Payment), 1, fp);
            printf("Payment status updated successfully.\n");
            break;
        }
    }
    if(!found) printf("Payment ID not found.\n");
    fclose(fp);
    printf("Press Enter..."); getchar();
}

void operatorViewHistory() {
    Booking bk;
    Payment p;
    clearScreen();
    header("GLOBAL SYSTEM HISTORY");
    FILE *bfp = fopen("bookings.dat", "rb");
    FILE *pfp = fopen("payments.dat", "rb");
    
    printf(">> GLOBAL BOOKINGS LOG\n");
    printf("------------------------------------------------------------\n");
    if (bfp) {
        printf("%-10s %-10s %-10s %-10s %-10s\n", "TicketID", "CustID", "BusID", "SeatNum", "Status");
        while(fread(&bk, sizeof(Booking), 1, bfp)) {
            printf("%-10d %-10d %-10d %-10d %-10s\n", bk.ticketID, bk.customerID, bk.busID, bk.seatNumber, bk.status);
        }
        fclose(bfp);
    }
    printf("\n>> GLOBAL PAYMENTS LOG\n");
    printf("----------------------------------------------------------------------------\n");
    if (pfp) {
        printf("%-10s %-10s %-12s %-15s %-15s\n", "PayID", "TicketID", "Amount", "Method", "Status");
        while(fread(&p, sizeof(Payment), 1, pfp)) {
            printf("%-10d %-10d %-12.2f %-15s %-15s\n", p.paymentID, p.ticketID, p.amount, p.method, p.status);
        }
        fclose(pfp);
    }
    printf("\nPress Enter to return..."); getchar();
}

void customerSignup() {
    Customer c;
    clearScreen();
    header("CUSTOMER SIGNUP");
    FILE *fp = fopen("customers.dat", "ab");
    if (!fp) return;
    
    c.customerID = getNextID("customers.dat", sizeof(Customer));
    printf("Enter Your Name: ");
    fgets(c.name, MAX_STR, stdin); c.name[strcspn(c.name, "\n")] = 0;
    printf("Enter Your CNIC: ");
    fgets(c.cnic, MAX_STR, stdin); c.cnic[strcspn(c.cnic, "\n")] = 0;
    printf("Enter Your Age: ");
    scanf("%d", &c.age); getchar();
    printf("Enter Your Phone Number: ");
    fgets(c.phone, MAX_STR, stdin); c.phone[strcspn(c.phone, "\n")] = 0;
    printf("Enter Your Email: ");
    fgets(c.email, MAX_STR, stdin); c.email[strcspn(c.email, "\n")] = 0;
    printf("Create Password: ");
    fgets(c.password, MAX_STR, stdin); c.password[strcspn(c.password, "\n")] = 0;
    
    fwrite(&c, sizeof(Customer), 1, fp);
    fclose(fp);
    printf("\nSignup Successful! Your Customer ID is: %d\n", c.customerID);
    printf("Press Enter to continue..."); getchar();
}

void customerLogin() {
    char loginName[MAX_STR], loginPass[MAX_STR];
    Customer c;
    clearScreen();
    header("CUSTOMER LOGIN");
    printf("Enter Name: ");
    fgets(loginName, MAX_STR, stdin); loginName[strcspn(loginName, "\n")] = 0;
    printf("Enter Password: ");
    fgets(loginPass, MAX_STR, stdin); loginPass[strcspn(loginPass, "\n")] = 0;
    
    FILE *fp = fopen("customers.dat", "rb");
    if (!fp) {
        printf("No customers registered yet. Please signup first.\n");
        printf("Press Enter..."); getchar();
        return;
    }
    int verified = 0;
    while(fread(&c, sizeof(Customer), 1, fp)) {
        if (strcmp(c.name, loginName) == 0 && strcmp(c.password, loginPass) == 0) {
            currentCustomer = c;
            verified = 1;
            break;
        }
    }
    fclose(fp);
    if (verified) {
        customerMenu();
    } else {
        printf("\nInvalid Name or Password! Please try again.\n");
        printf("Press Enter..."); getchar();
    }
}

void customerResetPassword() {
    char targetName[MAX_STR], targetCnic[MAX_STR], newPass[MAX_STR];
    Customer c;
    int found = 0;
    clearScreen();
    header("RESET PASSWORD");
    
    FILE *fp = fopen("customers.dat", "rb+");
    if (!fp) {
        printf("No customer records found.\n");
        printf("Press Enter..."); getchar();
        return;
    }
    
    printf("Enter Your Registered Name: ");
    fgets(targetName, MAX_STR, stdin); targetName[strcspn(targetName, "\n")] = 0;
    printf("Enter Your CNIC for Identity Check: ");
    fgets(targetCnic, MAX_STR, stdin); targetCnic[strcspn(targetCnic, "\n")] = 0;
    
    long pos;
    while((pos = ftell(fp)), fread(&c, sizeof(Customer), 1, fp)) {
        if (strcmp(c.name, targetName) == 0 && strcmp(c.cnic, targetCnic) == 0) {
            found = 1;
            printf("\nVerification successful!\n");
            printf("Enter Your New Password: ");
            fgets(newPass, MAX_STR, stdin); newPass[strcspn(newPass, "\n")] = 0;
            
            strcpy(c.password, newPass);
            fseek(fp, pos, SEEK_SET);
            fwrite(&c, sizeof(Customer), 1, fp);
            printf("Password updated successfully!\n");
            break;
        }
    }
    if(!found) printf("\nVerification failed. Details do not match.\n");
    fclose(fp);
    printf("Press Enter..."); getchar();
}

void customerMenu() {
    int choice;
    while(1) {
        clearScreen();
        header("CUSTOMER MENU");
        printf("Logged in as: %s\n\n", currentCustomer.name);
        printf("1. View Onboard Facilities\n");
        printf("2. Search Routes & Request Ticket\n");
        printf("3. Check Ticket Approval Status\n");
        printf("4. Cancel Ticket (Get 50%% Refund)\n");
        printf("5. View Personal Booking History\n");
        printf("6. Logout\n\n");
        printf("Enter choice (1-6): ");
        scanf("%d", &choice); getchar();
        
        switch(choice) {
            case 1: viewFacilities(); break;
            case 2: customerSearchAndBook(); break;
            case 3: customerCheckStatus(); break;
            case 4: customerCancelTicket(); break;
            case 5: customerViewHistory(); break;
            case 6: return;
            default: printf("Invalid choice! Press Enter."); getchar();
        }
    }
}

void viewFacilities() {
    clearScreen();
    header("ONBOARD FACILITIES");
    printf("------------------------------------------------------------\n");
    printf("* Free High-Speed Onboard Wi-Fi Access\n");
    printf("* Full Air Conditioning & Climate Control\n");
    printf("* Premium Reclining Seats with Leg Rest Support\n");
    printf("* Dedicated 220V Outlets & USB Charging Ports at every seat\n");
    printf("* Complimentary Snacks and Mineral Water\n");
    printf("------------------------------------------------------------\n");
    printf("\nPress Enter to return to menu...");
    getchar();
}

void customerSearchAndBook() {
    Bus b;
    char targetRoute[MAX_STR], targetDate[MAX_STR];
    clearScreen();
    header("SEARCH & REQUEST TICKET");
    FILE *fp = fopen("buses.dat", "rb+");
    if (!fp) {
        printf("No active buses available at the moment.\n");
        printf("Press Enter..."); getchar();
        return;
    }
    printf("Enter Route: ");
    fgets(targetRoute, MAX_STR, stdin); targetRoute[strcspn(targetRoute, "\n")] = 0;
    printf("Enter Date (DD/MM/YYYY): ");
    fgets(targetDate, MAX_STR, stdin); targetDate[strcspn(targetDate, "\n")] = 0;
    
    int available = 0;
    printf("\n%-5s %-15s %-12s %-8s %-10s %-5s\n", "ID", "Bus Name", "Date", "Time", "Fare", "Available Seats");
    printf("--------------------------------------------------------------------------------------\n");
    while(fread(&b, sizeof(Bus), 1, fp)) {
        if (strstr(b.route, targetRoute) && strcmp(b.date, targetDate) == 0) {
            printf("%-5d %-15s %-12s %-8s %-10.2f %-5d\n", b.busID, b.busName, b.date, b.time, b.fare, b.availableSeats);
            available = 1;
        }
    }
    if (!available) {
        printf("\nNo buses found matching your search values.\n");
        fclose(fp);
        printf("Press Enter..."); getchar();
        return;
    }
    
    int selectedBusID, selectedSeat;
    printf("\nEnter Bus ID to book: ");
    scanf("%d", &selectedBusID); getchar();
    
    rewind(fp);
    available = 0;
    long pos;
    while((pos = ftell(fp)), fread(&b, sizeof(Bus), 1, fp)) {
        if (b.busID == selectedBusID) {
            available = 1;
            if (b.availableSeats <= 0) {
                printf("Sorry, this bus has no seats available.\n");
                break;
            }
            printf("\n--- SEAT MAP ---\nAvailable Seats:\n");
            for(int i = 0; i < MAX_SEATS; i++) {
                if(b.seats[i] == 0) printf("[%02d] ", i + 1);
                if((i + 1) % 10 == 0) printf("\n");
            }
            printf("\nSelect an open seat number: ");
            scanf("%d", &selectedSeat); getchar();
            
            if (selectedSeat < 1 || selectedSeat > MAX_SEATS || b.seats[selectedSeat - 1] == 1) {
                printf("Invalid seat number selection or seat already taken.\n");
            } else {
                b.seats[selectedSeat - 1] = 1;
                b.availableSeats--;
                fseek(fp, pos, SEEK_SET);
                fwrite(&b, sizeof(Bus), 1, fp);
                
                Booking bk;
                bk.ticketID = getNextID("bookings.dat", sizeof(Booking));
                bk.customerID = currentCustomer.customerID;
                bk.busID = b.busID;
                bk.seatNumber = selectedSeat;
                strcpy(bk.status, "Pending");
                
                FILE *bfp = fopen("bookings.dat", "ab");
                fwrite(&bk, sizeof(Booking), 1, bfp);
                fclose(bfp);
                
                Payment p;
                p.paymentID = getNextID("payments.dat", sizeof(Payment));
                p.ticketID = bk.ticketID;
                p.amount = b.fare;
                
                printf("\nSelect Payment Method:\n1. EasyPaisa\n2. JazzCash\n3. Credit Card\nEnter choice (1-3): ");
                int paymentChoice;
                scanf("%d", &paymentChoice); getchar();
                if(paymentChoice == 1) strcpy(p.method, "EasyPaisa");
                else if(paymentChoice == 2) strcpy(p.method, "JazzCash");
                else strcpy(p.method, "Credit Card");
                strcpy(p.status, "Pending");
                
                FILE *pfp = fopen("payments.dat", "ab");
                fwrite(&p, sizeof(Payment), 1, pfp);
                fclose(pfp);
                
                printf("\nTicket request sent! Ticket ID: %d.\nStatus is currently pending operator confirmation.\n", bk.ticketID);
            }
            break;
        }
    }
    fclose(fp);
    printf("Press Enter..."); getchar();
}

void customerCheckStatus() {
    Booking bk;
    clearScreen();
    header("CHECK TICKET STATUS");
    FILE *bfp = fopen("bookings.dat", "rb");
    if (!bfp) {
        printf("No bookings found.\n");
        printf("Press Enter..."); getchar();
        return;
    }
    printf("%-12s %-12s %-12s %-15s\n", "Ticket ID", "Bus ID", "Seat Number", "Status");
    printf("-----------------------------------------------------------------\n");
    int found = 0;
    while(fread(&bk, sizeof(Booking), 1, bfp)) {
        if (bk.customerID == currentCustomer.customerID) {
            printf("%-12d %-12d %-12d %-15s\n", bk.ticketID, bk.busID, bk.seatNumber, bk.status);
            found = 1;
        }
    }
    if(!found) printf("No ticket reservations found under your name.\n");
    fclose(bfp);
    printf("\nPress Enter to return..."); getchar();
}

void customerCancelTicket() {
    Booking bk;
    Bus b;
    clearScreen();
    header("CANCEL TICKET");
    FILE *bfp = fopen("bookings.dat", "rb+");
    if (!bfp) {
        printf("No bookings found.\n");
        printf("Press Enter..."); getchar();
        return;
    }
    printf("%-12s %-12s %-12s %-15s\n", "Ticket ID", "Bus ID", "Seat Number", "Status");
    printf("-----------------------------------------------------------------\n");
    int count = 0;
    while(fread(&bk, sizeof(Booking), 1, bfp)) {
        if (bk.customerID == currentCustomer.customerID && strcmp(bk.status, "Cancelled") != 0 && strcmp(bk.status, "Rejected") != 0) {
            printf("%-12d %-12d %-12d %-15s\n", bk.ticketID, bk.busID, bk.seatNumber, bk.status);
            count++;
        }
    }
    if (count == 0) {
        printf("\nYou have no active tickets available to cancel.\n");
        fclose(bfp);
        printf("Press Enter..."); getchar();
        return;
    }
    
    int targetTicketID;
    printf("\nEnter Ticket ID to cancel: ");
    scanf("%d", &targetTicketID); getchar();
    
    rewind(bfp);
    int found = 0;
    long pos;
    while((pos = ftell(bfp)), fread(&bk, sizeof(Booking), 1, bfp)) {
        if (bk.ticketID == targetTicketID && bk.customerID == currentCustomer.customerID) {
            found = 1;
            strcpy(bk.status, "Cancelled");
            fseek(bfp, pos, SEEK_SET);
            fwrite(&bk, sizeof(Booking), 1, bfp);
            
            FILE *pfp = fopen("payments.dat", "rb+");
            if(pfp) {
                Payment p;
                long pPos;
                while((pPos = ftell(pfp)), fread(&p, sizeof(Payment), 1, pfp)) {
                    if(p.ticketID == targetTicketID) {
                        strcpy(p.status, "Refunded 50%");
                        fseek(pfp, pPos, SEEK_SET);
                        fwrite(&p, sizeof(Payment), 1, pfp);
                        break;
                    }
                }
                fclose(pfp);
            }
            
            FILE *busfp = fopen("buses.dat", "rb+");
            if(busfp) {
                while(fread(&b, sizeof(Bus), 1, busfp)) {
                    if (b.busID == bk.busID) {
                        b.seats[bk.seatNumber - 1] = 0;
                        b.availableSeats++;
                        fseek(busfp, -(long)sizeof(Bus), SEEK_CUR);
                        fwrite(&b, sizeof(Bus), 1, busfp);
                        break;
                    }
                }
                fclose(busfp);
            }
            printf("\nTicket cancelled successfully. Your 50%% refund has been logged.\n");
            break;
        }
    }
    if(!found) printf("Invalid Ticket ID or access denied.\n");
    fclose(bfp);
    printf("Press Enter..."); getchar();
}

void customerViewHistory() {
    Booking bk;
    Payment p;
    clearScreen();
    header("YOUR HISTORY");
    
    FILE *bfp = fopen("bookings.dat", "rb");
    FILE *pfp = fopen("payments.dat", "rb");
    
    int ownedTickets[500];
    int ticketCount = 0;
    
    printf(">> YOUR BOOKINGS LOG\n");
    printf("-----------------------------------------------------------------\n");
    if (bfp) {
        printf("%-12s %-12s %-12s %-15s\n", "Ticket ID", "Bus ID", "Seat Number", "Status");
        while(fread(&bk, sizeof(Booking), 1, bfp)) {
            if (bk.customerID == currentCustomer.customerID) {
                printf("%-12d %-12d %-12d %-15s\n", bk.ticketID, bk.busID, bk.seatNumber, bk.status);
                if(ticketCount < 500) {
                    ownedTickets[ticketCount] = bk.ticketID;
                    ticketCount++;
                }
            }
        }
        fclose(bfp);
    }
    
    printf("\n>> YOUR PAYMENTS LOG\n");
    printf("----------------------------------------------------------------------------\n");
    if (pfp) {
        printf("%-10s %-10s %-12s %-15s %-15s\n", "PayID", "TicketID", "Amount", "Method", "Status");
        while(fread(&p, sizeof(Payment), 1, pfp)) {
            int isOwned = 0;
            for(int i = 0; i < ticketCount; i++) {
                if(p.ticketID == ownedTickets[i]) {
                    isOwned = 1;
                    break;
                }
            }
            if (isOwned) {
                printf("%-10d %-10d %-12.2f %-15s %-15s\n", p.paymentID, p.ticketID, p.amount, p.method, p.status);
            }
        }
        fclose(pfp);
    }
    printf("\nPress Enter to return..."); getchar();
}

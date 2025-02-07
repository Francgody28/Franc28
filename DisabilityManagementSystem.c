#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h> // For sleep function (useful for loading screen)


// Structures
typedef struct UserNode {
    char fullName[100];
    char email[100];
    char username[50];
    char password[50];
    struct UserNode* left;  // Left child (for BST)
    struct UserNode* right; // Right child (for BST)
} UserNode;


typedef struct RecordNode {
    int id;
    char name[50];
    char disability[50];
    char assistance[100];
    struct RecordNode* next; // For linked list
} RecordNode;


typedef struct MeetingQueue {
    int meetingID;
    char purpose[100];
    char date[20];  // YYYY-MM-DD format
    struct MeetingQueue* next; // For queue
} MeetingQueue;


typedef struct Hospital {
    char name[100];
    char location[100];
    char contact[50];
    struct Hospital* next;
} Hospital;


typedef struct Sponsor {
    char name[100];
    char contact[50];
    char contribution[100];
    struct Sponsor* next;
} Sponsor;

typedef struct StackNode {
    char username[50];
    struct StackNode* next;
} StackNode;


// Global Variables
UserNode* userRoot = NULL;     // BST root for user accounts
RecordNode* recordHead = NULL; // Linked list head for records
MeetingQueue* meetingFront = NULL; // Queue front for meetings
MeetingQueue* meetingRear = NULL;  // Queue rear for meetings
StackNode* userStack = NULL;   // Stack for user session management
Hospital* hospitalList = NULL;
Sponsor* sponsorList = NULL;


// Function Prototypes
UserNode* createUserNode(const char* fullName, const char* email, const char* username, const char* password);
UserNode* insertUser(UserNode* root, const char* fullName, const char* email, const char* username, const char* password);
UserNode* searchUser(UserNode* root, const char* username, const char* password);
void createAccount();
int login();
void logout();
void pushSession(const char* username);
void popSession();
char* getSessionUser();
void addRecord(int id, const char* name, const char* disability, const char* assistance);
void displayRecords();
void deleteRecord();
void enqueueMeeting(int meetingID, const char* purpose, const char* date);
void displayMeetings();
void dequeueMeeting();
int  getIntInput();
void getStringInput(char* buffer, size_t size);
void addHospital();
void displayHospitals();
void deleteHospital(const char* name);
void addSponsor();
void displaySponsors();
void deleteSponsor(const char* name);
bool validatePassword(const char* password);
bool isUniqueUsername(UserNode* root,const char* username);
void loadingScreen();
void clearScreen();
int getValidInteger(const char *prompt);
void getValidPhoneNumber(char *phone, int maxSize);
void getValidName(char *name, int maxSize);


// Function Implementations

// User Account Management (BST)
UserNode* createUserNode(const char* fullName, const char* email, const char* username, const char* password) {
    UserNode* newUser = (UserNode*)malloc(sizeof(UserNode));
    strcpy(newUser->fullName, fullName);
    strcpy(newUser->email, email);
    strcpy(newUser->username, username);
    strcpy(newUser->password, password);
    newUser->left = newUser->right = NULL;
    return newUser;
}


UserNode* insertUser(UserNode* root, const char* fullName, const char* email, const char* username, const char* password) {
    if (root == NULL)
        return createUserNode(fullName, email, username, password);

    if (strcmp(username, root->username) < 0)
        root->left = insertUser(root->left, fullName, email, username, password);
        
    else if (strcmp(username, root->username) > 0)
        root->right = insertUser(root->right, fullName, email, username, password);

    return root;
}


UserNode* searchUser(UserNode* root, const char* username, const char* password) {
    if (root == NULL)
        return NULL;

    if (strcmp(username, root->username) == 0 && strcmp(password, root->password) == 0)
        return root;

    if (strcmp(username, root->username) < 0)
        return searchUser(root->left, username, password);

    return searchUser(root->right, username, password);
}


void createAccount() {
    char fullName[100], email[100], username[50], password[50];

    printf("Enter your full name: ");
    getStringInput(fullName, sizeof(fullName));
    
    // Validate email format (simple check for "@" character)
    printf("Enter your email: ");
    getStringInput(email, sizeof(email));
    if (strchr(email, '@') == NULL) {
        printf("Invalid email format.\n");
        return;
    }

    // Username validation
    printf("Enter a username: ");
    getStringInput(username, sizeof(username));
    
    // Check if username is unique
    if (!isUniqueUsername(userRoot, username)) {
        printf("Username already exists. Please choose a different one.\n");
        return;
    }

    // Password input and validation
    printf("Enter a password: ");
    getStringInput(password, sizeof(password));
    
    // Validate password
    if (!validatePassword(password)) {
        return;  // If password is invalid, return to avoid account creation
    }

    // Insert user into BST if all validations pass
    userRoot = insertUser(userRoot, fullName, email, username, password);
    printf("Account created successfully!\n");
}


// Function to validate password (simple example: minimum 6 characters and at least one number)
bool validatePassword(const char* password) {
    if (strlen(password) < 6) {
        printf("Password must be at least 6 characters long.\n");
        return false;
    }

    bool hasNumber = false;
    for (int i = 0; i < strlen(password); i++) {
        if (isdigit(password[i])) {
            hasNumber = true;
            break;
        }
    }

    if (!hasNumber) {
        printf("Password must contain at least one number.\n");
        return false;
    }

    return true;
}

// Function to check if a username already exists
bool isUniqueUsername(UserNode* root, const char* username) {
    if (root == NULL)
        return true;

    if (strcmp(username, root->username) == 0) {
        return false; // Username already exists
    }

    if (strcmp(username, root->username) < 0) {
        return isUniqueUsername(root->left, username);
    }

    return isUniqueUsername(root->right, username);

}


void trimSpaces(char* str) {
    int i, start = 0, end = strlen(str) - 1;
    
    // Remove leading spaces
    while (isspace(str[start])) start++;
    
    // Remove trailing spaces
    while (end > start && isspace(str[end])) end--;
    
    // Shift and null-terminate
    for (i = 0; start <= end; i++, start++) {
        str[i] = str[start];
    }
    str[i] = '\0';
}

// Function to get string input with validation
void getStringInput(char* buffer, size_t size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;  // Remove newline character if present
}


int login() {
    char username[50], password[50];
    printf("Enter username: ");
    getStringInput(username, sizeof(username));
    printf("Enter password: ");
    getStringInput(password, sizeof(password));

    UserNode* user = searchUser(userRoot, username, password);
    if (user) {
        printf("Login successful. Welcome, %s!\n", username);
        pushSession(username);
        return 1; // Successful login
    }
	 else {
        printf("Invalid username or password. Please try again.\n");
        return 0; // Failed login
    }
}


void logout() {
    char* loggedOutUser = getSessionUser();
    if (loggedOutUser) {
        printf("Goodbye, %s!\n", loggedOutUser);
        popSession();
    }
	 else {
        printf("No user is currently logged in.\n");
    }
}

// Stack for User Session Management
void pushSession(const char* username) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    strcpy(newNode->username, username);
    newNode->next = userStack;
    userStack = newNode;
}


void popSession() {
    if (userStack == NULL)
        return;

    StackNode* temp = userStack;
    userStack = userStack->next;
    free(temp);
}

char* getSessionUser() {
    return userStack ? userStack->username : NULL;
}

// Record Management (Linked List)
void addRecord(int id, const char* name, const char* disability, const char* assistance) {
    RecordNode* newRecord = (RecordNode*)malloc(sizeof(RecordNode));
    newRecord->id = id;
    strcpy(newRecord->name, name);
    strcpy(newRecord->disability, disability);
    strcpy(newRecord->assistance, assistance);
    newRecord->next = recordHead;
    recordHead = newRecord;
    printf("Record added successfully!\n");
}


void displayRecords() {
    RecordNode* current = recordHead;
    if (!current) {
        printf("No records available.\n");
        return;
    }
    printf("Disability Records:\n");
    while (current) {
        printf("ID: %d, Name: %s, Disability: %s, Assistance: %s\n",
               current->id, current->name, current->disability, current->assistance);
        current = current->next;
    }
}


void deleteRecord(int id) {
    if (recordHead == NULL) {
        printf("No records to delete.\n");
        return;
    }

    RecordNode* current = recordHead;
    RecordNode* previous = NULL;

    // Check if the head node is the one to delete
    if (current != NULL && current->id == id) {
        recordHead = current->next;
        free(current);
        printf("Record with ID %d deleted successfully.\n", id);
        return;
    }

    // Traverse the list to find the node to delete
    while (current != NULL && current->id != id) {
        previous = current;
        current = current->next;
    }

    // If ID is not found
    if (current == NULL) {
        printf("Record with ID %d not found.\n", id);
        return;
    }

    // Remove the node from the list
    previous->next = current->next;
    free(current);
    printf("Record with ID %d deleted successfully.\n", id);
}

// Meeting Management (Queue)
void enqueueMeeting(int meetingID, const char* purpose, const char* date) {
    MeetingQueue* newMeeting = (MeetingQueue*)malloc(sizeof(MeetingQueue));
    newMeeting->meetingID = meetingID;
    strcpy(newMeeting->purpose, purpose);
    strcpy(newMeeting->date, date);
    newMeeting->next = NULL;

    if (meetingRear == NULL) {
        meetingFront = meetingRear = newMeeting;
    }
	 else {
        meetingRear->next = newMeeting;
        meetingRear = newMeeting;
    }
    printf("Meeting scheduled successfully!\n");
}


void displayMeetings() {
    MeetingQueue* current = meetingFront;
    if (!current) {
        printf("No meetings scheduled.\n");
        return;
    }
    printf("Scheduled Meetings:\n");
    while (current) {
        printf("ID: %d, Purpose: %s, Date: %s\n",
               current->meetingID, current->purpose, current->date);
        current = current->next;
    }
}


void dequeueMeeting() {
    if (meetingFront == NULL) {
        printf("No meetings to remove.\n");
        return;
    }

    MeetingQueue* temp = meetingFront;
    meetingFront = meetingFront->next;
    if (meetingFront == NULL)
        meetingRear = NULL;

    free(temp);
    printf("Meeting removed successfully.\n");
}


void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Clear all characters until newline or EOF
}
int getIntInput() {
    int input;
    char buffer[256];

    while (1) {
        printf("Enter your choice: ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = '\0';

            int valid = 1;
            for (int i = 0; buffer[i] != '\0'; i++) {
                if (!isdigit(buffer[i])) {
                    valid = 0;
                    break;
                }
            }

            if (valid) {
                if (sscanf(buffer, "%d", &input) == 1) {
                	//clearInputBuffer();
                    return input;
                }
            }
        }
        printf("Invalid input. Please enter a number.\n");
    }
}


void addHospital() {
    Hospital* newHospital = (Hospital*)malloc(sizeof(Hospital));
    printf("Enter Hospital Name: ");
    fgets(newHospital->name, sizeof(newHospital->name), stdin);
    strtok(newHospital->name, "\n");
    
    printf("Enter Hospital Location: ");
    fgets(newHospital->location, sizeof(newHospital->location), stdin);
    strtok(newHospital->location, "\n");
    
    printf("Enter Contact Information: ");
    fgets(newHospital->contact, sizeof(newHospital->contact), stdin);
    strtok(newHospital->contact, "\n");

    newHospital->next = hospitalList;
    hospitalList = newHospital;
    printf("Hospital details added successfully!\n");
}


void displayHospitals() {
    Hospital* current = hospitalList;
    if (current == NULL) {
        printf("No hospital details available.\n");
		return;
    }
        printf("\nList of Hospitals:\n");
        while (current != NULL) {
            printf("Name: %s, Location: %s, Contact: %s\n", current->name, current->location, current->contact);
            current = current->next;
        }
        printf("\nPress any key to return to main menu...");
        while(getchar()!='\n');
        getchar();
    }


void deleteHospital(const char* name) {
    Hospital* current = hospitalList;
    Hospital* prev = NULL;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (prev == NULL) {
                hospitalList = current->next;
            } 
			else {
                prev->next = current->next;
            }
            free(current);
            printf("Hospital %s deleted successfully.\n", name);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Hospital with name %s not found.\n", name);
}


void addSponsor() {
    Sponsor* newSponsor = (Sponsor*)malloc(sizeof(Sponsor));
    printf("Enter Sponsor Name: ");
    fgets(newSponsor->name, sizeof(newSponsor->name), stdin);
    strtok(newSponsor->name, "\n");
    
    printf("Enter Contact Information: ");  
    fgets(newSponsor->contact, sizeof(newSponsor->contact), stdin);
    strtok(newSponsor->contact, "\n");
    
    printf("Enter Contribution Details: ");
    fgets(newSponsor->contribution, sizeof(newSponsor->contribution), stdin);
    strtok(newSponsor->contribution, "\n");

    newSponsor->next = sponsorList;
    sponsorList = newSponsor;
    printf("Sponsor details added successfully!\n");
}


void displaySponsors() {
    Sponsor* current = sponsorList;
    if (current == NULL) {
        printf("No sponsor details available.\n");
    } 
	else {
        printf("\nList of Sponsors:\n");
        
        while (current != NULL) {
            printf("Name: %s, Contact: %s, Contribution: %s\n", current->name, current->contact, current->contribution);
            current = current->next;
        }
    }
}


void deleteSponsor(const char* name) {
    if (name == NULL || strlen(name) == 0) {
        printf("Error: Sponsor name cannot be empty.\n");
        return;
    }

    Sponsor* current = sponsorList;
    Sponsor* prev = NULL;
    int found = 0;

    while (current != NULL) {
        char storedName[100];
        strcpy(storedName, current->name);
        trimSpaces(storedName);  // Remove extra spaces

        if (strcasecmp(storedName, name) == 0) {  // Case-insensitive comparison
            found = 1;
            if (prev == NULL) {
                sponsorList = current->next;
            }
			 else {
                prev->next = current->next;
            }
            free(current);
            printf("Sponsor '%s' deleted successfully.\n", name);
            return;
        }
        prev = current;
        current = current->next;
    }

    if (!found) {
        printf("Error: Sponsor '%s' not found.\n", name);
    }
}


void clearScreen() {
    #ifdef _WIN32
        system("cls"); // For Windows
    #else
        system("clear"); // For Unix/Linux
    #endif
}


void loadingScreen() {
    printf("Loading, please wait...\n");
    for (int i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout);
        sleep(1); // Simulate loading time
    }
    printf("\n");
}


// Function to validate integer input (ID, Phone Number)
int getValidInteger(const char *prompt) {
    int value;
    char buffer[100];

    while (1) {
        printf("%s", prompt);
        fgets(buffer, sizeof(buffer), stdin);
        strtok(buffer, "\n"); // Remove newline

        // Check if input is a valid integer
        int valid = 1;
        for (int i = 0; i < strlen(buffer); i++) {
            if (!isdigit(buffer[i])) {
                valid = 0;
                break;
            }
        }

        if (valid && strlen(buffer) > 0) {
            return atoi(buffer);
        }
		 else {
            printf("Error: Please enter a valid positive integer.\n");
        }
    }
}


void getValidPhoneNumber(char *phone, int maxSize) {
    while (1) {
        printf("Enter phone number: ");
        fgets(phone, maxSize, stdin);
        strtok(phone, "\n"); // Remove newline

        int valid = 1;
        for (int i = 0; i < strlen(phone); i++) {
            if (!isdigit(phone[i])) {
                valid = 0;
                break;
            }
        }

        if (valid && strlen(phone) >= 10 && strlen(phone) <= 15) {
            break;
        }
		 else {
            printf("Error: Phone number must contain only digits and be between 10-15 characters.\n");
        }
    }
}


// Function to validate name (only letters and spaces)
void getValidName(char *name, int maxSize) {
    while (1) {
        printf("Enter name: ");
        fgets(name, maxSize, stdin);
        strtok(name, "\n"); // Remove newline

        int valid = 1;
        for (int i = 0; i < strlen(name); i++) {
            if (!isalpha(name[i]) && name[i] != ' ') {
                valid = 0;
                break;
            }
        }

        if (valid && strlen(name) > 0) {
            break;
        } 
		else {
            printf("Error: Name must contain only letters and spaces, and cannot be empty.\n");
        }
    }
}


void voiceOutput(const char* message) {
    printf("[Voice Output]: %s\n", message);
}


void largeFontOutput(const char* message) {
    // For simplicity, just print message as is; ideally, this would adjust font size on compatible platforms
    printf("[Large Font Mode]: %s\n", message);
}




// Main Function
int main() {
    int choice;
    int isLoggedIn = 0;

    // Accessibility menu
    printf("Do you need accessibility assistance?\n");
    printf("1. Yes\n2. No\n");
    choice = getIntInput();

    if (choice == 1) {
        printf("Choose assistance type:\n1. Voice Assistance\n2. Large Font Mode\n");
        choice = getIntInput();
        
        if (choice == 1) {
            // Activate voice assistance
            voiceOutput("Voice assistance mode activated. All commands will be read aloud.");
            
        } else if (choice == 2) {
            // Activate large font mode
            largeFontOutput("Large font mode activated.");
            
        } else {
            printf("Invalid choice. Defaulting to standard mode.\n");
        }
    }


    // Account creation and login loop
    while (!isLoggedIn) {
        loadingScreen();
        clearScreen();
        printf("\nDISABILITY MANAGEMENT SYSTEM\n");
        printf("1. Create Account\n2. Login\n3. Exit\n");
        printf("***************\n");
        choice = getIntInput();

        switch (choice) {
            case 1:
                createAccount();
                printf("Welcome!\n");
                break;
                
            case 2:
                isLoggedIn = login();
                clearScreen();
                break;
                
            case 3:
                printf("Goodbye!\n");
                return 0;
                
            default:
                printf("Invalid option. Please try again.\n");
        }
    }


    // Main menu after successful login
    do {
        printf("\nMain Menu\n");
        printf("1. Record Management\n");
        printf("2. Sponsor Management\n");
        printf("3. Hospital Management\n");
        printf("4. Meeting Management\n");
        printf("5. Exit\n");
        choice = getIntInput();

        switch (choice) {
            case 1: // Record Management
                do {
                    printf("\nRecord Management\n");
                    printf("1. Add Record\n");
                    printf("2. Display Records\n");
                    printf("3. Delete Record\n");
                    printf("4. Back to Main Menu\n");
                    choice = getIntInput();

                    switch (choice) {
                        case 1:
						 {
                            int id;
                            char name[50], disability[50], assistance[100];
                            printf("Enter ID: ");
                            id = getIntInput();
                            printf("Enter Name: ");
                            getStringInput(name, sizeof(name));
                            printf("Enter Disability: ");
                            getStringInput(disability, sizeof(disability));
                            printf("Enter Assistance: ");
                            getStringInput(assistance, sizeof(assistance));
                            addRecord(id, name, disability, assistance);
                            break;
                        }
                        
                        case 2:
                            displayRecords();
                            break;
                        
                        case 3:
                        {
                         int idToDelete;
                         printf("Enter the ID of the record to delete: ");
                         idToDelete = getIntInput();
                         deleteRecord(idToDelete);
                            break;
                        }
                        case 4:
                            printf("Returning to Main Menu...\n");
                            break;
                            
                        default:
                            printf("Invalid option. Please try again.\n");
                    }
                } while (choice != 4);
                break;

            // Sponsor Management
			case 2: 
                do {
                    printf("\nSponsor Management\n");
                    printf("1. Add Sponsor\n");
                    printf("2. Display Sponsors\n");
                    printf("3. Delete Sponsor\n");
                    printf("4. Back to Main Menu\n");
                    choice = getIntInput();

                    switch (choice) {
                        case 1:
                            addSponsor();
                            break;
                            
                        case 2:
                            displaySponsors();
                            break;
                            
                        case 3:
						 {
                         char name[100];
                       		printf("Enter Sponsor Name to delete: ");
    
                       	// Ensure input buffer is clear before taking input
    						while (getchar() != '\n');

					    	fgets(name, sizeof(name), stdin);
    						strtok(name, "\n");  // Remove newline
  							trimSpaces(name);     // Trim spaces

						    // Debugging: Print the entered name
    						printf("Debug: You entered sponsor name -> '%s'\n", name);

    						if (strlen(name) == 0) {
       						 printf("Error: Sponsor name cannot be empty.\n");
    						} else {
        					deleteSponsor(name);
    						}
    						break;
								}
								
                        case 4:
                            printf("Returning to Main Menu...\n");
                            break;
                            
                        default:
                            printf("Invalid option. Please try again.\n");
                    }
                } while (choice != 4);
                break;

            // Hospital Management
			case 3: 
                do {
                    printf("\nHospital Management\n");
                    printf("1. Add Hospital\n");
                    printf("2. Display Hospitals\n");
                    printf("3. Delete Hospital\n");
                    printf("4. Back to Main Menu\n");
                    choice = getIntInput();

                    switch (choice) {
                        case 1:
                            addHospital();
                            break;
                            
                        case 2:
                            displayHospitals();
                            break;
                            
                        case 3: 
						{
                            char name[100];
                            printf("Enter Hospital Name to delete: ");
                            while (getchar() != '\n');
                            fgets(name, sizeof(name), stdin);
                            strtok(name, "\n");
                            deleteHospital(name);
                            break;
                        }
                        
                        case 4:
                            printf("Returning to Main Menu...\n");
                            break;
                            
                        default:
                            printf("Invalid option. Please try again.\n");
                    }
                } 
				while (choice != 4);
                break;


            // Meeting Management
			case 4: 
                do {
                    printf("\nMeeting Management\n");
                    printf("1. Schedule Meeting\n");
                    printf("2. View Meetings\n");
                    printf("3. Remove Meeting\n");
                    printf("4. Back to Main Menu\n");
                    choice = getIntInput();

                    switch (choice) {
                        case 1: 
						{
                            int meetingID;
                            char purpose[100], date[20];
                            printf("Enter Meeting ID: ");
                            meetingID = getIntInput();
                            printf("Enter Purpose: ");
                            getStringInput(purpose, sizeof(purpose));
                            printf("Enter Date (YYYY-MM-DD): ");
                            getStringInput(date, sizeof(date));
                            enqueueMeeting(meetingID, purpose, date);
                            break;
                        }
                        case 2:
                            displayMeetings();
                            break;
                            
                        case 3:
                            dequeueMeeting();
                            break;
                            
                        case 4:
                            printf("Returning to Main Menu...\n");
                            break;
                            
                        default:
                            printf("Invalid option. Please try again.\n");
                    }
                } 
				while (choice != 4);
                break;

            case 5:
                printf("Goodbye!\n");
                break;

            default:
                printf("Invalid option. Please try again.\n");
        }
    } 
	while (choice != 5);

    return 0;
}
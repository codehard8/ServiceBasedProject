#include <stdio.h>
#define MAX_USERS 500
#define MAX_SERVICES 3
#define MAX_CATEGORIES 3
#define MAX_TITLE_LEN 100
#define MAX_DESC_LEN 200
#define MAX_PRICE_LEN 10
#define MAX_LINE_LEN (MAX_TITLE_LEN + MAX_DESC_LEN + MAX_PRICE_LEN + 3)

struct User {
    char username[51]; 
    char password[51]; 
    int isLoggedIn;
};


typedef struct {
    char title[MAX_TITLE_LEN];
    char description[MAX_DESC_LEN];
    char price[MAX_PRICE_LEN];
} Service;

typedef struct {
    char title[MAX_TITLE_LEN];
    Service services[MAX_SERVICES];
} Category;

void displayCategories(Category categories[]) {
    printf("Categories:\n");
    for (int i = 0; i < MAX_CATEGORIES; i++) {
        printf("%d. %s\n", i + 1, categories[i].title);
    }
}

void displayServices(Category categories[], int categoryIndex, int *selectedService) {
    printf("Services in %s:\n", categories[categoryIndex].title);
    for (int i = 0; i < MAX_SERVICES; i++) {
        printf("%d. %s\n", i + 1, categories[categoryIndex].services[i].title);
    }
    printf("Select a service (1-%d): ", MAX_SERVICES);
    scanf("%d", selectedService);
    if (*selectedService < 1 || *selectedService > MAX_SERVICES) {
        printf("Invalid service selection.\n");
        *selectedService = -1; // Set to invalid value
    }
}

void checkout(Category categories[], int categoryIndex, int serviceIndex) {
    printf("Checkout Details:\n");
    printf("Service: %s\n", categories[categoryIndex].services[serviceIndex].title);
    printf("Description: %s\n", categories[categoryIndex].services[serviceIndex].description);
    printf("Price: %s\n", categories[categoryIndex].services[serviceIndex].price);
    printf("Thank you for your purchase!\n");
}

void services() {
    FILE *filePointer;
    char line[MAX_LINE_LEN];
    Category categories[MAX_CATEGORIES];
    int selectedCategory, selectedService;

    
    filePointer = fopen("services.txt", "r");
    if (filePointer == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    
    for (int i = 0; i < MAX_CATEGORIES; i++) {
        fgets(categories[i].title, MAX_TITLE_LEN, filePointer);
        for (int j = 0; j < MAX_SERVICES; j++) {
            fgets(categories[i].services[j].title, MAX_TITLE_LEN, filePointer);
            fgets(categories[i].services[j].description, MAX_DESC_LEN, filePointer);
            fgets(categories[i].services[j].price, MAX_PRICE_LEN, filePointer);
          
            fgets(line, MAX_LINE_LEN, filePointer);
        }
    }

    
    fclose(filePointer);

   
    displayCategories(categories);

    
    printf("Select a category (1-%d): ", MAX_CATEGORIES);
    scanf("%d", &selectedCategory);
    if (selectedCategory < 1 || selectedCategory > MAX_CATEGORIES) {
        printf("Invalid category selection.\n");
        return;
    }

    
    displayServices(categories, selectedCategory - 1, &selectedService);
    if (selectedService == -1) {
        printf("Exiting...\n");
        return;
    }


    checkout(categories, selectedCategory - 1, selectedService - 1);
}

int confirm(char input1[], char input2[]) {
    int i = 0;
    while (input1[i] != '\0' && input2[i] != '\0') {
        if (input1[i] != input2[i]) {
            return 0; 
        }
        i++;
    }
    return (input1[i] == '\0' && input2[i] == '\0'); 
}

void signUp(FILE *file, struct User users[], int *userCount) {

    printf("Enter username: "); 
    scanf("%s", users[*userCount].username);
    printf("Enter password: "); 
    scanf("%s", users[*userCount].password);
    users[*userCount].isLoggedIn = 0;

    fprintf(file, "%s %s\n", users[*userCount].username, users[*userCount].password); // after signig up the user data will be stored in file
    fflush(file); // direcly

    *userCount += 1;
    printf("You are sign up!\n");
}

int login(FILE *file, struct User users[], int userCount, char username[], char password[]) {
    char tempUsername[51];
    char tempPassword[51];
    rewind(file);  
    for (int i = 0; i < userCount; i++) {
        fscanf(file, "%49s %49s", tempUsername, tempPassword); 
        if (confirm(username, tempUsername) && confirm(password, tempPassword)) {
            users[i].isLoggedIn = 1;
            return i; 
        }
    }
    return -1; 
}

void logout(struct User users[], int index) {
    if (index >= 0) {
        users[index].isLoggedIn = 0;
        printf("Logged out successfully.\n");
    } else {
        printf("Invalid user index.\n");
    }
}

int main() {
    struct User users[MAX_USERS];
    int userCount = 0;

    FILE *file = fopen("data.txt", "a+"); 

    if (file == NULL) {
        printf("Error opening file.\n");
        return 1; 
    }

    
    char tempUsername[51]; 
    char tempPassword[51];
    rewind(file); 
     /*for storing complete data from user to array
	 userCount used as index that at which index the person data is stored*/
    while (fscanf(file, "%s %s", tempUsername, tempPassword) == 2) {
        int i = 0;
        while (tempUsername[i] != '\0') {
            users[userCount].username[i] = tempUsername[i];
            i++;
        }
        users[userCount].username[i] = '\0'; 

        i = 0;
        while (tempPassword[i] != '\0') {
            users[userCount].password[i] = tempPassword[i];
            i++;
        }
        users[userCount].password[i] = '\0'; 
        users[userCount].isLoggedIn = 0;
        userCount++;
    }

    int currentUserIndex = -1;

    int choice;
    do {
        printf("1. Sign Up\n2. Login\n3. Logout\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                signUp(file, users, &userCount);
                break;
            case 2:
                char username[50];
                char password[50];
                printf("Enter username: ");
                scanf("%s", username);
                printf("Enter password: ");
                scanf("%s", password);
                currentUserIndex = login(file, users, userCount, username, password);
                if (currentUserIndex >= 0) {
                	int option;
                    printf("\nLogin successful!\n");
                    printf("\nDo you want to browse services press 1 yes and 0 for no :\n");
                    scanf("%d", &option);
                    if(option==1){
                    	services();
					}
                } else {
                    printf("Invalid username or password.\n");
                }
                break;
            case 3:
                logout(users, currentUserIndex);
                currentUserIndex = -1;
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 4.\n");
        }
    } while (choice != 4);

    fclose(file); 
    return 0;
}

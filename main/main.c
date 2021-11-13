#include <stdio.h>
#include <conio.h> //For getch() function.
#include <stdlib.h>	
#include <string.h>
#include <windows.h> //for opening the output window in custom height and width.
#include <time.h> //for entry date.

/* Constant Declaration */
#define YES 1
#define NO 0

/* Structure Declaration */
typedef struct{
	unsigned short int day;
	unsigned short int month;
	unsigned short int year;
}Date;

struct Person{
	//Person(common details of both student and teacher.)
	int id;
	char first_name[17];
	char last_name[17];
	char username[20];
	char password[8];
	char address[30];
	unsigned long long int phone_num;
	char email[30];
	char entryDate[12];
}person;

struct Student{
	//Student
	Date date;
	unsigned short int grade;
}student;

struct Teacher{
	//Teacher
	char subject[25];
}teacher;

/* Function Declaration */
void welcome();
void loginMenu();
void options();
void login();
const char* chooseWhoseInfo();
void entryDate();
void addRecord();
void viewGeneralRecord();
//void UpdateRecord();
//void DeleteRecord();
void search();
void searchRecordById(int);
void searchRecordsByName(int);

/* Variable Declaration */
unsigned short int menuChoice, i;
char *who = NULL;
static int login_id;
const char* whom = NULL;
char *fullname;
FILE *fptr = NULL;

int main(){
	/*
	- To set the width and height of window to 1200px and 650px respectively. 
	- This code works in windows only.
	*/
	IsWindow()
	HWND wh = GetConsoleWindow();
	MoveWindow(wh, 40, 20, 1200, 650, TRUE);
	
	welcome();
	loginMenu();

	switch(menuChoice) {
    case 1:
        who = "Student";
        login();
        break;
    case 2:
        who = "Teacher";
        login();
        break;
    case 3:
        who = "Administration";
        login();
        break;
    case 4:
    	printf("\n\tExiting Program...");
    	exit(0);
    default:
        printf("\n\tInvalid Choice!!");
    }
    
	return 0;
}

/* School mgmt system(Title) display msg. */
void welcome(){
	system("cls");
    printf("\t School Management System\t\n");
    printf("\t**************************\n\n");
    if(who == NULL){
    	printf("\t<== Login ==>\n");	
	}else{
    	printf("\t<== %s Panel ==>\n", who);
	}
}

/* lets user choose who s/he want to login as. */
void loginMenu(){
	printf("\n\t1. Student Login.");
    printf("\n\t2. Teacher Login.");
    printf("\n\t3. Accounts Login.");
    printf("\n\t4. Exit.\n");
    printf("\n\tEnter your choice: ");
    scanf("%d", &menuChoice);
}

/* Main options of the system */
void options(){
	welcome();
	
	/* Displays menu */
	printf("\t1. View Record(s)\n");
	printf("\t2. Update Record\n");
	if(!strcmp("Administration", who)){
		printf("\t3. Add Record(s)\n");
		printf("\t4. Delete Record\n");
		printf("\t5. Search Record\n");
		printf("\t6. Sort Records\n");
	}
	printf("\t7. Logout\n");
	printf("\t0. Exit\n");
	printf("\n\n\tEnter your choice: ");
	scanf("%d", &menuChoice);

	/* Prevents Students and Teachers from accessing Administrative Controls. */
	if( (!strcmp("Student", who) || !strcmp("Teacher", who)) && (menuChoice > 2 && menuChoice < 7)){
		printf("\n\tInvalid input...\n\t");
		system("pause");
		system("cls");
		options();
	} 


	switch(menuChoice){
		case 1:
			if(!strcmp("Administration", who)){
				viewGeneralRecord();
			} else {
				printf("\n\n\tRuko Jara... Sabar karo!!");
				printf("\n\tYesko lagi xuttai Function banauna baki nai xa");
			}
			break;
		case 2:
			printf("\tUpdate Record. ");
			break;
		case 3:
			printf("\tAdd Record(s). ");
			addRecord();
			break;
		case 4:
			printf("\tDelete Record. ");
			break;
		case 5:
			printf("\tSearch Record(s). ");
			search();
			break;
		case 6:
			printf("\tSort Record(s). ");
			break;
		case 7:
			printf("\n\tLogging out...\n\t");
			system("pause");
			who = NULL;
			main();
			break;
		case 0:
			printf("\tExiting...");
			exit(0);
		default:
			printf("\tInvalid choice!!!\n\t");
			system("pause");
			options();

	}
}

/* Asks for username and password and Verifies login credentials accessing respective file(`who`.txt). */
void login(){

    char login_username[20], login_password[7];
    
    printf("\tNote: Password Must be exactly 7 characters long and username\n\tcan't exceed 20 characters\n\n");

	printf("\tEnter your ID: ");
	scanf("%d", &login_id);

    printf("\tEnter your username: ");
    scanf(" %[^\n]s", login_username);

    printf("\tEnter your password: ");
    /* Accepts Password and displays '*' instead of entered character. */
    for(i=0; i<7; i++) {
        login_password[i] = getch();
        printf("*");
    }
    login_password[i]='\0';

    if( login_id == 11 && !strcmp("Aashish", login_username) && !strcmp("1234567", login_password) ) {
        printf("\n\tLogin Successful!!\n\n\t");
    	system("pause");
        options();

    } else {
        printf("\n\tLogin Failed\n\n\t");
        system("pause");
        who = NULL;
        welcome();
        main();
    }
}

/* Takes required data with the user and saves it to the respective file. */
void addRecord(){
	system("cls");
	char userResponse;
	
	while(1){
		system("cls");
		whom = chooseWhoseInfo();
	
		printf("\n\tEnter the ID: ");
		scanf("%d", &person.id);
		
		if(person.id <= 0){
			printf("\n\n\tEntered ID is invalid!!!\n\t");
			system("pause");
			addRecord();
		}
		
		printf("\n\tEnter the first name(max. 16 characters, no spaces): ");
		scanf(" %s", &person.first_name);

		printf("\n\tEnter the last name(max. 16 characters, no spaces): ");
		scanf(" %s", &person.last_name);
				
		if(!strcmp("Student", whom)){
			printf("\n\tEnter the grade: ");
			scanf("%lu", &student.grade);
			printf("\tGrade: %lu", student.grade);

			printf("\n\tEnter the Date Of Birth(MM/DD/YYYY): ");
			scanf("%lu/%lu/%lu", &student.date.month, &student.date.day, &student.date.year);
			
			if(student.date.day > 31 || student.date.month > 12 || (student.date.year < 1920 || student.date.year >= 2021)){
				printf("\n\n\tEntered date is invalid!!!\n\t");
				system("pause");
				addRecord();
			}
		} else 	if(!strcmp("Teacher", whom)){
			printf("\n\tEnter the subject: ");
			scanf(" %s", &teacher.subject);
		}
		printf("\n\tEnter the address(max. 30 characters, no spaces): ");
		scanf(" %s", &person.address);
		
		printf("\n\tEnter the contact number: ");
		scanf("%lld", &person.phone_num);
		
		printf("\n\tEnter the email(max. 30 characters, , no spaces): ");
		scanf(" %s", &person.email);
		
		entryDate();

		printf("\n\n\tDo you want to add more records? 'n' NO: ");
		scanf(" %c", &userResponse);
		
		if(!strcmp("Student", whom)){
			fptr = fopen("Student/StudentRecord.dat", "ab");
		
			fprintf(fptr, "%d\t%s %s\t%lu\t%lld\t%s\t%lu/%lu/%lu\t%s\t%s\n", person.id, person.first_name, person.last_name,
			student.grade, person.phone_num, person.email, student.date.month, student.date.day, student.date.year, person.address,
			person.entryDate);
			
			fclose(fptr);

			/*
				>-- Format of Student Table --<
			 | ID | Full Name | Grade | Ph-Num | Email | DoB | Address | Entry Date |
			*/
			
			fptr = fopen("Student/StudentLogin.dat", "ab");
			fprintf(fptr, "%s%d\t%s\n", person.first_name, person.id, "1234567\0");
			
		} else if(!strcmp("Teacher", whom)){
			
			fptr = fopen("Teacher/TeacherRecord.dat", "ab");
		
			fprintf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%s\n", person.id, person.first_name, person.last_name, person.phone_num,
			person.email, person.address, teacher.subject, person.entryDate);
			
			fclose(fptr);
			
			/*
				>-- Format of Teacher Table --</
			 | ID | Full Name | Ph-Num | E-mail | Address | Subject | Entry Date |
			*/
			
			fptr = fopen("Teacher/TeacherLogin.dat", "ab");
			fprintf(fptr, "%d_%s\t%s\n", person.id, person.first_name, "qwertyu\0");
			
		} else if(!strcmp("Administration", whom)){
			
			fptr = fopen("Administration/AdministrationRecord.dat", "ab");
		
			fprintf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\n", person.id, person.first_name, person.last_name, person.phone_num,
			person.email, person.address, person.entryDate);
			
			fclose(fptr);
			
			/*
				>-- Format of Administration Table --</
			 | ID | Full Name | Ph-Num | E-mail | Address | Entry Date |
			*/
			
			fptr = fopen("Administration/AdminLogin.dat", "ab");
			fprintf(fptr, "%d_%s\t%s\n", person.id, person.first_name, "123asdf\0"); //make a function to get fname
			
		}
		fclose(fptr);
		
		printf("\n\tRecord added successfully!\n\t");
		system("pause");
		
		if(userResponse == 'n' || userResponse == 'N'){
			options();
			break;
		}
	}
}

/* Lets user determine whose record is to be played with and returns respective String. */
const char* chooseWhoseInfo() {
	char choice;
	printf("\n\tEnter whose info do you want to enter\n\tstudent(s)\tteacher(t)\tAdministration(a)\t");
  	scanf(" %c", &choice);
	switch(choice){
  		case 's':
  			return "Student";
  			break;
  		case 't':
  			return "Teacher";
  			break;
  		case 'a':
  			return "Administration";
  			break;
  		default:
  			printf("\n\n\tInvalid Choice\n\t");
  			system("pause");
  			system("cls");
  			chooseWhoseInfo();
  	}
}

/* Sets date of the entry time to entryDate memeber of Person structure. */
void entryDate(){
	char entryDate[12];
	time_t t = time(NULL);
	struct tm dates = *localtime(&t);
	sprintf(entryDate, "%02d/%02d/%04d", dates.tm_mon, dates.tm_mday, (dates.tm_year + 1900));
	strcpy(person.entryDate, entryDate);
}

/* Displays all the data of respective category to the user. */
void viewGeneralRecord(){
	system("cls");
	whom = chooseWhoseInfo();
	system("cls");
	printf("\n<== %s Record ==>\n\n", whom);

	if(!strcmp("Student", whom)) {
		
		fptr = fopen("Student/StudentRecord.dat", "rb");
		if(fptr == NULL){
			printf("\n\t%s File Not Found!!!", whom);
		} else {
			printf("%-7s %-24s %-7s %-15s %-30s %-12s %-16s %s", "ID", "Full Name", "Grade", "Phone Number", "E-mail", "DoB", "Address", "Entry Date");
			
			while(fscanf(fptr, "%d\t%s %s\t%d\t%lld\t%s\t%d/%d/%d\t%s\t%s\n", &person.id, person.first_name, person.last_name,
			&student.grade, &person.phone_num, person.email, &student.date.month, &student.date.day, &student.date.year,
			person.address, person.entryDate) != -1){
			
				fullname = person.first_name;
    			strcat(strcat(fullname, " "), person.last_name);
			
				printf("\n%-7d %-24s %-7d %-15lld %-30s %02lu/%02lu/%-5lu  %-15s  %s", person.id, fullname, student.grade, person.phone_num,
				person.email, student.date.month, student.date.day, student.date.year, person.address, person.entryDate);
							
			}
		}
		
	} else if(!strcmp("Teacher", whom)) {
		
		fptr = fopen("Teacher/TeacherRecord.dat", "rb");
		
		if(fptr == NULL){
			printf("\n\t%s File Not Found!!!", whom);
		} else {
			printf("%-7s %-25s %-15s %-35s %-15s %-13s %s", "ID", "Full Name", "Phone Number", "E-mail", "Address", "Subject", "Entry Date");
			
			while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%s\n", &person.id, person.first_name, person.last_name, &person.phone_num,
			person.email, person.address, teacher.subject, person.entryDate) != -1){
		
				fullname = person.first_name;
	    		strcat(strcat(fullname, " "), person.last_name);
			
				printf("\n%-7d %-25s %-15lld %-35s %-15s %-13s %s", person.id, fullname, person.phone_num, person.email,
				person.address, teacher.subject, person.entryDate);
		
			}
		}
		
	} else if(!strcmp("Administration", whom)) {
		
		fptr = fopen("Administration/AdministrationRecord.dat", "rb");
		
		if(fptr == NULL){
			printf("\n\t%s File Not Found!!!", whom);
		} else {
			printf("%-7s %-25s %-15s %-35s %-15s %s", "ID", "Full Name", "Phone Number", "E-mail", "Address", "Entry Date");
			
			while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\n", &person.id, person.first_name, person.last_name, &person.phone_num,
			person.email, person.address, person.entryDate) != -1){
		
				fullname = person.first_name;
    			strcat(strcat(fullname, " "), person.last_name);
		
				printf("\n%-7d %-25s %-15lld %-35s %-15s %s", person.id, fullname, person.phone_num, person.email,
				person.address, person.entryDate);
		
			}
		}
	}
	
	fclose(fptr);
	printf("\n\n\t");
	system("pause");
	options();
	
}

/* Lets Administrative user choose where to search by name or by id or return back to main menu. */
void search(){
	int foundStatus = 0;
	system("cls");
  	welcome();
	printf("\n\t1. Search Record By Id");
	printf("\n\t2. Search Records By Name");
	printf("\n\t0. Back to Menu");
	printf("\n\n\tEnter your choice: ");
  	scanf("%d", &menuChoice);
	switch(menuChoice){
  		case 1:
  			searchRecordById(foundStatus);
  			break;
  		case 2:
  			searchRecordsByName(foundStatus);
  			break;
  		case 0:
  			options();
  			break;
  		default:
  			printf("\n\n\tInvalid Choice\n\t");
  			system("pause");
  			system("cls");
  			search();
  	}
  	
}

/* Lets Administrative user search record through id */
void searchRecordById(int foundStatus){
	int idToSearch;
	
	printf("\n\tEnter ID to search record: ");
	scanf("%d", &idToSearch);
	whom = chooseWhoseInfo();
	
	system("cls");
	welcome();
	
	if(!strcmp("Student", whom)) {
		
		fptr = fopen("Student/StudentRecord.dat", "rb");
		
		if(fptr == NULL){
			printf("\n\t%s File Not Found!!!", whom);
		} else {
			printf("\n%-7s %-24s %-7s %-15s %-30s %-12s %-16s %s", "ID", "Full Name", "Grade", "Phone Number", "E-mail", "DoB", "Address", "Entry Date");
			
			while(fscanf(fptr, "%d\t%s %s\t%d\t%lld\t%s\t%d/%d/%d\t%s\t%s\n", &person.id, person.first_name, person.last_name,
			&student.grade, &person.phone_num, person.email, &student.date.month, &student.date.day, &student.date.year,
			person.address, person.entryDate) != -1){
				
				fullname = person.first_name;
    			strcat(strcat(fullname, " "), person.last_name);
				
				if(idToSearch == person.id){
					foundStatus = 1;
					printf("\n%-7d %-24s %-7d %-15lld %-30s %02lu/%02lu/%-5lu  %-15s  %s", person.id, fullname, student.grade, person.phone_num,
					person.email, student.date.month, student.date.day, student.date.year, person.address, person.entryDate);
					break;
				}
			}
		}
		
	} else if(!strcmp("Teacher", whom)) {
		
		fptr = fopen("Teacher/TeacherRecord.dat", "rb");
		
		if(fptr == NULL){
			printf("\n\t%s File Not Found!!!", whom);
		} else {
			
			printf("\n%-7s %-25s %-15s %-35s %-15s %-13s %s", "ID", "Full Name", "Phone Number", "E-mail", "Address", "Subject", "Entry Date");
			
			while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%s\n", &person.id, person.first_name, person.last_name, &person.phone_num,
			person.email, person.address, teacher.subject, person.entryDate) != -1){
		
				fullname = person.first_name;
    			strcat(strcat(fullname, " "), person.last_name);
    			
				if(idToSearch == person.id){
					foundStatus = 1;
					printf("\n%-7d %-25s %-15lld %-35s %-15s %-13s %s", person.id, fullname, person.phone_num, person.email,
					person.address, teacher.subject, person.entryDate);
					break;	
				}
			}
		}
		
	} else if(!strcmp("Administration", whom)) {
		
		fptr = fopen("Administration/AdministrationRecord.dat", "rb");
		
		if(fptr == NULL){
			printf("\n\t%s File Not Found!!!", whom);
		} else {
			printf("\n%-7s %-25s %-15s %-35s %-15s %s", "ID", "Full Name", "Phone Number", "E-mail", "Address", "Entry Date");
			
			while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\n", &person.id, person.first_name, person.last_name, &person.phone_num,
			person.email, person.address, person.entryDate) != -1){
				
				fullname = person.first_name;
    			strcat(strcat(fullname, " "), person.last_name);
    			
				if(idToSearch == person.id){
					foundStatus = 1;
					printf("\n%-7d %-25s %-15lld %-35s %-15s %s", person.id, fullname, person.phone_num, person.email,
					person.address, person.entryDate);
					break;
				}	
			}
		}
	}
	fclose(fptr);
	
	if(foundStatus == 0){
  		printf("\n\tRecord Not Found!!!	");
	}
	
	printf("\n\n\t");
	system("pause");
	options();
}

/* Lets Administrative user search record(s) through name */
void searchRecordsByName(int foundStatus){
	char *nameToSearch = NULL;
	
	printf("\n\tEnter name to search record: ");
	scanf(" %[^\n]s", &nameToSearch);
	whom = chooseWhoseInfo();
	
	system("cls");
	welcome();
	
	if(!strcmp("Student", whom)) {
		
		fptr = fopen("Student/StudentRecord.dat", "rb");
		
		if(fptr == NULL){
			printf("\n\t%s File Not Found!!!", whom);
		} else {
			printf("\n%-7s %-24s %-7s %-15s %-30s %-12s %-16s %s", "ID", "Full Name", "Grade", "Phone Number", "E-mail", "DoB", "Address", "Entry Date");
		
			while(fscanf(fptr, "%d\t%s %s\t%d\t%lld\t%s\t%d/%d/%d\t%s\t%s\n", &person.id, person.first_name, person.last_name,
			&student.grade, &person.phone_num, person.email, &student.date.month, &student.date.day, &student.date.year,
			person.address, person.entryDate) != -1){
				
				fullname = person.first_name;
    			strcat(strcat(fullname, " "), person.last_name);
				
				if(!strcmp(nameToSearch, person.first_name) || !strcmp(nameToSearch, person.last_name) || !strcmp(nameToSearch, fullname)){
					foundStatus = 1;
					printf("\n%-7d %-24s %-7d %-15lld %-30s %02lu/%02lu/%-5lu  %-15s  %s", person.id, fullname, student.grade, person.phone_num,
					person.email, student.date.month, student.date.day, student.date.year, person.address, person.entryDate);
				}
			}
		}
		
	} else if(!strcmp("Teacher", whom)) {
		
		fptr = fopen("Teacher/TeacherRecord.dat", "rb");
		
		if(fptr == NULL){
			printf("\n\t%s File Not Found!!!", whom);
		} else {
			
			printf("\n%-7s %-25s %-15s %-35s %-15s %-13s %s", "ID", "Full Name", "Phone Number", "E-mail", "Address", "Subject", "Entry Date");
			
			while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%s\n", &person.id, person.first_name, person.last_name, &person.phone_num,
			person.email, person.address, teacher.subject, person.entryDate) != -1){
		
				fullname = person.first_name;
    			strcat(strcat(fullname, " "), person.last_name);
    			
				if(!strcmp(nameToSearch, person.first_name) || !strcmp(nameToSearch, person.last_name) || !strcmp(nameToSearch, fullname)){
					foundStatus = 1;
					printf("\n%-7d %-25s %-15lld %-35s %-15s %-13s %s", person.id, fullname, person.phone_num, person.email,
					person.address, teacher.subject, person.entryDate);
					
				}
			}
		}
		
	} else if(!strcmp("Administration", whom)) {
		
		fptr = fopen("Administration/AdministrationRecord.dat", "rb");
		
		if(fptr == NULL){
			printf("\n\t%s File Not Found!!!", whom);
		} else {
			printf("\n%-7s %-25s %-15s %-35s %-15s %s", "ID", "Full Name", "Phone Number", "E-mail", "Address", "Entry Date");
			
			while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\n", &person.id, person.first_name, person.last_name, &person.phone_num,
			person.email, person.address, person.entryDate) != -1){
				
				fullname = person.first_name;
    			strcat(strcat(fullname, " "), person.last_name);
    			
				if(!strcmp(nameToSearch, person.first_name) || !strcmp(nameToSearch, person.last_name) || !strcmp(nameToSearch, fullname)){
					foundStatus = 1;
					printf("\n%-7d %-25s %-15lld %-35s %-15s %s", person.id, fullname, person.phone_num, person.email,
					person.address, person.entryDate);
				}	
			}	
		}
	}
	fclose(fptr);
	
	if(foundStatus == 0){
  		printf("\n\tRecord Not Found!!!	");
	}
	
	printf("\n\n\t");
	system("pause");
	options();
}





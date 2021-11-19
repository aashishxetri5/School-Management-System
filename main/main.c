#include <stdio.h>
#include <conio.h> //For getch() function.
#include <stdlib.h>	
#include <string.h>
#include <windows.h> //for opening the output window in custom height and width.
#include <time.h> //for entry date.

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
int isValidDate(unsigned short int, unsigned short int, unsigned short int);
void viewGeneralRecord();
void search();
void searchRecordById(int);
void searchRecordsByName(int);
void updateRecord();
void deleteRecord();
void deleteLoginRecord(FILE *, FILE *, int);
void viewLoginInfo();

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
    case 0:
    	printf("\n\tExiting Program...");
    	exit(0);
    default:
        printf("\n\tInvalid Choice!!");
    }
    
	return 0;
}

/* School management system(Title) display message. */
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
    printf("\n\t0. Exit.\n");
    printf("\n\tEnter your choice: ");
    scanf("%d", &menuChoice);
}

/* Main menu of the system .*/
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
		printf("\t7. View Login Information\n");
	}
	printf("\t8. Logout\n");
	printf("\t0. Exit\n");
	printf("\n\n\tEnter your choice: ");
	scanf("%d", &menuChoice);

	/* Prevents Students and Teachers from accessing Administrative Controls. */
	if( (!strcmp("Student", who) || !strcmp("Teacher", who)) && (menuChoice > 2 && menuChoice < 8)){
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
			updateRecord();
			break;
		case 3:
			addRecord();
			break;
		case 4:
			deleteRecord();
			break;
		case 5:
			search();
			break;
		case 6:
			printf("\tSort Record(s). ");
			break;
		case 7:
			viewLoginInfo();
			break;
		case 8:
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
			printf("\n\tEnter the grade(numerical value): ");
			scanf("%hu", &student.grade);
	
			printf("\n\tEnter the Date Of Birth in AD(MM/DD/YYYY): ");
			scanf("%hu/%hu/%hu", &student.date.month, &student.date.day, &student.date.year);
			
			if((student.date.day > 0 && student.date.day <= 31) && (student.date.month > 0 && student.date.month <= 12) && 
			(student.date.year >= 1920 && student.date.year <= 2022)) {
				
				if (!isValidDate(student.date.month, student.date.day, student.date.year)) {
					printf("\n\n\tEntered date is invalid!!!\n\t");
					system("pause");
					addRecord();
        		}
			
			}else{
				system("pause");
				printf("\n\n\tEntered date is invalid!!!\n\t");
			}
		} else 	if(!strcmp("Teacher", whom)){
			printf("\n\tEnter the subject: ");
			scanf(" %s", &teacher.subject);
		}
		printf("\n\tEnter the address(max. 30 characters, no spaces): ");
		scanf(" %s", &person.address);
		
		printf("\n\tEnter the contact number: ");
		scanf("%lld", &person.phone_num);
		
		printf("\n\tEnter the email(max. 30 characters, no spaces): ");
		scanf(" %s", &person.email);
		
		entryDate();

		printf("\n\n\tDo you want to add more records? 'n' NO: ");
		scanf(" %c", &userResponse);
		
		if(!strcmp("Student", whom)){
			fptr = fopen("Student/StudentRecord.dat", "ab");
		
			fprintf(fptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", person.id, person.first_name, person.last_name,
			student.grade, person.phone_num, person.email, student.date.month, student.date.day, student.date.year, person.address,
			person.entryDate);
			
			fclose(fptr);

			/*
				>-- Format of Student Table --<
			 | ID | Full Name | Grade | Ph-Num | Email | DoB | Address | Entry Date |
			*/
			
			fptr = fopen("Student/StudentLogin.dat", "ab");
			fprintf(fptr, "%d\t%s%d\t%s\n", person.id, person.first_name, person.id, "1234567\0");
			
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
			fprintf(fptr, "%d\t%s%d\t%s\n", person.id, person.first_name, person.id, "qwertyu\0");
			
		} else if(!strcmp("Administration", whom)){
			
			fptr = fopen("Administration/AdministrationRecord.dat", "ab");
		
			fprintf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\n", person.id, person.first_name, person.last_name, person.phone_num,
			person.email, person.address, person.entryDate);
			
			fclose(fptr);
			
			/*
				>-- Format of Administration Table --</
			 | ID | Full Name | Ph-Num | E-mail | Address | Entry Date |
			*/
			
			fptr = fopen("Administration/AdministrationLogin.dat", "ab");
			fprintf(fptr, "%d\t%s%d\t%s\n", person.id, person.first_name, person.id, "123asdf\0"); //make a function to get fname
			
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

/* Validates DoB of student. */
int isValidDate(unsigned short int month, unsigned short int day, unsigned short int year){
	if (month == 2 && day == 29 && (year%400==0 || (year%100 != 0 && year%4 == 0))) { //if user has entered feb 29 as DoB this checks if entered year is leap year or not.
        return 1;
    } else if ((month == 4 || month == 6 || month == 9 || month == 11) &&  day <= 30) { //checking month with 30 days only.
        return 1;
    } else if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) &&  day <= 31) { //checking months with 31 days.
        return 1;
    } else if(month == 2 && day <= 28){ //checking if feb has more than 28 days on normal years. if yes, it would switch to else block.
        return 1;
    } else {
        return 0;
    }	
}

/* Lets user determine whose record is to be played with and returns respective String. */
const char* chooseWhoseInfo() {
	char choice;
	printf("\n\tEnter whose info do you want to enter\n\tstudent(s)\tteacher(t)\tadministration(a)");
	printf("\n\tEnter your choice: ");
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
	sprintf(entryDate, "%02d/%02d/%04d", (dates.tm_mon+1), dates.tm_mday, (dates.tm_year + 1900));
	strcpy(person.entryDate, entryDate);
}

/* Displays all the data of respective category to the user. */
void viewGeneralRecord(){
	whom = chooseWhoseInfo();
	system("cls");
	printf("\n<== %s Record ==>\n\n", whom);

	if(!strcmp("Student", whom)) {
		
		fptr = fopen("Student/StudentRecord.dat", "rb");
		if(fptr == NULL){
			printf("\n\t%s File Not Found!!!", whom);
		} else {
			printf("%-7s %-24s %-7s %-15s %-30s %-12s %-16s %s", "ID", "Full Name", "Grade", "Phone Number", "E-mail", "DoB", "Address", "Entry Date");
			printf("\n----------------------------------------------------------------------------------------------------------------------------------");
			
			while(fscanf(fptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", &person.id, person.first_name, person.last_name,
			&student.grade, &person.phone_num, person.email, &student.date.month, &student.date.day, &student.date.year,
			person.address, person.entryDate) != -1){
			
				fullname = person.first_name;
    			strcat(strcat(fullname, " "), person.last_name);
			
				printf("\n%-7d %-24s %-7hu %-15lld %-30s %02hu/%02hu/%-5hu  %-15s  %s", person.id, fullname, student.grade, person.phone_num,
				person.email, student.date.month, student.date.day, student.date.year, person.address, person.entryDate);
							
			}
		}
		
	} else if(!strcmp("Teacher", whom)) {
		
		fptr = fopen("Teacher/TeacherRecord.dat", "rb");
		
		if(fptr == NULL){
			printf("\n\t%s File Not Found!!!", whom);
		} else {
			printf("%-7s %-25s %-15s %-35s %-15s %-13s %s", "ID", "Full Name", "Phone Number", "E-mail", "Address", "Subject", "Entry Date");
			printf("\n--------------------------------------------------------------------------------------------------------------------------------");
			
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
			printf("\t%s File Not Found!!!", whom);
		} else {
			printf("%-7s %-25s %-15s %-35s %-15s %s", "ID", "Full Name", "Phone Number", "E-mail", "Address", "Entry Date");
			printf("\n------------------------------------------------------------------------------------------------------------------");
			
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

/* Lets Administrative user search record through id. */
void searchRecordById(int foundStatus){
	int idToSearch;
	
	printf("\n\tEnter ID to search record: ");
	scanf("%d", &idToSearch);
	
	if(idToSearch <= 0){
		printf("\n\tPlease enter a valid Id!!!");
		
	} else {
	
		whom = chooseWhoseInfo();
		system("cls");
		welcome();
	
		if(!strcmp("Student", whom)) {
		
			fptr = fopen("Student/StudentRecord.dat", "rb");
		
			if(fptr == NULL){
				printf("\n\t%s File Not Found!!!", whom);
			} else {
				printf("\n%-7s %-24s %-7s %-15s %-30s %-12s %-16s %s", "ID", "Full Name", "Grade", "Phone Number", "E-mail", "DoB", "Address", "Entry Date");
				printf("\n----------------------------------------------------------------------------------------------------------------------------------");
				
				while(fscanf(fptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", &person.id, person.first_name, person.last_name,
				&student.grade, &person.phone_num, person.email, &student.date.month, &student.date.day, &student.date.year,
				person.address, person.entryDate) != -1){
				
				
					if(idToSearch == person.id){
						fullname = person.first_name;
    					strcat(strcat(fullname, " "), person.last_name);
						printf("\n%-7d %-24s %-7hu %-15lld %-30s %02hu/%02hu/%-5hu  %-15s  %s", person.id, fullname, student.grade, person.phone_num,
						person.email, student.date.month, student.date.day, student.date.year, person.address, person.entryDate);
						foundStatus = 1;
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
				printf("\n--------------------------------------------------------------------------------------------------------------------------------");
				
				while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%s\n", &person.id, person.first_name, person.last_name, &person.phone_num,
				person.email, person.address, teacher.subject, person.entryDate) != -1){
		
    			
					if(idToSearch == person.id){
						fullname = person.first_name;
    					strcat(strcat(fullname, " "), person.last_name);
						printf("\n%-7d %-25s %-15lld %-35s %-15s %-13s %s", person.id, fullname, person.phone_num, person.email,
						person.address, teacher.subject, person.entryDate);
						foundStatus = 1;
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
				printf("\n------------------------------------------------------------------------------------------------------------------");
				
				while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\n", &person.id, person.first_name, person.last_name, &person.phone_num,
				person.email, person.address, person.entryDate) != -1){
				
    			
					if(idToSearch == person.id){
						fullname = person.first_name;
    					strcat(strcat(fullname, " "), person.last_name);
						printf("\n%-7d %-25s %-15lld %-35s %-15s %s", person.id, fullname, person.phone_num, person.email,
						person.address, person.entryDate);
						foundStatus = 1;
						break;
					}	
				}
			}
		}
		fclose(fptr);
	
		if(foundStatus == 0){
	  		printf("\n\tRecord Not Found!!!	");
		}
	}
	printf("\n\n\t");
	system("pause");
	options();
}

/* Lets Administrative user search record(s) through name. !There's Some error in this function's code */
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
			printf("\n----------------------------------------------------------------------------------------------------------------------------------");
			
			while(fscanf(fptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", &person.id, person.first_name, person.last_name,
			&student.grade, &person.phone_num, person.email, &student.date.month, &student.date.day, &student.date.year,
			person.address, person.entryDate) != -1){
				
				fullname = person.first_name;
    			strcat(strcat(fullname, " "), person.last_name);
				
				if(!strcmp(nameToSearch, person.first_name) || !strcmp(nameToSearch, person.last_name) || !strcmp(nameToSearch, fullname)){
					foundStatus = 1;
					printf("\n%-7d %-24s %-7hu %-15lld %-30s %02hu/%02hu/%-5hu  %-15s  %s", person.id, fullname, student.grade, person.phone_num,
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
			printf("\n--------------------------------------------------------------------------------------------------------------------------------");
			
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
			printf("\n------------------------------------------------------------------------------------------------------------------");
			
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

/* Lets Administrative user update record using id. Login details is also updated */
void updateRecord(){
	int updateId, isUpdated = 0;
	printf("\n\tEnter id of the record to be updated: ");
	scanf("%d", &updateId);
	
	if(updateId <= 0){
		printf("\n\tPlease enter a valid Id!!!\n\t");
		
	} else {
		struct Person p1;
		struct Student s1;
		struct Teacher t1;
		whom = chooseWhoseInfo();
	
		system("cls");
	
		printf("\n\tEnter the ID: ");
		scanf("%d", &p1.id);
		
		if(p1.id <= 0){
			printf("\n\n\tEntered ID is invalid!!!\n\t");
			system("pause");
			options();
		}
	
		printf("\n\tEnter the first name(max. 16 characters, no spaces): ");
		scanf(" %s", &p1.first_name);

		printf("\n\tEnter the last name(max. 16 characters, no spaces): ");
		scanf(" %s", &p1.last_name);
				
		if(!strcmp("Student", whom)){
			printf("\n\tEnter the grade(numerical value): ");
			scanf("%hu", &s1.grade);
	
			printf("\n\tEnter the Date Of Birth in AD(MM/DD/YYYY): ");
			scanf("%hu/%hu/%hu", &s1.date.month, &s1.date.day, &s1.date.year);
			
			if((s1.date.day > 0 && s1.date.day <= 31) && (s1.date.month > 0 && s1.date.month <= 12) && 
			(s1.date.year >= 1920 && s1.date.year <= 2022)) {
				
				if (!isValidDate(s1.date.month, s1.date.day, s1.date.year)) {
					printf("\n\n\tEntered date is invalid!!!\n\t");
					system("pause");
					options();
        		}
			
			}else{
				system("pause");
				printf("\n\n\tEntered date is invalid!!!\n\t");
			}
		} else 	if(!strcmp("Teacher", whom)){
			printf("\n\tEnter the subject: ");
			scanf(" %s", &t1.subject);
		}
	
		printf("\n\tEnter the address(max. 30 characters, no spaces): ");
		scanf(" %s", &p1.address);
		
		printf("\n\tEnter the contact number: ");
		scanf("%lld", &p1.phone_num);
		
		printf("\n\tEnter the email(max. 30 characters, no spaces): ");
		scanf(" %s", &p1.email);
		
		entryDate();
		
		FILE *tempFptr = NULL;
		if(!strcmp("Student", whom)) {
		
			fptr = fopen("Student/StudentRecord.dat", "rb");
			tempFptr = fopen("Student/TempRec.dat", "ab");
			
			if(fptr == NULL) {
				
				printf("\n\tNo such record was found!!");
				
			} else {
			
				while(fscanf(fptr, "%d\t%s %s\t%d\t%lld\t%s\t%d/%d/%d\t%s\t%s\n", &person.id, person.first_name, person.last_name,
				&student.grade, &person.phone_num, person.email, &student.date.month, &student.date.day, &student.date.year,
				person.address, person.entryDate) != -1) {
				
					if(updateId == person.id) {
						isUpdated = 1;
						fprintf(tempFptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", p1.id, p1.first_name, 
						p1.last_name, s1.grade, p1.phone_num, p1.email, s1.date.month, s1.date.day,
						s1.date.year, p1.address, person.entryDate);
					} else {
						fprintf(tempFptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", person.id, person.first_name, 
						person.last_name, student.grade, person.phone_num, person.email, student.date.month, student.date.day,
						student.date.year, person.address, person.entryDate);
					}
				}
				fclose(fptr);
				fclose(tempFptr);
				/*
				* Deletes StudentRecord.dat file
				* Renames TempRec.dat file to StudentRecord.dat
				*/
				remove("Student/StudentRecord.dat");
				rename("Student/TempRec.dat", "Student/StudentRecord.dat");
				
				fptr = fopen("Student/StudentLogin.dat", "rb");
				tempFptr = fopen("Student/TempLogin.dat", "ab");
			}
		
		} else if(!strcmp("Teacher", whom)) {
		
			fptr = fopen("Teacher/TeacherRecord.dat", "rb");
			tempFptr = fopen("Teacher/TempRec.dat", "ab");
			
			if(fptr == NULL) {
				
				printf("\n\tNo such record was found!!");
				
			} else {
				/* Reads data from TeacherRecord.dat file and writes it in TempRec.dat file */
				while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%s\n", &person.id, person.first_name, person.last_name, 
				&person.phone_num, person.email, person.address, teacher.subject, person.entryDate) != -1) {
    			
					if(updateId == person.id) {
						isUpdated = 1;
						fprintf(tempFptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%s\n", p1.id, p1.first_name, p1.last_name, 
						p1.phone_num, p1.email, p1.address, t1.subject, person.entryDate);
					} else {
						fprintf(tempFptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%s\n", person.id, person.first_name, person.last_name, 
						person.phone_num, person.email, person.address, teacher.subject, person.entryDate);
					}
				}
				
				fclose(fptr);
				fclose(tempFptr);
				/*
				* Removes 'TeacherRecord.dat' file.
				* Renames 'TempRec.dat' file to 'TeacherRecord.dat' file.
				*/
				remove("Teacher/TeacherRecord.dat");
				rename("Teacher/TempRec.dat", "Teacher/TeacherRecord.dat");
				
				fptr = fopen("Teacher/TeacherLogin.dat", "rb");
				tempFptr = fopen("Teacher/TempLogin.dat", "ab");
				
			}
		
		} else if(!strcmp("Administration", whom)) {
		
			fptr = fopen("Administration/AdministrationRecord.dat", "rb");
			tempFptr = fopen("Administration/TempRec.dat", "ab");
			
			if(fptr == NULL) {
				
				printf("\n\tNo such record was found!!");
				
			} else {
				/* Reads data from AdministrationRecord.dat file and writes it in TempRec.dat file */
				while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\n", &person.id, person.first_name, person.last_name, 
				&person.phone_num, person.email, person.address, person.entryDate) != -1) {
    			
					if(updateId == person.id) {
						isUpdated = 1;
						printf("\n\n\tfound!!");
						fprintf(tempFptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\n", p1.id, p1.first_name, p1.last_name,
						p1.phone_num, p1.email, p1.address, person.entryDate);
					} else {
						
						fprintf(tempFptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\n", person.id, person.first_name, person.last_name,
						person.phone_num, person.email, person.address, person.entryDate);
						
					}
				}
				fclose(fptr);
				fclose(tempFptr);
				/*
				* Removes 'AdministrationRecord.dat' File.
				* Renames 'TempRec.dat' file to 'AdministrationRecord.dat'.
				*/
				remove("Administration/AdministrationRecord.dat");
				rename("Administration/TempRec.dat", "Administration/AdministrationRecord.dat");
				
				fptr = fopen("Administration/AdministrationLogin.dat", "rb");
				tempFptr = fopen("Administration/TempLogin.dat", "ab");
				
			}
		}
		
		while(fscanf(fptr, "%d\t%s\t%s\n", &person.id, &person.username, &person.password) != -1){
					
			if(updateId == person.id) {
				fprintf(tempFptr, "%d\t%s%d\t%s\n", p1.id, p1.first_name, p1.id, person.password);
			} else {
				fprintf(tempFptr, "%d\t%s\t%s\n", person.id, person.username, person.password);
			}	
		}
		
		fclose(fptr);
		fclose(tempFptr);
		
		if(!strcmp("Student", whom)) {
			/*
			*Deletes 'StudentLogin.dat' files.
			*Renames 'TempLogin.dat' file to 'StudentLogin.dat' file.
			*/
			remove("Student/StudentLogin.dat");
			rename("Student/TempLogin.dat", "Student/StudentLogin.dat");
		}else if(!strcmp("Teacher", whom)) {
			/*
			*Deletes 'TeacherLogin.dat' files.
			*Renames 'TempLogin.dat' file to 'TeacherLogin.dat' file.
			*/
			remove("Teacher/TeacherLogin.dat");
			rename("Teacher/TempLogin.dat", "Teacher/TeacherLogin.dat");
		} else if(!strcmp("Administration", whom)) {
			/*
			*Deletes 'AdministrationLogin.dat' files.
			*Renames 'TempLogin.dat' file to 'AdministrationLogin.dat' file.
			*/
			remove("Administration/AdministrationLogin.dat");
			rename("Administration/TempLogin.dat", "Administration/AdministrationLogin.dat");
		}	
	}
	
	if(isUpdated){
		printf("\n\tRecord Updated successfully!\n\n\t");
	}else{
		printf("\n\tProblem updating the Record!! PLease make sure the entered Id is correct.\n\n\t");
	}
	
	system("pause");
	options();
	
}

/* Lets Administrative user delete record using id. */
void deleteRecord(){
	int deleteId, isDeleted = 0;
	printf("\n\tEnter id of the record to be deleted: ");
	scanf("%d", &deleteId);
		
	if(deleteId <= 0){
		printf("\n\tPlease enter a valid Id!!!\n\t");
		
	} else {
	
		whom = chooseWhoseInfo();
		
		FILE *tempFptr = NULL;
		if(!strcmp("Student", whom)) {
		
			fptr = fopen("Student/StudentRecord.dat", "rb");
			tempFptr = fopen("Student/TempRec.dat", "ab");
			
			if(fptr == NULL) {
				
				printf("\n\tNo such record was found!!");
				
			} else {
			
				while(fscanf(fptr, "%d\t%s %s\t%d\t%lld\t%s\t%d/%d/%d\t%s\t%s\n", &person.id, person.first_name, person.last_name,
				&student.grade, &person.phone_num, person.email, &student.date.month, &student.date.day, &student.date.year,
				person.address, person.entryDate) != -1) {
				
					if(deleteId == person.id) {
						isDeleted = 1;
						continue;
					} else {
						fprintf(tempFptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", person.id, person.first_name, 
						person.last_name, student.grade, person.phone_num, person.email, student.date.month, student.date.day,
						student.date.year, person.address, person.entryDate);
					}
				}
				fclose(fptr);
				fclose(tempFptr);
				fptr = fopen("Student/StudentLogin.dat", "rb");
				tempFptr = fopen("Student/TempLogin.dat", "ab");
			
				deleteLoginRecord(fptr, tempFptr, deleteId);
				
				fclose(fptr);
				fclose(tempFptr);
		
				remove("Student/StudentRecord.dat"); //Deletes StudentRecord.dat file
				remove("Student/StudentLogin.dat"); //Deletes StudentLogin.dat file
				
				/*
				* Renames TempRec.dat file to StudentRecord.dat
				* Renames TempLogin.dat file to StudentLogin.dat
				*/
				rename("Student/TempRec.dat", "Student/StudentRecord.dat");
				rename("Student/TempLogin.dat", "Student/StudentLogin.dat");
			}
		
		} else if(!strcmp("Teacher", whom)) {
		
			fptr = fopen("Teacher/TeacherRecord.dat", "rb");
			tempFptr = fopen("Teacher/TempRec.dat", "ab");
			
			if(fptr == NULL) {
				
				printf("\n\tNo such record was found!!");
				
			} else {
				/* Reads data from TeacherRecord.dat file and writes it in TempRec.dat file */
				while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%s\n", &person.id, person.first_name, person.last_name, 
				&person.phone_num, person.email, person.address, teacher.subject, person.entryDate) != -1) {
    			
					if(deleteId == person.id) {
						isDeleted = 1;
						continue;
					} else {
						fprintf(tempFptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%s\n", person.id, person.first_name, person.last_name, 
						person.phone_num, person.email, person.address, teacher.subject, person.entryDate);
					}
				}
				
				fclose(fptr);
				fclose(tempFptr);
				
				fptr = fopen("Teacher/TeacherLogin.dat", "rb");
				tempFptr = fopen("Teacher/TempLogin.dat", "ab");
			
				/* Reads data from TeacherLogin.dat file and writes it in TempLogin.dat file */
				deleteLoginRecord(fptr, tempFptr, deleteId);
				
				fclose(fptr);
				fclose(tempFptr);
		
				/* Removes 'TeacherRecord.dat' and 'TeacherLogin.dat' files. */
				remove("Teacher/TeacherRecord.dat");
				remove("Teacher/TeacherLogin.dat");
				
				/* Renames 'TempRec.dat' file to 'TeacherRecord.dat' and 'TempLogin.dat' file to 'TeacherLogin.dat' file. */
				rename("Teacher/TempRec.dat", "Teacher/TeacherRecord.dat");
				rename("Teacher/TempLogin.dat", "Teacher/TeacherLogin.dat");
			}
		
		} else if(!strcmp("Administration", whom)) {
		
			fptr = fopen("Administration/AdministrationRecord.dat", "rb");
			tempFptr = fopen("Administration/TempRec.dat", "ab");
			
			if(fptr == NULL) {
				
				printf("\n\tNo such record was found!!");
				
			} else {
				/* Reads data from AdministrationRecord.dat file and writes it in TempRec.dat file */
				while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\n", &person.id, person.first_name, person.last_name, 
				&person.phone_num, person.email, person.address, person.entryDate) != -1) {
    			
					if(deleteId == person.id) {
						isDeleted = 1;
						continue;
					} else {
						
						fprintf(tempFptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\n", person.id, person.first_name, person.last_name,
						person.phone_num, person.email, person.address, person.entryDate);
						
					}
				}
				fclose(fptr);
				fclose(tempFptr);
				
				fptr = fopen("Administration/AdministrationLogin.dat", "rb");
				tempFptr = fopen("Administration/TempLogin.dat", "ab");
				
				/* Reads data from AdministrationLogin.dat file and writes it in TempLogin.dat file. */
				deleteLoginRecord(fptr, tempFptr, deleteId);
				
				fclose(fptr);
				fclose(tempFptr);
				/* Deletes 'AdministrationRecord.dat' and 'AdministrationLogin.dat' files. */
				remove("Administration/AdministrationRecord.dat");
				remove("Administration/AdministrationLogin.dat");
				
				/* Renames 'TempRec.dat' file to 'AdministrationRecord.dat' and 'TempLogin.dat' file to 'AdministrationLogin.dat' file. */
				rename("Administration/TempRec.dat", "Administration/AdministrationRecord.dat");
				rename("Administration/TempLogin.dat", "Administration/AdministrationLogin.dat");
			}
		}
	
		if(isDeleted) {
			printf("\n\tRecord Deleted Successfully..\n\n\t");
		} else {
			printf("\n\tProblem deleting the Record!! PLease make sure the entered Id is correct.\n\n\t");
		}
	
	}
	
	system("pause");
	options();
	
}

/* Deletes Login record of the user whose record was deleted recently using id. */
void deleteLoginRecord(FILE *fileptr, FILE *tempFileptr, int deleteId){
	
	while(fscanf(fileptr, "%d\t%s\t%s\n", &person.id, &person.username, &person.password) != -1){
					
		if(deleteId == person.id) {
			continue;
		} else {
			fprintf(tempFileptr, "%d\t%s\t%s\n", person.id, person.username, person.password);
		}	
	}
}

/* Lets Administrative user view login information of all category of users. */
void viewLoginInfo(){
	
	whom = chooseWhoseInfo();
	system("cls");
	printf("\n<== %s Login Record ==>\n\n", whom);
	
	printf("\t%-10s %-20s %s", "ID", "Username", "Password");
	printf("\n\t-----------------------------------------");
	
	if(!strcmp("Student", whom)) {
		
		fptr = fopen("Student/StudentLogin.dat", "rb");
		
	} else if(!strcmp("Teacher", whom)) {
		
		fptr = fopen("Teacher/TeacherLogin.dat", "rb");
		
	} else if(!strcmp("Administration", whom)) {
		
		fptr = fopen("Administration/AdministrationLogin.dat", "rb");
		
	}
	
	if(fptr == NULL){
			printf("\n\tRequested File Not Found!!!");
	} else {
		
		while(fscanf(fptr, "%d\t%s\t%s\n", &person.id, &person.username, &person.password) != -1){
			printf("\n\t%-10d %-20s %s", person.id, person.username, person.password);	
		}
	}
	
	fclose(fptr);
	printf("\n\n\t");
	system("pause");
	options();
	
}	




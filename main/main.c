#include <stdio.h>
#include <conio.h> //For getch() function.
#include <stdlib.h>	
#include <string.h>
#include <windows.h> //for opening the output window in custom height and width (1200x650)px.
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

typedef struct{
	//For marks in exam
	char subject1[15];
	short int marksOfSubject1;
	char subject2[15];
	short int marksOfSubject2;
	char subject3[15];
	short int marksOfSubject3;
	char subject4[15];
	short int marksOfSubject4;
	char subject5[15];
	short int marksOfSubject5;
}Subjects;

struct Student{
	//Student
	Date dob;
	Subjects subject;
	unsigned short int grade;
}student;

struct Teacher{
	//Teacher
	char subject[25];
	unsigned int salary;
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
void searchRecordById();
void searchRecordsByName();
void updateRecord();
void deleteRecord();
void deleteLoginRecord(FILE *, int);
void viewLoginInfo();
void sortRecords();
int getNumOfRecords();
void sortLoginRecords(FILE *, int);
void changePassword();
int isValidUser(char*, char*);
void getLoggedUserInfo();
void updateSelfRecord();
int isUniqueId(int idToSearch);
void saveMarks();
void displayMarksheet();
void displayFullName();

/* Variable Declaration */
unsigned short int menuChoice, i;
char *who = NULL;
static int loggedin_userid;
int isSuccessful = 0;
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
        printf("\n\tInvalid Choice!!\n\t");
        system("pause");
        main();
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
	printf("\n\t1. Student Login");
    printf("\n\t2. Teacher Login");
    printf("\n\t3. Accounts Login");
    printf("\n\t0. Exit\n");
    printf("\n\tEnter your choice: ");
    scanf("%d", &menuChoice);
}

/* Main menu of the system .*/
void options(){
	welcome();
	
	/* Displays main menu of the program */
	printf("\t1. View Record(s)\n");
	printf("\t2. Update Record\n");
	if(strcmp("Teacher", who) != 0){
		printf("\t3. Show Marksheet\n");
	}
	
	/* Only administrative users will be able to access these options. */
	if(!strcmp("Administration", who)){
		printf("\t4. Add Record(s)\n");
		printf("\t5. Delete Record\n");
		printf("\t6. Search Record\n");
		printf("\t7. Sort Records\n");
		printf("\t8. View Login Information\n");
		printf("\t9. Save Marks\n");
	}
	/*Super admin won't be able to access this option. */
	if(loggedin_userid != 20580913){
		printf("\t10.Change Password\n");
	}
	printf("\t11.Logout\n");
	printf("\t0. Exit\n");
	printf("\n\tEnter your choice: ");
	scanf("%hu", &menuChoice);
	
	/* Prevents Students and Teachers from accessing Administrative Controls. */
	if(!strcmp("Teacher", who) && menuChoice == 3){
		printf("\n\tInvalid input...\n\t");
		system("pause");
		system("cls");
		options();
	}else if( (!strcmp("Student", who) || !strcmp("Teacher", who)) && (menuChoice > 3 && menuChoice < 10)){
		printf("\n\tInvalid input...\n\t");
		system("pause");
		system("cls");
		options();
	/*Prevents Super Admin from accessing change password feature.*/
	} else if(loggedin_userid == 20580913 && menuChoice == 10){
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
				getLoggedUserInfo();
			}
			break;
		case 2:
			if(!strcmp("Administration", who)){
				updateRecord();
			} else {
				updateSelfRecord();
			}
			break;
		case 3:
			displayMarksheet();
			break;
		case 4:
			addRecord();
			break;
		case 5:
			deleteRecord();
			break;
		case 6:
			search();
			break;
		case 7:
			sortRecords();
			break;
		case 8:
			viewLoginInfo();
			break;
		case 9:
			saveMarks();
			break;
		case 10:
			changePassword();
			break;
		case 11:
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
	scanf("%d", &loggedin_userid);

    printf("\tEnter your username: ");
    scanf(" %[^\n]s", login_username);

    printf("\tEnter your password: ");
    /* Accepts Password and displays '*' instead of entered character. */
    for(i=0; i<7; i++) {
        login_password[i] = getch();
        printf("*");
    }
    login_password[i]='\0';

	/*
		* Super Admin login credentials is the first expression of the below if condition
		* The second expression makes a function call where login details given by user is passed as an argument and checked if the credentials match with those in the file.
		* Both super admin and admins will have access to all the administrative controls.
	*/
    if( (loggedin_userid == 20580913 && !strcmp("hari^Bdr", login_username) && !strcmp("2001128", login_password)) || isValidUser(login_username, login_password) ) {
        printf("\n\tLogin Successful!!\n\n\t");
    	system("pause");
        options();

    } else {
        printf("\n\n\tEntered credentials doesn't match!!\n\n\t");
        system("pause");
        who = NULL;
        main();
    }
}

/* Takes required data with the user and saves it to the respective file. */
void addRecord(){
//	system("cls");
	char userResponse;
	
	while(1){
		system("cls");
		whom = chooseWhoseInfo();
	
		printf("\n\tEnter the ID: ");
		scanf("%d", &person.id);
		int temp = person.id;
		if(person.id <= 0 || isUniqueId(temp)){
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
			scanf("%hu/%hu/%hu", &student.dob.month, &student.dob.day, &student.dob.year);
			
			if((student.dob.day > 0 && student.dob.day <= 31) && (student.dob.month > 0 && student.dob.month <= 12) && 
			(student.dob.year >= 1920 && student.dob.year <= 2022)) {
				
				if (!isValidDate(student.dob.month, student.dob.day, student.dob.year)) {
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
		if(!strcmp("Teacher", whom) || !strcmp("Administration", whom)){
			printf("\n\tEnter the salary(in Rupees): ");
			scanf("%i", &teacher.salary);
			if(teacher.salary <=0){
				printf("\n\tInvalid Data!!");
				system("pause");
				addRecord();
			}
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
		
			fprintf(fptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", temp, person.first_name, person.last_name,
			student.grade, person.phone_num, person.email, student.dob.month, student.dob.day, student.dob.year, person.address,
			person.entryDate);
			
			fclose(fptr);

			/*
				>-- Format of Student Record Table --<
			 | ID | Full Name | Grade | Ph-Num | Email | DoB | Address | Entry Date |
			*/
			
			fptr = fopen("Student/StudentLogin.dat", "ab");
			/*
			>-- Format of Student Login Record Table --<
			| ID | Username | Password |
			*/
			fprintf(fptr, "%d\t%s%d\t%s\n", temp, person.first_name, temp, "1234567\0");
			
		} else if(!strcmp("Teacher", whom)){
			
			fptr = fopen("Teacher/TeacherRecord.dat", "ab");
		
			fprintf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%i\t%s\n", temp, person.first_name, person.last_name, person.phone_num,
			person.email, person.address, teacher.subject, teacher.salary, person.entryDate);
			
			fclose(fptr);
			
			/*
				>-- Format of Teacher Record Table --<
			 | ID | Full Name | Ph-Num | E-mail | Address | Subject | Salary | Entry Date |
			*/
			
			fptr = fopen("Teacher/TeacherLogin.dat", "ab");
			/*
			>-- Format of Teacher Login Record Table --<
			| ID | Username | Password |
			*/
			fprintf(fptr, "%d\t%s%d\t%s\n", temp, person.first_name, temp, "qwertyu\0");
			
		} else if(!strcmp("Administration", whom)){
			
			fptr = fopen("Administration/AdministrationRecord.dat", "ab");
		
			fprintf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%i\t%s\n", temp, person.first_name, person.last_name, person.phone_num,
			person.email, person.address, teacher.salary, person.entryDate);
			
			fclose(fptr);
			
			/*
				>-- Format of Administration Record Table --</
			 | ID | Full Name | Ph-Num | E-mail | Address | Salary | Entry Date |
			*/
			
			fptr = fopen("Administration/AdministrationLogin.dat", "ab");
			/*
			>-- Format of Administration Login Record Table --<
			| ID | Username | Password |
			*/
			fprintf(fptr, "%d\t%s%d\t%s\n", temp, person.first_name, temp, "123asdf\0"); 
			
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
	printf("\n\tChoose on whose info do you want to perform the requested action\n\tstudent(s)\tteacher(t)\tadministration(a)");
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
			&student.grade, &person.phone_num, person.email, &student.dob.month, &student.dob.day, &student.dob.year,
			person.address, person.entryDate) != -1){
			
				fullname = person.first_name;
    			strcat(strcat(fullname, " "), person.last_name);
			
				printf("\n%-7d %-24s %-7hu %-15lld %-30s %02hu/%02hu/%-5hu  %-15s  %s", person.id, fullname, student.grade, person.phone_num,
				person.email, student.dob.month, student.dob.day, student.dob.year, person.address, person.entryDate);
							
			}
		}
		
	} else if(!strcmp("Teacher", whom)) {
		
		fptr = fopen("Teacher/TeacherRecord.dat", "rb");
		
		if(fptr == NULL){
			printf("\n\t%s File Not Found!!!", whom);
		} else {
			printf("%-7s %-25s %-15s %-30s %-15s %-10s %-11s %s", "ID", "Full Name", "Phone Number", "E-mail", "Address", "Subject", "Salary", "Entry Date");
			printf("\n------------------------------------------------------------------------------------------------------------------------------------");
			
			while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%i\t%s\n", &person.id, person.first_name, person.last_name, &person.phone_num,
			person.email, person.address, teacher.subject, &teacher.salary, person.entryDate) != -1){
			
				fullname = person.first_name;
	    		strcat(strcat(fullname, " "), person.last_name);
			
				printf("\n%-7d %-25s %-15lld %-30s %-15s %-10s Rs.%-8i %s", person.id, fullname, person.phone_num, person.email,
				person.address, teacher.subject, teacher.salary, person.entryDate);
		
			}
		}
		
	} else if(!strcmp("Administration", whom)) {
		
		fptr = fopen("Administration/AdministrationRecord.dat", "rb");
		
		if(fptr == NULL){
			printf("\t%s File Not Found!!!", whom);
		} else {
			printf("%-7s %-25s %-15s %-35s %-15s %-11s %s", "ID", "Full Name", "Phone Number", "E-mail", "Address", "Salary", "Entry Date");
			printf("\n-----------------------------------------------------------------------------------------------------------------------------");
			
			while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%i\t%s\n", &person.id, person.first_name, person.last_name, &person.phone_num,
			person.email, person.address, &teacher.salary, person.entryDate) != -1){
		
				fullname = person.first_name;
    			strcat(strcat(fullname, " "), person.last_name);
		
				printf("\n%-7d %-25s %-15lld %-35s %-15s Rs.%-8i %s", person.id, fullname, person.phone_num, person.email,
				person.address, teacher.salary, person.entryDate);
		
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
	system("cls");
  	welcome();
	printf("\n\t1. Search Record By Id");
	printf("\n\t2. Search Records By Name");
	printf("\n\t0. Back to Menu");
	printf("\n\n\tEnter your choice: ");
  	scanf("%d", &menuChoice);
	switch(menuChoice){
  		case 1:
  			searchRecordById();
  			break;
  		case 2:
  			searchRecordsByName();
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
void searchRecordById(){
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
				&student.grade, &person.phone_num, person.email, &student.dob.month, &student.dob.day, &student.dob.year,
				person.address, person.entryDate) != -1){
				
				
					if(idToSearch == person.id){
						fullname = person.first_name;
    					strcat(strcat(fullname, " "), person.last_name);
						printf("\n%-7d %-24s %-7hu %-15lld %-30s %02hu/%02hu/%-5hu  %-15s  %s", person.id, fullname, student.grade, person.phone_num,
						person.email, student.dob.month, student.dob.day, student.dob.year, person.address, person.entryDate);
						isSuccessful = 1;
						break;
					}
				}
			}
		
		} else if(!strcmp("Teacher", whom)) {
		
			fptr = fopen("Teacher/TeacherRecord.dat", "rb");
		
			if(fptr == NULL){
				printf("\n\t%s File Not Found!!!", whom);
			} else {
				printf("%-7s %-25s %-13s %-30s %-15s %-13s %-11s %s", "ID", "Full Name", "Phone Number", "E-mail", "Address", "Subject", "Salary", "Entry Date");
				printf("\n------------------------------------------------------------------------------------------------------------------------------------");
				
				while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%i\t%s\n", &person.id, person.first_name, person.last_name, &person.phone_num,
				person.email, person.address, teacher.subject, &teacher.salary, person.entryDate) != -1){
		
    			
					if(idToSearch == person.id){
						fullname = person.first_name;
    					strcat(strcat(fullname, " "), person.last_name);
						printf("\n%-7d %-25s %-13lld %-30s %-15s %-13s Rs.%-8i %s", person.id, fullname, person.phone_num, person.email,
						person.address, teacher.subject, teacher.salary, person.entryDate);
						isSuccessful = 1;
						break;	
					}
				}
			}
		
		} else if(!strcmp("Administration", whom)) {
		
			fptr = fopen("Administration/AdministrationRecord.dat", "rb");
		
			if(fptr == NULL){
				printf("\n\t%s File Not Found!!!", whom);
			} else {
				printf("\n%-7s %-25s %-15s %-35s %-15s %-11s %s", "ID", "Full Name", "Phone Number", "E-mail", "Address", "Salary", "Entry Date");
				printf("\n--------------------------------------------------------------------------------------------------------------------------------");
				
				while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%i\t%s\n", &person.id, person.first_name, person.last_name, &person.phone_num,
				person.email, person.address, &teacher.salary, person.entryDate) != -1){
				
    			
					if(idToSearch == person.id){
						fullname = person.first_name;
    					strcat(strcat(fullname, " "), person.last_name);
						printf("\n%-7d %-25s %-15lld %-35s %-15s Rs.%-8i %s", person.id, fullname, person.phone_num, person.email,
						person.address, teacher.salary, person.entryDate);
						isSuccessful = 1;
						break;
					}	
				}
			}
		}
		fclose(fptr);
	
		if(isSuccessful == 0){
	  		printf("\n\tRecord Not Found!!!	");
		}
	}
	printf("\n\n\t");
	system("pause");
	options();
}

/* Lets Administrative user search record(s) through name. !There's Some error in this function's code */
void searchRecordsByName(){
	char nameToSearch[34];
	char fname[18];
	isSuccessful = 0;
	printf("\n\tEnter name to search record: ");
	scanf(" %[^\n]s", &nameToSearch);
	
	whom = chooseWhoseInfo();
	
	system("cls");
	welcome();
	
	if(!strcmp("Student", whom)) {
		
		fptr = fopen("Student/StudentRecord.dat", "rb");
		
		if(fptr == NULL){
			printf("\n\t%s Records Not Found!!!", whom);
		} else {
			printf("\n%-7s %-24s %-7s %-15s %-30s %-12s %-16s %s", "ID", "Full Name", "Grade", "Phone Number", "E-mail", "DoB", "Address", "Entry Date");
			printf("\n----------------------------------------------------------------------------------------------------------------------------------");
			
			while(fscanf(fptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", &person.id, person.first_name, person.last_name,
			&student.grade, &person.phone_num, person.email, &student.dob.month, &student.dob.day, &student.dob.year,
			person.address, person.entryDate) != -1){
				
				strcpy(fname, person.first_name); //copies firstname of the record and saves it to fname
				fullname = person.first_name;
    			strcat(strcat(fullname, " "), person.last_name);
				
				if(!strcmp(nameToSearch, fname) || !strcmp(nameToSearch, person.last_name) || *nameToSearch == *fullname){
					isSuccessful = 1;
					printf("\n%-7d %-24s %-7hu %-15lld %-30s %02hu/%02hu/%-5hu  %-15s  %s", person.id, fullname, student.grade, person.phone_num,
					person.email, student.dob.month, student.dob.day, student.dob.year, person.address, person.entryDate);
				}
			}
		}
		
	} else if(!strcmp("Teacher", whom)) {
		
		fptr = fopen("Teacher/TeacherRecord.dat", "rb");
		
		if(fptr == NULL){
			printf("\n\t%s Records Not Found!!!", whom);
		} else {
			
			printf("%-7s %-25s %-13s %-30s %-15s %-13s %-11s %s", "ID", "Full Name", "Phone Number", "E-mail", "Address", "Subject", "Salary", "Entry Date");
				printf("\n------------------------------------------------------------------------------------------------------------------------------------");
			
			while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%i\t%s\n", &person.id, person.first_name, person.last_name, &person.phone_num,
			person.email, person.address, teacher.subject, &teacher.salary, person.entryDate) != -1){
				strcpy(fname, person.first_name);
				fullname = person.first_name;
    			strcat(strcat(fullname, " "), person.last_name);
    			
				if(!strcmp(nameToSearch, fname) || !strcmp(nameToSearch, person.last_name) || (*nameToSearch == *fullname)){
					isSuccessful = 1;
					printf("\n%-7d %-25s %-13lld %-30s %-15s %-13s Rs.%-8i %s", person.id, fullname, person.phone_num, person.email,
					person.address, teacher.subject, teacher.salary, person.entryDate);
					
				}
			}
		}
		
	} else if(!strcmp("Administration", whom)) {
		
		fptr = fopen("Administration/AdministrationRecord.dat", "rb");
		
		if(fptr == NULL){
			printf("\n\t%s Records Not Found!!!", whom);
		} else {
			printf("\n%-7s %-25s %-15s %-35s %-15s %-11s %s", "ID", "Full Name", "Phone Number", "E-mail", "Address", "Salary", "Entry Date");
			printf("\n-----------------------------------------------------------------------------------------------------------------------------");
			
			while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%i\t%s\n", &person.id, person.first_name, person.last_name, &person.phone_num,
			person.email, person.address, &teacher.salary, person.entryDate) != -1){
				strcpy(fname, person.first_name);
				fullname = person.first_name;
    			strcat(strcat(fullname, " "), person.last_name);
    			
				if(!strcmp(nameToSearch, fname) || !strcmp(nameToSearch, person.last_name) || *nameToSearch == *fullname){
					isSuccessful = 1;
					printf("\n%-7d %-25s %-15lld %-35s %-15s Rs.%-8i %s", person.id, fullname, person.phone_num, person.email,
					person.address, teacher.salary, person.entryDate);
				}	
			}	
		}
	}
	fclose(fptr);
	
	if(!isSuccessful){
  		printf("\n\tRecord Not Found!!!	");
	}
	
	printf("\n\n\t");
	system("pause");
	options();
}

/* Lets Administrative user update record using id. Login details is also updated */
void updateRecord(){
	int updateId;
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
		
		int temp = p1.id;
		if(p1.id <= 0 || isUniqueId(temp)){
			printf("\n\tEntered ID is invalid!!!\n\t");
			system("pause");
			addRecord();
		}
	
		printf("\n\tEnter the first name(max. 16 characters, no spaces): ");
		scanf(" %s", &p1.first_name);

		printf("\n\tEnter the last name(max. 16 characters, no spaces): ");
		scanf(" %s", &p1.last_name);
				
		if(!strcmp("Student", whom)){
			printf("\n\tEnter the grade(numerical value): ");
			scanf("%hu", &s1.grade);
	
			printf("\n\tEnter the Date Of Birth in AD(MM/DD/YYYY): ");
			scanf("%hu/%hu/%hu", &s1.dob.month, &s1.dob.day, &s1.dob.year);
			
			if((s1.dob.day > 0 && s1.dob.day <= 31) && (s1.dob.month > 0 && s1.dob.month <= 12) && 
			(s1.dob.year >= 1920 && s1.dob.year <= 2022)) {
				
				if (!isValidDate(s1.dob.month, s1.dob.day, s1.dob.year)) {
					printf("\n\n\tEntered date is invalid!!!\n\t");
					system("pause");
					options();
        		}
			
			}else{
				system("pause");
				printf("\n\tEntered date is invalid!!!\n\t");
			}
		} else 	if(!strcmp("Teacher", whom)){
			printf("\n\tEnter the subject: ");
			scanf(" %s", &t1.subject);
		}
		
		if(!strcmp("Teacher", whom) || !strcmp("Administration", whom)){
			printf("\n\tEnter the salary(in Rupees): ");
			scanf("%i", &t1.salary);
			if(t1.salary <=0){
				printf("\n\tInvalid Data!!\n\t");
				system("pause");
				options();
			}
		}
	
		printf("\n\tEnter the address(max. 30 characters, no spaces): ");
		scanf(" %s", &p1.address);
		
		printf("\n\tEnter the contact number: ");
		scanf("%lld", &p1.phone_num);
		
		printf("\n\tEnter the email(max. 30 characters, no spaces): ");
		scanf(" %s", &p1.email);
		
		FILE *tempFptr = NULL;
		if(!strcmp("Student", whom)) {
		
			fptr = fopen("Student/StudentRecord.dat", "rb");
			tempFptr = fopen("Student/TempRec.dat", "ab");
			
			if(fptr == NULL) {
				
				printf("\n\tNo such record was found!!");
				
			} else {
			
				while(fscanf(fptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", &person.id, person.first_name, person.last_name,
				&student.grade, &person.phone_num, person.email, &student.dob.month, &student.dob.day, &student.dob.year,
				person.address, person.entryDate) != -1) {
					entryDate();
					if(updateId == person.id) {
						isSuccessful = 1;
						fprintf(tempFptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", temp, p1.first_name, 
						p1.last_name, s1.grade, p1.phone_num, p1.email, s1.dob.month, s1.dob.day, s1.dob.year, p1.address,
						person.entryDate);
					} else {
						fprintf(tempFptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", person.id, person.first_name, 
						person.last_name, student.grade, person.phone_num, person.email, student.dob.month, student.dob.day,
						student.dob.year, person.address, person.entryDate);
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
				while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%i\t%s\n", &person.id, person.first_name, person.last_name, 
				&person.phone_num, person.email, person.address, teacher.subject, &teacher.salary, person.entryDate) != -1) {
    				entryDate();
					if(updateId == person.id) {
						isSuccessful = 1;
						fprintf(tempFptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%i\t%s\n", temp, p1.first_name, p1.last_name, 
						p1.phone_num, p1.email, p1.address, t1.subject, t1.salary, person.entryDate);
					} else {
						fprintf(tempFptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%i\t%s\n", person.id, person.first_name, person.last_name, 
						person.phone_num, person.email, person.address, teacher.subject, teacher.salary, person.entryDate);
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
				while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%i\t%s\n", &person.id, person.first_name, person.last_name, 
				&person.phone_num, person.email, person.address, &teacher.salary, person.entryDate) != -1) {
    				entryDate();
					if(updateId == person.id) {
						isSuccessful = 1;
						fprintf(tempFptr, "%d\t%s %s\t%lld\t%s\t%s\t%i\t%s\n", temp, p1.first_name, p1.last_name,
						p1.phone_num, p1.email, p1.address, t1.salary, person.entryDate);
					} else {
						
						fprintf(tempFptr, "%d\t%s %s\t%lld\t%s\t%s\t%i\t%s\n", person.id, person.first_name, person.last_name,
						person.phone_num, person.email, person.address, teacher.salary, person.entryDate);
						
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
				fprintf(tempFptr, "%d\t%s%d\t%s\n", temp, p1.first_name, temp, person.password);
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
	
	if(isSuccessful){
		printf("\n\tRecord Updated successfully!\n\n\t");
	}else{
		printf("\n\tProblem updating the Record!! PLease make sure the entered Id is correct.\n\n\t");
	}
	
	system("pause");
	options();
	
}

/* Lets Administrative user delete record using id. */
void deleteRecord(){
	int deleteId;
	isSuccessful = 0;
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
			
				while(fscanf(fptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", &person.id, person.first_name, person.last_name,
				&student.grade, &person.phone_num, person.email, &student.dob.month, &student.dob.day, &student.dob.year,
				person.address, person.entryDate) != -1) {
				
					if(deleteId == person.id) {
						isSuccessful = 1;
						continue;
					} else {
						fprintf(tempFptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", person.id, person.first_name, 
						person.last_name, student.grade, person.phone_num, person.email, student.dob.month, student.dob.day,
						student.dob.year, person.address, person.entryDate);
					}
				}
				fclose(fptr);
				fclose(tempFptr);
				
				fptr = fopen("Student/StudentLogin.dat", "rb");
				tempFptr = fopen("Student/TempLogin.dat", "ab");
			
				deleteLoginRecord(tempFptr, deleteId);
				
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
				while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%i\t%s\n", &person.id, person.first_name, person.last_name, 
				&person.phone_num, person.email, person.address, teacher.subject, &teacher.salary, person.entryDate) != -1) {
    			
					if(deleteId == person.id) {
						isSuccessful = 1;
						continue;
					} else {
						fprintf(tempFptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%i\t%s\n", person.id, person.first_name, person.last_name, 
						person.phone_num, person.email, person.address, teacher.subject, teacher.salary, person.entryDate);
					}
				}
				
				fclose(fptr);
				fclose(tempFptr);
				
				fptr = fopen("Teacher/TeacherLogin.dat", "rb");
				tempFptr = fopen("Teacher/TempLogin.dat", "ab");
			
				/* Reads data from TeacherLogin.dat file and writes it in TempLogin.dat file */
				deleteLoginRecord(tempFptr, deleteId);
				
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
				while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%i\t%s\n", &person.id, person.first_name, person.last_name, 
				&person.phone_num, person.email, person.address, &teacher.salary, person.entryDate) != -1) {
    			
					if(deleteId == person.id) {
						isSuccessful = 1;
					} else {
						fprintf(tempFptr, "%d\t%s %s\t%lld\t%s\t%s\t%i\t%s\n", person.id, person.first_name, person.last_name,
						person.phone_num, person.email, person.address, teacher.salary, person.entryDate);
						
					}
				}
				fclose(fptr);
				fclose(tempFptr);
				
				fptr = fopen("Administration/AdministrationLogin.dat", "rb");
				tempFptr = fopen("Administration/TempLogin.dat", "ab");
				
				/* Reads data from AdministrationLogin.dat file and writes it in TempLogin.dat file. */
				deleteLoginRecord(tempFptr, deleteId);
				
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
	
		if(isSuccessful) {
			printf("\n\tRecord Deleted Successfully..\n\n\t");
		} else {
			printf("\n\tProblem deleting the Record!! PLease make sure the entered Id is correct.\n\n\t");
		}
	
	}
	
	system("pause");
	options();
	
}

/* Deletes Login record of the user whose record was deleted recently using id. */
void deleteLoginRecord(FILE *tempFileptr, int deleteId){
	
	while(fscanf(fptr, "%d\t%s\t%s\n", &person.id, &person.username, &person.password) != -1){
					
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
	
	printf("\n\t%-10s %-20s %s", "ID", "Username", "Password");
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

// Sorts record in a file(both record file and login record file.)
void sortRecords(){
	int recordCount = 0, j;
	isSuccessful = 0;
	system("cls");
	welcome();

	whom = chooseWhoseInfo();
	recordCount = getNumOfRecords();

	struct Person personData[recordCount];
	FILE *tempFptr = NULL;
	i = 0;
	if(!strcmp("Student", whom)){
	
		struct Student studentData[recordCount];
		
		fptr = fopen("Student/StudentRecord.dat", "rb");
		tempFptr = fopen("Student/tempRecord.dat", "ab");
		
		/* Fetch records and store into the array. */
		while(fscanf(fptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", &personData[i].id, personData[i].first_name, personData[i].last_name,
		&studentData[i].grade, &personData[i].phone_num, personData[i].email, &studentData[i].dob.month, &studentData[i].dob.day,
		&studentData[i].dob.year, personData[i].address, personData[i].entryDate) != -1){
			i++;
		}
	
		/* Sort records in Student Record file using bubble sort. */
		for(i = 0; i<recordCount - 1; i++){
			for(j = 0; j < recordCount - i - 1; j++){
				if(personData[j].id > personData[j+1].id){
					
					person.id = personData[j].id; strcpy(person.first_name, personData[j].first_name); strcpy(person.last_name, personData[j].last_name);
				 	student.grade = studentData[j].grade; person.phone_num = personData[j].phone_num; strcpy(person.email, personData[j].email);
					student.dob.month = studentData[j].dob.month; student.dob.day = studentData[j].dob.day; student.dob.year = studentData[j].dob.year;
					strcpy(person.address, personData[j].address); strcpy(person.entryDate, personData[j].entryDate);

					personData[j] = personData[j+1];
					studentData[j] = studentData[j+1];
					
					personData[j+1] = personData[j];
					studentData[j+1] = studentData[j];
					
					personData[j+1].id = person.id; strcpy(personData[j+1].first_name, person.first_name); strcpy(personData[j+1].last_name, person.last_name);
					studentData[j+1].grade = student.grade; personData[j+1].phone_num = person.phone_num; strcpy(personData[j+1].email, person.email);
					studentData[j+1].dob.month = student.dob.month; studentData[j+1].dob.day = student.dob.day; studentData[j+1].dob.year = student.dob.year;
					strcpy(personData[j+1].address, person.address); strcpy(person.entryDate, personData[j+1].entryDate);
				}
			}
		}
		
		/* Writes sorted data into the file */
		i = 0;
		while(i != recordCount){
			isSuccessful = 1;
			fprintf(tempFptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", personData[i].id, personData[i].first_name, personData[i].last_name,
			studentData[i].grade, personData[i].phone_num, personData[i].email, studentData[i].dob.month, studentData[i].dob.day, studentData[i].dob.year,
			personData[i].address, personData[i].entryDate);
			i++;
		}
		
		fclose(fptr);
		fclose(tempFptr);
		
		/*Removes the StudentRecord.dat file and renames the 'tempRecord.dat' file to 'StudentRecord.dat'*/
		remove("Student/StudentRecord.dat");
		rename("Student/tempRecord.dat", "Student/StudentRecord.dat");
		
		/* For sorting records in 'StudentLogin.dat' File.*/
		fptr = fopen("Student/StudentLogin.dat", "rb");
		tempFptr = fopen("Student/tempLogin.dat", "ab");
		
		sortLoginRecords(tempFptr, recordCount);
		
		fclose(fptr);
		fclose(tempFptr);
		
		/*Removes the StudentRecord.dat file and renames the 'tempRecord.dat' file to 'StudentRecord.dat'*/
		remove("Student/StudentLogin.dat");
		rename("Student/tempLogin.dat", "Student/StudentLogin.dat");
	
	} else if(!strcmp("Teacher", whom)){
	
		struct Teacher teacherData[recordCount];
		fptr = fopen("Teacher/TeacherRecord.dat", "rb");
		tempFptr = fopen("Teacher/tempRecord.dat", "ab");
		
		/* Fetch records and store into the array */
		while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%i\t%s\n", &personData[i].id, personData[i].first_name, personData[i].last_name,
		&personData[i].phone_num, personData[i].email, personData[i].address, teacherData[i].subject, &teacherData[i].salary, personData[i].entryDate) != -1){
			i++;
		}
		
		/* Sort records using bubble sort. */
		for(i = 0; i<recordCount; i++){
			for(j = 0; j < recordCount - i - 1; j++){
				if(personData[j].id > personData[j+1].id){
					
					person.id = personData[j].id; strcpy(person.first_name, personData[j].first_name); strcpy(person.last_name, personData[j].last_name);
					person.phone_num = personData[j].phone_num; strcpy(person.email, personData[j].email); strcpy(person.address, personData[j].address); strcpy(teacher.subject, teacherData[j].subject);
					teacher.salary = teacherData[j].salary; strcpy(person.entryDate, personData[j].entryDate);

					personData[j] = personData[j+1];
					teacherData[j] =teacherData[j+1];
					
					personData[j+1] = personData[j];
					teacherData[j+1] = teacherData[j];
					
					personData[j+1].id = person.id; strcpy(personData[j+1].first_name, person.first_name); strcpy(personData[j+1].last_name, person.last_name);
					personData[j+1].phone_num = person.phone_num; strcpy(personData[j+1].email, person.email); strcpy(personData[j+1].address, person.address); strcpy(teacherData[j+1].subject, teacher.subject);
					teacherData[j+1].salary = teacher.salary; strcpy(personData[j+1].entryDate, person.entryDate);
				}	
			}
		}
		
		i = 0;
		while(i != recordCount){
			isSuccessful = 1;
			fprintf(tempFptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%i\t%s\n", personData[i].id, personData[i].first_name, personData[i].last_name,
			personData[i].phone_num, personData[i].email, personData[i].address, teacherData[i].subject, teacherData[i].salary, personData[i].entryDate);
			i++;
		}
		
		fclose(fptr);
		fclose(tempFptr);
	
		/*Removes the TeacherRecord.dat file and renames the 'tempRecord.dat' file to 'TeacherRecord.dat'*/
		remove("Teacher/TeacherRecord.dat");
		rename("Teacher/tempRecord.dat", "Teacher/TeacherRecord.dat");
		
		/* For sorting records in 'TeacherLogin.dat' File.*/
		fptr = fopen("Teacher/TeacherLogin.dat", "rb");
		tempFptr = fopen("Teacher/tempLogin.dat", "ab");

		sortLoginRecords(tempFptr, recordCount);
		
		fclose(fptr);
		fclose(tempFptr);
		
		/*Removes the StudentRecord.dat file and renames the 'tempRecord.dat' file to 'StudentRecord.dat'*/
		remove("Teacher/TeacherLogin.dat");
		rename("Teacher/tempLogin.dat", "Teacher/TeacherLogin.dat");
	
	}  else if(!strcmp("Administration", whom)){
	
		struct Teacher teacherData[recordCount];
		
		fptr = fopen("Administration/AdministrationRecord.dat", "rb");
		tempFptr = fopen("Administration/tempRecord.dat", "ab");
		
		/* Fetch records and store into the array */
		while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%i\t%s\n", &personData[i].id, personData[i].first_name, personData[i].last_name, &personData[i].phone_num,
		personData[i].email, personData[i].address, &teacherData[i].salary, personData[i].entryDate) != -1) {
			i++;
		}
	
		/* Sort records using bubble sort. */
		for(i = 0; i<recordCount - 1; i++) {
			for(j = 0; j < recordCount - i - 1; j++) {
				if(personData[j].id > personData[j+1].id) {
					person.id = personData[j].id; strcpy(person.first_name, personData[j].first_name); strcpy(person.last_name, personData[j].last_name);
					person.phone_num = personData[j].phone_num; strcpy(person.email, personData[j].email); strcpy(person.address, personData[j].address);
					teacher.salary = teacherData[j].salary; strcpy(person.entryDate, personData[j].entryDate);

					personData[j] = personData[j+1];
					teacherData[j] = teacherData[j+1];
					
					personData[j+1] = personData[j];
					teacherData[j+1] = teacherData[j];
					
					personData[j+1].id = person.id; strcpy(personData[j+1].first_name, person.first_name); strcpy(personData[j+1].last_name, person.last_name);
					personData[j+1].phone_num = person.phone_num; strcpy(personData[j+1].email, person.email); strcpy(personData[j+1].address, person.address);
					teacherData[j+1].salary = teacher.salary; strcpy(person.entryDate, personData[j+1].entryDate);
				}
			}
		}
	
		i = 0;
		while(i != recordCount){
			isSuccessful = 1;
			fprintf(tempFptr, "%d\t%s %s\t%lld\t%s\t%s\t%i\t%s\n", personData[i].id, personData[i].first_name, personData[i].last_name,
			personData[i].phone_num, personData[i].email, personData[i].address, teacherData[i].salary, personData[i].entryDate);
			i++;
		}
	
		fclose(fptr);
		fclose(tempFptr);
		
		/*Removes the StudentRecord.dat file and renames the 'tempRecord.dat' file to 'StudentRecord.dat'*/
		remove("Administration/AdministrationRecord.dat");
		rename("Administration/tempRecord.dat", "Administration/AdministrationRecord.dat");

		/* For sorting records in 'AdministrationLogin.dat' File.*/
		fptr = fopen("Administration/AdministrationLogin.dat", "rb");
		tempFptr = fopen("Administration/tempLogin.dat", "ab");

		sortLoginRecords(tempFptr, recordCount);
		
		fclose(fptr);
		fclose(tempFptr);
		
		/*Removes the StudentRecord.dat file and renames the 'tempRecord.dat' file to 'StudentRecord.dat'*/
		remove("Administration/AdministrationLogin.dat");
		rename("Administration/tempLogin.dat", "Administration/AdministrationLogin.dat");
	}

	if(isSuccessful) {
		printf("\n\n\tData Sorted Successfully!!\n\n\t");
	} else {
		printf("\n\n\tProblem sorting the data!! Please recheck the data and try again later.\n\n\t");
	}
	
	system("pause");
	options();
}

// Goes through the records in file and return how many lines of data are there in the file.
int getNumOfRecords(){
	int count = 0;
	if(!strcmp("Student", whom)){
		fptr = fopen("Student/StudentLogin.dat", "rb");
	} else if(!strcmp("Teacher", whom)){
		fptr = fopen("Teacher/TeacherLogin.dat", "rb");
	} else if(!strcmp("Administration", whom)){
		fptr = fopen("Administration/AdministrationLogin.dat", "rb");
	}
	
	while(fscanf(fptr, "%d\t%s\t%s\n", &person.id, person.username, person.password) != -1){
		count++;
	}
	
	fclose(fptr);
	
	return count;
}

// Sorts record in the login record file.
void sortLoginRecords(FILE *tempFptr, int recordCount) {
	int j;
	struct Person personData[recordCount];
	i = 0;
	
	/* Fetches data from login file of respective category.*/
	while(fscanf(fptr, "%d\t%s\t%s\n", &personData[i].id, &personData[i].username, &personData[i].password) != -1){		
		i++;
	}
	
	/* Sorts data using bubble sort algorithm. */
	for(i = 0; i<recordCount - 1; i++){
		for(j = 0; j < recordCount - i - 1; j++){
			if(personData[j].id > personData[j+1].id){
				person.id = personData[j].id; strcpy(person.username, personData[j].username); strcpy(person.password, personData[j].password);

				personData[j] = personData[j+1];
				personData[j+1] = personData[j];
					
				personData[j+1].id = person.id; strcpy(personData[j+1].username, person.username); strcpy(personData[j+1].password, person.password);
			}
		}
	}
	
	/* Writes sorted data into the file.*/
	i = 0;
	while(recordCount != i){
		fprintf(tempFptr, "%d\t%s\t%s\n", personData[i].id, personData[i].username, personData[i].password);
		i++;
	}
	
}

// Allows user to change his/her password
void changePassword(){
	isSuccessful = 0;
	char newPassword[8], retypedPassword[8];
	FILE *tempFptr = NULL;
	
	if(!strcmp("Student", who)){
		fptr = fopen("Student/StudentLogin.dat", "rb");
		tempFptr = fopen("Student/TempLogin.dat", "ab");
	} else if(!strcmp("Teacher", who)){
		fptr = fopen("Teacher/TeacherLogin.dat", "rb");
		tempFptr = fopen("Teacher/TempLogin.dat", "ab");
	} else if(!strcmp("Administration", who)){
		fptr = fopen("Administration/AdministrationLogin.dat", "rb");
		tempFptr = fopen("Administration/TempLogin.dat", "ab");
	}
	
	printf("\n\tEnter new password: ");
	/* Accepts Password and displays '*' instead of entered character. */
    for(i=0; i<7; i++) {
        newPassword[i] = getch();
        printf("*");
    }
    newPassword[i]='\0';
	
	printf("\n\tRetype the new password: ");
	
    for(i=0; i<7; i++) {
        retypedPassword[i] = getch();
        printf("*");
    }
    retypedPassword[i]='\0';
	
	if(!strcmp(newPassword, retypedPassword)) {
	
		if(fptr == NULL) {
				printf("\n\tCould not perform requested operation!!!"); // after login is unique for all users, remove this!
		} else {
			
			while(fscanf(fptr, "%d\t%s\t%s\n", &person.id, &person.username, &person.password) != -1){
				
				if(loggedin_userid == person.id) {
					fprintf(tempFptr, "%d\t%s\t%s\n", person.id, person.username, newPassword);
					isSuccessful = 1;
				} else {
					fprintf(tempFptr, "%d\t%s\t%s\n", person.id, person.username, person.password);
				}
			}
		}
		
		fclose(fptr);
		fclose(tempFptr);
		
		if(!strcmp("Student", who)){
			remove("Student/StudentLogin.dat");
			rename("Student/TempLogin.dat", "Student/StudentLogin.dat");
		} else if(!strcmp("Teacher", who)){
			remove("Teacher/TeacherLogin.dat");
			rename("Teacher/TempLogin.dat", "Teacher/TeacherLogin.dat");
		} else if(!strcmp("Administration", who)){
			remove("Administration/AdministrationLogin.dat");
			rename("Administration/TempLogin.dat", "Administration/AdministrationLogin.dat");
		}
		
	} else {
		printf("\n\tPasswords do not match!! Please try again\n\n\t");
		system("pause");
	}
	
	if(isSuccessful){
		printf("\n\tPassword Changed Successfully");
	}else{
		printf("\n\tProblem performing requested action.");
	}
	options();
}

// Retrives data from file with login credentials and compares with login details entered by user.
int isValidUser(char uname[], char pwd[]){
	isSuccessful = 0; //Reseting the success value
	
	if(!strcmp("Student", who)){
		fptr = fopen("Student/StudentLogin.dat", "rb");
	} else if(!strcmp("Teacher", who)){
		fptr = fopen("Teacher/TeacherLogin.dat", "rb");
	} else if(!strcmp("Administration", who)){
		fptr = fopen("Administration/AdministrationLogin.dat", "rb");
	}
	
	if(fptr != NULL) {
		
		while(fscanf(fptr, "%d\t%s\t%s\n", &person.id, &person.username, &person.password) != -1){
			
			if( (person.id == loggedin_userid) && !strcmp(uname, person.username) && !strcmp(pwd, person.password) ) {
				isSuccessful = 1;
			}
			
		}
	}
	
	fclose(fptr);
	return isSuccessful;
}

// Displays information of logged in user.
void getLoggedUserInfo(){
	
	welcome();
	
	if(!strcmp("Student", who)) {
	
		fptr = fopen("Student/StudentRecord.dat", "rb");
		
		if(fptr == NULL){
			printf("\n\t%s File Not Found!!!", whom);
		} else {
			printf("\n%-7s %-24s %-7s %-15s %-30s %-12s %-16s %s", "ID", "Full Name", "Grade", "Phone Number", "E-mail", "DoB", "Address", "Entry Date");
			printf("\n----------------------------------------------------------------------------------------------------------------------------------");
				
			while(fscanf(fptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", &person.id, person.first_name, person.last_name,
			&student.grade, &person.phone_num, person.email, &student.dob.month, &student.dob.day, &student.dob.year,
			person.address, person.entryDate) != -1){
				
				if(loggedin_userid == person.id){
					fullname = person.first_name;
    				strcat(strcat(fullname, " "), person.last_name);
					printf("\n%-7d %-24s %-7hu %-15lld %-30s %02hu/%02hu/%-5hu  %-15s  %s\n\n\t", person.id, fullname, student.grade, person.phone_num,
					person.email, student.dob.month, student.dob.day, student.dob.year, person.address, person.entryDate);
					break;
				}
			}
		}
		
	} else if(!strcmp("Teacher", who)) {
		
		fptr = fopen("Teacher/TeacherRecord.dat", "rb");
		
		if(fptr == NULL){
			printf("\n\t%s File Not Found!!!", whom);
		} else {
			printf("\n%-7s %-25s %-13s %-30s %-15s %-13s %-11s %s", "ID", "Full Name", "Phone Number", "E-mail", "Address", "Subject", "Salary", "Entry Date");
			printf("\n------------------------------------------------------------------------------------------------------------------------------------");

			while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%i\t%s\n", &person.id, person.first_name, person.last_name, &person.phone_num,
			person.email, person.address, teacher.subject, &teacher.salary, person.entryDate) != -1){
    			
				if(loggedin_userid == person.id){
					fullname = person.first_name;
    				strcat(strcat(fullname, " "), person.last_name);
					printf("\n%-7d %-25s %-13lld %-30s %-15s %-13s Rs.%-8i %s\n\n\t", person.id, fullname, person.phone_num, person.email,
					person.address, teacher.subject, teacher.salary, person.entryDate);
					break;
				}
			}
		}	
	}
	
	fclose(fptr);
	system("pause");
	options();
	
}

// Lets teacher and student update their record(specific fields only).
void updateSelfRecord(){
	struct Person p1;
	struct Student s1;
	FILE *tempFptr = NULL;
	
	welcome();
			
	if(!strcmp("Student", who)){
		printf("\n\tEnter the grade(numerical value): ");
		scanf("%hu", &s1.grade);
	
		printf("\n\tEnter the Date Of Birth in AD(MM/DD/YYYY): ");
		scanf("%hu/%hu/%hu", &s1.dob.month, &s1.dob.day, &s1.dob.year);
			
		if((s1.dob.day > 0 && s1.dob.day <= 31) && (s1.dob.month > 0 && s1.dob.month <= 12) && 
		(s1.dob.year >= 1920 && s1.dob.year <= 2022)) {
				
			if (!isValidDate(s1.dob.month, s1.dob.day, s1.dob.year)) {
				printf("\n\n\tEntered date is invalid!!!\n\t");
				system("pause");
				options();
       		}
			
		}else{
			system("pause");
			printf("\n\n\tEntered date is invalid!!!\n\t");
		}
	} else 	if(!strcmp("Teacher", who)){
		printf("\n\tEnter the contact number: ");
		scanf("%lld", &p1.phone_num);
		
		printf("\n\tEnter the email(max. 30 characters, no spaces): ");
		scanf(" %s", &p1.email);
	}	
	
	printf("\n\tEnter the address(max. 30 characters, no spaces): ");
	scanf(" %s", &p1.address);
	
	if(!strcmp("Student", who)) {
		
		fptr = fopen("Student/StudentRecord.dat", "rb");
		tempFptr = fopen("Student/TempRec.dat", "ab");
		
		while(fscanf(fptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", &person.id, person.first_name, person.last_name,
		&student.grade, &person.phone_num, person.email, &student.dob.month, &student.dob.day, &student.dob.year,
		person.address, person.entryDate) != -1) {
			entryDate();
			if(loggedin_userid == person.id) {
				fprintf(tempFptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", person.id, person.first_name, person.last_name,
				s1.grade, person.phone_num, person.email, s1.dob.month, s1.dob.day, s1.dob.year, p1.address, person.entryDate);
			} else {
				fprintf(tempFptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", person.id, person.first_name, 
				person.last_name, student.grade, person.phone_num, person.email, student.dob.month, student.dob.day,
				student.dob.year, person.address, person.entryDate);
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
		
	} else if(!strcmp("Teacher", who)) {
		
		fptr = fopen("Teacher/TeacherRecord.dat", "rb");
		tempFptr = fopen("Teacher/TempRec.dat", "ab");
		
		/* Reads data from TeacherRecord.dat file and writes it in TempRec.dat file */
		while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%i\t%s\n", &person.id, person.first_name, person.last_name, &person.phone_num,
		person.email, person.address, teacher.subject, &teacher.salary, person.entryDate) != -1) {
    		entryDate();
			if(loggedin_userid == person.id) {
				fprintf(tempFptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%i\t%s\n", person.id, person.first_name, person.last_name, 
				p1.phone_num, p1.email, p1.address, teacher.subject, teacher.salary, person.entryDate);
			} else {
				fprintf(tempFptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%i\t%s\n", person.id, person.first_name, person.last_name, 
				person.phone_num, person.email, person.address, teacher.subject, teacher.salary, person.entryDate);
			}
		}
				
		fclose(fptr);
		fclose(tempFptr);
		/*
		* Removes 'TeacherRecord.dat' file.
		* Renames 'TempRec.dat' file to 'TeacherRecord.dat' file.
		*/
		remove("Teacher/TeacherRecord.dat"); /* NOT WORKING */
		rename("Teacher/TempRec.dat", "Teacher/TeacherRecord.dat");
	}
	
	printf("\n\tRecord Updated successfully!\n\n\t");
	system("pause");
	options();
}

// Checks ID duplication of records
int isUniqueId(int idToSearch){
	isSuccessful = 0;

	if(!strcmp("Student", whom)) {
		
		struct Student stdRec;
		fptr = fopen("Student/StudentRecord.dat", "rb");
		
		while(fscanf(fptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", &person.id, person.first_name, person.last_name,
		&student.grade, &person.phone_num, person.email, &student.dob.month, &student.dob.day, &student.dob.year, person.address,
		person.entryDate) != -1){
				
			if(idToSearch == person.id) {
				isSuccessful = 1;
				break;
			}
		}
		
	} else if(!strcmp("Teacher", whom)) {

		fptr = fopen("Teacher/TeacherRecord.dat", "rb");
		
		while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%s\t%i\t%s\n", &person.id, person.first_name, person.last_name, &person.phone_num,
		person.email, person.address, teacher.subject, &teacher.salary, person.entryDate) != -1){
    		
			if(idToSearch == person.id){
				isSuccessful = 1;
				break;	
			}
		}
		
	} else if(!strcmp("Administration", whom)) {

		fptr = fopen("Administration/AdministrationRecord.dat", "rb");
			
		while(fscanf(fptr, "%d\t%s %s\t%lld\t%s\t%s\t%i\t%s\n", &person.id, person.first_name, person.last_name, &person.phone_num,
		person.email, person.address, &teacher.salary, person.entryDate) != -1){		
    			
			if(idToSearch == person.id){
				isSuccessful = 1;
				break;
			}	
		}
	}
	fclose(fptr);
	
	return isSuccessful;
}

//saves subject names are marks obtained in it and saves it to the file.
void saveMarks(){
	char userResponse;
	
	while(1){
		system("cls");
		welcome();
		printf("\n\tEnter Student id: ");
		scanf("%d", &person.id);

		printf("\n\tEnter Subject1's title (No Spaces): ");
		scanf(" %s", &student.subject.subject1);
			
		printf("\n\tEnter marks obtained in %s: ", student.subject.subject1);
		scanf(" %hi", &student.subject.marksOfSubject1);
	
		printf("\n\tEnter Subject2's title (No Spaces): ");
		scanf(" %s", &student.subject.subject2);
			
		printf("\n\tEnter marks obtained in %s: ", student.subject.subject2);
		scanf(" %hi", &student.subject.marksOfSubject2);
			
		printf("\n\tEnter Subject3's title (No Spaces): ");
		scanf(" %s", &student.subject.subject3);
			
		printf("\n\tEnter marks obtained in %s: ", student.subject.subject3);
		scanf(" %hi", &student.subject.marksOfSubject3);
			
		printf("\n\tEnter Subject4's title (No Spaces): ");
		scanf(" %s", &student.subject.subject4);
			
		printf("\n\tEnter marks obtained in %s: ", student.subject.subject4);
		scanf(" %hi", &student.subject.marksOfSubject4);
			
		printf("\n\tEnter Subject5's title (No Spaces): ");
		scanf(" %s", &student.subject.subject5);
			
		printf("\n\tEnter marks obtained in %s: ", student.subject.subject5);
		scanf(" %hi", &student.subject.marksOfSubject5);
	
		fptr = fopen("Student/gradesheet.dat", "ab");
			
		if(fptr == NULL){
			printf("\n\tSuch record doesn't exist");
		} else {
			fprintf(fptr, "%d\t%s\t%hi\t%s\t%hi\t%s\t%hi\t%s\t%hi\t%s\t%hi\n", person.id, student.subject.subject1, student.subject.marksOfSubject1, student.subject.subject2,
			student.subject.marksOfSubject2, student.subject.subject3, student.subject.marksOfSubject3, student.subject.subject4, student.subject.marksOfSubject4,
			student.subject.subject5, student.subject.marksOfSubject5);				
		
			printf("\n\tRecord Added successfully!!");
		}
		fclose(fptr);

		printf("\n\n\tDo you want to enter more data? 'n' NO: ");
		scanf(" %c", &userResponse);
		
		if(userResponse == 'n' || userResponse == 'N'){
			options();
			break;
		}
	}
	printf("\n\n\t");
	system("pause");
	options();
}

//displays the marksheet details.
void displayMarksheet(){
	int totalMarks;
	system("cls");
	welcome();
	
	fptr = fopen("Student/gradesheet.dat", "rb");
	
	if(fptr == NULL){
		printf("\n\tNo Records available at the moment.");
	} else {
		if(!strcmp("Administration", who)){
			printf("\n%-7s %-16s %-7s %-16s %-7s %-16s %-7s %-16s %-7s %-16s %s\n", "ID", "Subject1", "Marks", "Subject2", "Marks", "Subject3", "Marks", "Subject4", "Marks", "Subject5", "Marks");
			printf("-----------------------------------------------------------------------------------------------------------------------------------");
		}
		
		while(fscanf(fptr, "%d\t%s\t%hi\t%s\t%hi\t%s\t%hi\t%s\t%hi\t%s\t%hi\n", &person.id, student.subject.subject1, &student.subject.marksOfSubject1, student.subject.subject2,
		&student.subject.marksOfSubject2, student.subject.subject3, &student.subject.marksOfSubject3, student.subject.subject4, &student.subject.marksOfSubject4,
		student.subject.subject5, &student.subject.marksOfSubject5) != -1){
			if(!strcmp("Student", who)){
				totalMarks = student.subject.marksOfSubject1 + student.subject.marksOfSubject2 + student.subject.marksOfSubject3 + student.subject.marksOfSubject4 + student.subject.marksOfSubject5;
				if(loggedin_userid == person.id){
					printf("\n\t**MARKSHEET**");
					printf("\n\t--------------------------------------------------");
					printf("\n\t ID: %-20d Name: ", person.id);
					displayFullName();
					printf("\n\t---------------------------------------------------");
					printf("\n\t %-40s %hi", student.subject.subject1, student.subject.marksOfSubject1);
					printf("\n\t %-40s %hi", student.subject.subject2, student.subject.marksOfSubject2);
					printf("\n\t %-40s %hi", student.subject.subject3, student.subject.marksOfSubject3);
					printf("\n\t %-40s %hi", student.subject.subject4, student.subject.marksOfSubject4);
					printf("\n\t %-40s %hi", student.subject.subject5, student.subject.marksOfSubject5);
					printf("\n\t---------------------------------------------------");
					printf("\n\t Full Marks: %d", totalMarks);
					printf("\n\t Percentage: %d%%",totalMarks/5);
					printf("\n\t Result: ");
					(totalMarks/5 > 40) ? printf("Passed") : printf("Failed");
					printf("\n\t---------------------------------------------------");
					break;
				} else {
					printf("\n\tNo record of your marksheet was found!");
				}
			} else {
				printf("\n%-7d %-16s %-7hi %-16s %-7hi %-16s %-7hi %-16s %-7hi %-16s %hi", person.id, student.subject.subject1, student.subject.marksOfSubject1, student.subject.subject2, student.subject.marksOfSubject2,
				student.subject.subject3, student.subject.marksOfSubject3, student.subject.subject4, student.subject.marksOfSubject4, student.subject.subject5, student.subject.marksOfSubject5);
			}
		}
	}
	fclose(fptr);
	printf("\n\n\t");
	system("pause");
	options();
}

//displays full name on the marksheet
void displayFullName(){
	fptr = fopen("Student/StudentRecord.dat", "rb");
	
	if(fptr != NULL){
		while(fscanf(fptr, "%d\t%s %s\t%hu\t%lld\t%s\t%hu/%hu/%hu\t%s\t%s\n", &person.id, person.first_name, person.last_name,
		&student.grade, &person.phone_num, person.email, &student.dob.month, &student.dob.day, &student.dob.year,
		person.address, person.entryDate) != -1){
				
			if(loggedin_userid == person.id){
				fullname = person.first_name;
    			strcat(strcat(fullname, " "), person.last_name);
				printf("%s", fullname);
				break;
			}
		}
	}
}



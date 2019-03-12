/*this is a program that creates the status of a student according to the available courses,add delete , search and average printing are the 
 * available functions*/


#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>

#define MAX_SIZE 64
/*prototypes*/
void add(int subject_file,int status_file);
void search(int status_file,int subject_file);
void average(int subject_file,int status_file);
void delete(int subject_file,int status_file);

int main (int argc, char* argv []) {
	int choice = 0,subject_file,status_file,check_close;
	
	subject_file = open(argv[1],O_RDONLY,S_IRWXU); /*open file with subjects*/
	if (subject_file < 0) {
		perror("Open subject_file failure");
		_exit(1);
	}
	status_file = open(argv[2],O_RDWR|O_CREAT,S_IRWXU|S_IROTH|S_IWOTH); /*open the status file for current student*/
	if (status_file < 0) {
		perror("Open status_file failure");
		_exit(1);
	}
	
	
	
	do {
		printf("Select function:\n1 for add\n2 for delete\n3 for search\n4 for average\n5 for exit\n"); /*interactive menu*/
		scanf("%d", &choice);
		switch (choice) {
			case 1: {add(subject_file,status_file); break;}
			case 2: {delete(subject_file,status_file); break;}
			case 3: {search(status_file,subject_file); break;}
			case 4: {average(subject_file,status_file); break;}
			case 5: { break;}
			default: {printf("Wrong choice please select a valid function\n"); break;}
		}
		
	} while (choice != 5);
	
	
	check_close = close(subject_file);
	if (check_close < 0) {perror("Close failure in subject_file");}
		
	check_close = close(status_file); 
	if (check_close < 0) {perror("Close failure in subject_file");}
	
	return 0;
}

void add(int subject_file,int status_file) { 
	int i = 0,month = 0,year = 0,read_check = 0,check_subjects = 0,check_dates = 0,system_check = 0; 
	float grade = 0,check_grades = 0;
	char subject_buffer[MAX_SIZE + 1];
	char check_str[6];
	char line_buffer[82];
	char check_buffer[MAX_SIZE + 1];
	char load_str[6];
	
	system_check = lseek(status_file,0,SEEK_END); /*set the offset at the end of the file*/
	if (system_check < 0){
		perror("lseek failure");
		close(subject_file);
		close(status_file);
		_exit(1);
	}
	printf("Enter subject:\n");
	getchar();
	do {
		subject_buffer[i] = getchar(); /*read the subject given by user character by character until end of name or end of file is reached*/
		i++;
	} while (i < 66 && subject_buffer[i - 1] != '\n');
	subject_buffer[i - 1] = '\0';
	
	system_check = lseek(subject_file,0,SEEK_SET);/*set the offset at the beginning of the subject_file*/
	if (system_check < 0){
		perror("lseek failure");
		close(subject_file);
		close(status_file);
		_exit(1);
	}
	
	do { /*check if the subject given by the user exists in file with subjects*/
		
		read_check = read(subject_file,check_buffer,MAX_SIZE);/*read subject's name*/
		if(read_check < 0){
			perror("read failure\n");
			close(subject_file);
			close(status_file);
			_exit(1);
		}
		system_check = lseek(subject_file,3,SEEK_CUR);/*set the offset to the next line of subject_file*/
		if (system_check < 0){
			perror("lseek failure");
			close(subject_file);
			close(status_file);
			_exit(1);
		}
		check_subjects = strncmp(subject_buffer,check_buffer,strlen(subject_buffer));/*check if the subject given by user matches a subject in subject_file*/
		
	} while (check_subjects != 0 && read_check != 0);/*repeat until either match for the given subject is found or end of subject file is reached*/
	
	if (check_subjects != 0) {/*if the subject is not found error message is printed*/
		printf("Invalid subject\n");
		return;
	}
	else {
		for(i = strlen(subject_buffer);i <= MAX_SIZE;i++){
			subject_buffer[i] = ' ';/*fill the rest subject_buffer with space characters*/
		}
		
		subject_buffer[MAX_SIZE] = '\0';
		strcpy(line_buffer,subject_buffer);/*copy subject_buffer to line_buffer and put 4 spaces in line_buffer after the subject_buffer is copied*/
		strcat(line_buffer,"    ");  
		
		
		printf("Enter date(MM/YY):\n");
		scanf("%d/%d",&month,&year);
		if (month > 0 && month < 13 && year > 0 && year < 15) {/*if date is valid*/
			
			sprintf(load_str,"%02d/%02d",month,year);/*transform month and year to string*/
			system_check = lseek(status_file,0,SEEK_SET);
			if (system_check < 0){
				perror("lseek failure");
				close(subject_file);
				close(status_file);
				_exit(1);
			}
			do { /*check if the subject given by the user exists in status file*/	
				read_check = read(status_file,check_buffer,MAX_SIZE);
				if(read_check < 0){
					perror("read failure\n");
					close(subject_file);
					close(status_file);
					_exit(1);
				}
				for(i = strlen(check_buffer);i < MAX_SIZE;i++){
					check_buffer[i] = ' ';/*fill the rest buffer with space characters*/
				}
				check_buffer[MAX_SIZE] = '\0';
				check_subjects = strncmp(subject_buffer,check_buffer,strlen(subject_buffer));/*check if the subject given by user matches a subject in status file*/
				if (check_subjects == 0) {/*if match is found*/
					system_check = lseek(status_file,4,SEEK_CUR);
					if (system_check < 0){
						perror("lseek failure");
						close(subject_file);
						close(status_file);
						_exit(1);
					}
					read_check = read(status_file,check_str,5);/*read date*/
					if (read_check < 0) {
						perror("read failure");
						close(subject_file);
						close(status_file);
						_exit(1);
					}
					
						system_check = lseek(status_file,8,SEEK_CUR);/*set the offset to the next line of status_file*/
						if (system_check < 0){
							perror("lseek failure");
							close(subject_file);
							close(status_file);
							_exit(1);
						}
						check_dates = strncmp(load_str,check_str,strlen(check_str));/*check if date given by user matches the date in status_file*/
						if (check_dates == 0) {/*if there is already added a subject with the specific date*/
							printf("You have already added this subject with the date above\n");
							return;
						}
					
				}
				else {
					system_check = lseek(status_file,81 - MAX_SIZE,SEEK_CUR);/*if there is not subject match found in the current line continue to next line*/
					if (system_check < 0){
						perror("lseek failure");
						close(subject_file);
						close(status_file);
						_exit(1);
					}
				}
		
			} while (read_check != 0);	/*until the end of status_file*/	
			strcat(line_buffer,load_str);/*copy check_str to line_buffer and add 4 space characters at the end of it*/
			strcat(line_buffer,"   ");
			
		}
		
		else {
			printf("Invalid date\n");/*if date is not valid*/
			return;
		}
		
		printf("Enter grade(NN.N):\n");
		scanf("%f",&grade);
		if (grade >= 0.0 && grade <= 10.0) { /*if grade is valid*/
			
			sprintf(load_str,"%04.1f",grade);/*put the grade in load_str*/
			
			system_check = lseek(status_file,0,SEEK_SET);
			if (system_check < 0){
				perror("lseek failure");
				close(subject_file);
				close(status_file);
				_exit(1);
			}
			
			do { /*check if the subject given by the user exists in status_file*/	
				read_check = read(status_file,check_buffer,MAX_SIZE);
				if(read_check < 0){
					perror("read failure\n");
					close(subject_file);
					close(status_file);
					_exit(1);
				}
				for(i = strlen(check_buffer);i < MAX_SIZE;i++){
					check_buffer[i] = ' ';/*fill the rest buffer with space characters*/
				}
				check_buffer[MAX_SIZE] = '\0';
				check_subjects = strncmp(subject_buffer,check_buffer,strlen(subject_buffer));/*check if the subject given by the user matches a subject in status_file*/
				
				if (check_subjects == 0) {/*if match is found*/
					system_check = lseek(status_file,12,SEEK_CUR);/*set offset at grade position*/
					if (system_check < 0){
						perror("lseek failure");
						close(subject_file);
						close(status_file);
						_exit(1);
					}
					read_check = read(status_file,check_str,4);/*read grade*/ 
					if(read_check < 0){
						perror("read failure\n");
						close(subject_file);
						close(status_file);
						_exit(1);
					}
					system_check = lseek(status_file,1,SEEK_CUR);
					if (system_check < 0){
						perror("lseek failure");
						close(subject_file);
						close(status_file);
						_exit(1);
					}                   
					sscanf(check_str,"%f",&check_grades);/*transform the string that contains  the existing grade to a float*/
					if (check_grades >= 5.0) {/*if there is already added a subject with the specific date*/
					printf("This subject is already added with a passing grade\n");
					return;
					}
				}
				else {
					system_check = lseek(status_file,81 - MAX_SIZE,SEEK_CUR);/*set offset to the next line of status_file*/
					if (system_check < 0){
						perror("lseek failure");
						close(subject_file);
						close(status_file);
						_exit(1);
					}
				}
				
				
		
			} while (read_check != 0);/*until end of file is reached*/
				
		strcat(line_buffer,load_str);/*write the grade to line_buffer*/
		}
		
		else {
			printf("Invalid grade\n");
			return;
		}
		system_check = lseek(status_file,0,SEEK_END);/*set offset at end of status_file*/
		if (system_check < 0){
			perror("lseek failure");
			close(subject_file);
			close(status_file);
			_exit(1);
		}
		system_check = write(status_file,line_buffer,80);/*write to status_file the line_buffer*/
		if (system_check < 0){
			perror("write failure");
			close(subject_file);
			close(status_file);
			_exit(1);
		}
		system_check = write(status_file,"\n",1);
		if (system_check < 0){
			perror("write failure");
			close(subject_file);
			close(status_file);
			_exit(1);
		}
		system_check = fsync(status_file);/*synchronize written data*/
		if (system_check < 0){
			perror("fsync failure");
			close(subject_file);
			close(status_file);
			_exit(1);
		}
		
		printf("The subject has been successfully added to your status_file\n");/*confirmation message*/
	}
}

void search (int status_file,int subject_file) {
	int i = 0,read_check = 0,check_subjects = 0,system_check = 0,j = 0;
	char subject_buffer_search[MAX_SIZE + 1];
	char check_buffer[MAX_SIZE + 1];
	char line_buffer[82];
	
	
	
		printf("Enter subject for search:\n");
		
		
		getchar();
		
		do {
			subject_buffer_search[i] = getchar(); /*read the subject given by user character by character*/
			i++;
		} while (i < 66 && subject_buffer_search[i - 1] != '\n');
		subject_buffer_search[i - 1] = '\0';

		system_check = lseek(status_file,0,SEEK_SET); /*set the offset at the beginning of the status file*/
		if (system_check < 0){
			perror("lseek failure");
			close(subject_file);
			close(status_file);
			_exit(1);
		}
		
		printf("Search results for your subjects:\n");
		
		while(1) { /*check if the subject given by the user exists in file with subjects*/	
			system_check = read_check = read(status_file,check_buffer,MAX_SIZE);/*read a subject name from status file*/
			if(read_check < 0){
				perror("read failure\n");
				close(subject_file);
				close(status_file);
				_exit(1);
			}
			
			if (read_check == 0) break;	/*if end of file is reached break*/
			for(i = strlen(check_buffer);i < MAX_SIZE;i++){
				check_buffer[i] = ' ';/*fill the rest buffer_search with space characters*/
			}
			check_buffer[MAX_SIZE] = '\0';
			
		
			check_subjects = strncmp(subject_buffer_search,check_buffer,strlen(subject_buffer_search));/*check if the subject given by user matches a subject in status file*/
			
			if (check_subjects == 0) {/*if match is found*/
				j++;/*counter if j = 0 there are no matches if not at least one match is found*/
				system_check = lseek(status_file,-MAX_SIZE,SEEK_CUR);/*set offset at the beginning of current line*/
				if (system_check < 0){
					perror("lseek failure");
					close(subject_file);
					close(status_file);
					_exit(1);
				}
				system_check = read(status_file,line_buffer,81);/*read the whole current line*/
				if(system_check < 0){
					perror("read failure\n");
					close(subject_file);
					close(status_file);
					_exit(1);
				}
				line_buffer[81] = '\0';
				printf("%s\n",line_buffer);/*print the whole current line*/
				
			}
			else {
				system_check = lseek(status_file,81 - MAX_SIZE,SEEK_CUR);/*if no match is found in the current line continue to next line*/
				if (system_check < 0){
					perror("lseek failure");
					close(subject_file);
					close(status_file);
					_exit(1);
				}
			}
			
		}
		if (j == 0) printf("No search results were found for this subject\n");
		return;
	
	
}


void average(int subject_file,int status_file) {
	
	char status_buffer[MAX_SIZE + 1];
	char subject_buffer[MAX_SIZE + 1];
	char check_grade[5];
	char check_weight[2];
	float check_grades = 0,average = 0;
	int i = 0,check_subjects = 0,read_subject = 0,read_status = 0,weight = 0,total_weight = 0,product = 0,j = 0,system_check = 0;
	
	system_check = lseek(subject_file,0,SEEK_SET);/*set offset at the beginning of subject file*/
	if (system_check < 0){
		perror("lseek failure");
		close(subject_file);	
		close(status_file);
		_exit(1);
	}
	system_check = lseek(status_file,0,SEEK_SET);/*set offset at the beginning of status file*/
	if (system_check < 0){
		perror("lseek failure");
		close(subject_file);
		close(status_file);
		_exit(1);
	}
	do {
		system_check = lseek(subject_file,0,SEEK_SET);/*begin again from the beginning of subject_file*/
		if (system_check < 0){
			perror("lseek failure");
			close(subject_file);
			close(status_file);
			_exit(1);
		}
		system_check = read_status = read(status_file,status_buffer,64);/*read a subject's name from status_file*/
		if(system_check < 0){
			perror("read failure\n");
			close(subject_file);
			close(status_file);
			_exit(1);
		}
		if (read_status == 0) break;/*if end of status file is reached break and return*/
		for(i = strlen(status_buffer);i <= MAX_SIZE;i++){
				status_buffer[i] = ' ';/*fill the rest buffer with space characters*/
			}
		status_buffer[MAX_SIZE] = '\0';
		system_check = lseek(status_file,12,SEEK_CUR);/*set offset at grade position of the current line in status_file*/
		if (system_check < 0){
			perror("lseek failure");
			close(subject_file);
			close(status_file);
			_exit(1);
		}
		system_check = read(status_file,check_grade,5);/*read the grade*/
		if(system_check < 0){
			perror("read failure\n");
			close(subject_file);
			close(status_file);
			_exit(1);
		}
		check_grade[5] = '\0';
		sscanf(check_grade,"%f",&check_grades);/*transform check_grade string to grade float*/
		
	
		if (check_grades >= 5.0) {
			j++;/*counter that shows how many subjects have been passed*/
			do {
				
				system_check = read_subject = read(subject_file,subject_buffer,64);/*read a subject's name in subject file*/
				if(system_check < 0){
					perror("read failure\n");
					close(subject_file);
					close(status_file);
					_exit(1);
				}
				if (read_subject == 0) break;/*if end of subject file is reached break from loop and continue with the next subject from status file*/
				
				for(i = strlen(subject_buffer);i < MAX_SIZE;i++){
					subject_buffer[i] = ' ';/*fill the rest subject_buffer with space characters*/
				}
				subject_buffer[MAX_SIZE] = '\0';

				check_subjects = strncmp(subject_buffer,status_buffer,strlen(subject_buffer));/*check if subject's name from status file matches a subject's name from subject_file*/

				if (check_subjects == 0) {/*if match is found*/
					system_check = lseek(subject_file,1,SEEK_CUR);/*set offset at weight position*/
					if (system_check < 0){
						perror("lseek failure");
						close(subject_file);
						close(status_file);
						_exit(1);
					}
					system_check = read(subject_file,check_weight,1);/*read the weight from subject_file*/
					if(system_check < 0){
						perror("read failure\n");
						close(subject_file);
						close(status_file);
						_exit(1);
					}
					system_check = lseek(subject_file,1,SEEK_CUR);/*set offset at the next line*/
					if (system_check < 0){
						perror("lseek failure");
						close(subject_file);
						close(status_file);
						_exit(1);
					}
					sscanf(check_weight,"%d",&weight);/*get a decimal from check weight string*/
					total_weight = total_weight + weight;
					product = product + check_grades * weight;/*product is the product of the grade and weight of the current subject plus the same product from other subjects*/
					average = product / total_weight;
					
				}
				else {
					system_check = lseek(subject_file,3,SEEK_CUR);/*if no match is found set offset to the next line of subject_file*/
					if (system_check < 0){
						perror("lseek failure");
						close(subject_file);
						close(status_file);
						_exit(1);
					}
				}
			} while(read_subject != 0);/*until end of subject_file is reached*/
		}
	} while(1);
	printf("The number of subjects with grade five or higher is:%d\n",j);
	printf("Your average is:%.1f\n",average);
	
			
}


void delete(int subject_file,int status_file) {
	int i = 0;
	char subject_buffer[MAX_SIZE + 1];
	char check_subject_buffer[MAX_SIZE + 1];
	char check_date_buffer[6];
	char date_buffer[6];
	int check_subjects = 0,check_dates = 0,check_read = 0,end = 0,check = 0,system_check = 0;
	char line_buffer[83];
	
	
	
	system_check = lseek(subject_file,0,SEEK_SET);/*set offset at the beginning of subject_file*/
	if (system_check < 0){
		perror("lseek failure");
		close(subject_file);
		close(status_file);
		_exit(1);
	}
	system_check = lseek(status_file,0,SEEK_SET);/*set offset at the beginning of status_file*/
	if (system_check < 0){
		perror("lseek failure");
		close(subject_file);
		close(status_file);
		_exit(1);
	}
	printf("Please enter subject to be deleted:\n");
	getchar();
	do {
		subject_buffer[i] = getchar(); /*read the subject to be deleted given by user character by character*/
		i++;
	} while (i < 66 && subject_buffer[i - 1] != '\n');
	subject_buffer[i - 1] = '\0';
	printf("Please enter the date of the register you want to be deleted (MM/YY):\n");
	
	i = 0;
	do {
		
		
		date_buffer[i] = getchar(); /*read the date given by user character by character*/
		i++;
	} while (i < 5);
	date_buffer[5] = '\0';
	while (1) {
		system_check = read(status_file,check_subject_buffer,64);/*read a subject's name from status_file*/
		if(system_check < 0){
			perror("read failure\n");
			close(subject_file);
			close(status_file);
			_exit(1);
		}
		if (system_check == 0) break;
		check_subject_buffer[MAX_SIZE] = '\0';
		
		check_subjects = strncmp(check_subject_buffer,subject_buffer,strlen(subject_buffer));/*check whether the subject that is to be deleted matches a subject in status_file*/
		
		if (check_subjects == 0) {/*if match is found*/
	
			system_check = lseek(status_file,4,SEEK_CUR);/*set offset at date position*/
			if (system_check < 0){
				perror("lseek failure");
				close(subject_file);
				close(status_file);
				_exit(1);
			}
			system_check = read(status_file,check_date_buffer,5);/*read date*/
			if(system_check < 0){
				perror("read failure\n");
				close(subject_file);
				close(status_file);
				_exit(1);
			}
			
			check_dates = strncmp(check_date_buffer,date_buffer,strlen(date_buffer));/*check if the date given by user matches the date of the current registration*/
			
			system_check = lseek(status_file,8,SEEK_CUR);/*set offset to the next line of status_file*/
			if (system_check < 0){
				perror("lseek failure");
				close(subject_file);
				close(status_file);
				_exit(1);
			}
			if (check_dates == 0) {/*if dates are matching each other*/
				
				
				system_check = check = read(status_file,line_buffer,81);
				if(system_check < 0){
					perror("read failure\n");
					close(subject_file);
					close(status_file);
					_exit(1);
				}
				if (check != 0) {/*if the next line isnt vacant*/
					system_check = lseek(status_file,-162,SEEK_CUR);/*set offset 2 lines before the current line*/
					if (system_check < 0){
						perror("lseek failure");
						close(subject_file);
						close(status_file);
						_exit(1);
					}
					system_check = write(status_file,line_buffer,81);/*write the subject of the next line of the deleted registration to the line of the deleted registration*/
					if (system_check < 0){
						perror("write failure");
						close(subject_file);
						close(status_file);
						_exit(1);
					}
					while (1) {
						system_check = lseek(status_file,81,SEEK_CUR);/*continue to next line*/
						if (system_check < 0){
							perror("lseek failure");
							close(subject_file);
							close(status_file);
							_exit(1);
						}
						system_check = check_read = read(status_file,line_buffer,81);/*read the next subject (the subject that is after the subject i just overwrote to the deleting position*/
						if(system_check < 0){
							perror("read failure\n");
							close(subject_file);
							close(status_file);
							_exit(1);
						}
						if (check_read == 0) break;
						system_check = lseek(status_file,-162,SEEK_CUR);/*set offset position two lines above*/
						if (system_check < 0){
							perror("lseek failure");
							close(subject_file);
							close(status_file);
							_exit(1);
						}
						system_check = write(status_file,line_buffer,81);/*write the new subject to the current line*/
						if (system_check < 0){
							perror("write failure");
							close(subject_file);
							close(status_file);
							_exit(1);
						}
					}
				}
				
				end = (off_t) lseek(status_file,-81,SEEK_END);/*set offset a line before end of current status file*/
				if (end < 0){
					perror("lseek failure");
					close(subject_file);
					close(status_file);
					_exit(1);
				}
				system_check = ftruncate(status_file,(off_t)end);/*short the current status_file by one line that happens also if the last line of status file is to be deleted*/
				if (system_check < 0){
					perror("ftruncate failure");
					close(subject_file);
					close(status_file);
					_exit(1);
				}
				fsync(status_file);/*synchronize data*/
				if (system_check < 0){
					perror("fsync failure");
					close(subject_file);
					close(status_file);
					_exit(1);
				}
				printf("You have successfully deleted the register\n");/*confirmation message*/
				return;
			}
			
		}
		else {
			system_check = lseek(status_file,81 - MAX_SIZE,SEEK_CUR);/*if no match is found continue to the next line of status_file*/
			if (system_check < 0){
				perror("lseek failure");
				close(subject_file);
				close(status_file);
				_exit(1);
			}
		}
			
	}
	printf("Delete Failed\n");/*error message*/
}

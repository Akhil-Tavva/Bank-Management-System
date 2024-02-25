#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<netinet/in.h>
#include "header.h"

int getnormaluser();
int getjointuser();
int getadminuser();

int getnormaluser(){
	int fd=open("NormalUserFile",O_RDONLY,0744);
	NormalUser user;
	lseek(fd,-sizeof(NormalUser),SEEK_END);
	read(fd,&user,sizeof(NormalUser));
	close(fd);
	return user.ID+1;
}

int getjointuser(){
	int fd=open("JointUserFile",O_RDONLY,0744);
	JointUser user;
	lseek(fd,-sizeof(JointUser),SEEK_END);
	read(fd,&user,sizeof(JointUser));
	close(fd);
	return user.ID+1;
}

int getadminuser(){
	int fd=open("AdminFile",O_RDONLY,0744);
	Admin user;
	lseek(fd,-sizeof(Admin),SEEK_END);
	read(fd,&user,sizeof(Admin));
	close(fd);
	return user.ID+1;
}

int main()
{
    int fd1 = open("NormalUserFile", O_RDWR | O_CREAT, 0744);
    int fd2 = open("JointUserFile", O_RDWR | O_CREAT, 0744);
    int fd3 = open("AdminFile", O_RDWR | O_CREAT, 0744);

    int choice=0;
	Admin new;
	printf("Enter name of admin: ");
	scanf(" %s",new.name);
	printf("Enter the password(max 10 char): ");
	scanf(" %s",new.password);
	new.ID=1000;
	printf("Your userID is : %d\n",new.ID);
	write(fd3, &new,sizeof(new));
	printf("Do you want to continue(0/1)? ");
	scanf("%d",&choice);
    while(choice){
		printf("Enter name of admin: ");
		scanf(" %[^\n]",new.name);
		printf("Enter the password(max 10 char): ");
		scanf(" %[^\n]",new.password);
		new.ID=getadminuser();
		printf("Your UserID is : %d\n",new.ID);
		write(fd3,&new,sizeof(Admin));
		printf("Do you want to continue (0/1)? ");
		scanf("%d",&choice);
	}

    choice=1;
	NormalUser newuser;
	printf("Enter name of the normal user: ");
	scanf(" %[^\n]",newuser.name);
	printf("Enter the password(max 10 char): ");
	scanf(" %[^\n]",newuser.password);
	newuser.ID=1000;
	newuser.balance=1000;
	newuser.acc_no=(newuser.ID-1000)+100000;
	printf("Your userID is : %d\n",newuser.ID);
	strcpy(newuser.status,"ACTIVE");
	write(fd1,&newuser,sizeof(NormalUser));
	printf("Do you want to continue(0/1)? ");
	scanf("%d",&choice);
	while(choice){
		printf("Enter name of normal user: ");
		scanf(" %[^\n]",newuser.name);
		printf("Enter the password(max 10 char): ");
		scanf(" %[^\n]",newuser.password);
		newuser.ID=getnormaluser();
		newuser.balance=1000;
		newuser.acc_no=(newuser.ID-1000)+100000;
		printf("Your userID is : %d\n",newuser.ID);
		strcpy(newuser.status,"ACTIVE");
		write(fd1,&newuser,sizeof(NormalUser));
		printf("Do you want to continue(0/1)? ");
		scanf("%d",&choice);
	}

	choice=1;
	JointUser newJUser;
	printf("Enter main name of joint user: ");
	scanf(" %[^\n]",newJUser.name1);
	printf("Enter second name of joint user: ");
	scanf(" %[^\n]",newJUser.name2);
	printf("Enter the password(max 10 char): ");
	scanf(" %[^\n]",newJUser.password);
	newJUser.ID=1000;
	newJUser.balance=1000;
	newJUser.acc_no=(newJUser.ID-1000)+100000;
	printf("Your userID is : %d\n",newJUser.ID);
	strcpy(newJUser.status,"ACTIVE");
	write(fd2,&newJUser,sizeof(JointUser));
	printf("Do you want to continue(0/1)? ");
	scanf("%d",&choice);
	while(choice){
		printf("Enter main name of joint user: ");
		scanf(" %[^\n]",newJUser.name1);
		printf("Enter second name of joint user: ");
		scanf(" %[^\n]",newJUser.name2);
		printf("Enter the password(max 10 char): ");
		scanf(" %[^\n]",newJUser.password);
		newJUser.ID=getjointuser();
		newJUser.balance=1000;
		newJUser.acc_no=(newJUser.ID-1000)+100000;
		printf("Your userID is : %d\n",newJUser.ID);
		strcpy(newJUser.status,"ACTIVE");
		write(fd2,&newJUser,sizeof(JointUser));
		printf("Do you want to continue(0/1)? ");
		scanf("%d",&choice);
	}
    close(fd1);
    close(fd2);
    close(fd3);


}
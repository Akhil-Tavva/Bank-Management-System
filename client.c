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

void checkNormalUser(int sd);
void checkJointUser(int sd);
void checkAdmin(int sd);
void Menu(int sd);
void User_Menu(int sd);
void Admin_Menu(int sd);
void DepositMoney(int sd);
void WithdrawMoney(int sd);
void Balance(int sd);
void ChangePassword(int sd);
void AccountHandler(int sd);
void addAccount(int sd);
void deleteAccount(int sd);
void modifyAccount(int sd);
void searchAccount(int sd);
int option;

void chooseChoice(int sd){
    write(1, "Login in the database as a:\n", sizeof("Login in the database as a:\n"));
    write(1, "1. Normal User\n", sizeof("1. Normal User\n"));
    write(1, "2. Joint Account User\n", sizeof("2. Joint Account User\n")); 
    write(1, "3. Administrator\n", sizeof("3. Administrator\n"));
    write(1, "0. Exit\n", sizeof("0. Exit\n"));

    write(1, "Enter your choice: ", sizeof("Enter your choice: "));
    scanf("%d", &option);
    printf("choice: %d\n", option);

    switch(option){
        case 1:
		    checkNormalUser(sd);
			break;
			
        case 2:
		    checkJointUser(sd);
			break;

		case 3:
			checkAdmin(sd);
			break;
			
        case 0:
            exit(0);

        default:
		    write(1, "Invalid option!!\n", sizeof("Invalid option!!\n"));
			break;
    }
    chooseChoice(sd);
}

void checkNormalUser(int sd){
    bool res;
    NormalUser user;
	write(1, "User ID: ", sizeof("User ID: "));
    scanf("%d", &user.ID);
    write(1, "User Password: ", sizeof("User Password: "));
    scanf("%s", user.password);

    write(sd,&option,sizeof(int));
	write(sd,&user, sizeof(user));
	read(sd, &res, sizeof(res));

    if(res){
        write(1, "Successfully logged in\n", sizeof("Successfully logged in\n"));
        Menu(sd);
    }
    else{
        write(1, "Invalid login\n", sizeof("Invalid login\n"));
        chooseChoice(sd);
    }
    return ;
}

void checkJointUser(int sd){
    bool res;
    JointUser user;
	write(1, "User ID : ", sizeof("User ID : "));
    scanf("%d", &user.ID);
    write(1, "User Password: ", sizeof("User Password: "));
    scanf("%s", user.password);
    
    write(sd,&option,sizeof(int));    
    write(sd,&user, sizeof(user));
	read(sd, &res, sizeof(res));
    if(res){
        write(1, "Successfully logged in\n", sizeof("Successfully logged in\n"));
        Menu(sd);
    }
    else{
        write(1, "Invalid login\n", sizeof("Invalid login\n"));
        chooseChoice(sd);
    }
    return ;
}

void checkAdmin(int sd){
    bool res;
    Admin user;
	write(1, "User ID : ", sizeof("User ID : "));
    scanf("%d", &user.ID);
    write(1, "User Password: ", sizeof("User Password: "));
    scanf("%s", user.password);

    write(sd,&option,sizeof(int));
    write(sd, &user, sizeof(user));
 	read(sd, &res, sizeof(res));
   
    if(res){
        write(1, "Successfully logged in\n", sizeof("Successfully logged in\n"));
        Menu(sd);
    }
    else{
        write(1, "Invalid login\n", sizeof("Invalid login\n"));
        chooseChoice(sd);
    }
    return ;
}

void Menu(int sd){
    if(option == 1 || option == 2){
        User_Menu(sd);
    }
    else{
        Admin_Menu(sd);
    }
    return ;
}

void User_Menu(int sd){
    int choice;
    write(1,"1 : Deposit Money\n",sizeof("1 : Deposit Money\n"));
	write(1,"2 : Withdraw Money\n",sizeof("2 : Withdraw Money\n"));
	write(1,"3 : Balance Enquiry\n",sizeof("3 : Balance Enquiry\n"));
	write(1,"4 : Change the Password\n",sizeof("4 : Change the Password\n"));
	write(1,"5 : View Details\n",sizeof("5 : View Details\n"));
	write(1,"0 : Exit\n",sizeof("0 : Exit\n"));

    write(1,"Choose an option : ",sizeof("Choose an option : "));
	scanf("%d",&choice);
	printf("Option : %d\n", choice);

    switch(choice){
		case 1:
			DepositMoney(sd);
			break;

		case 2:
			WithdrawMoney(sd);
			break;

		case 3:
			Balance(sd);
			break;
		
        case 4:
			ChangePassword(sd);
			break;
		
        case 5:
			AccountHandler(sd);
			break;
		
        case 0:
			write(sd, &choice, sizeof(int));
            write(1, "Thank You\n", sizeof("Thank You\n"));
			return;
		
        default :
			write(1,"Invalid option!!\n\n",sizeof("Invalid option!!\n\n"));
			break;
	}
    User_Menu(sd);
}

void Admin_Menu(int sd){
    int choice;
    write(1,"1 : Add a Account\n",sizeof("1 : Add a Account\n"));
	write(1,"2 : Delete a Account\n",sizeof("2 : Delete a Account\n"));
	write(1,"3 : Modify a Account\n",sizeof("3 : Modify a Account\n"));
	write(1,"4 : Search a Account\n",sizeof("4 : Search a Account\n"));
	write(1,"0 : Exit\n",sizeof("0 : Exit\n"));

	write(1,"Choose an option : ",sizeof("Choose an option : "));
	scanf("%d",&choice);
	printf("Option : %d\n", choice);
			
	switch(choice){
        case 1: 
			addAccount(sd);
			break;

		case 2:
			deleteAccount(sd);
			break;

		case 3:
			modifyAccount(sd);
			break;

		case 4:
			searchAccount(sd);
			break;

		case 0:
			write(sd, &choice, sizeof(int));
            write(1, "Thank you\n", sizeof("Thank you\n"));
			return;

		default :
			write(1,"Invalid option!!\n\n",sizeof("Invalid option!!\n\n"));
			break;
	}
    Admin_Menu(sd);
}

void DepositMoney(int sd){
    float amount;
    int select=1;
	bool result;

	write(1,"Amount to Deposit : Rs.",sizeof("Amount to Deposit : Rs."));
	scanf("%f",&amount);

	while(amount<=0){
		printf("Enter a valid amount\n");
		write(1,"Amount to Deposit : Rs.",sizeof("Amount to Deposit : Rs."));
		scanf("%f",&amount);
	}

	//to the server
	write(sd, &select, sizeof(int));
	write(sd, &amount, sizeof(float));

	read(sd, &result, sizeof(result)); //from the server

	if(result){
        write(1,"Succesfully deposited\n",sizeof("Succesfully deposited\n"));
	}
	else{
		write(1,"Error depositing your money\n\n",sizeof("Error depositing your money\n\n"));
	}
    return ;
}

void WithdrawMoney(int sd){
    float amount;
	int select=2;
	bool result;

	write(1,"Amount to Withdraw : Rs.",sizeof("Amount to Withdraw : Rs."));
	scanf("%f",&amount);

	while(amount<=0){
		printf("Enter a valid amount!!\n");
		write(1,"Amount to Withdraw : Rs.",sizeof("Amount to Withdraw : Rs."));
		scanf("%f",&amount);
	}

	//to the server
	write(sd, &select, sizeof(int));
	write(sd, &amount, sizeof(float));

	read(sd,&result,sizeof(result)); //from the server

	if(!result){
		write(1,"Error withdrawing your money, please check your balance\n",sizeof("Error depositing your money, please check your balance\n"));
	}
	else{
		write(1,"Succesfully withdrawn the money\n",sizeof("Succesfully withdrawn the money\n"));
	}
    return ;
}

void Balance(int sd){
    float amount;
	int select=3;

	//to the server
	write(sd, &select, sizeof(int));
	
	int len = read(sd, &amount, sizeof(float));	//from server

	write(1,"Available Balance in your account :: Rs.",sizeof("Available Balance in your account :: Rs."));
	printf("%0.2f\n",amount);
}

void ChangePassword(int sd){
    int select=4;
	char newPass[10];
	bool result;

	write(1,"Enter the new password(max 10 characters) : ",sizeof("Enter the new password(max 10 characters) : "));
	scanf("%s",newPass);

	//to the server
	write(sd,&select,sizeof(int));
	write(sd,newPass,sizeof(newPass));

	read(sd,&result,sizeof(result)); //from the server

	if(!result){
		write(1,"Error changing your password\n",sizeof("Error changing your password\n"));
	}
	else{
		write(1,"Succesfully changed your password\n",sizeof("Succesfully changed your password\n"));
	}
    return ;
}

void AccountHandler(int sd){
    int select=5;
	//to the server
	write(sd,&select,sizeof(int));
	if(option==1){
		NormalUser user;
		read(sd, &user, sizeof(NormalUser));
		printf("User ID: %d\n",user.ID);
		printf("Name: %s\n",user.name);
		printf("Account Number: %d\n",user.acc_no);
		printf("Available Balance: Rs.%0.2f\n",user.balance);
		printf("Status : %s\n",user.status);
	}
	else if(option==2){
		JointUser user2;
		read(sd,&user2,sizeof(JointUser));
		
		printf("User ID : %d\n",user2.ID);
		printf("Main Account Holder's Name : %s\n",user2.name1);
		printf("Other Account Holder's Name : %s\n",user2.name2);
		printf("Account Number : %d\n",user2.acc_no);
		printf("Available Balance : Rs.%0.2f\n",user2.balance);
		printf("Status : %s\n\n",user2.status);
	}
    return ;
}

void addAccount(int sd){
    int select=1;
	int type;
	bool res;

	write(sd, &select, sizeof(int));

	write(1,"Enter the type of account(1: Normal Account 2: Joint Account): ",sizeof("Enter the type of account(1: Normal Account 2: Joint Account): "));
	scanf("%d",&type);

	write(sd,&type,sizeof(int));

	if(type==1){
		NormalUser adduser;
		write(1,"Name of the account holder : ",sizeof("Name of the account holder : "));
		scanf(" %[^\n]",adduser.name);
		write(1,"Password(max 10 characters) : ",sizeof("Password(max 10 characters) : "));
		scanf("%s",adduser.password);
		write(1,"Initial Deposit : Rs.",sizeof("Initial Deposit : Rs."));
		scanf("%f",&adduser.balance);
		write(sd,&adduser,sizeof(NormalUser));
	}

	if(type==2){
		JointUser adduser;
		write(1,"Name of the primary account holder : ",sizeof("Name of the primary account holder : "));
		scanf(" %[^\n]",adduser.name1);
		write(1,"Name of the other account holder : ",sizeof("Name of the other account holder : "));
		scanf(" %[^\n]",adduser.name2);
		write(1,"Password(max 10 characters) : ",sizeof("Password(max 10 characters) : "));
		scanf("%s",adduser.password);
		write(1,"Initial Deposit : Rs.",sizeof("Initial Deposit : Rs."));
		scanf("%f",&adduser.balance);
		write(sd,&adduser,sizeof(JointUser));
	}
	
	read(sd,&res,sizeof(res)); //from the server

	if(!res){
		write(1,"Error adding the account\n",sizeof("Error adding the account\n"));
	}
	else{
		write(1,"Succesfully added the account\n",sizeof("Succesfully added the account\n"));
	}
}

void deleteAccount(int sd){
    int select=2;
	int type,ID;
	bool res;

	write(sd, &select, sizeof(int));

	write(1,"Enter the type of the account(1: Normal Account 2: Joint Account): ",sizeof("Enter the type of the account(1: Normal Account 2: Joint Account) : "));
	scanf("%d",&type);
	
	write(sd,&type,sizeof(int));

	write(1,"User ID : ",sizeof("User ID : "));

	scanf("%d",&ID);
	write(sd,&ID,sizeof(int));
	
	read(sd,&res, sizeof(res)); //from the server

	if(!res){
		write(1,"Error deleting the account,please recheck the ID\n",sizeof("Error deleting the account,please recheck the ID\n"));
	}
	else{
		write(1,"Succesfully deleted the account\n",sizeof("Succesfully deleted the account\n"));
	}
    return ;
}

void modifyAccount(int sd){
    int select=3,type;
	bool result;

	write(sd, &select, sizeof(int));

	write(1,"Enter the type of the account(1: Normal Account 2: Joint Account) : ",sizeof("Enter the type of the account(1: Normal Account 2: Joint Account) : "));
	scanf("%d",&type);

	write(sd,&type,sizeof(int));

	if(type==1){
		NormalUser user;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&user.ID);
		write(1,"Account Number : ",sizeof("Account Number : "));
		scanf("%d",&user.acc_no);
		write(1,"New Name of the account holder : ",sizeof("New Name of the account holder : "));
		scanf(" %[^\n]",user.name);
		write(1,"New Password(max 10 characters) : ",sizeof("New Password(max 10 characters) : "));
		scanf("%s",user.password);
		write(1,"New Balance : ",sizeof("New Balance : "));
		scanf("%f",&user.balance);
		write(sd,&user,sizeof(NormalUser));
	}

	if(type==2){
		JointUser user;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&user.ID);
		write(1,"Account Number : ",sizeof("Account Number : "));
		scanf("%d",&user.acc_no);
		write(1,"New Name of the primary account holder : ",sizeof("New Name of the primary account holder : "));
		scanf(" %[^\n]",user.name1);
		write(1,"New Name of the other account holder : ",sizeof("New Name of the other account holder : "));
		scanf(" %[^\n]",user.name2);
		write(1,"New Password(max 10 characters) : ",sizeof("New Password(max 10 characters) : "));
		scanf("%s",user.password);
		write(1,"New Balance : ",sizeof("New Balance : "));
		scanf("%f",&user.balance);
		write(sd,&user,sizeof(JointUser));
	}
	
	read(sd,&result,sizeof(result)); //from the server

	if(!result){
		write(1,"Error modifying the account,please recheck the User ID and Account No\n",sizeof("Error modifying the account,please recheck the User ID and Account No\n"));
	}
	else{
		write(1,"Succesfully modified the account\n",sizeof("Succesfully modified the account\n"));
	}
    return ;
}

void searchAccount(int sd){
    int select=4;
	int type,len;
	bool res;

	write(sd, &select, sizeof(int));

	write(1,"Enter the type of the account(1: Normal Account 2: Joint Account): ",sizeof("Enter the type of the account(1: Normal Account 2: Joint Account): "));
	scanf("%d",&type);

	write(sd,&type,sizeof(int));

	if(type==1){
		NormalUser user;
		int ID;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&ID);
		write(sd,&ID,sizeof(int));
		
		len=read(sd,&user,sizeof(NormalUser));
		if(len==0){
			write(1,"Please recheck the User ID\n",sizeof("Please recheck the User ID\n"));
		}
		else{
			printf("User ID : %d\n",user.ID);
			printf("Name : %s\n",user.name);
			printf("Account Number : %d\n",user.acc_no);
			printf("Available Balance : Rs.%0.2f\n",user.balance);
			printf("Status : %s\n",user.status);
		}
	}

	if(type==2){
		JointUser user;
		int ID;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&ID);
		write(sd,&ID,sizeof(int));
		
		len=read(sd,&user,sizeof(JointUser));
		if(len==0){
			write(1,"Please recheck the User ID\n",sizeof("Please recheck the User ID\n"));
		}
		else{
			printf("User ID : %d\n",user.ID);
			printf("Main Account Holder's Name : %s\n",user.name1);
			printf("Other Account Holder's Name : %s\n",user.name2);
			printf("Account Number : %d\n",user.acc_no);
			printf("Available Balance : Rs.%0.2f\n",user.balance);
			printf("Status : %s\n",user.status);
		}
	}
    return ;
}

int main()
{
    struct sockaddr_in server;
    int sd = socket(AF_INET,SOCK_STREAM,0);

	server.sin_family=AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5555);

    connect(sd,(struct sockaddr *)&server,sizeof(server));
    write(1, "Client is connected to the server...\n", strlen("Client is connected to the server...\n"));
    
    chooseChoice(sd);

	close(sd);
	return 0;
}
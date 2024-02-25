typedef struct normalUser{
	int ID;
	char name[30];
	char password[10];
	int acc_no;
	float balance;
	char status[20];
}NormalUser;

typedef struct jointUser{
	int ID;
	char name1[30];
	char name2[30];
	char password[30];
	int acc_no;
	float balance;
	char status[20];
}JointUser;

typedef struct admin{
	int ID;
	char name[30];
	char password[30];
}Admin;
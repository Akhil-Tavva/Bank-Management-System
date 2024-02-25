#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <pthread.h>
#include "header.h"

bool checkNormalUser(NormalUser user);
bool checkJointUser(JointUser user);
bool checkAdmin(Admin user);
NormalUser getNormalUser(int ID);
JointUser getJointUser(int ID);
Admin getAdmin(int ID);
bool addNormalUser(NormalUser user);
bool addJointUser(JointUser user);
bool deleteNormalUser(int ID);
bool deleteJointUser(int ID);
bool modifyNormalUser(NormalUser user);
bool modifyJointUser(JointUser user);
bool depositMoney(int acc_type, int ID, float amount);
bool withdrawMoney(int acc_type, int ID, float amount);
float getBalance(int acc_type, int ID);
bool alterPassword(int acc_type, int ID, char newpass[10]);
void server_task(int nsd);
void *conn_handler(void *nsd);

bool checkNormalUser(NormalUser user){
	int i = user.ID - 1000;
	struct flock lock;
	int fd = open("NormalUserFile", O_RDONLY, 0744);
	bool output;
	NormalUser temp;

	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(NormalUser);
	lock.l_len = sizeof(NormalUser);
	lock.l_pid = getpid();

	fcntl(fd, F_SETLKW, &lock);
	lseek(fd, (i) * sizeof(NormalUser), SEEK_SET);
	read(fd, &temp, sizeof(NormalUser));
	if (!strcmp(temp.password, user.password) && !strcmp(temp.status, "ACTIVE"))
	{
		output = true;
	}
	else
	{
		output = false;
	}

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return output;
}

bool checkJointUser(JointUser user){
	int i = user.ID - 1000;
	struct flock lock;
	int fd = open("JointUserFile", O_RDONLY, 0744);
	bool output;
	JointUser ju;

	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(JointUser);
	lock.l_len = sizeof(JointUser);
	lock.l_pid = getpid();

	fcntl(fd, F_SETLKW, &lock);
	lseek(fd, (i) * sizeof(JointUser), SEEK_SET);
	read(fd, &ju, sizeof(JointUser));
	if (!strcmp(ju.password, user.password) && !strcmp(ju.status, "ACTIVE"))
	{
		output = true;
	}
	else
	{
		output = false;
	}

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return output;
}

bool checkAdmin(Admin user){
	int i = user.ID - 1000;
	struct flock lock;
	int fd = open("AdminFile", O_RDONLY, 0744);
	bool output;
	Admin ad;

	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(Admin);
	lock.l_len = sizeof(Admin);
	lock.l_pid = getpid();

	fcntl(fd, F_SETLKW, &lock);
	lseek(fd, (i) * sizeof(Admin), SEEK_SET);
	read(fd, &ad, sizeof(Admin));
	if (!strcmp(ad.password, user.password))
	{
		output = true;
	}
	else
	{
		output = false;
	}
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);

	close(fd);
	return output;
}

NormalUser getNormalUser(int ID){
	int i = ID - 1000;
	struct flock lock;
	NormalUser user;
	int fd = open("NormalUserFile", O_RDONLY, 0744);

	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(NormalUser);
	lock.l_len = sizeof(NormalUser);
	lock.l_pid = getpid();

	fcntl(fd, F_SETLKW, &lock);
	lseek(fd, (i) * sizeof(NormalUser), SEEK_SET);
	read(fd, &user, sizeof(NormalUser));

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return user;
}

JointUser getJointUser(int ID){
	int i = ID - 1000;
	struct flock lock;
	JointUser user;
	int fd = open("JointUserFile", O_RDONLY, 0744);

	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(JointUser);
	lock.l_len = sizeof(JointUser);
	lock.l_pid = getpid();

	fcntl(fd, F_SETLKW, &lock);

	lseek(fd, (i) * sizeof(JointUser), SEEK_SET);
	read(fd, &user, sizeof(JointUser));

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return user;
}

Admin getAdmin(int ID){
	int i = ID - 1000;
	Admin user;
	struct flock lock;
	int fd = open("AdminFile", O_RDONLY, 0744);

	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(Admin);
	lock.l_len = sizeof(Admin);
	lock.l_pid = getpid();

	fcntl(fd, F_SETLKW, &lock);
	lseek(fd, (i) * sizeof(Admin), SEEK_SET);
	read(fd, &user, sizeof(Admin));

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return user;
}

bool addNormalUser(NormalUser user){
	int fd = open("NormalUserFile", O_RDWR, 0744);
	NormalUser enduser;
	bool output;

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_END;
	lock.l_start = (-1) * sizeof(NormalUser);
	lock.l_len = sizeof(NormalUser);
	lock.l_pid = getpid();

	fcntl(fd, F_SETLKW, &lock);
	lseek(fd, (-1) * sizeof(NormalUser), SEEK_END);
	read(fd, &enduser, sizeof(NormalUser));

	user.ID = enduser.ID + 1;
	user.acc_no = enduser.acc_no + 1;
	strcpy(user.status, "ACTIVE");

	if (write(fd, &user, sizeof(NormalUser)) != 0)
	{
		output = true;
	}
	else
		output = false;

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return output;
}

bool addJointUser(JointUser user){
	int fd = open("JointUserFile", O_RDWR, 0744);
	struct flock lock;
	JointUser enduser;
	bool output;

	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_END;
	lock.l_start = (-1) * sizeof(JointUser);
	lock.l_len = sizeof(JointUser);
	lock.l_pid = getpid();

	fcntl(fd, F_SETLKW, &lock);

	lseek(fd, (-1) * sizeof(JointUser), SEEK_END);
	read(fd, &enduser, sizeof(JointUser));

	user.ID = enduser.ID + 1;
	user.acc_no = enduser.acc_no + 1;
	strcpy(user.status, "ACTIVE");

	if (write(fd, &user, sizeof(JointUser)) != 0)
	{
		output = true;
	}
	else
		output = false;

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return output;
}

bool deleteNormalUser(int ID){
	int i = ID - 1;
	int fd = open("NormalUserFile", O_RDWR, 0744);
	struct flock lock;
	bool output;

	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(NormalUser);
	lock.l_len = sizeof(NormalUser);
	lock.l_pid = getpid();

	fcntl(fd, F_SETLKW, &lock);

	NormalUser user;
	lseek(fd, (i) * sizeof(NormalUser), SEEK_SET);
	read(fd, &user, sizeof(NormalUser));

	if (!strcmp(user.status, "ACTIVE"))
	{
		strcpy(user.status, "CLOSED");
		user.balance = 0;
		lseek(fd, (-1) * sizeof(NormalUser), SEEK_CUR);
		if (write(fd, &user, sizeof(NormalUser)) != 0)
		{
			output = true;
		}
		else
			output = false;
	}

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return output;
}

bool deleteJointUser(int ID){
	int i = ID - 1;
	int fd = open("JointUserFile", O_RDWR, 0744);
	struct flock lock;
	JointUser user;
	bool output;

	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(JointUser);
	lock.l_len = sizeof(JointUser);
	lock.l_pid = getpid();

	fcntl(fd, F_SETLKW, &lock);
	lseek(fd, (i) * sizeof(JointUser), SEEK_SET); // changing the file pointer to the selected record
	read(fd, &user, sizeof(JointUser));

	if (!strcmp(user.status, "ACTIVE"))
	{
		strcpy(user.status, "CLOSED");
		user.balance = 0;

		lseek(fd, (-1) * sizeof(JointUser), SEEK_CUR);
		if (write(fd, &user, sizeof(JointUser)) != 0)
		{
			output = true;
		}
		else
			output = false;
	}

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return output;
}

bool modifyNormalUser(NormalUser user){
	printf("id mod user = %d\n", user.ID);
	int i = user.ID - 1;
	NormalUser currUser;
	int fd = open("NormalUserFile", O_RDWR, 0744);
	bool output = false;

	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(NormalUser);
	lock.l_len = sizeof(NormalUser);
	lock.l_pid = getpid();

	fcntl(fd, F_SETLKW, &lock);

	lseek(fd, (i) * sizeof(NormalUser), SEEK_SET);
	read(fd, &currUser, sizeof(NormalUser));

	if (!strcmp(currUser.status, "ACTIVE") && (user.acc_no == currUser.acc_no))
	{
		strcpy(user.status, "ACTIVE");
		lseek(fd, (-1) * sizeof(NormalUser), SEEK_CUR);
		if (write(fd, &user, sizeof(NormalUser)) != 0)
		{
			output = true;
		}
		else
			output = false;
	}

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return output;
}

bool modifyJointUser(JointUser user){
	int i = user.ID - 1;
	int fd = open("JointUserFile", O_RDWR, 0744);
	struct flock lock;
	JointUser currUser;
	bool output = false;

	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (i) * sizeof(JointUser);
	lock.l_len = sizeof(JointUser);
	lock.l_pid = getpid();

	fcntl(fd, F_SETLKW, &lock);

	lseek(fd, (i) * sizeof(JointUser), SEEK_SET);
	read(fd, &currUser, sizeof(JointUser));
	if (!strcmp(currUser.status, "ACTIVE") && (user.acc_no == currUser.acc_no))
	{
		strcpy(user.status, "ACTIVE");
		lseek(fd, (-1) * sizeof(JointUser), SEEK_CUR);
		if (write(fd, &user, sizeof(JointUser)) != 0)
		{
			output = true;
		}
		else
			output = false;
	}

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return output;
}

bool depositMoney(int acc_type, int ID, float amount){
	int i = ID - 1;
	if (acc_type == 1)
	{
		int fd = open("NormalUserFile", O_RDWR, 0744);
		NormalUser currUser;
		bool output;
		struct flock lock;

		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = (i) * sizeof(NormalUser);
		lock.l_len = sizeof(NormalUser);
		lock.l_pid = getpid();

		fcntl(fd, F_SETLKW, &lock);
		lseek(fd, (i) * sizeof(NormalUser), SEEK_SET);
		read(fd, &currUser, sizeof(NormalUser));

		if (!strcmp(currUser.status, "ACTIVE"))
		{
			currUser.balance += amount;
			lseek(fd, sizeof(NormalUser) * (-1), SEEK_CUR);
			write(fd, &currUser, sizeof(NormalUser));
			output = true;
		}
		else
			output = false;
		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);

		close(fd);
		return output;
	}
	else if (acc_type == 2)
	{
		int fd = open("JointUserFile", O_RDWR, 0744);
		bool output;
		struct flock lock;
		JointUser currUser;

		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = (i) * sizeof(JointUser);
		lock.l_len = sizeof(JointUser);
		lock.l_pid = getpid();

		fcntl(fd, F_SETLKW, &lock);
		lseek(fd, (i) * sizeof(JointUser), SEEK_SET);
		read(fd, &currUser, sizeof(JointUser));

		if (!strcmp(currUser.status, "ACTIVE"))
		{
			currUser.balance += amount;
			// getchar();
			lseek(fd, sizeof(JointUser) * (-1), SEEK_CUR);
			write(fd, &currUser, sizeof(JointUser));
			getchar();
			output = true;
		}
		else
		{
			output = false;
		}
		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);
		close(fd);
		return output;
	}
	return false;
}

bool withdrawMoney(int acc_type, int ID, float amount){
	int i = ID - 1;
	if (acc_type == 1)
	{
		int fd = open("NormalUserFile", O_RDWR, 0744);
		bool output;
		struct flock lock;

		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = (i) * sizeof(NormalUser);
		lock.l_len = sizeof(NormalUser);
		lock.l_pid = getpid();

		fcntl(fd, F_SETLKW, &lock);

		NormalUser currUser;
		lseek(fd, (i) * sizeof(NormalUser), SEEK_SET);
		read(fd, &currUser, sizeof(NormalUser));

		if (!strcmp(currUser.status, "ACTIVE") && currUser.balance >= amount)
		{
			currUser.balance -= amount;
			lseek(fd, sizeof(NormalUser) * (-1), SEEK_CUR);
			write(fd, &currUser, sizeof(NormalUser));
			output = true;
		}
		else
		{
			output = false;
		}
		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);
		close(fd);
		return output;
	}
	else if (acc_type == 2)
	{
		int fd = open("JointUserFile", O_RDWR, 0744);
		bool output;
		struct flock lock;

		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = (i) * sizeof(JointUser);
		lock.l_len = sizeof(JointUser);
		lock.l_pid = getpid();

		fcntl(fd, F_SETLKW, &lock);

		JointUser currUser;
		lseek(fd, (i) * sizeof(JointUser), SEEK_SET);
		read(fd, &currUser, sizeof(JointUser));

		if (!strcmp(currUser.status, "ACTIVE") && currUser.balance >= amount)
		{
			currUser.balance -= amount;
			lseek(fd, sizeof(JointUser) * (-1), SEEK_CUR);
			write(fd, &currUser, sizeof(JointUser));
			output = true;
		}
		else
		{
			output = false;
		}
		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);
		close(fd);
		return output;
	}
	return false;
}

float getBalance(int acc_type, int ID){
	int i = ID - 1;
	float output;
	if (acc_type == 1)
	{
		int i = ID - 1;
		int fd = open("NormalUserFile", O_RDONLY, 0744);
		NormalUser user;
		struct flock lock;

		lock.l_type = F_RDLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = (i) * sizeof(NormalUser);
		lock.l_len = sizeof(NormalUser);
		lock.l_pid = getpid();

		fcntl(fd, F_SETLKW, &lock);
		lseek(fd, (i) * sizeof(NormalUser), SEEK_SET);
		read(fd, &user, sizeof(NormalUser));
		if (!strcmp(user.status, "ACTIVE"))
		{
			output = user.balance;
		}
		else
		{
			output = 0;
		}

		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);
		close(fd);
		return output;
	}
	else if (acc_type == 2)
	{
		int i = ID - 1;
		int fd = open("JointUserFile", O_RDONLY, 0744);
		JointUser user;
		struct flock lock;

		lock.l_type = F_RDLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = (i) * sizeof(JointUser);
		lock.l_len = sizeof(JointUser);
		lock.l_pid = getpid();

		fcntl(fd, F_SETLKW, &lock);
		lseek(fd, (i) * sizeof(JointUser), SEEK_SET);
		read(fd, &user, sizeof(JointUser));
		if (!strcmp(user.status, "ACTIVE"))
		{
			output = user.balance;
		}
		else
		{
			output = 0;
		}
		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);
		close(fd);
		return output;
	}
	return 0;
}

bool alterPassword(int acc_type, int ID, char newpass[10]){
	int i = ID - 1;
	if (acc_type == 1)
	{
		int fd = open("NormalUserFile", O_RDWR, 0744);
		bool output;
		struct flock lock;
		NormalUser user;

		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = (i) * sizeof(NormalUser);
		lock.l_len = sizeof(NormalUser);
		lock.l_pid = getpid();

		fcntl(fd, F_SETLKW, &lock);

		lseek(fd, (i) * sizeof(NormalUser), SEEK_SET);
		read(fd, &user, sizeof(NormalUser));
		if (!strcmp(user.status, "ACTIVE"))
		{
			strcpy(user.password, newpass);
			lseek(fd, sizeof(NormalUser) * (-1), SEEK_CUR);
			write(fd, &user, sizeof(NormalUser));
			output = true;
		}
		else
		{
			output = false;
		}
		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);
		close(fd);
		return output;
	}
	else if (acc_type == 2)
	{
		int fd = open("JointUserFile", O_RDWR, 0744);
		bool output;
		struct flock lock;
		JointUser user;

		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = (i) * sizeof(JointUser);
		lock.l_len = sizeof(JointUser);
		lock.l_pid = getpid();

		fcntl(fd, F_SETLKW, &lock);
		lseek(fd, (i) * sizeof(JointUser), SEEK_SET); // changing the file pointer to the selected record
		read(fd, &user, sizeof(JointUser));

		if (!strcmp(user.status, "ACTIVE"))
		{
			strcpy(user.password, newpass);
			lseek(fd, sizeof(JointUser) * (-1), SEEK_CUR);
			write(fd, &user, sizeof(JointUser));
			output = true;
		}
		else
		{
			output = false;
		}
		lock.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &lock);
		close(fd);
		return output;
	}
	return false;
}

void server_task(int nsd){
	int msg_len, select, acc_type, option, account_type, ID;
	bool output;
	while (1)
	{
		read(nsd, &option, sizeof(option));
		printf("Option : %d\n", option);

		if (option == 1)
		{
			NormalUser user;
			account_type = 1;
			msg_len = read(nsd, &user, sizeof(NormalUser));
			printf("Username: %d\n", user.ID);
			printf("Password: %s\n", user.password);
			ID = user.ID;
			output = checkNormalUser(user);
			write(nsd, &output, sizeof(output));
		}
		else if (option == 2)
		{
			JointUser user;
			account_type = 2;
			msg_len = read(nsd, &user, sizeof(JointUser));
			ID = user.ID;
			printf("Username: %d\n", user.ID);
			printf("Password: %s\n", user.password);
			output = checkJointUser(user);
			write(nsd, &output, sizeof(output));
		}
		else if (option == 3)
		{
			Admin user;
			account_type = 3;
			msg_len = read(nsd, &user, sizeof(Admin));
			ID = user.ID;
			printf("Username: %d\n", user.ID);
			printf("Password: %s\n", user.password);
			output = checkAdmin(user);
			write(nsd, &output, sizeof(output));
		}
		else
		{
			output = false;
			write(nsd, &output, sizeof(output));
		}

		if (output)
		{
			break;
		}
	}

	while (1)
	{
		read(nsd, &select, sizeof(int));
		if (option == 1 || option == 2)
		{
			if (select == 1)
			{
				float amount;
				read(nsd, &amount, sizeof(float));
				output = depositMoney(account_type, ID, amount);
				write(nsd, &output, sizeof(output));
			}
			else if (select == 2)
			{
				float amount;
				read(nsd, &amount, sizeof(float));
				output = withdrawMoney(account_type, ID, amount);
				write(nsd, &output, sizeof(output));
			}
			else if (select == 3)
			{
				float amount;
				amount = getBalance(account_type, ID);
				write(nsd, &amount, sizeof(float));
			}
			else if (select == 4)
			{
				char pass[10];
				read(nsd, pass, sizeof(pass));
				output = alterPassword(account_type, ID, pass);
				write(nsd, &output, sizeof(output));
			}
			else if (select == 5)
			{
				if (option == 1)
				{
					NormalUser user = getNormalUser(ID);
					write(nsd, &user, sizeof(NormalUser));
				}
				else if (option == 2)
				{
					JointUser user = getJointUser(ID);
					write(nsd, &user, sizeof(JointUser));
				}
			}
			else if (select == 6)
			{
				break;
			}
		}
		else if (option == 3)
		{
			read(nsd, &acc_type, sizeof(int));
			if (select == 1)
			{
				if (acc_type == 1)
				{
					NormalUser user;
					read(nsd, &user, sizeof(NormalUser));
					output = addNormalUser(user);
					write(nsd, &output, sizeof(output));
				}
				else if (acc_type == 2)
				{
					JointUser user;
					read(nsd, &user, sizeof(JointUser));
					output = addJointUser(user);
					write(nsd, &output, sizeof(output));
				}
			}
			else if (select == 2)
			{
				if (acc_type == 1)
				{
					int ID;
					read(nsd, &ID, sizeof(int));
					output = deleteNormalUser(ID);
					write(nsd, &output, sizeof(output));
				}
				else if (acc_type == 2)
				{
					int ID;
					read(nsd, &ID, sizeof(int));
					output = deleteJointUser(ID);
					write(nsd, &output, sizeof(output));
				}
			}
			else if (select == 3)
			{
				if (acc_type == 1)
				{
					NormalUser user;
					read(nsd, &user, sizeof(NormalUser));
					output = modifyNormalUser(user);
					write(nsd, &output, sizeof(output));
				}
				else if (acc_type == 2)
				{
					JointUser user;
					read(nsd, &user, sizeof(JointUser));
					output = modifyJointUser(user);
					write(nsd, &output, sizeof(output));
				}
			}
			else if (select == 4)
			{
				if (acc_type == 1)
				{
					NormalUser user;
					int ID;
					read(nsd, &ID, sizeof(int));
					user = getNormalUser(ID);
					write(nsd, &user, sizeof(NormalUser));
				}
				else if (acc_type == 2)
				{
					JointUser user;
					int ID;
					read(nsd, &ID, sizeof(int));
					user = getJointUser(ID);
					write(nsd, &user, sizeof(JointUser));
				}
			}
			else if (select == 5)
			{
				break;
			}
		}
	}
	close(nsd);
}

void *conn_handler(void *nsd){
	int nsfd = *(int *)nsd;
	server_task(nsfd);
}

int main()
{
	struct sockaddr_in server, client;
	int sd = socket(AF_INET, SOCK_STREAM, 0);

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5555);

	bind(sd, (struct sockaddr *)&server, sizeof(server));
	listen(sd, 5);

	while (1)
	{
		int k = sizeof(client);
		int nsd = accept(sd, (void *)&client, &k);

		pthread_t tid;
		if (pthread_create(&tid, NULL, conn_handler, (void *)&nsd) < 0)
		{
			printf("thread cannot be created\n");
		}
	}
	pthread_exit(NULL);
	close(sd);
	return 0;
}
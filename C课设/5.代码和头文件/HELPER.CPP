#include "helper.h"

// compare two date
int CompareDate(struct Date date1, struct Date date2) {
	int ret = date1.year - date2.year;
	if (ret) return ret;
	ret = date1.month - date2.month;
	if (ret) return ret;
	ret = date1.day - date2.day;
	return ret;
	//return 0 if equal
}

// read data from file
void ReadDataFromFile(AccountNode** accounts, NewsNode** news, CommentNode** comments) {
	
	// read accounts
	FILE* fp = fopen("acc.txt", "r");
	if (fp == NULL) {
		perror("ERROR: ");
		exit(EXIT_FAILURE);
	}
	fscanf(fp, "%*[^\n]\n");
	char user[20], psw[20], type[20];
	AccountNode* p1 = *accounts;
	
	//read every line
	while (fscanf(fp, "%[^,],%[^,],%[^\n]\n", user, psw, type) == 3) {
		// define a accounts
		Account* account = (Account*)calloc(1, sizeof(Account));
		strcpy(account->user, user);
		strcpy(account->psw, psw);
		strcpy(account->type, type);
		AccountNode* accountNode = (AccountNode*)calloc(1, sizeof(AccountNode));
		accountNode->account = account;
		accountNode->next = NULL;
		// 尾插法
		if (p1) {
			p1->next = accountNode;
			p1 = p1->next;
		}
		else {
			*accounts = accountNode;
			p1 = *accounts;
		}
	}
	fclose(fp);

	// read news
	fp = fopen("news.txt", "r");
	if (fp == NULL) {
		perror("ERROR: ");
		exit(EXIT_FAILURE);
	}
	fscanf(fp, "%*[^\n]\n");
	int id;
	char title[50], publisher[20], tmp1[20], tmp2[10], content[10000];
	NewsNode* p2 = *news;
	// read every line
	while (fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", 
		&id, title, publisher, type, tmp1, tmp2, content) == 7) {
		// define a news
		News* mNews = (News*)calloc(1, sizeof(News));
		mNews->id = id;
		mNews->title = (char*)calloc(strlen(title) + 1, sizeof(char));
		strcpy(mNews->title, title);
		strcpy(mNews->publisher, publisher);
		strcpy(mNews->type, type);
		char* tmp = strtok(tmp1, "-");
		mNews->date.year = atoi(tmp);
		tmp = strtok(NULL, "-");
		mNews->date.month = atoi(tmp);
		tmp = strtok(NULL, "-");
		mNews->date.day = atoi(tmp);
		if (strcmp(tmp2, "Y") == 0) {
			mNews->verified = 1;
		}
		mNews->content = (char*)calloc(strlen(content) + 1, sizeof(char));
		strcpy(mNews->content, content);
		mNews->deleted = 0;
		NewsNode* newsNode = (NewsNode*)calloc(1, sizeof(NewsNode));
		newsNode->news = mNews;
		newsNode->next = NULL;
		// 尾插法
		if (p2) {
			p2->next = newsNode;
			p2 = p2->next;
		}
		else {
			*news = newsNode;
			p2 = *news;
		}
	}
	fclose(fp);

	fp = fopen("del.txt", "r");
	if (fp == NULL) {
		perror("ERROR: ");
		exit(EXIT_FAILURE);
	}
	fscanf(fp, "%*[^\n]\n");
	// read every line
	while (fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n",
		&id, title, publisher, type, tmp1, tmp2, content) == 7) {
		// define del news
		News* mNews = (News*)calloc(1, sizeof(News));
		mNews->id = id;
		mNews->title = (char*)calloc(strlen(title) + 1, sizeof(char));
		strcpy(mNews->title, title);
		strcpy(mNews->publisher, publisher);
		strcpy(mNews->type, type);
		char* tmp = strtok(tmp1, "-");
		mNews->date.year = atoi(tmp);
		tmp = strtok(NULL, "-");
		mNews->date.month = atoi(tmp);
		tmp = strtok(NULL, "-");
		mNews->date.day = atoi(tmp);
		if (strcmp(tmp2, "Y") == 0) {
			mNews->verified = 1;
		}
		mNews->content = (char*)calloc(strlen(content) + 1, sizeof(char));
		strcpy(mNews->content, content);
		mNews->deleted = 1;
		NewsNode* newsNode = (NewsNode*)calloc(1, sizeof(NewsNode));
		newsNode->news = mNews;
		newsNode->next = NULL;
		//尾插法
		if (p2) {
			p2->next = newsNode;
			p2 = p2->next;
		}
		else {
			*news = newsNode;
			p2 = *news;
		}
	}
	fclose(fp);

	//read comments
	fp = fopen("com.txt", "r");
	if (fp == NULL) {
		perror("ERROR: ");
		exit(EXIT_FAILURE);
	}
	fscanf(fp, "%*[^\n]\n");
	CommentNode* p3 = *comments;
	// read every line
	while (fscanf(fp, "%d,%[^,],%[^\n]\n", &id, user, content) == 3) {
		// define comments
		Comment* comment = (Comment*)calloc(1, sizeof(Comment));
		comment->newsId = id;
		strcpy(comment->user, user);
		comment->content = (char*)calloc(strlen(content) + 1, sizeof(char));
		strcpy(comment->content, content);
		CommentNode* commentNode = (CommentNode*)calloc(1, sizeof(CommentNode));
		commentNode->comment = comment;
		commentNode->next = NULL;
		// 尾插
		if (p3) {
			p3->next = commentNode;
			p3 = p3->next;
		}
		else {
			*comments = commentNode;
			p3 = *comments;
		}
	}
	fclose(fp);
}


// write to file
void WriteDataFromFile(AccountNode* accounts, NewsNode* news, CommentNode* comments) {
	//write accounts
	if (accounts) {
		FILE* fp = fopen("acc.txt", "w");
		if (fp == NULL) {
			perror("ERROR: ");
			exit(EXIT_FAILURE);
		}
		AccountNode* p = accounts;
		fprintf(fp, "user,psw,type\n");
		while (p) {
			fprintf(fp, "%s,%s,%s\n", p->account->user, p->account->psw, p->account->type);
			p = p->next;
		}
		fclose(fp);
	}
	// write news
	if (news) {
		FILE* fp = fopen("news.txt", "w");
		if (fp == NULL) {
			perror("ERROR: ");
			exit(EXIT_FAILURE);
		}
		// write news no del
		NewsNode* p = news;
		fprintf(fp, "id,title,publisher,type,date,verified,content\n");
		while (p) {
			if (!p->news->deleted) {
				fprintf(fp, "%d,%s,%s,%s,%4d-%02d-%02d,%s,%s\n", p->news->id, p->news->title, 
					p->news->publisher, p->news->type, p->news->date.year, p->news->date.month,
					p->news->date.day, p->news->verified?"Y":"N", p->news->content);
			}
			p = p->next;
		}
		fclose(fp);
		fp = fopen("del.txt", "w");
		if (fp == NULL) {
			perror("ERROR: ");
			exit(EXIT_FAILURE);
		}
		fprintf(fp, "id,title,publisher,type,date,verified,content\n");
		// write news del
		p = news;
		while (p) {
			if (p->news->deleted) {
				fprintf(fp, "%d,%s,%s,%s,%4d-%02d-%02d,%s,%s\n", p->news->id, p->news->title,
					p->news->publisher, p->news->type, p->news->date.year, p->news->date.month,
					p->news->date.day, p->news->verified ? "Y" : "N", p->news->content);
			}
			p = p->next;
		}
		fclose(fp);
	}
	// write comments
	if (comments) {
		FILE* fp = fopen("com.txt", "w");
		if (fp == NULL) {
			perror("ERROR: ");
			exit(EXIT_FAILURE);
		}
		CommentNode* p = comments;
		fprintf(fp, "newsId,user,content\n");
		while (p) {
			fprintf(fp, "%d,%s,%s\n", p->comment->newsId, p->comment->user, p->comment->content);
			p = p->next;
		}
		fclose(fp);
	}
}

// add news
void AddNews(NewsNode* news, Account* account) {
	clrscr();
	printf("****input news****\n");
	// define a news
	News* mNews = (News*)calloc(1, sizeof(News));
	// get id
	int id = 0;
	NewsNode* p = news;
	while (p) {
		if (p->news->id > id) {
			id = p->news->id;
		}
		p = p->next;
	}
	id++;
	printf("id %d\n", id);
	mNews->id = id;
	char* str = InputString("topic: ");
	mNews->title = (char*)calloc(strlen(str) + 1, sizeof(char));
	strcpy(mNews->title, str);
	free(str);
	printf("input news type: 1.notice 2.announce 3.news in brief\n");
	int choice = InputInteger("Your choice:", 1, 3);
	if (choice == 1) {
		strcpy(mNews->type, "notice");
	}
	else if (choice == 2) {
		strcpy(mNews->type, "announce");
	}
	else {
		strcpy(mNews->type, "brief");
	}
	printf("publisher: %s\n", account->user);
	strcpy(mNews->publisher, account->user);
	time_t now;
	struct tm* now_struct;
	time(&now);
	now_struct = localtime(&now);
	mNews->date.year = now_struct->tm_year + 1900;
	mNews->date.month = now_struct->tm_mon + 1;
	mNews->date.day = now_struct->tm_mday;
	printf("date: %4d-%02d-%02d\n", mNews->date.year,
		mNews->date.month, mNews->date.day);
	str = InputString("News content: \n");
	mNews->content = (char*)calloc(strlen(str) + 1, sizeof(char));
	strcpy(mNews->content, str);
	free(str);
	mNews->verified = 0;
	mNews->deleted = 0;

	//add
	NewsNode* sNews = (NewsNode*)calloc(1, sizeof(NewsNode));
	sNews->news = mNews;
	sNews->next = NULL;
	p = news;
	while (p && p->next) {
		p = p->next;
	}
	p->next = sNews;

	// save to file
	WriteDataFromFile(NULL, news, NULL);
	printf("success\n");
	WaitForEnterKey();
}


int InputInteger(char* msg, int low, int high) {
	int isValid = 0;
	int choice = -1,i,len;
	char buf[256];
	while (!isValid) {
		choice = -1;
		printf("%s", msg);
		ClearInputBuffer();
		scanf("%[^\n]%*c", buf);
		// check if is num
		for (i = 0, len = strlen(buf); i < len; i++) {
			if (buf[i] < '0' || buf[i] > '9') {
				choice = -2;
				break;
			}
		}
		if (choice == -1) {
			choice = atoi(buf);
		}
		if (choice < low || choice > high) {
			printf("illegal input, plz input again\n");
		}
		else {
			isValid = 1;
		}
	}
	ClearInputBuffer();
	return choice;
}


char* InputString(char* msg) {
	char* str = (char*)calloc(1000, sizeof(char));
	printf("%s", msg);
	scanf("%[^\n]%*c", str);
	ClearInputBuffer();
	return str;
}

void WaitForEnterKey() {
	printf("press ENTER to continue...");
	scanf("%*[^\n]%*c");
	ClearInputBuffer();
}

void ClearInputBuffer() {
	while (kbhit())
	{
		getch();
	}
	rewind(stdin);
}
//login
Account* Login(AccountNode* accounts) {
	// input 
	char* user = InputString("Username: ");
	char* psw = InputString("Password: ");
	// find accounts
	AccountNode* p1 = accounts;
	while (p1) {
		if (strcmp(user, p1->account->user) == 0 &&
			strcmp(psw, p1->account->psw) == 0) {
			break;
		}
		p1 = p1->next;
	}
	free(user);
	free(psw);
	return p1 ? p1->account : NULL;
}

// find news
NewsNode* CopyNews(NewsNode* news, 
				   int needVerified, 
				   int excluedDeleted, 
				   char* type,
				   int* year, 
				   int* month, 
				   int* day, 
				   struct Date* startDate, 
				   struct Date* endDate,
				   char* titleKeyword, 
				   Account* account) {
	
	NewsNode* sNews = NULL;
	NewsNode* p = news;
	NewsNode* p2 = sNews;

	while (p) {
		// check if it have
		if (
			(needVerified ? p->news->verified : 1) 
			&&(excluedDeleted ? !p->news->deleted : 1) 
			&&(type ? strcmp(p->news->type, type) == 0 : 1) 
			&&(year ? p->news->date.year == *year : 1) 
			&&(month ? p->news->date.month == *month : 1) 
			&&(day ? p->news->date.day == *day : 1) 
			&&(startDate ? CompareDate(p->news->date, *startDate) >= 0 : 1) 
			&&(endDate ? CompareDate(p->news->date, *endDate) <= 0 : 1) 
			&&(titleKeyword ? strstr(p->news->title, titleKeyword) != NULL : 1) 
			&&(account ? strcmp(p->news->publisher, account->user) == 0 : 1)
			)
			{
				// copy News
				News* mNews = (News*)calloc(1, sizeof(News));
				mNews->id = p->news->id;
				mNews->title = (char*)calloc(strlen(p->news->title) + 1, sizeof(char));
				
				strcpy(mNews->title, p->news->title);
				strcpy(mNews->publisher, p->news->publisher);
				strcpy(mNews->type, p->news->type);
				
				mNews->content = (char*)calloc(strlen(p->news->content) + 1, sizeof(char));
				strcpy(mNews->content, p->news->content);
				
				mNews->date = p->news->date;
				mNews->verified = p->news->verified;
				mNews->deleted = p->news->deleted;

				NewsNode* newsNode = (NewsNode*)calloc(1, sizeof(NewsNode));
				newsNode->news = mNews;
				newsNode->next = NULL;
				//尾插(weicha)
				if (p2) {
					p2->next = newsNode;
					p2 = p2->next;
				}
				else {
					sNews = newsNode;
					p2 = sNews;
				}
			}
		p = p->next;
	}
	return sNews;
}

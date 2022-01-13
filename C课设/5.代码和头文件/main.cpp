#include "helper.h"
#include "user.h"
#include "admin.h"

int main() {
	// read data
	AccountNode* accounts = NULL;
	NewsNode* news = NULL;
	CommentNode* comments = NULL;
	ReadDataFromFile(&accounts, &news, &comments);

	int isExit = 0;
	while (!isExit) {
	clrscr();
		// menu
		printf("***GDUT News System***\n");
		printf("1.Read News\n");
		printf("2.Login\n");
		printf("0.Exit\n");

		int choice = InputInteger("Input your choice: ", 0, 2);

		// deal user input
		switch (choice) {
			case 0: {
				// exit
				isExit = 1;
				break;
			}
			case 1: {
				// view news
				NewsNode* sNews = CopyNews(news, 1, 1, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL, NULL);
				ViewNews(sNews, comments, NULL);
				// free
				NewsNode* p2 = sNews;
				while (p2) {
					sNews = sNews->next;
					free(p2->news->title);
					free(p2->news->content);
					free(p2->news);
					free(p2);
					p2 = sNews;
				}
				break;
			}
			case 2: {
				// login
				Account* account = Login(accounts);
				if (account) {
					if (strcmp(account->type, "admin") != 0) {
						UserSubSystem(accounts, news, comments, &account);
					}
					else {
						AdminSubSystem(accounts, news, comments, &account);
					}
				}
				else {
					printf("Wrong Username or Password\n");
					WaitForEnterKey();
				}
				break;
			}
		}
	}

	// free
	AccountNode* p1 = accounts;
	while (p1) {
		accounts = accounts->next;
		free(p1->account);
		free(p1);
		p1 = accounts;
	}
	NewsNode* p2 = news;
	while (p2) {
		news = news->next;
		free(p2->news->title);
		free(p2->news->content);
		free(p2->news);
		free(p2);
		p2 = news;
	}
	CommentNode* p3 = comments;
	while (p3) {
		comments = comments->next;
		free(p3->comment->content);
		free(p3->comment);
		free(p3);
		p3 = comments;
	}
	return 0;
}
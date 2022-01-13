#include "user.h"

// 普通用户子系统
void UserSubSystem(AccountNode* accounts, NewsNode* news, CommentNode* comments, Account** account) {
	int isExit = 0;
	while (!isExit) {
	clrscr();
		// 显示菜单
		printf("***GDUT News System***\n");
		printf("1.View news\n");
		printf("2.Search news\n");
		printf("3.Add news\n");
		printf("4.statistical data\n");
		printf("0.Logout\n");

		int choice = InputInteger("Input your chioce: ", 0, 4);
		// 处理用户输入
		switch (choice) {
			case 0:{
				// 退出登录
				isExit = 1;
				*account = NULL;
				continue;
				break;
			}
			case 1: {
				// 新闻浏览
				NewsNode* sNews = CopyNews(news, 1, 1, NULL, NULL,
					NULL, NULL, NULL, NULL, NULL, NULL);
				ViewNews(sNews, comments, *account);
				// 释放内存
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
				// 新闻检索
				SearchNews(news, comments, *account);
				break;
			}
			case 3: {
				// 新闻录入
				AddNews(news, *account);
				break;
			}
			case 4: {
				// 统计功能
				UserNewsStats(news, *account);
				break;
			}
		}
	}
}

// 浏览新闻
void ViewNews(NewsNode* news, CommentNode* comments, Account* account) {
	if (news == NULL) {
	clrscr();
		printf("No news\n");
		WaitForEnterKey();
		return;
	}

	int isExit = 0;
	// 计算新闻总数
	int total = 0;
	NewsNode* p = news;
	while (p) {
		total++;
		p = p->next;
	}
	
	int startIndex = 0;
	int pageSize = 5;
	int oldSortMethod = 0;
	int newSortMethod = 1;

	while (!isExit) {
	clrscr();
		// 显示新闻
		printf("***Read News***\n");
		// 排序新闻
		if (newSortMethod != oldSortMethod) {
			oldSortMethod = newSortMethod;
			int isSorted = 0;
			while (!isSorted) {
				isSorted = 1;
				NewsNode* p = news;
				NewsNode* pre = NULL;
				while (p && p->next) {
					// 判断是否需要交换
					int needSwap = 0;
					switch (newSortMethod)
					{
						case 1: {
							if (CompareDate(p->news->date, p->next->news->date) > 0) {
								needSwap = 1;
							}
							break;
						}
						case 2: {
							if (CompareDate(p->news->date, p->next->news->date) < 0) {
								needSwap = 1;
							}
							break;
						}
						case 3: {
							if (strcmp(p->news->publisher, p->next->news->publisher) > 0) {
								needSwap = 1;
							}
							break;
						}
						case 4: {
							if (strcmp(p->news->publisher, p->next->news->publisher) < 0) {
								needSwap = 1;
							}
							break;
						}
					}
					// 交换
					if (needSwap) {
						isSorted = 0;
						NewsNode* q = p->next;
						if (pre) {
							pre->next = q;
							p->next = q->next;
							q->next = p;
						}else {
							news = q;
							p->next = q->next;
							q->next = p;
						}
						p = q;
					}
					pre = p;
					p = p->next;
				}
			}
		}
		// 显示
		int k = 0;
		NewsNode* p = news;
		while (k < startIndex) {
			p = p->next;
			k++;
		}
		printf("%-3s%-28s%-14s%-10s%-10s%-10s\n", "ID", "topic", "News_type", "publiser", "isVerify", "date");
		int i;
		for (i = 0; p && i < pageSize; i++, p = p->next) {
			printf("%-3d%-27.27s %-14s%-10.9s%-10s%4d-%02d-%02d\n", p->news->id, p->news->title, p->news->type, 
				p->news->publisher, p->news->verified?"Y":"N", p->news->date.year, p->news->date.month,
				p->news->date.day);
		}
		

		// 设置显示方式或内容
		printf("***Functional Area***\n");
		printf("1.Ascending(shengxu) by release date\n");
		printf("2.Descending(jiangxu) by release date\n");
		printf("3.Ascending by pubisher\n");
		printf("4.Descending by publisher\n");
		printf("5.Last page\n");
		printf("6.Next page\n");
		printf("7.View news details\n");
		printf("0.return\n");
		int choice = InputInteger("Input your choice: ", 0, 7);
		// 处理用户输入
		switch (choice) {
			case 0: {
				isExit = 1;
				continue;
				break;
			}
			case 1: {
				newSortMethod = 1;
				break;
			}
			case 2: {
				newSortMethod = 2;
				break;
			}
			case 3: {
				newSortMethod = 3;
				break;
			}
			case 4: {
				newSortMethod = 4;
				break;
			}
			case 5: {
				if (startIndex > 0) {
					startIndex -= pageSize;
				}
				else {
					printf("It's first page\n");
					WaitForEnterKey();
				}
				break;
			}
			case 6: {
				if (startIndex + pageSize >= total) {
					printf("It's last page\n");
					WaitForEnterKey();
				}
				else {
					startIndex += pageSize;
				}
				break;
			}
			case 7: {
				int id = InputInteger("Input id you want to view: ", 1, INT_MAX);
				// 找到新闻
				k = 0;
				p = news;
				while (k < startIndex) {
					p = p->next;
					k++;
				}
				int isFound = 0;
				int i;
				for (i = 0; p && i < pageSize; i++, p = p->next) {
					if (p->news->id == id && p->news->verified) {
						isFound = 1;
						break;
					}
				}
				if (isFound) {
					NewsDetail(p, comments, account);
				}else{
					printf("Can't find this news\n");
					WaitForEnterKey();
				}
				break;
			}
		}
	}
}

// 查看新闻详情
void NewsDetail(NewsNode* news, CommentNode* comments, Account* account) {
	int isExit = 0;
	while (!isExit) {
	clrscr();
		// 显示新闻详情
		printf("ID: %d\n", news->news->id);
		printf("topic: %s\n", news->news->title);
		printf("publisher: %s\n", news->news->publisher);
		printf("News type: %s\n", news->news->type);
		printf("Isverified: %s\n", news->news->verified ? "Y" : "N");
		printf("date: %4d-%02d-%02d\n", news->news->date.year,
			news->news->date.month, news->news->date.day);
		printf("content: \n%s\n", news->news->content);

		// 评论
		printf("***Comment Area***\n");
		// 显示评论
		CommentNode* p = comments;
		while (p) {
			if (p->comment->newsId == news->news->id) {
				printf("%s: %s\n", p->comment->user, p->comment->content);
			}
			p = p->next;
		}

		if (account == NULL) {
			printf("\n****comment****\n");
			printf("Input 0 to return\n");
			int choice = InputInteger("input your choice: ", 0, 0);
			if (choice == 0) {
				isExit = 1;
			}
		}
		else {
			printf("\n****comment****\n");
			printf("1.comment\n");
			printf("0.return\n");
			int choice = InputInteger("Input your choice: ", 0, 1);
			if (choice == 0) {
				isExit = 1;
				continue;
			}
			else {
				char* s1 = InputString("Input your comment: ");
				// 添加评论
				Comment* comment = (Comment*)calloc(1, sizeof(Comment));
				comment->newsId = news->news->id;
				strcpy(comment->user, account->user);
				comment->content = (char*)calloc(strlen(s1) + 1, sizeof(char));
				strcpy(comment->content, s1);
				free(s1);
				CommentNode* commentNode = (CommentNode*)calloc(1, sizeof(CommentNode));
				commentNode->comment = comment;
				commentNode->next = NULL;
				
				// 加入链表中
				CommentNode* p = comments;
				while (p && p->next) {
					p = p->next;
				}
				if (comments) {
					p->next = commentNode;
				}
				else {
					comments = commentNode;
				}
				WriteDataFromFile(NULL, NULL, comments);
				printf("comment success\n");
				WaitForEnterKey();
			}
		}
	}
}

// 检索新闻
void SearchNews(NewsNode* news, CommentNode* comments, Account* account) {
	int isExit = 0;
	while (!isExit) {
		clrscr();
		printf("***Search news***\n");
		printf("1.Search by year\n");
		printf("2.Search by month\n");
		printf("3.Search by day\n");
		printf("4.Search by time quantum\n");
		printf("5.Search by combination\n");
		printf("0.return\n");
		int choice = InputInteger("Input your choice: ", 0, 5);
		int * year = NULL, * month = NULL, * day = NULL;
		char* type = NULL, * keyword = NULL;
		struct Date* startDate = NULL, * endDate = NULL;

		switch (choice) {
			case 0: {
				isExit = 1;
				continue;
				break;
			}
			case 1: {
				year = (int*)calloc(1, sizeof(int));
				*year = InputInteger("Year: ", 2000, 2030);
				break;
			}
			case 2: {
				year = (int*)calloc(1, sizeof(int));
				month = (int*)calloc(1, sizeof(int));
				*year = InputInteger("Year: ", 2000, 2030);
				*month = InputInteger("month:", 1, 12);
				break;
			}
			case 3: {
				year = (int*)calloc(1, sizeof(int));
				month = (int*)calloc(1, sizeof(int));
				day = (int*)calloc(1, sizeof(int));
				*year = InputInteger("year: ", 2000, 2030);
				*month = InputInteger("month: ", 1, 12);
				*day = InputInteger("day: ", 1, 31);
				break;
			}
			case 4: {
				// 输入日期
				startDate = (struct Date*)calloc(1, sizeof(struct Date));
				endDate = (struct Date*)calloc(1, sizeof(struct Date));
				printf("Start date\n");
				startDate->year = InputInteger("year: ", 2000, 2030);
				startDate->month = InputInteger("month: ", 1, 12);
				startDate->day = InputInteger("day: ", 1, 31);
				printf("End date:\n");
				endDate->year = InputInteger("year: ", 2000, 2030);
				endDate->month = InputInteger("month: ", 1, 12);
				endDate->day = InputInteger("day: ", 1, 31);
				break;
			}
			case 5: {
				keyword = InputString("Topic keyword(skip by enter) ");
				if (strlen(keyword) == 0) {
					free(keyword);
					keyword = NULL;
				}
				printf("News type:1.notice 2.announcement 3.news in brief 4.skip\n");
				int choice = InputInteger("input your choice: ", 1, 4);
				if (choice != 4) {
					type = (char*)calloc(10, sizeof(char));
				}
				if (choice == 1) {
					strcpy(type, "notice");
				}
				else if (choice == 2) {
					strcpy(type, "announcement");
				}
				else if (choice == 3){
					strcpy(type, "brief");
				}
				printf("time limits: 1.release time 2.release time quantum 3. skip");
				choice = InputInteger("input your choice: ", 1, 3);
				if (choice != 3) {
					startDate = (struct Date*)calloc(1, sizeof(struct Date));
					endDate = (struct Date*)calloc(1, sizeof(struct Date));
				}
				if (choice == 1) {
					printf("date:\n");
					startDate->year = InputInteger("year: ", 2000, 2030);
					startDate->month = InputInteger("month: ", 1, 12);
					startDate->day = InputInteger("day: ", 1, 31);
					endDate->year = startDate->year;
					endDate->month = startDate->month;
					endDate->day = startDate->day;
				}
				else if (choice == 2) {
					printf("Start date\n");
					startDate->year = InputInteger("year: ", 2000, 2030);
					startDate->month = InputInteger("month: ", 1, 12);
					startDate->day = InputInteger("day: ", 1, 31);
					printf("End date\n");
					endDate->year = InputInteger("year: ", 2000, 2030);
					endDate->month = InputInteger("month: ", 1, 12);
					endDate->day = InputInteger("day: ", 1, 31);
				}
				break;
			}
		}
		// 检索符合条件的新闻
		NewsNode* sNews = NULL;
		if (strcmp(account->type, "admin") == 0) {
			sNews = CopyNews(news, 0, 0, type, year,
				month, day, startDate, endDate, keyword, NULL);
		}
		else {
			sNews = CopyNews(news, 0, 1, type, year,
				month, day, startDate, endDate, keyword, account);
		}
		ViewNews(sNews, comments, account);
		// 释放内存
		free(year);
		free(month);
		free(day);
		free(startDate);
		free(endDate);
		NewsNode* p = sNews;
		while (p) {
			sNews = sNews->next;
			free(p->news->title);
			free(p->news->content);
			free(p->news);
			free(p);
			p = sNews;
		}
	}
}

// 统计新闻
void UserNewsStats(NewsNode* news, Account* account) {
	clrscr();
	printf("***statistical data***\n");
	// 输入日期
	struct Date startDate, endDate;
	printf("Start date\n");
	startDate.year = InputInteger("Years: ", 2000, 2030);
	startDate.month = InputInteger("month: ", 1, 12);
	startDate.day = InputInteger("day: ", 1, 31);
	printf("End date\n");
	endDate.year = InputInteger("Years: ", 2000, 2030);
	endDate.month = InputInteger("month: ", 1, 12);
	endDate.day = InputInteger("day: ", 1, 31);

	// 统计
	int num = 0;
	NewsNode* p = news;
	while (p) {
		if (strcmp(p->news->publisher, account->user) == 0 &&
			!p->news->deleted && CompareDate(p->news->date, startDate) >= 0 &&
			CompareDate(p->news->date, endDate) <= 0) {
			num++;
		}
		p = p->next;
	}

	printf("The number of news releases in this period is %d\n", num);
	WaitForEnterKey();
}



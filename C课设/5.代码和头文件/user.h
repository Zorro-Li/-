#ifndef USER_H
#define USER_H
#include "helper.h"

void UserSubSystem(AccountNode* accounts, NewsNode* news, CommentNode* comments, Account** account);

void ViewNews(NewsNode* news, CommentNode* comments, Account* account);

void NewsDetail(NewsNode* news, CommentNode* comments, Account* account);

void SearchNews(NewsNode* news, CommentNode* comments, Account* account);

void UserNewsStats(NewsNode* news, Account* account);

#endif // USER_H
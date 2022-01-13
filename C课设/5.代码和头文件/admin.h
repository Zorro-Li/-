#ifndef ADMIN_H
#define ADMIN_H
#ifndef HELPER_H
#include "helper.h"
#endif

void AdminSubSystem(AccountNode* accounts, NewsNode* news, CommentNode* comments, Account** account);

void NewsClassification(NewsNode* news, CommentNode* comments, Account* account);

void UserManage(AccountNode* accounts);

void NewsManage(NewsNode* news, CommentNode* comments, Account* account);

void NewsStats(NewsNode* news, AccountNode* accounts);

void DeleteNews(NewsNode* news);

void VerifyNews(NewsNode* news);

void ModifyNews(NewsNode* news);

#endif // ADMIN_H
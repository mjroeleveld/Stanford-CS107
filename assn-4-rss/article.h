//
// Created by Joachim Roeleveld on 13/05/2020.
//

#ifndef RSS_NEWS_SEARCH_ARTICLE_H
#define RSS_NEWS_SEARCH_ARTICLE_H

typedef struct {
  char title[1024];
  char description[1024];
  char url[1024];
} Article;

#endif //RSS_NEWS_SEARCH_ARTICLE_H

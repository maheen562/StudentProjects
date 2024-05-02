import os,csv,json,math
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import StrMethodFormatter


def task7():
    
    real_news_articles= []
    fake_news_articles = []
    word_in_real_articles = 0
    word_in_fake_articles = 0
    probability_list = []
 
    with open('/course/data/a1/reviews/HealthStory.json') as json_file:
        review_file = json.load(json_file)
    with open('task6.json') as json_file:
        vocabulary = json.load(json_file)
    for review_info in review_file:
        if (review_info["rating"] >= 3):
            real_news_articles.append(review_info['news_id'])
        else:
            fake_news_articles.append(review_info['news_id'])
            
    for each_word in vocabulary:
        if(len(vocabulary[str(each_word)]) >= 10):
            for each_article in vocabulary[str(each_word)]:
                if(each_article in real_news_articles):
                    word_in_real_articles = word_in_real_articles + 1
                elif(each_article in fake_news_articles):
                    word_in_fake_articles = word_in_fake_articles + 1

            pr_w_r = word_in_real_articles/len(real_news_articles)
            pr_w_o = word_in_fake_articles/len(fake_news_articles)
            if not(pr_w_r == 0 or pr_w_r == 1 or pr_w_o == 0 or pr_w_o == 1):
                o_w_r = (pr_w_r)/(1 - pr_w_r)
                o_w_o = (pr_w_o)/(1 - pr_w_o)
                odds_ratio = o_w_o / o_w_r
                log_odds_ratio = math.log(odds_ratio,10)
                probability_list.append({
                    'word' : each_word,
                    'log_odds_ratio' : odds_ratio
                })

        word_in_real_articles = 0
        word_in_fake_articles = 0
    
    probability_list = pd.DataFrame(probability_list)
    probability_list['log_odds_ratio'] = probability_list['log_odds_ratio'].round(5)

    probability_list.to_csv('task7a.csv', encoding='utf-8', index=False)

    double_bar_graph_plot(probability_list)
    log_odds_ratio_graph(probability_list)
    return

def log_odds_ratio_graph(probability_list):

    probability_list = probability_list.sort_values(by='log_odds_ratio')
    plt.gca().yaxis.set_major_formatter(StrMethodFormatter('{x:,.2f}'))    
    plt.figure(figsize=(23, 10))
    plt.scatter(x=probability_list['word'], y=probability_list['log_odds_ratio'], alpha= 0.5)
    plt.suptitle('Logs Odds Ratio of words',fontsize=25)
    plt.subplots_adjust(bottom=0.3)
    plt.xticks(rotation = 45)
    plt.xticks(fontsize=5)

    plt.xlabel('Words',fontsize = 20)
    plt.ylabel('Log Odds Ratio',fontsize=20)
    
    plt.savefig("task7b.png")
    

def double_bar_graph_plot(probability_list):

    fig = plt.figure()

    highest_odd_ratio = probability_list.sort_values(by='log_odds_ratio').head(15)
    lowest_odd_ratio = probability_list.sort_values(by='log_odds_ratio').tail(15)

    x = []
    for i in range(15):
        x.append(i+1)
    
    plt.gca().yaxis.set_major_formatter(StrMethodFormatter('{x:,.2f}'))
    plt.plot(x, highest_odd_ratio['log_odds_ratio'], label = "Real News",linestyle='dashed', marker='o')
    plt.plot(x,lowest_odd_ratio['log_odds_ratio'], label = "Fake News",linestyle='dashed', marker='o')

    plt.legend()
    plt.title("Log Odds Ratio of Fake News vs. Real News")
    plt.xlabel("Word Index")
    plt.ylabel("log odds ratio")

    plt.savefig("task7c.png")


import json,csv,os
import pandas as pd
import re
from datetime import datetime
import time
import itertools
#import plotly.graph_objects as go
import matplotlib.pyplot as plt

def task3():
    df_rows = []
    graph_rows =[]
    news_id = []
    date = []
    month = []
    year = []
    article_files_loc = '/course/data/a1/content/HealthStory'
    #article_files = ['story_reviews_01670.json', 'story_reviews_01671.json', 'story_reviews_01672.json', 'story_reviews_01673.json', 'story_reviews_01674.json']
    article_files = [pos_json for pos_json in os.listdir(article_files_loc) if pos_json.endswith('.json')]
    #print(article_files)
    readable_date_list = []
    news_id_list = []
    
    for each_article in article_files: 
        with open('/course/data/a1/content/HealthStory/'+each_article) as json_file:
            each_article_info = json.load(json_file)
        date_float = each_article_info["publish_date"]
        
        if(date_float != None):
            readable_date = datetime.fromtimestamp(date_float)
            data_alterations(df_rows,each_article,readable_date)
        

    
    df_rows.sort(key=lambda x: x["news_id"])
    df_rows = pd.DataFrame(df_rows)
    news_id = sum(df_rows["news_id"], [])
    year = sum(df_rows["year"], [])
    month = sum(df_rows["month"], [])
    date = sum(df_rows["day"], [])
    news_id = pd.DataFrame(news_id)
    year = pd.DataFrame(year)
    month = pd.DataFrame(month)
    date = pd.DataFrame(date)
    
    news_id.columns = ['news_id']
    year.columns = ['year']
    month.columns = ['month']
    date.columns = ['day']
    combined_df = [news_id,year,month,date]
    task3a_file = pd.concat(combined_df, axis = 1)

    task3a_file.to_csv('task3a.csv', encoding='utf-8',index=False)
    #task3a_file = pd.read_csv('task3a.csv')

    plot_graph(year)

  
    return

def data_alterations(df_rows,each_article,readable_date):
    news_id_pattern = r'story_reviews_\d{5}'
    year_pattern = r'\d{4}'

    news_id = re.findall(news_id_pattern,each_article)
    year = re.findall(year_pattern,str(readable_date))          
    month = re.findall(r'-\d{2}-',str(readable_date))
    month = re.findall(r'\d{2}',str(month))
    date = re.findall(r'-\d{2} ',str(readable_date))
    date = re.findall(r'\d{2}',str(date))   
    df_rows.append({
                'news_id' : news_id,
                'year' : year,
                'month' : month,
                'day' : date
            })


def plot_graph(year):
    year_avg = year.value_counts()
    year_avg.plot(x ='Year', y='Number of articles written', kind = 'bar')
    plt.title('Articles Written over the Years')
    plt.xticks(fontsize=7)
    plt.savefig("task3b.png")
    


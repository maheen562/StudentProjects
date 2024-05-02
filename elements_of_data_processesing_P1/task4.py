import json,csv
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def task4():
    df_rows = []
    with open('/course/data/a1/reviews/HealthStory.json') as json_file:
        review_file = json.load(json_file)
    for each_review in review_file:
        news_source = each_review['news_source']
        rating = each_review['rating']
        df_rows.append({
            'news_source' : news_source,
            'rating' : rating 
            })
    df_rows = pd.DataFrame(df_rows) 
    no_of_articles = df_rows.groupby('news_source').sum()
    rating_avg = df_rows.groupby('news_source')['rating'].mean().round(2)
    no_of_articles.columns = ['num_articles']
    rating_avg.columns = ['avg_rating']
    combined_df = [no_of_articles,rating_avg]
    result = pd.concat(combined_df, axis=1, join="inner")
    result = result.rename(columns = {'rating' : 'avg_rating'})
    
    
    result.to_csv('task4a.csv', encoding='utf-8')


    plot_graph(result)
 
    return

def plot_graph(result):

    df_graph = result.loc[result['num_articles'] > 5]
    df_graph = df_graph.drop(['num_articles'], axis='columns')
    df_graph = df_graph.sort_values(by = 'avg_rating')
    df_graph.plot(kind = 'bar')
    plt.title("Average Rating of Each ")
    plt.savefig("task4b.png")

import json,csv
import pandas as pd

def task2():

    df_rows = []

    with open('/course/data/a1/reviews/HealthStory.json') as json_file:
        review_file = json.load(json_file)


    for review_info in review_file:
        news_id = review_info['news_id']
        rating = review_info['rating']
        news_title = review_info['news_source']
        review_title = review_info['title']
        num_satisfactory=0
        

        for satisfactory in review_info['criteria']:
            answer = satisfactory['answer']
            if (answer == "Satisfactory"):
                 num_satisfactory = num_satisfactory + 1
            #print(num_satisfactory)
        df_rows.append({
            'news_id' : news_id,
            'news_title' : news_title,
            'review_title' : review_title,
            'rating' : rating,
            'num_satisfactory' : num_satisfactory
          })
    task2_file = pd.DataFrame(df_rows)

   
    task2_file.to_csv('task2.csv', encoding='utf-8', index=False)    
    

    return

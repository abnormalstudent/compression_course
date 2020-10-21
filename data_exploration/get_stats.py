import pandas
def get_stats():
    pandas.set_option('display.expand_frame_repr', False)
    RESULT_PATH = "../results.csv"
    df = pandas.read_csv(RESULT_PATH)
    df['compression_rate'] = pandas.Series(df['compressed_size'] / df['original_size'])
    mean = df['compression_rate'].mean(skipna=True)
    print("Results :\n {}".format(df))
    print("Mean compression rate : {}".format(df['compression_rate'].mean()))
get_stats()
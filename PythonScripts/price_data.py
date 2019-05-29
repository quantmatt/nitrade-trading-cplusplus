# -*- coding: utf-8 -*-

import numpy as np
import pandas as pd


def csv_to_binary(source_filepath, dest_filepath, start_date = None, 
                  end_date = None, timestamp_index = 0, return_data = False,
                  column_map = {"bidopen": "bidopen",
                                "bidclose": "bidclose",
                                "bidhigh": "bidhigh",
                                "bidlow": "bidlow",
                                "askopen": "askopen",
                                "askclose": "askclose",
                                "askhigh": "askhigh",
                                "asklow": "asklow",
                                "volume": "tickqty"}):
    """Converts a csv price data file into binary suitable for Nitrade 
        
        Args:
            source_filepath (string): the full or relative filepath for csv input file           
            dest_filepath (string): the full or relative filepath for binary output file       
            start_date (string): optional filter for price data
            end_date (string): optional filter for price data
            timestamp_index (int): column index for the start of bar timestamp
            return_data (bool): optional returns the dataframe if set to True
            column_map (dict): maps the csv file columns (value) to the predefined
                column names (key)
                
        Returns:
            Pandas.DataFrame: if return_data set to True returns the price data
                
    """
        
    #load the csv in the correct format
    df = load_csv(source_filepath, start_date, end_date, timestamp_index, column_map)
    
    #convert dataframe to records and save as a binary file
    ra = df.to_records()
    ra.tofile(dest_filepath)
    
    #return the dataframe if requested
    if return_data:
        return df
    
def binary_to_csv(source_filepath, dest_filepath, start_date = None, 
                  end_date = None, return_data = False):
    """Converts a binary price data file into a csv file
        
        Args:
            source_filepath (string): the full or relative filepath for binary input file           
            dest_filepath (string): the full or relative filepath for csv output file       
            start_date (string): optional filter for price data
            end_date (string): optional filter for price data
            return_data (bool): optional returns the dataframe if set to True
                
        Returns:
            Pandas.DataFrame: if return_data set to True returns the price data
                
    """
    
    #load from binary
    df = load_binary(source_filepath, start_date, end_date)
    
    #save as csv
    df.to_csv(dest_filepath, header=True, index=True)
    
    #return the dataframe if requested
    if return_data:
        return df

def load_binary(source_filepath, start_date = None, 
                  end_date = None):
    """Loads a binary price data file
        
        Args:
            source_filepath (string): the full or relative filepath for binary input file  
            start_date (string): optional filter for price data
            end_date (string): optional filter for price data
                
        Returns:
            Pandas.DataFrame: returns the price data
                
    """
    
    #declare the datatypes for the binary file
    dtypes = [('index', 'M8[ns]'),      
     ('bidopen', 'f4'), 
     ('bidclose', 'f4'),
     ('bidhigh', 'f4'),
     ('bidlow', 'f4'),
     ('askopen', 'f4'),
     ('askclose', 'f4'),
     ('askhigh', 'f4'),
     ('asklow', 'f4'),
     ('volume', 'i4')]
    
    #read the file from binary
    records = np.fromfile(source_filepath, dtypes)
    df = pd.DataFrame(records)
    
    #set the index
    df = df.set_index("index", drop=True)
    
    #filter on dates if required
    if start_date is not None:
        df = df[df.index >= start_date]
    if end_date is not None:
        df = df[df.index <= end_date]
    
    return df
    
def load_csv(source_filepath, start_date = None, end_date = None, 
             timestamp_index = 0, 
                  column_map = {"bidopen": "bidopen",
                                "bidclose": "bidclose",
                                "bidhigh": "bidhigh",
                                "bidlow": "bidlow",
                                "askopen": "askopen",
                                "askclose": "askclose",
                                "askhigh": "askhigh",
                                "asklow": "asklow",
                                "volume": "tickqty"}):  
    
    """Loads a csv price data file
        
        Args:
            source_filepath (string): the full or relative filepath for csv input file               
            start_date (string): optional filter for price data
            end_date (string): optional filter for price data
            timestamp_index (int): column index for the start of bar timestamp
            column_map (dict): maps the csv file columns (value) to the predefined
                column names (key)
                
        Returns:
            Pandas.DataFrame: returns the price data
            
    """
    
    #read the csv file
    df = pd.read_csv(source_filepath, sep=",", index_col=timestamp_index, parse_dates=True)
    
    #convert the columns to the correct data type
    df["askopen"] = df[column_map["askopen"]].astype('float32')
    df["askclose"] = df[column_map["askclose"]].astype('float32')
    df["askhigh"] = df[column_map["askhigh"]].astype('float32')
    df["asklow"] = df[column_map["asklow"]].astype('float32')
    df["bidopen"] = df[column_map["bidopen"]].astype('float32')
    df["bidclose"] = df[column_map["bidclose"]].astype('float32')
    df["bidhigh"] = df[column_map["bidhigh"]].astype('float32')
    df["bidlow"] = df[column_map["bidlow"]].astype('float32')
    df["volume"] = df[column_map["volume"]].astype('int32')
    
    #reorder the columns in the correct order
    cols = ["askopen", "askclose", "askhigh", "asklow", "bidopen", "bidclose",
            "bidhigh", "bidlow", "volume"]
    df = df[cols]
    
    #filter on dates if required
    if start_date is not None:
        df = df[df.index >= start_date]
    if end_date is not None:
        df = df[df.index <= end_date]
        
    return df

        
        
        
        
        
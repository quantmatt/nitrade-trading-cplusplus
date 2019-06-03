# -*- coding: utf-8 -*-

import numpy as np
from numpy import dtype
import pandas as pd
import struct
from datetime import timedelta

class TradeCollection:
    """Loads Nitrade trades from file and stores them in memory.    
    """
    
    def __init__(self):
        """ Init function to declare class variables
        
        Attributes:
            trades (Pandas.dataframe): a table of all the trades and their 
                properties
            pl_data (Pandas.datframe): Daily Profit & Loss data - realised and
                unrealised
            types (<string, string>tuple[]): an array of tuples that holds 
                column name and data type for each property of a trade                
            pl_types(<string, string>tuple[]): an array of tuples that holds 
                column name and data type for each property of daily P&L
        """
        self.trades = pd.DataFrame()
        self.pl_data = pd.DataFrame()
        self.types = [('tradeId', 'i4'), 
         ('variantId', 'i4'),
         ('openLevel', 'f4'), 
         ('closeLevel', 'f4'),
         ('commission', 'f4'),
         ('spread', 'f4'),
         ('profit', 'f4'),
         ('pip', 'f4'),
         ('pipCost', 'f4'),
         ('size', 'i4'),
         ('stopLoss', 'f4'),
         ('takeProfit', 'f4'),
         ('stopLevel', 'f4'),
         ('takeProfitLevel', 'f4'),
         ('openTime', 'M8[ns]'),
         ('closeTime', 'M8[ns]'),
         ('assetName', 'S10'),
         ('direction', 'i4'),
         ('exitType', 'i4')]
        
        self.pl_types = [('assetName', 'S10'),
         ('variantId', 'i4'),
         ('date', 'M8[ns]'),
         ('realised_profit', 'f8'),
         ('unrealised_profit', 'f8')]
        
    def load_trade_data_from_binary(self, filepath):
        fh = open(filepath, "rb")
        
        #first read the number of fields from the metadata
        field_count = struct.unpack("<i", fh.read(4))[0]
        
        #setup a dtype for the trade id and then append a double in the loop
        m_types = [("tradeId", "i4")]
        
        #now loop through field_count times reading 30 byte strings as the field
        #names
        for i in range(field_count):            
            bin_str = fh.read(30);   
            field = bin_str.decode("utf-8").strip()
            m_types.append((field, "f8"))
      
        #read the rest of the file into a dataframe
        records = np.fromfile(fh, m_types)
        trade_data = pd.DataFrame(records)
            
        fh.close()
        
        return trade_data
        
    def load_from_binary(self, filepath, 
                         additional_data_filepath = None,
                         index_on= "closeTime",
                         sort_on = "closeTime"):
        """Loads the trade table from a binary represenation produced by Nitrade c++
        
        Args:
            filepath (string): the full or relative filepath for the trades binary file            
            additional_data_filepath (string): Additional data that was recorded
                at the open of each trade is saved in a separate file. Including
                this parameter will merge the two datasets together on tradeId
            index_on (string): optional field to use as an index. 
                Pass None for no index
            sort_on (string): optional field to sort on.
                Pass None for no sort
        """
        #create the datatype for the dataframe from binary
        dt = dtype((np.record, self.types))
        
        #get the binary file into a records object and convert to dataframe  
        #to store in the class variable
        records = np.fromfile(filepath, dt)
        self.trades = pd.DataFrame(records)
        
        #decode the binary text fields with utf-8
        self.trades['assetName'] = self.trades['assetName'].str.decode("utf-8").str.strip()

        #sort the dataframe
        if sort_on is not None:
            self.trades = self.trades.sort_values(sort_on, axis=0)
                        
        if additional_data_filepath is not None:
            trade_data = self.load_trade_data_from_binary(additional_data_filepath)
            self.trades = pd.merge(self.trades, trade_data, how='left', on=['tradeId'])
        
        #create an index for the dataframe
        if index_on is not None:                 
            self.trades.index = self.trades[index_on]
            self.trades.index.name = "index"
                    
    def load_daily_pl_from_binary(self, filepath):
        """Loads in the Profit and Loss data from Nitrade - benifit
        of this data is that it also has unrealised profit which
        can't be calculated from closed trades alone
        
        Args:
            filepath (string): The p&l file to load
            
        """
        #create the datatype for the dataframe from binary
        dt = dtype((np.record, self.pl_types))
        
        #get the binary file into a records object and convert to dataframe  
        #to store in the class variable
        records = np.fromfile(filepath, dt)
        self.pl_data = pd.DataFrame(records)
        
        #decode the binary text fields with utf-8
        self.pl_data['assetName'] = self.pl_data['assetName'].str.decode("utf-8").str.strip()
        
        #add in total equity column which is just unrealised
        #plus realised profits ie. open trades and closed trades
        self.pl_data['total_equity'] = self.pl_data['realised_profit'] + self.pl_data['unrealised_profit']
            
        #set the date to the index
        self.pl_data.index = self.pl_data['date']

    def select_trades(self, asset = None, variant_id = -1):
        """Selects a trade set based on asset, variantId or both. Returns all 
        trades if no params are given
        
        Args:
            asset (string): The name of the asset
            variant_id (int): the unique variant id of the trade set
          
        Returns:
            Pandas.dataframe: A dataframe of the selected trades
        """
        
        #create a copy of what is in memory
        filtered = self.trades.copy();
        
        #filter based on variant id if passed
        if variant_id > -1:
            filtered = filtered[filtered["variantId"]==variant_id]
            
        #filtered based on asset if passed
        if asset is not None:
            filtered = filtered[filtered["assetName"]==asset]
            
        #return result, will be all trades if no parameter is passed
        return filtered
    
    def select_pl_data(self, asset = None, variant_id = -1):
        """Selects a P&L data set based on asset, variantId or both. Returns all 
        P&L data if no params are given
        
        Args:
            asset (string): The name of the asset
            variant_id (int): the unique variant id of the trade set
          
        Returns:
            Pandas.dataframe: A dataframe of the selected P&L data
        """
        
        #create a copy of what is in memory
        filtered = self.pl_data.copy();
        
        #filter based on variant id if passed
        if variant_id > -1:
            filtered = filtered[filtered["variantId"]==variant_id]
            
        #filtered based on asset if passed
        if asset is not None:
            filtered = filtered[filtered["assetName"]==asset]
            
        #return result, will be all p&l data if no parameter is passed
        return filtered
    
    def group_pl_data_by_variant(self):
        """Groups P&L data set based on variantId.
          
        Returns:
            numpy.array((id, Pandas.dataframe)): Groups pl data by variant id and 
                returns all the pl dataframes in an array as a tuple with the variant
                id
        """
        
         #build up an array of dataframes for each variant in the trade collection
        
        return list(self.pl_data.groupby(['variantId', 'assetName']))
        
    def get_assets(self):
        """ Get unique asset names
        
        Returns:
            np.array(str): the names of the assets in the trade collection
        """
        
        return self.trades["assetName"].unique()
        
    def get_variants(self):
        """ Get unique variantIds
        
        Returns:
            np.array(int): the variant id numbers in the trade collection
        """
        
        return self.trades["variantId"].unique()
    
    def get_asset_variants(self):
        
        unique = self.trades.drop_duplicates(subset=['assetName', 'variantId'])
        subset = unique[['assetName', 'variantId']]
        tuples = [tuple(x) for x in subset.values]
        return list(tuples)
    
    def test_correlation_pearsons(self, set1, set2, field, period = 90, return_summary = True):
        #more negative the number the better the strategies go together
        
        start_date = set1.index[0]
        end_date = start_date + timedelta(days=period)
        last_date = set1.index[-1]
        r_vals = []

        #llop through the periods - only use full periods
        while end_date <= last_date:
            #get the values for this period
            set1_chunk = set1[field][(set1.index >= start_date) & (set1.index < end_date)]
            set2_chunk = set2[field][(set2.index >= start_date) & (set2.index < end_date)]
            #calcualte the difference in the cumsum to get daily profit loss
            set1_chunk = set1_chunk.diff()
            set2_chunk = set2_chunk.diff()
            #remove nas
            set1_chunk = set1_chunk.dropna()
            set2_chunk = set2_chunk.dropna()

            #calculate the pearson coefficient and hence r2
            r = np.corrcoef(set1_chunk, set2_chunk)[0][1]
            
            r_vals.append(r)

            #move to the next chunk
            start_date = start_date + timedelta(days=period)
            end_date = start_date + timedelta(days=period)
        
        if return_summary:
            return (np.median(r_vals), np.mean(r_vals), np.std(r_vals))
        else:
            return r_vals
     
    def reduce_correlated(self, grouped_sets = None, r_cutoff = 0, period = 90, field='total_equity', metric='Profit'):
        
        #group the P&L data by variant id if no group sets where provieded
        if grouped_sets is None:
            grouped_sets = self.group_pl_data_by_variant()
        
        #setup some dates for the start and end of the cycle
        start_date = grouped_sets[0][1].index[0]
        end_date = start_date + timedelta(days=period)
        last_date = grouped_sets[0][1].index[-1]
        
        #make sure trade period spans larger than the chunk periods
        if (last_date - start_date).days <= period:
            print("Please reduce the period to less than the total trade period" )
            return
        
        
        #create a list which will hold a list of 2d arrays of the correlation coeff
        #for each dataset vs every other dataset
        r_vals = []
        
        #loop through the periods - only use full periods
        while end_date <= last_date:
            #build up an array of dataframes for each variant in the trade collection
            #for the current period
            all_data = []
            for dataset in grouped_sets:
                #get the dataframe from the tuple
                data = dataset[1] 
                #get the values for this period
                chunk = data[field][(data.index >= start_date) & (data.index < end_date)]    
                #calcualte the difference in the cumsum to get daily profit loss
                chunk = chunk.diff()
                #remove nas
                chunk = chunk.dropna()
                #append this chunk to all data for this period
                all_data.append(chunk)
            
            #now all data has been subseted for the period run the corelation calc
            corr = np.corrcoef(all_data)
        
            #shows a warning if no trades were made so replace all nan values with 1
            corr[np.isnan(corr)]=1
            
            #add this 2d array to the r_vals
            r_vals.append(corr)
            
            #move to the next chunk
            start_date = start_date + timedelta(days=period)
            end_date = start_date + timedelta(days=period)
        
        if len(r_vals) == 0:
            print("No data")
            return
        
        #create a new 2d array that has the median value for each comparison
        #across the periods. Median is used because it gives a better picture of the
        #the stability of correlation across the whole period
        results = np.median(r_vals, axis=0)
        
        
        #calculate performance metrics used for ranking variants
        #poorer performing correlated variants are removed
        pms = []
        ids = []
        for v in grouped_sets:
            #get a performance report dictionary using the variantId
            #in v[0]
            if metric == "Profit":
                pms.append(v[1]['total_equity'][-1])
            ids.append(v[0])
        
        #create a dataframe of variantIds and performance_metrics so they can be
        #easily ranked
        data_tuples = list(zip(ids, pms))
        performance_metrics = pd.DataFrame(data_tuples, columns = ["variantId", "metric"])
        performance_metrics = performance_metrics.sort_values("metric", ascending=False)
        
        #start from the best performer and remove all corelated variants
        removals = []
        keep = []
        for index, pm in performance_metrics.iterrows():            
            if pm["variantId"] not in removals:
                keep.append(pm["variantId"])
                for column_index in range(results.shape[1]):
                    if column_index != index and performance_metrics["variantId"][column_index] not in keep:                        
                        if results[index][column_index] > r_cutoff:
                            removals.append(performance_metrics["variantId"][column_index])
          
        #reduce the list of trades to only the trades from the uncorrelated 
        #variants
        reduced_trades = pd.DataFrame()
        reduced_pl_data = pd.DataFrame()
        for tup in grouped_sets:
            if tup[0] not in removals:
                reduced_trades = reduced_trades.append(self.select_trades(variant_id = tup[0][0], asset = tup[0][1]))
                reduced_pl_data.append(tup[1])
        
        #sort the trades so that they can be nicely graphed
        reduced_trades = reduced_trades.sort_index()
        
        #create a new trade collection object with only the reduced variatns
        reduced_trade_collection = TradeCollection()
        reduced_trade_collection.trades = reduced_trades
        reduced_trade_collection.pl_data = reduced_pl_data
        
        return reduced_trade_collection
        
        
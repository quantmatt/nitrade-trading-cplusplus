# -*- coding: utf-8 -*-
"""
Created on Mon May 20 16:11:05 2019

@author: matth
"""

from trade_collection import TradeCollection
import price_data
import matplotlib.pyplot as plt        


#Load trades from Nitrade for analysis in Python
filepath = "D:\\CPlusPlusApps\\NitradeTradeConsole\\Debug\\trades.bin"  
additional_data = "D:\\CPlusPlusApps\\NitradeTradeConsole\\Debug\\trades_data.bin"  
pl_filepath = "D:\\CPlusPlusApps\\NitradeTradeConsole\\Debug\\daily_pl.bin"  
tc = TradeCollection()
#trade data
tc.load_from_binary(filepath, additional_data)
#P&L data
tc.load_daily_pl_from_binary(pl_filepath)

#reduced corelated variants based on daily P&L
# r is between -1 to 1 with -1 being uncorrelated and 1 being correlated
uncorrelated_tc = tc.reduce_correlated(r_cutoff = 0.5, period = 30)
plt.plot(uncorrelated_tc.trades.index, uncorrelated_tc.trades['profit'].cumsum())
print("Uncorrelated Variants: ",len(uncorrelated_tc.get_asset_variants()))
print("Total Variants: ",len(tc.get_asset_variants()))








#load from csv into binary file for Nitrade
csv_file = "C:\\ForexData\\BaseData\\AUDUSD_m1.csv"
binary_file= "C:\\ForexData\\BaseData\\AUDUSD_m1_temp.bin"
df = price_data.csv_to_binary(csv_file, binary_file, 
                              start_date = '2018-01-01', 
                              end_date = '2019-01-01', 
                              return_data = True)

#loads a binary file from Nitrade
df = price_data.binary_to_csv(binary_file, "C:\\ForexData\\BaseData\\AUDUSD_m1_temp.csv", return_data=True)

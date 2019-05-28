# Nitrade Trading Platform
An open source financial trading back testing engine and live trading platform for linux and windows

# Features

* Multi asset back testing
* Multicore testing
* Bid / Ask data feeds
* Optimisable strategies
* Records additional trade data with each trade to use for trade filtering Machine Learning models in Python
* Trades exported for further analysis in Python
* Use the python scripts attached to load in trades and do your research in Python
* Live trading available soon

# Get Started

* Download the project from github and load into Visual Studio or Eclipse.
* Make sure the Assets.csv and AssetsData directory is in the current working directory
* Run the NitradeApp.exe 
* Strategies will have to be manually coded and compiles. 
* Sub class the Strategy class to build your own strategy or change the TestStrategy class that is used as an example
* Set the number of cores and the assets to test in NitradeApp.cpp

# Price Data

* Some example data is included, but to load in your own data use the python scripts to load from csv into a binary file and put in the AssetsData directory
* Update the Assets.csv file with the information about the asset.



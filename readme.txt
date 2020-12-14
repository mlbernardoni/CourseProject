Link to Video:
https://illinois.zoom.us/rec/play/p1k2d8OXy9zlMU52qAKeYekW6uxafweLBO2aqz6R_DGSbHkY06jjyYQTueHspfeX-Fep54MHEkpEDyt1.ORGucOcp6XSypU7d?continueMode=true

Setup:
Windows: Open an anaconda prompt navigate to the project's directory
         type:  conda env create -f Gensim.yml
              the created environment will be called "Gensim"
              when you open the notebook, you will have to change kernels to Gensim
              detailed instructions are in the project write-up's appendix

Manual Setup: If you can't use the windows yml file
Optional – create a new Anaconda environment to install the Gensim package:
1.	Open Anaconda Navigator
2.	Select Environments
3.	Create an environment (i.e. “gensim”)
Install genism in Anaconda
1.	Open the Anaconda command prompt
2.	If you created a new environment in the previous step:
	a.	Activate the newly created environment if you created one (“Activate gensim”)
	b.	Run: conda install nb_conda_kernels  (Proceed Y)
	c.	Run: python -m ipykernel install --user --name myenv --display-name "Gensim" 
		(you can use any display name you wish, this is what will show up on Jupyter Notebook)
	d.	Run: pip install environment_kernels
3.	Run: pip install --upgrade genism
4.	Run: pip install –upgrade pyldavis
5.	Run: pip install –upgrade glob2
6.	Run: pip install –upgrade matplotlib

Quick start:    open ITMTF in jupyter notebook, look at the parameters and press run
                after a run .csv files can be examined in the Run_data directory
                or one of the saved models can be examined in the ITMTF_vis_topic notebook

                                
Primary notebooks:
ITMTF               	- the main notebook to run

itmtf_improved      	- has the code for the "improved" ITMTF algorithm; called from ITMTF
itmtf_withsplit     	- has the code the the "classic" ITMTF algorithm, with splits and buffers; called from ITMTF
ITMTF_vis_topic    	- used to visualize the words in a saved model
run_plot		- used to visualize stats collected in a series of runs
run_word		- used to visualize top words from significant topics
load_helper             - loads preprocessed files; called from ITMTF
itmtf_prerun_stats      - not really used in processing, but preprocessed pearson and granger stats per word stream
                          was important in creating the "improved" algorithm

Other notebooks:
baseline_create_helper  - used to create baselines
coherence_create_helper - used to test baselines to estimate the best number of topics
itmtf_cleaning          - a bunch of cells used to clean, mine, and reduce data

LDA_data directory      - used to store pre-processed data
                          also used to store artifacts from algorithm runs
                          (models must be stored in order to use the ITMTF_vis_topic notebook) 
Classic_Baseline_Plot	- plot baseline stats
  

ITMTF_analysis directory:
Demonstration Spreadsheets used in the project write-up

    "Improved" algorithm:
    wordanalysis.xls        - analysis of words in significant topics for 20, 30, 40 topics
    topicanalysis.xls       - analysis of avg confidence scores for runs of 20, 30, 40 topics
    Decayanalysis.xls       - analysis of decay parameters on 30 topic runs
    30iterationanalysis.xls - analysis of confidence for a 30 topic run over 30 iterations

    "Classic" algorithm:
    Paper.sigwords.xls      - analysis of words in significant topics for a classic run (30 topics, 5 buffers)
    Paper.confidence.xls    - analysis of the confidence scores for the classic run (30 topics, 5 buffers)

Other files:
*.log                   - left over artifact files showing the tuning of the model
CS410.cpp               - MP3 re-writen in c++ to improve speed, 
                          not used but there if anyone was interested (spent too many hours to discard)
                          
Libraries used:
Gensim Python LDA:                       https://radimrehurek.com/gensim/models/ldamodel.html
SciPy's pearson r:                       https://docs.scipy.org/doc/scipy/reference/generated/scipy.stats.pearsonr.html
statsmodels granger causality tests:     https://www.statsmodels.org/stable/generated/statsmodels.tsa.stattools.grangercausalitytests.html 
pyLDAvis:				 https://pyldavis.readthedocs.io/en/latest/index.html   
glob:					 https://docs.python.org/3/library/glob.html 
Matplotlib:				 https://matplotlib.org/ 

Library tested but not used:
pypi.orgs PLSA:                         https://pypi.org/project/plsa/


// basic file operations

#include "CS410.h"
#include <dirent.h>
#include <iomanip>
#include <chrono>
#include <ctime>


// Driver function to sort the vector elements 
// by second element of pairs 
bool sortbysec(const pair<string,double> &a, 
              const pair<string,double> &b) 
{ 
    return (a.second > b.second); 
} 


/*
    Constructor
 */
CorpusClass::CorpusClass() {
    pdocarray = 0;
}

/*
    Destructor
 */
CorpusClass::~CorpusClass() {
}

void CorpusClass::write_topics()
{
    //cout << "write_topics: \n";
    //cout << output_path << "\n";
    //cout << time_slice << "\n";
    //cout << feedback_iteration << "\n";
    //string countstr = to_string(feedback_iteration);
    //cout << countstr << "\n";
    fstream newfileout;
    ostringstream countstream;
    countstream << std::setw(3) << std::setfill('0') << feedback_iteration;
    string countstr = countstream.str();
    string outfile = output_path + "\\" + time_slice + "." + countstr + ".csv";
    cout << outfile << "\n";
    newfileout.open(outfile,ios::out); //open a file to perform write operation using file object
    if (newfileout.is_open()) //checking whether the file is open
    {   
        
        for(int topicindex = 0; topicindex < number_of_topics; topicindex++)
        {
            vector<pair<string,double>> topicout;
            for(int wordindex = 0; wordindex < VocublarySize; wordindex++)
            {
                topicout.push_back(make_pair(VocabularyList[wordindex],TopicWordProb[topicindex][wordindex]));
                //newfileout << VocabularyList[wordindex] << "," << TopicWordProb[topicindex][wordindex] << ",";
            }
            //newfileout << "\n";
            sort(topicout.begin(), topicout.end(), sortbysec); 
            for(int wordindex = 0; wordindex < VocublarySize; wordindex++)
            {
                newfileout << topicout[wordindex].first << "," << topicout[wordindex].second << ",";
            }
            newfileout << "\n";
         }

        newfileout.close(); //close the file object.
    }
}

/*
    Construct a list of unique words in the whole corpus. Put it in self.vocabulary
    for example: ["rain", "the", ...]
 */
void CorpusClass::build_vocabulary()
{
    feedback_iteration = 0;
    string datadate = time_slice;
    string datadir = document_path;
    string myfile;


    struct dirent *entry = nullptr;
    DIR *dp = nullptr;

    DocumentNumber = 0;
    VocublarySize = 0;
    VocabularyMap.clear();
    DocumentWordMatrix.clear();
    VocabularyList.clear();
    TopicWordProb.clear();
    DocTopicProb.clear();
    delete_array(); // pdocarray
    fstream newfile;
    newfile.open(import_file,ios::in); //open a file to perform read operation using file object
    if (newfile.is_open()) //checking whether the file is open
    {   
        string tp;
        while(getline(newfile, tp)) //read data from file object and put it into string.
        {
            istringstream iss(tp);
            vector<string> results(istream_iterator<string>{iss}, istream_iterator<string>());      
            for (vector<string>::iterator wordit = results.begin() ; wordit != results.end(); ++wordit)
            {
                datadate = *wordit;
  
                //int count = 0;
                dp = opendir(datadir.c_str());
                if (dp != nullptr) 
                {
                    while ((entry = readdir(dp)))
                    {
                        myfile = entry->d_name;
                        size_t found = myfile.find(datadate); 
                        if (found != string::npos) 
                        {
                            string filename = datadir + "//" + myfile;
                            //cout << filename << "\n"; //print the data of the string
                            fstream newfile;
                            newfile.open(filename,ios::in); //open a file to perform read operation using file object
                            if (newfile.is_open()) //checking whether the file is open
                            {   
                                string tp;
                                while(getline(newfile, tp)) //read data from file object and put it into string.
                                {
                                    istringstream iss(tp);
                                    vector<string> results(istream_iterator<string>{iss}, istream_iterator<string>());      
                                    for (vector<string>::iterator wordit = results.begin() ; wordit != results.end(); ++wordit)
                                    {
                                        //std::cout << ' ' << *it; 
                                        // update vocabulary map
                                        string newline = *wordit;
                                        std::string::size_type pos = newline.find(',');
                                        string newword = newline.substr(0, pos);
                                        map <string, int>::iterator  mapit;
                                        mapit = VocabularyMap.find(newword);
                                        if (mapit != VocabularyMap.end())
                                        {
                                            mapit->second++;
                                        }
                                        else
                                        {
                                            VocabularyMap.insert(make_pair(newword, 1));
                                            VocabularyList.push_back(newword);
                                        } 
                                    }
                                }

                                //newfile.clear();
                                //newfile.seekg(0, ios::beg);
                                newfile.close(); //close the file object.
                            }   
                        }                
                    }
                    closedir(dp);
               }
            }
        }
    }
    newfile.close();
    VocublarySize = VocabularyMap.size();
    cout << "Vocublary Size: " << VocublarySize << "\n";
    //cout << "Count: " << count << "\n";
    newfile.open(import_file,ios::in); //open a file to perform read operation using file object
    if (newfile.is_open()) //checking whether the file is open
    {   
        string tp;
        while(getline(newfile, tp)) //read data from file object and put it into string.
        {
            istringstream iss(tp);
            vector<string> results(istream_iterator<string>{iss}, istream_iterator<string>());      
            for (vector<string>::iterator wordit = results.begin() ; wordit != results.end(); ++wordit)
            {
                datadate = *wordit;

                dp = opendir(datadir.c_str());
                if (dp != nullptr) 
                {
                    while ((entry = readdir(dp)))
                    {
                        myfile = entry->d_name;
                        size_t found = myfile.find(datadate); 
                        if (found != string::npos) 
                        {
                            string filename = datadir + "//" + myfile;
                            fstream newfile;
                            newfile.open(filename,ios::in); //open a file to perform read operation using file object
                            if (newfile.is_open()) //checking whether the file is open
                            {   
                                string tp;
                                
                                map <string, int> docwords;
                                while(getline(newfile, tp)) //read data from file object and put it into string.
                                {
                                    //cout << tp << "\n"; //print the data of the string
                                    //cout << "oy \n";
                                    istringstream iss(tp);
                                    vector<string> results(istream_iterator<string>{iss}, istream_iterator<string>());                         
                                    map <string, int>::iterator  mapit;
                                    for (vector<string>::iterator wordit = results.begin() ; wordit != results.end(); ++wordit)
                                    {
                                        string newline = *wordit;
                                        std::string::size_type pos = newline.find(',');
                                        string newword = newline.substr(0, pos);
                                        mapit = docwords.find(newword);
                                        if (mapit != docwords.end())
                                        {
                                            mapit->second++;
                                            //cout << "OY " << mapit->second << "\n";
                                        }
                                        else
                                        {
                                            docwords.insert(make_pair(newword, 1));
                                        }
                                    }
                                 }

                                //cout << "Words in this doc: " << docwords.size() << "\n";
                                //DocumentWords.push_back(docwords);
                                vector<int> wordcount;
                                // for each word
                                for (vector <string>::iterator wordit = VocabularyList.begin() ; wordit != VocabularyList.end(); ++wordit)
                                {
                                    map <string, int>::iterator  mapit;
                                    int vocabcount = 0;
                                    mapit = docwords.find(*wordit);
                                    if (mapit != docwords.end())
                                    {
                                        vocabcount = mapit->second;
                                    }
                                    wordcount.push_back(vocabcount);
                                    //cout << "word Count: " << vocabcount << "\n";
                                }
                                //cout << "words: " << wordcount.size() << "\n";
                                DocumentWordMatrix.push_back(wordcount);
                                newfile.close(); //close the file object.
                            }          
                        }
                    }
                //cout << "Count: " << count << "\n";

                }   
                closedir(dp);
            }
        }
    }
    newfile.close();
    DocumentNumber = DocumentWordMatrix.size();
    cout << "Documents: " << DocumentNumber << "\n";
    //cout << DocumentWordMatrix  << "\n";
    cout << "Doc Word matrix complete: " << "\n";
  
}
void CorpusClass::initialize_prob()
{
    initialize_doctopicprob();
    initialize_topicwordprob();
}
void CorpusClass::initialize_doctopicprob()
{
    srand(static_cast<unsigned int>(std::time(nullptr))); 
    // doc topic prob
    for(int i = 0; i < DocumentNumber; i++)
    {
        vector<double> prob;
        double probsum = 0;
        int yy;
        for(yy = 0; yy < number_of_topics; yy++)
        {
            //double newprob = 1;
            double newprob = (double)rand()/RAND_MAX; 
            probsum += newprob;
            prob.push_back(newprob);
        }
        for(yy = 0; yy < number_of_topics; yy++)
        {
            prob[yy] = prob[yy]/probsum;
        }
        DocTopicProb.push_back(prob);
    }
}
void CorpusClass::initialize_topicwordprob()
{
    //vector<vector<double>> DocTopicProb;
    //vector<vector<double>> TopicWordProb;
    for(int ii = 0; ii < number_of_topics; ii++)
    {
        vector<double> prob;
        double probsum = 0;
        int yy;
        for(yy = 0; yy < VocublarySize; yy++)
        {
            double newprob = (double)rand()/RAND_MAX; 
            //double newprob = 1;
            probsum += newprob;
            prob.push_back(newprob);
        }
        for(yy = 0; yy < VocublarySize; yy++)
        {
            prob[yy] = prob[yy]/probsum;
        }
        TopicWordProb.push_back(prob);
    }
    //cout << "Initialization complete: " << "\n";

}

void CorpusClass::setup_array()
{
    pdocarray = new double**[DocumentNumber];
    for (int i = 0; i < DocumentNumber; i++)
    {
        double** ptopicarray = new double*[number_of_topics];
        //cout << "Allocated topic array \n";
        pdocarray[i] = ptopicarray;
        for (int ii = 0; ii < number_of_topics; ii++)
        {
            double* pwordarray = new double[VocublarySize];
            for (int iii = 0; iii < VocublarySize; iii++)
                pwordarray[iii] = 0;
            //cout << "Allocated word array \n";
            ptopicarray[ii] = pwordarray;
            //for (int iii = 0; iii < VocublarySize; iii++)
                //pwordarray[iii] = 0;
        }
    }
    cout << "E Initialization Setup \n";
}
void CorpusClass::delete_array()
{
    if ( pdocarray != 0)
    {
        for (int i = 0; i < DocumentNumber; i++)
        {
            double** ptopicarray = pdocarray[i];
            for (int ii = 0; ii < number_of_topics; ii++)
            {
                double* pwordarray = ptopicarray[ii];
                delete[] pwordarray;
            }
            delete[] ptopicarray;
        }
        delete[] pdocarray;    
    }
    pdocarray = 0;
}

void CorpusClass::run_iteration()
{
    //cout << "E Step \n";
    //cout << "Allocated doc array \n";
    for(int docindex = 0; docindex < DocumentNumber; docindex++)
    {
        //cout << "E Doc \n";
        for(int wordindex = 0; wordindex < VocublarySize; wordindex++)
        {
            double mysum = 0;
            //cout << "E Word \n";
            for(int topicindex = 0; topicindex < number_of_topics; topicindex++)
            {
                //cout << "E Topic \n";
                double newval = TopicWordProb[topicindex][wordindex] * DocTopicProb[docindex][topicindex];
                //cout << "E Topic1 \n";
                double** ptopicarray = pdocarray[docindex];
                double* pwordarray = ptopicarray[topicindex];
                pwordarray[wordindex] = newval;

                mysum += newval;
                //cout << " " << mysum << " ";
            }
            //cout << "\n";
            if (mysum != 0)
                for(int topicindex = 0; topicindex < number_of_topics; topicindex++)
                {
                    //cout << "E Topic2 \n";
                    double** ptopicarray = pdocarray[docindex];
                    double* pwordarray = ptopicarray[topicindex];
                    pwordarray[wordindex] = pwordarray[wordindex]/mysum;
                }
        }
   }
    
    //cout << "M Step \n";
    for(int docindex = 0; docindex < DocumentNumber; docindex++)
    {
        double tonormal = 0;
        for(int topicindex = 0; topicindex < number_of_topics; topicindex++)
        {
            double mysum = 0;
            for(int wordindex = 0; wordindex < VocublarySize; wordindex++)
            {
                double** ptopicarray = pdocarray[docindex];
                double* pwordarray = ptopicarray[topicindex];
                double newval = pwordarray[wordindex] * DocumentWordMatrix[docindex][wordindex];
                mysum += newval;
                //cout << "probs: " << DocumentWordMatrix[docindex][wordindex] << "\n";
            }
            DocTopicProb[docindex][topicindex] = mysum;
            //cout << "DocTopicProb: " << DocTopicProb[docindex][topicindex] << "\n";
            tonormal += mysum;
        }
        // normalize
        if (tonormal != 0)
            for(int topicindex = 0; topicindex < number_of_topics; topicindex++)
            {
                DocTopicProb[docindex][topicindex] = DocTopicProb[docindex][topicindex]/tonormal;
                //cout << " " << DocTopicProb[docindex][topicindex] << " ";
            }
        //cout << "\n";
    }
    for(int topicindex = 0; topicindex < number_of_topics; topicindex++)
    {
        double tonormal = 0;
        for(int wordindex = 0; wordindex < VocublarySize; wordindex++)
        {
            double mysum = 0;
            for(int docindex = 0; docindex < DocumentNumber; docindex++)
            {
                double** ptopicarray = pdocarray[docindex];
                double* pwordarray = ptopicarray[topicindex];
                double newval = pwordarray[wordindex] * DocumentWordMatrix[docindex][wordindex];
                mysum += newval;
            }
            TopicWordProb[topicindex][wordindex] = mysum;
            tonormal += mysum;
            //cout << " " << tonormal << " ";
        }
        if (tonormal != 0)
            for(int wordindex = 0; wordindex < VocublarySize; wordindex++)
            {
                TopicWordProb[topicindex][wordindex] = TopicWordProb[topicindex][wordindex]/tonormal;
                //cout << " " << TopicWordProb[topicindex][wordindex] << " ";
            }
        //cout << "\n";
    }
}

double CorpusClass::calculate_likelyhood()
{
    double newlikely = 0;
    for(int docindex = 0; docindex < DocumentNumber; docindex++)
    {
        long double docsum = 0;
        for(int wordindex = 0; wordindex < VocublarySize; wordindex++)
        {
            long double mysum = 0;
            //cout << "DocTopicProb: " ;
            for(int topicindex = 0; topicindex < number_of_topics; topicindex++)
            {
                //cout <<  TopicWordProb[topicindex][wordindex]  << " ";
                //cout <<  DocTopicProb[docindex][topicindex] << " ";
                //cout <<  TopicWordProb[topicindex][wordindex] * DocTopicProb[docindex][topicindex] << ", ";
                mysum += TopicWordProb[topicindex][wordindex] * DocTopicProb[docindex][topicindex];
            }
            //cout << "mysum: " << mysum << "\n";
            //cout << "\n";
            //cout << " " << mysum << ",";
            if (mysum != 0)
                mysum = log10(mysum);
            //cout << mysum << ",";
            //cout << DocumentWordMatrix[docindex][wordindex] << ",";
            //cout << mysum << ",";
            mysum = mysum * DocumentWordMatrix[docindex][wordindex];
            //cout << mysum << " ";
            docsum += mysum;
        }
        //cout << "\n";
        newlikely += docsum;
        //cout << docsum << " ";
        //cout << "docsum: " << docsum << "\n";
        
        //docindex = DocumentNumber;
    }
    //cout << "\n";
    return newlikely;
}

void CorpusClass::PLSA()
{

    double current_likelihood = calculate_likelyhood();
    //cout << "Baseline Likely: " << current_likelihood << "\n";
    for (int i = 0; i < max_iterations; i++)
    {
        //cout << "Iteration: " << i << "\n";
        try 
        {
            run_iteration();         
        }
        catch (const std::bad_alloc& e) 
        {
            std::cout << "Allocation failed: " << e.what() << '\n';
            return;
        }        
        double newlikely = calculate_likelyhood();
        if(abs(current_likelihood - newlikely) < epsilon)
        {
            cout << "Converge \n";
            cout << "Iteration: " << i << "\n";
            write_topics();
            return;       
        }
        current_likelihood = newlikely; 
    }
    cout << "Converge Failed \n";
    write_topics();
}

void CorpusClass::input_topics(string strfile)
{
    feedback_iteration++;
    fstream newfile;
    newfile.open(strfile,ios::in); //open a file to perform read operation using file object
    if (newfile.is_open()) //checking whether the file is open
    {   
        string tp;
        while(getline(newfile, tp)) //read data from file object and put it into string.
        {
            istringstream iss(tp);
            vector<string> results(istream_iterator<string>{iss}, istream_iterator<string>());      
            for (vector<string>::iterator wordit = results.begin() ; wordit != results.end(); ++wordit)
            {
                // put the comma separated string input into a vector
                string newtopic  = *wordit;
                vector<string> cells;
                stringstream ss(newtopic);
             
                while (ss.good()) 
                {
                    string substr;
                    getline(ss, substr, ',');
                    cells.push_back(substr);
                }

                vector<double> prob;
                double probsum = 0;
                int yy;
                for(yy = 0; yy < VocublarySize; yy++)
                {
                    // initialize to 0
                    double newprob = 0; 
                    
                    // see if this word is in vector
                    for (int xx = 0; xx < cells.size(); xx = xx + 2)
                    {
                        if (cells[xx] == VocabularyList[yy])
                        {
                            newprob = stod(cells[xx+1]); 
                            break;
                        }
                    }
                    //double newprob = 1;
                    probsum += newprob;
                    prob.push_back(newprob);
                }
                for(yy = 0; yy < VocublarySize; yy++)
                {
                    prob[yy] = prob[yy]/probsum;
                }
                TopicWordProb.push_back(prob);
                number_of_topics++;
            }
        }
        initialize_doctopicprob();
        setup_array();
        cout << "setup_array2 \n";
      
        PLSA();
        
        delete_array();
}   
 }

void initalgorithm(CorpusClass* corpus)
{
/*
    fstream newfile;
    newfile.open(corpus->import_file,ios::in); //open a file to perform read operation using file object
    if (newfile.is_open()) //checking whether the file is open
    {   
        string tp;
        while(getline(newfile, tp)) //read data from file object and put it into string.
        {
            istringstream iss(tp);
            vector<string> results(istream_iterator<string>{iss}, istream_iterator<string>());      
            for (vector<string>::iterator wordit = results.begin() ; wordit != results.end(); ++wordit)
            {
                corpus->time_slice = *wordit;
*/
                corpus->build_vocabulary();
                cout << "build_vocabulary \n";
                corpus->initialize_prob();
                cout << "initialize_prob \n";
                corpus->setup_array();
                cout << "setup_array \n";
                
                corpus->PLSA();
                corpus->delete_array();
/*                
            }
        }
     }
*/
}

int main (int argc, char** argv) 
{
    auto timenow = 
      chrono::system_clock::to_time_t(chrono::system_clock::now()); 
  
    cout << ctime(&timenow) << "\n";    
    if (argc > 2)
    {
        string param = argv[1];
        string inputfile = argv[2];
        CorpusClass corpus;
        corpus.number_of_topics = 30;
        corpus.max_iterations = 900;
        corpus.epsilon = 0.001;
        if (param == "baseline")
        {
            corpus.import_file = inputfile;
            //corpus.document_path = "c:\\programs\\CS410_Project\\CS410_cleaned";
            //corpus.output_path = "c:\\programs\\CS410_Project\\CS410_baseline";
            corpus.document_path = ".\\CS410_cleaned";
            corpus.output_path = ".\\CS410_baseline";
            initalgorithm(&corpus);
            
        }
        if (param == "timeslice")
        {
            corpus.import_file = inputfile;
            corpus.document_path = ".\\CS410_cppinput";
            corpus.output_path = ".\\CS410_timeslice";
            initalgorithm(&corpus);
            //corpus.input_topics("topic_in1.csv");
            //corpus.input_topics("topic_in2.csv");
        }
        if (param == "import")
        {
             corpus.import_file = inputfile;           
        }
        
         if (param == "test")
        {
            corpus.DocumentNumber = 2673 + 1;
            corpus.number_of_topics = 30 + 1;
            corpus.VocublarySize = 28092 + 1;
            //corpus.setup_array();
            cout << corpus.DocumentNumber << " " << corpus.number_of_topics << " " << corpus.VocublarySize << "\n";
            unsigned int totalsize = corpus.DocumentNumber * corpus.number_of_topics * corpus.VocublarySize;
            unsigned int count = 0;
            cout << totalsize << "\n";
            //double* buffer = (double*) malloc(totalsize * sizeof(double));
            double* buffer = new double[totalsize];
            cout << "Malloced" << "\n";
            cout << buffer << "\n";
            try 
            {
                for (unsigned int  qq = 0; qq < totalsize; qq++)
                    buffer[qq] = 0;
            }
            catch (const char* e) 
            {
                cout << "OY NO \n";
                std::cout << e  <<  '\n';
                return 1;
            }        
        
            cout << "Inited" << "\n";
            for (int x = 0; x < corpus.DocumentNumber; x++)
            {
                for (int y = 0; y < corpus.number_of_topics; y++)
                {
                     for (int z = 0; z < corpus.VocublarySize; z++)
                     {
                        unsigned int newindex = (x * corpus.number_of_topics * corpus.VocublarySize) + (y * corpus.VocublarySize) + z;
                        if(newindex > totalsize - 1)
                            //int yyyy = 0;
                            cout << count << " " << newindex << " " << totalsize << " Bad News\n";
                        else
                        {
                            buffer[newindex] = 0;
                            count++;
                        }
                    }
                }
            }
            cout << count << "\n";
            delete(buffer);
        }
       

    }   
    else
    {
        cout << "format is:\ncs410 timeslice filename\ncs410 import filenamecs\ncs410 baseline filename\n";   
    }
    
    timenow = 
      chrono::system_clock::to_time_t(chrono::system_clock::now()); 
  
    cout << ctime(&timenow) << "\n";    
    return 0;
}
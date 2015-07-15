#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <fstream>
#include <iomanip>

const int MX_UW_TW = 70;   //at most 70 unique words in a tweet

class pWord
{
       
      int count[MX_UW_TW];  //histogram of unique words
      char *pos;            //position of the pointer in the buffer
      std::vector<std::string> uw;             //pointer to array of unique words in a twitter
      std::ofstream file1, file2;                  
      char *word;           //pointer to word scanned by getword()
      int uninum, mwlen, elind;
      struct tWord
      {
         tWord *left;
         tWord *right;
         char *pw;            //pointer to the word 
         size_t w_count;      //count of the word in the file  
      };   
      tWord* tw_insert(tWord *wp, char *str);
      void uwit();
      int getword();
      void inorder(tWord *wp);
      void median(int num, int size);
   public:  
      pWord(char *buffer, int h);
      void analyse();
};

//Constructor with initialization
pWord::pWord(char *buffer, int h)
{
   //Opening ofstream and initializing variables
   file1.open("tweet_output/ft1.txt");
   file2.open("tweet_output/ft2.txt");
   pos = buffer;
   uninum = 0;
   mwlen = elind = 0;
   for(short int i = 0; i<MX_UW_TW; i++)
      count[i] = 0;
}

//Get the next word from the buffer and returns its length to be used to determine 
//column width in output file ft1.txt 
int pWord::getword()
{
   char *w = pos;
   word = w;
   int wlen = 0;
   elind = 0;

   if ( !( isascii(*w) && !isspace(*w) ) ) 
   {
      if(*w != EOF)
      {
         *w = '\x00';
         pos++;
      }
      return 0;
   }
   while((isascii(*w) && !isspace(*w))) 
   {
      w++;
      wlen++; 
   }
   //flags end of line
   if (*w == '\n')
      elind = 1;

//chopping the sequence
   *w = '\x00';
   pos = w+1;
   return wlen;
}

void pWord::uwit()
{
//Look in the vector to look for already existing word
//The sequence is not ordered with a tree as it is pretty small <70 elements
   std::string str(word);
   for (short int i = 0; i<uw.size(); i++)
      if (str == uw[i])
         return;
   uw.push_back(str);
   uninum++;
}

//Scans through the buffer
void pWord::analyse()
{
   mwlen=std::max(mwlen,getword());
   tWord *wp = '\x00';
   int nlines= 0;

   while(*word != EOF)
   {
      if(*word != '\x00')
         {
//this function can be used for other things, which is why has been kept in its original format with two arguments
//in reality, word is a class member and does notneed to explicitly appear 
            wp = tw_insert(wp,word);
            uwit();
            //if end of line clear vector of unique word in single tweet
            if (elind)
            {
               nlines++;
               median(uninum, nlines);
               uninum = 0;
               uw.clear();
            }
         }
      mwlen=std::max(mwlen, getword());
   }
//reading out
   inorder(wp);
//closing
   file1.close();
   file2.close();
}
 
pWord::tWord * pWord::tw_insert(tWord *wp, char *str)
{
//Initialize if empty, new word
   if(wp=='\x00')
   {
      pWord::tWord* wp = new pWord::tWord;
      wp->pw = str;
      wp->left = wp->right = '\x00';
      wp->w_count = 1;
      return wp;
   }
 
//start looking in the tree, if not found increment the counter
   int str_val = strcmp(str, wp->pw);
   if(str_val < 0)         
      wp->left = tw_insert(wp->left,str);         
   else if(str_val> 0)     
      wp->right = tw_insert(wp->right,str);
   else
      wp->w_count++;
   return wp;
}
 
void pWord::inorder(tWord *wp)
{
   //If the tree exist, start the recursive readout
   if(wp!='\x00')
   {
      inorder(wp->left);
      //Here I assumed, arbitrarly, mwlen+5 as width of the column, it can be made narrower or larger
      file1 << std::left << std::setw(mwlen+5) << wp->pw << wp->w_count << "\n";
      inorder(wp->right);
   }
}

void pWord::median(int num, int size)
{
    //Adapted from the counting sort algorithm
    //Just updating the frequency of unique words
    //we do not need to generate it from the vector of unique numbers

    //It is probably overall faster to always evaluate 70 elements
    //rahter than updating the histogram vector whenever a tweet with
    //a larger number of words comes in
    count[num-1]++;
    int input[size];
    int z = 0;
   
    int dump[MX_UW_TW];
    
    //dump vector will be depleted by the algorithm, while count has to stay constant
    //in order to be properly update   
    for (int i = 0; i<MX_UW_TW; i++)
    {
       dump[i] = count[i];
    }


    for (int i = 1; i<MX_UW_TW; i++)
    {
        while (dump[i-1]>0)
        {
           input[z]=i;
           z++;
           dump[i-1]--;
        } 
    }
    
    if (size % 2)
         file2 << input[size/2] << "\n";
    else
         file2 << input[size/2-1]+0.5*(input[size/2]-input[size/2-1]) << "\n";
}

main(int argc,char *argv[])
{
   FILE  *in, *out;
   long int FSize;
   int i;
   char *buffer;
   size_t result;

   if(argc!=2)
   {
      fputs("Invalid arguments\n",stderr);
      exit(1);
   }

   in = fopen (argv[1], "r" );
   if (in == NULL) 
   {
      fputs ("File error: input file not found.\n", stderr); 
      exit (1);
   }
   
   // obtain file size:
   fseek(in, 0, SEEK_END);
   FSize = ftell(in);
   rewind(in);

   // allocate memory to contain the whole file:
   //buffer = (char*) malloc (sizeof(char)*FSize);
   buffer = new char[FSize];
   if (buffer == NULL) 
   {
      fputs ("Memory error: buffer does not match file size.\n", stderr); 
      exit (2);
   }

   // copy the file into the buffer:
   result = fread(buffer,1,FSize,in);
   if (result != FSize) 
   {
      fputs ("Reading error: buffer does not match file size.\n", stderr); 
      exit (3);
   }

   buffer[FSize]=EOF; 

   //Find positions of new tweets in the file, only indices will then be passed to processor(s)
   //std::vector<int> pos;
   int h = 0; //vector with number of tweets, to be used to optimize code in the future when MPI is required
   //posi[0] = 0;
   
  /* for (int i = 0; i<FSize-1; i++)
   {
      if (buffer[i] == '\n')
      {         
         posi[h] = i+1;    
         h++;
      }
   }*/
    
   //Calling functions
   //Declaring and initializing
   pWord wp(buffer, h);
   
   
   //analysis
   wp.analyse();

   // terminate
   fclose(in);
   free(buffer);
}   



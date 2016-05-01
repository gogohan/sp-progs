#include<iostream>
#include<map>
#include<list>
#include<string>
#include<fstream>
#include<sstream>
using namespace std;
ifstream fin;
ofstream fout;
class Line;
Line readline();
void expand(Line);
void process(Line);
void define(Line);
class Line
{
   public:
   string label,opcode,operand;
   Line()
   {}
   Line(string l,string oc,string op)
   {
      label=l;
	  opcode=oc;
	  operand=op;
   }
   string tostring()
   {
      return label+" "+opcode+" "+operand+"\n";
   }
}; 
Line readline()
  {
     string l,oc,op;
	 getline(fin,l,' ');
	 getline(fin,oc,' ');
	 getline(fin,op,'\n');
	 return Line(l,oc,op);
  }
  map<string,list<Line> > deftab; 
void define (Line line)
 {
   int level=1;
   deftab[line.label].push_back(line);
   while(level)
     {
	    Line defline;
		defline=readline();
		if(defline.opcode!="MEND")
		   deftab[line.label].push_back(defline);
		else 
           level=0;		
	 }
 }
 void process(Line line)
 {
    if(deftab.find(line.opcode)!=deftab.end())
	   {
	     //cout<<"expanding"; 
		 expand(line);
		}  
    else if(line.opcode=="MACRO")
	      {
		    //cout<<"reading macro";
	       define(line);
		  }
    else
	    {
	      fout<<line.tostring();
		}
 }
 void expand (Line line)
 {
    fout<<"."+line.tostring();
	stringstream from((*(deftab[line.opcode].begin())).operand);
	stringstream to(line.operand);
	map<string,string> replacement;
	string fromstring,tostring;
	while(getline(from,fromstring,',')&&getline(to,tostring,','))
	           replacement[fromstring]=tostring;
    for(list<Line>:: iterator it=++deftab[line.opcode].begin();it!=deftab[line.opcode].end();++it)
	{
	        string towrite=(*it).tostring();
			for(map<string,string>:: iterator iter=replacement.begin();iter!=replacement.end();iter++)
			{
			     int pos=towrite.find(iter->first);
				 while(pos!=-1)
				  {
				      towrite.replace(pos,iter->first.size(),iter->second);
					  pos=towrite.find(iter->first);
				  }
			}fout<<towrite;
	}
 }
 
 int main()
 {
     fin.open("inputm.txt");
	 fout.open("outputm.txt");
	 Line line=Line("","","");
	 line=readline();
	 //cout<<line.label;
	 //cout<<endl<<line.opcode<<endl<<line.operand;
     while(line.opcode!="END")
	 {     
	       line=readline();
	   //   cout<<line.label<<" "<<line.opcode<<" "<<line.operand<<endl;
	      process(line);
     }
	 fin.close();
	 fout.close();
	 return 0;
 }
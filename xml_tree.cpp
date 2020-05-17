#include "xml_tree.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QStack>


XML_Tree::XML_Tree()
{
    Head = NULL;
}

XML_Tree::XML_Tree(Node* Head)
{
    this->Head = Head;
}


XML_Tree::~XML_Tree()
{
}


void XML_Tree::insertChild(Node* child)
{
    //FirstNode
    if (Head == NULL)
    {
        Head = child;
        MostRecent = child;
        Head->Parent = NULL;
        return;
    }
    if (MostRecent->done == false)
    {
        child->Parent = MostRecent;
        MostRecent->Children.push_back(child);
        MostRecent = child;
    }
    else if (MostRecent->done == true)
    {
        MostRecent = MostRecent->Parent;
        insertChild(child);
    }

}


Node * XML_Tree::returnRecentNode()
{
    return MostRecent;
}



void XML_Tree::DoneNode()
{
    MostRecent->done = true;
    MostRecent = MostRecent->Parent;
}


void XML_Tree::FillTree (QString FilePath)
{

    int count =0;
    QFile inFile(FilePath);

    if(! inFile.open(QFile::ReadOnly | QFile::Text))
    {
        //QMessageBox::warning(this,"Error","File Not Opened");
        qDebug() << "INReady is not found";
    }
    QTextStream in(&inFile);
    QString  input;
    while(!in.atEnd())
     {
        input= in.readLine();

        //Opening Tag			// We can take Tag name and Attribute from this line
                if(input[0] == '<' && input[1] != '/' )
                {

                    count++;
                    int index;
                    // Searching for the space to get the tag name only from the line
                    for (int i = 1; i < input.length(); i++)
                    {
                        index = i;
                        if (input[i] == ' ')
                        {
                                //First white space in the tag
                            break;
                        }
                    }
                    QString tag =  input.mid(1,index-1);
                    QString att =  input.mid(index+1,input.length()-1);


                    // check if <tag attr/> "Opening and closing tag shortcut"
                    if (input[input.length() - 2] == '/')
                    {

                        Node* In = new Node(att, tag);
                        insertChild(In);
                        DoneNode();
                    }
                    else
                    {
                        Node* In = new Node(att, tag);
                        insertChild(In);
                    }
                }
                //Closing tag
                else if (input[0] == '<' && input[1] == '/')
                {
                    count--;
                    DoneNode();
                }
                //Data
                else
                {
                    Node* mostRec = returnRecentNode();
                    mostRec->Data = input;
                }
            }
     inFile.close();
    }





 void  XML_Tree::MakeItReady(QString InputFilePath)
 {
     int index1 , index2 , index_Space;
     QStack <QString> my_stack;
     QString value,comp;
     QFile inFile(InputFilePath);
     QFile Out("C:/Users/LEGION/Documents/InRdy.txt");

     if(! inFile.open(QFile::ReadOnly | QFile::Text))
     {
         return ;
     }
     if(!Out.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
     {
          return ;
     }
     QTextStream in(&inFile);
     QTextStream out(&Out);
     QString  input;
     while(!in.atEnd())
      {
         input= in.readLine();
         index1 = 0, index2 = 0;
                 int i;
                 for (i=0; i < input.length(); i++)
                 {

                     if (input[i] == '<')
                     {
                         //Check for comments
                         if (input[i + 1] == '!' || input[i + 1] == '?')
                         {
                             index2 = 0;
                             i = 1;
                             break;
                         }
                         index1 = i;
                         if (index2 + 1 < index1)
                         {
                             QString data = input.mid(index2 +1, index1 - index2 -1);
                             out << data<< endl;
                         }
                     }
                     else if (input[i] == '>')
                     {
                         index2 = i;
                         QString tag_Arr = input.mid(index1, index2 - index1+1);

                         for (int j = 1; j < tag_Arr.length(); j++)
                         {
                             index_Space = j;
                             if (tag_Arr[j] == ' ')
                             {
                                 //First white space in the tag
                                 break;
                             }
                         }

                         QString tag = tag_Arr.mid(1, index_Space-1);

                         //check if opening or closing to check error
                         if (tag[0] != '/') {

                             out <<  tag_Arr << endl;
                             if (tag_Arr[tag_Arr.length() - 2] != '/')
                                 my_stack.push(tag);
                         }
                         else if (tag[0] == '/')
                         {
                             if (my_stack.size() != 0)
                             {
                                 value = my_stack.top();
                                 comp = tag.mid(1);
                                 if (comp != value) //
                                 {
                                     out << "</"+ value + ">\n";


                                     my_stack.pop();
                                 }
                                 else
                                 {
                                     out << "</" + value + ">\n" ;
                                     my_stack.pop();
                                 }
                             }

                         }

                     }

                 }
                 if (index2 != i-1)
                 {
                     QString data = input.mid(index2+1);
                     out << data << endl;
                     value = my_stack.top();
                     out << "</" + value + ">\n" ;
                     my_stack.pop();
                 }
     }
     inFile.close();
     Out.flush();
     Out.close();
 }





QString space(int& t)
{
      QString tabe;
      for(int i=0; i<t; i++)
      {
          tabe+="\t";
      }
      return tabe;
}

int counter=0;
void formatting_xml(Node *N, QString &out,int counter)
{

    QString TN="";


    //print opening tag
    if (N->Attribute == "" && N->Data == "")
    {
        TN =  "<" + N->TagName + ">";
        out += TN + "\n";
    }

    else if (N->Attribute != "" && N->Data == "")
    {

        TN =  "<" + N->TagName + " " + N->Attribute + "";
        out +=  TN + "\n";

    }

    else if (N->Attribute == "" && N->Data != "")
    {
        TN =  "<" + N->TagName + ">";
        out +=  TN + "\n";
        counter++;
        out  += space(counter) + N->Data + "\n";
        counter--;


        /*TN = "<" + N->TagName + ">" + N->Data;
        string CTN= "</" + N->TagName + ">";
        cout << TN << CTN <<endl;*/
    }

    else
    {
        TN = "<" + N->TagName + " "+ N->Attribute + "";
        out +=  TN + "\n";
        counter++;
        out += space(counter) + N->Data + "\n";
        counter--;
    }



    for (int i = 0; i<N->Children.size(); i++)
    {

        if (N->Children[i] != NULL)
        {
            counter++;
           out +=space(counter);
            formatting_xml(N->Children[i],out,counter);
        }

        counter--;
    }


//closing tab

//if(*(N->Attribute.rbegin()) != '/' && (N->Data == ""))
if(*(N->Attribute.rbegin()) != '/')
{

    out +=space(counter);
    TN = "</" + N->TagName + ">";
    out += TN + "\n";
}
}




#include "xml_tree.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QStack>
#include <QDir>

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
                        attSeperator(In);
                        insertChild(In);
                        DoneNode();
                    }
                    else
                    {
                        Node* In = new Node(att, tag);
                        attSeperator(In);
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
     QFile Out(QDir::currentPath() +"/InRdy.txt");
      qDebug() << QDir::currentPath();
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
                             bool flag = false;
                             foreach(QChar str, data)
                             {
                                 if(str != ' ')
                                     flag = true;
                             }
                             if(flag == true)
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

        out  += space(counter) + N->Data + "\n";



        /*TN = "<" + N->TagName + ">" + N->Data;
        string CTN= "</" + N->TagName + ">";
        cout << TN << CTN <<endl;*/
    }

    else
    {
        TN = "<" + N->TagName + " "+ N->Attribute + "";
        out +=  TN + "\n";

        out += space(counter) + N->Data + "\n";

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


void XML_Tree::word_info(Node* head, QString input_1 , QString &out, QString &num) {
    QString att;
    if (head->Children.size() != 0) {
        num += QString::number(head->Children.size()) + "\n";
        for (int i = 0; i <head->Children.size(); i++) {
            if (head->Children[i]->TagName == "synset") {
                for (int k = 0; k <head->Children[i]->Children.size(); k++) {
                    if (head->Children[i]->Children[k]->TagName == "word") {
                        if (head->Children[i]->Children[k]->Data == input_1) {
                            Node* search = head->Children[i]->Children[k]->Parent;
                            for (int j = 0; j<search->Children.size(); j++) {
                                if (search->Children[j]->TagName == "def") {
                                    out += search->Children[j]->Data + "\n";
                                }
                            }
                        }
                    }
                }
            }
            if (head->Children.size() == 0) {
                return;
            }
        }
    }
}



void XML_Tree::word_id(Node* head, QString input_1 , QString &out) {
QVector<QString>s(0);
    QString att;
    if (head->Children.size() != 0) {
        for (int i = 0;i < head->Children.size();i++) {
            if (head->Children[i]->TagName == "synset") {
                for (int j=0;j<head->Children[i]->Children.size();j++) {
                    if (head->Children[i]->Children[j]->TagName == "word") {
                        if (head->Children[i]->Children[j]->Data == input_1){
                            Node* search = head->Children[i]->Children[j]->Parent;
                            for (int k= 0;k< search->Children.size();k++) {
                                if (search->Children[k]->Data == "Hypernym")
                                {
                                    att = search->Children[k]->Attribute;
                                    while (att.indexOf("n") != -1) {
                                        int x = att.indexOf("n");
                                        s.push_back(att.mid(x, 9));
                                        att.remove(x, 9);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        for (int k = 0;k<s.size();k++) {
            for (int i = 0;i < head->Children.size();i++) {
                if ((head->Children[i]->TagName == "synset") &&
                    (head->Children[i]->Attribute.indexOf(s[k])) != -1) {
                    for (int j = 0; j <head->Children[i]->Children.size();j++){
                        if (head->Children[i]->Children[j]->TagName == "word") {
                            out  += head->Children[i]->Children[j]->Data + "\n";
                        }
                }
              }
            }
        }
    }
}



void XML_Tree::XMLtoJSON(Node * node, int &lvl , QString &outfile)
{



    if (node->Parent == NULL) {
        outfile += "{\n";
    }

    if (node->Children.size() != 0) {
        node->sorted = node->Children;
        sorting(node->sorted);
        for (int i = 0; i < node->Children.size(); i++) {
            for (int j = 0; j < node->Children.size(); j++) {
                if (node->Children[i]->TagName == node->sorted[j]->TagName)
                {
                    node->Children[i]->repeated++;

                    //note : law repeated marten fa hwa mawgod 3 mrat

                    if (node->Children[i]->repeated == 1) {
                        node->Children[i]->starting_pos = j;
                    }
                }
            }
        }

        if (node->repeated > 1 && node->printed == false) {
            int fasttemp = node->starting_pos;
            for (int i = 0; i < node->repeated; i++) {
                node->Parent->sorted[fasttemp]->printed = true;
                fasttemp++;
            }
            //spaces
            lvl++;
            for (int y = 0; y < lvl; y++) {
                outfile += "\t";
            }
            outfile += "\"" + node->TagName + "\": [" + "\n";
            for (int i = 0; i < node->repeated; i++) {
                for (int y = 0; y < lvl; y++) {
                    outfile += "\t";
                }
                outfile += "{\n";
                if (i != 0) {
                    node->Parent->sorted[node->starting_pos]->sorted = node->Parent->sorted[node->starting_pos]->Children;
                    sorting(node->Parent->sorted[node->starting_pos]->sorted);
                    for (int h = 0; h < node->Parent->sorted[node->starting_pos]->Children.size(); h++) {
                        for (int j = 0; j < node->Parent->sorted[node->starting_pos]->Children.size(); j++) {
                            if (node->Parent->sorted[node->starting_pos]->Children[h]->TagName == node->Parent->sorted[node->starting_pos]->sorted[j]->TagName)
                            {
                                node->Parent->sorted[node->starting_pos]->Children[h]->repeated++;

                                //note : law repeated marten fa hwa mawgod 3 mrat

                                if (node->Parent->sorted[node->starting_pos]->Children[h]->repeated == 1) {
                                    node->Parent->sorted[node->starting_pos]->Children[h]->starting_pos = j;
                                }
                            }
                        }
                    }
                }
                for (int j = 0; j < node->Parent->sorted[node->starting_pos]->Children.size(); j++) {
                    XMLtoJSON(node->Parent->sorted[node->starting_pos]->Children[j], lvl,outfile);

                }
                if (node->Parent->sorted[node->starting_pos]->AttributeName.size() != 0) {
                    for (int z = 0; z < node->Parent->sorted[node->starting_pos]->AttributeName.size(); z++) {
                        if (z + 1 == node->Parent->sorted[node->starting_pos]->AttributeName.size() && node->Parent->sorted[node->starting_pos]->Data == "") {
                            for (int y = 0; y < lvl; y++) {
                                outfile += "\t";
                            }
                            outfile += "\"@" + node->Parent->sorted[node->starting_pos]->AttributeName[z] + "\": \"" + node->Parent->sorted[node->starting_pos]->AttributeVal[z] + "\"" + "\n";
                        }
                        else {
                            for (int y = 0; y < lvl; y++) {
                                outfile += "\t";
                            }
                            outfile += "\"@" + node->Parent->sorted[node->starting_pos]->AttributeName[z] + "\": \"" + node->Parent->sorted[node->starting_pos]->AttributeVal[z] + "\"," + "\n";
                        }
                    }
                    //check comma
                }
                //ghyrt de mn node ll bta3a eltwela de
                if (node->Parent->sorted[node->starting_pos]->Data != "") {
                    for (int y = 0; y < lvl; y++) {
                        outfile += "\t";
                    }
                    outfile += "\"#text\": \""  + node->Parent->sorted[node->starting_pos]->Data + "\"" + "\n";
                    //comma check
                }
                if (i + 1 == node->repeated) {
                    for (int y = 0; y < lvl; y++) {
                        outfile += "\t";
                    }
                    outfile += "}\n";
                }
                else {
                    for (int y = 0; y < lvl; y++) {
                        outfile += "\t";
                    }
                    outfile += "},\n";
                }
                node->starting_pos = node->starting_pos + 1;
            }

            if (node == node->Parent->Children[node->Parent->Children.size() - 1]) {
                for (int y = 0; y < lvl; y++) {
                    outfile += "\t";
                }
                outfile += "]\n" ;

            }
            else {
                for (int y = 0; y < lvl; y++) {
                    outfile += "\t";
                }
                outfile += "],\n";

            }
            //comma check
            lvl--;
        }
        else if (node->repeated == 1 || node->repeated == 0) {
            lvl++;
            for (int y = 0; y < lvl; y++) {
                outfile += "\t";
            }
            outfile += "\"" + node->TagName + "\": {" + "\n";
            for (int i = 0; i < node->Children.size(); i++) {
                XMLtoJSON(node->Children[i], lvl,outfile);
            }
            if (node->Data != "" && node->AttributeName.size() == 0) {
                for (int y = 0; y < lvl; y++) {
                    outfile += "\t";
                }
                outfile += "\"#text\": \"" + node->Data + "\"" + "\n";

            }
            else if (node->AttributeName.size() != 0) {

                for (int i = 0; i < node->AttributeName.size(); i++) {

                    if (i + 1 == node->AttributeName.size() && node->Data == "") {
                        for (int y = 0; y < lvl; y++) {
                            outfile += "\t";
                        }
                        outfile += "\"@" + node->AttributeName[i] + "\": \"" + node->AttributeVal[i] + "\"" + "\n";
                    }
                    else {
                        for (int y = 0; y < lvl; y++) {
                            outfile += "\t";
                        }
                        outfile += "\"@" + node->AttributeName[i] + "\": \"" + node->AttributeVal[i] + "\"," + "\n";
                    }

                }

                if (node->Data != "") {
                    for (int y = 0; y < lvl; y++) {
                        outfile += "\t";
                    }
                    outfile += "\"#text\": \"" + node->Data + "\"" + "\n";

                }

            }


            if (node->Parent == NULL) {
                for (int y = 0; y < lvl; y++) {
                    outfile += "\t";
                }
                outfile += "}\n";

            }
            else if (node == node->Parent->Children[node->Parent->Children.size() - 1]) {
                for (int y = 0; y < lvl; y++) {
                    outfile += "\t";
                }
                outfile += "}\n";
            }
            else {
                for (int y = 0; y < lvl; y++) {
                    outfile += "\t";
                }
                outfile += "},\n";

            }
            //comma check
            lvl--;
        }

    }

    else if (node->Children.size() == 0) {

        if (node->repeated > 1 && node->printed == false) {
            lvl++;
            for (int y = 0; y < lvl; y++) {
                outfile += "\t";
            }
            outfile += "\"" + node->TagName + "\": [" + "\n";
            for (int i = 0; i < node->repeated; i++) {

                if (node->Parent->sorted[node->starting_pos]->AttributeName.size() != 0) {
                    for (int y = 0; y < lvl; y++) {
                        outfile += "\t";
                    }
                    outfile += "{\n";
                    for (int z = 0; z < node->Parent->sorted[node->starting_pos]->AttributeName.size(); z++) {
                        if (z + 1 == node->Parent->sorted[node->starting_pos]->AttributeName.size() && node->Parent->sorted[node->starting_pos]->Data == "") {
                            for (int y = 0; y < lvl; y++) {
                                outfile += "\t";
                            }
                            outfile += "\"@" + node->Parent->sorted[node->starting_pos]->AttributeName[z] + "\": \"" + node->Parent->sorted[node->starting_pos]->AttributeVal[z] + "\"" + "\n";
                        }
                        else {
                            for (int y = 0; y < lvl; y++) {
                                outfile += "\t";
                            }
                            outfile += "\"@" + node->Parent->sorted[node->starting_pos]->AttributeName[z] + "\": \"" + node->Parent->sorted[node->starting_pos]->AttributeVal[z] + "\"," + "\n";
                        }


                    }
                    if (node->Parent->sorted[node->starting_pos]->Data != "") {
                        for (int y = 0; y < lvl; y++) {
                            outfile += "\t";
                        }
                        outfile += "\"#text\": \""  + node->Parent->sorted[node->starting_pos]->Data + "\"" + "\n";
                        //comma check
                    }

                    if (i + 1 == node->repeated) {
                        for (int y = 0; y < lvl; y++) {
                            outfile += "\t";
                        }
                        outfile += "}\n";
                    }
                    else {
                        for (int y = 0; y < lvl; y++) {
                            outfile += "\t";
                        }
                        outfile += "},\n";
                    }

                    //check comma
                }

                else if (node->Parent->sorted[node->starting_pos]->AttributeName.size() == 0 && node->Parent->sorted[node->starting_pos]->Data != "") {
                    if (i + 1 == node->repeated) {
                        for (int y = 0; y < lvl; y++) {
                            outfile += "\t";
                        }
                        outfile += "\"" + node->Parent->sorted[node->starting_pos]->Data + "\"" + "\n";
                    }
                    else {
                        for (int y = 0; y < lvl; y++) {
                            outfile += "\t";
                        }
                        outfile += "\"" + node->Parent->sorted[node->starting_pos]->Data + "\"," + "\n";

                    }
                }
                else {

                    if (i + 1 == node->repeated) {
                        for (int y = 0; y < lvl; y++) {
                            outfile += "\t";
                        }
                        outfile += "\"";
                         outfile += "\"\n";

                    }
                    else {
                        for (int y = 0; y < lvl; y++) {
                            outfile += "\t";
                        }
                        outfile += "\"" ;
                        outfile +=  "\",\n";
                    }
                }

                node->Parent->sorted[node->starting_pos]->printed = true;

                node->starting_pos++;
            }


            if (node == node->Parent->Children[node->Parent->Children.size() - 1]) {
                for (int y = 0; y < lvl; y++) {
                    outfile += "\t";
                }
                outfile += "]\n" ;

            }
            else {
                for (int y = 0; y < lvl; y++) {
                    outfile += "\t";
                }
                outfile += "],\n" ;

            }
            //comma check
            lvl--;
        }

        else if (node->repeated == 1 || node->repeated == 0) {

            lvl++;

            if (node->Data != "" && node->AttributeName.size() == 0) {

                if (node == node->Parent->Children[node->Parent->Children.size() - 1] && node->Parent->Data == "" && node->Parent->AttributeName.size() == 0) {
                    for (int y = 0; y < lvl; y++) {
                        outfile += "\t";
                    }
                    outfile += "\"" + node->TagName + "\": \"" + node->Data + "\"" + "\n";
                }
                else {
                    for (int y = 0; y < lvl; y++) {
                        outfile += "\t";
                    }
                    outfile += "\"" + node->TagName + "\": \"" + node->Data + "\"," + "\n";
                }

            }
            else if (node->AttributeName.size() != 0) {
                for (int y = 0; y < lvl; y++) {
                    outfile += "\t";
                }
                outfile += "\"" + node->TagName + "\": {" + "\n";


                for (int i = 0; i < node->AttributeName.size(); i++) {

                    if (i + 1 == node->AttributeName.size() && node->Data == "") {
                        for (int y = 0; y < lvl; y++) {
                            outfile += "\t";
                        }
                        outfile += "\"@" + node->AttributeName[i] + "\": \"" + node->AttributeVal[i] + "\"" + "\n";
                    }
                    else {
                        for (int y = 0; y < lvl; y++) {
                            outfile += "\t";
                        }
                        outfile += "\"@" + node->AttributeName[i] + "\": \"" + node->AttributeVal[i] + "\"," + "\n";
                    }

                }

                if (node->Data != "") {
                    for (int y = 0; y < lvl; y++) {
                        outfile += "\t";
                    }
                    outfile += "\"#text\": \"" + node->Data + "\"" + "\n";

                }

                if (node == node->Parent->Children[node->Parent->Children.size() - 1] && node->Parent->Data == "" && node->Parent->AttributeName.size() == 0) {
                    for (int y = 0; y < lvl; y++) {
                        outfile += "\t";
                    }
                    outfile += "}\n";

                }
                else {
                    for (int y = 0; y < lvl; y++) {
                        outfile += "\t";
                    }
                    outfile += "},\n";

                }

            }
            else {
                for (int y = 0; y < lvl; y++) {
                    outfile += "\t";
                }
                outfile += "\"" + node->TagName + "\": " + "\"" + "\"" + "\n";
                if (node == node->Parent->Children[node->Parent->Children.size() - 1]) {
                    for (int y = 0; y < lvl; y++) {
                        outfile += "\t";
                    }
                    outfile += "}\n" ;

                }
                else {
                    for (int y = 0; y < lvl; y++) {
                        outfile += "\t";
                    }
                    outfile += "},\n" ;

                }
            }
            lvl--;
        }

    }

    if (node->Parent == NULL) {
        outfile += "}\n" ;
    }


}



void XML_Tree::commaChecker(Node * node)
{
    if (node->Children.size() != 0) {


    }
    else {
        if (node->TagName == node->Parent->Children[node->Parent->Children.size() - 1]->TagName || node->Parent == NULL) {
            //outfile << "}" << endl;
        }
        else {
            //outfile << "}," << endl;
        }
    }

}


void XML_Tree::attSeperator(Node * In)
{
    if (In->Attribute.length() > 0) {
        int starting = 0;
        bool checkForQuot = false;

        for (int i = 0; i < In->Attribute.length(); i++) {
            if (In->Attribute[i] == '=') {
                In->AttributeName.push_back(In->Attribute.mid(starting, i - starting));

            }
            else if (In->Attribute[i] == '"' && checkForQuot == false) {
                checkForQuot = true;
                starting = i + 1;
            }
            else if (In->Attribute[i] == '"' && checkForQuot == true) {
                In->AttributeVal.push_back(In->Attribute.mid(starting, i - starting));
                checkForQuot = false;
            }
            else if (In->Attribute[i] == ' ' && checkForQuot == false) {
                starting = i + 1;
            }
        }

    }
}

void XML_Tree::sorting(QVector<Node*> &vec)
{

    int i, j, min;
    Node* temp;
        for (i = 0; i < vec.size() ; i++) {
            min = i;
            for (j = i + 1; j < vec.size(); j++)
                if (vec[j]->TagName < vec[min]->TagName)
                    min = j;
            temp = vec[i];
            vec[i] = vec[min];
            vec[min] = temp;
        }

}

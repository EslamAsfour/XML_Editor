#ifndef XML_TREE_H
#define XML_TREE_H
#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>


struct Node
{
    QString Data;
    QString Attribute;
    QVector<QString>AttributeName;
    QVector<QString>AttributeVal;
    QString TagName;
    QVector<Node*>Children;
    QVector<Node*>sorted;
    int repeated = 0;
    int starting_pos = 0;
    bool printed = false;
    Node* Parent;
    bool done;
    //Constructor
    Node( QString Attr , QString Name , QString DataIn = "")
    {
        Data = DataIn;
        Attribute = Attr;
        TagName = Name;
        done = false;
        Parent = NULL;
    }
};

void formatting_xml(Node *N, QString &out,int counter);
QString space(int& t);

class XML_Tree
{
private:
    Node* Head;
    Node* MostRecent;

public:
    XML_Tree();
    XML_Tree(Node* Head);
    ~XML_Tree();
    Node* GetHead() { return Head; }
    void insertChild(Node *child);
    Node* returnRecentNode();
    void DoneNode();
    void FillTree (QString FilePath);
   void MakeItReady(QString InputFilePath);

   //Rasha
   void word_info(Node* head, QString input_1,QString &out, QString &num);
   void word_id(Node* head, QString input_1,QString &out);



   // Ahmed
   void attSeperator(Node* In);
   void sorting(QVector <Node*> &vec);
   void XMLtoJSON(Node * node, int &lvl , QString &outfile);
   void commaChecker(Node* node);
};

#endif // XML_TREE_H

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
    QString TagName;
    QVector<Node*> Children;
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
};

#endif // XML_TREE_H

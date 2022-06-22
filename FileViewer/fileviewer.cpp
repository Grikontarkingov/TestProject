#include "fileviewer.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QApplication>

FileViewer::FileViewer(QWidget *parent)
    : QWidget(parent),
      gridLay(new QGridLayout(this)),
      modelTree(nullptr),
      modelList(nullptr),
      currentPathText(new QTextEdit(this)),
      backPath(new QPushButton(this)),
      treePath(new QTreeView(this)),
      listPath(new QListView(this))
{
    backPath->setMaximumHeight(30);
    backPath->setMaximumWidth(25);
    backPath->setText("\u2191");
    gridLay->addWidget(backPath, 0, 0, 1, 1);
    connect(backPath, &QPushButton::clicked, this, &FileViewer::upLevel);

    currentPathText->setMaximumHeight(23);
    gridLay->addWidget(currentPathText, 0, 1, 1, 10);
    currentPathText->setText("This Computer\\");
    currentPathText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    gridLay->addWidget(treePath, 1, 0, 10, 2);
    treePath->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    gridLay->addWidget(listPath, 1, 2, 10, 10);
    listPath->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(listPath, &QListView::doubleClicked, this, &FileViewer::openResoursec);

    this->setLayout(gridLay);
    this->setMinimumSize(800, 600);

    rebuildModelList(currentPathText->toPlainText());
    rebuildModelTree();
}

FileViewer::~FileViewer()
{
    if(modelTree) delete modelTree;
    if(modelList) delete modelList;
    if(currentPathText) delete currentPathText;
    if(backPath) delete backPath;
    if(treePath) delete treePath;
    if(listPath) delete listPath;
}

void FileViewer::setNewModelTree(QStandardItemModel* newmodel)
{
    treePath->setModel(newmodel);
    modelTree = newmodel;
}

void FileViewer::setNewModelList(QStandardItemModel* newmodel)
{
    listPath->setModel(newmodel);
    modelList = newmodel;
}

void FileViewer::rebuildModelList(QString str)
{
    if("This Computer\\" == str )
    {
        QStandardItemModel* model = new QStandardItemModel(this);

        QFileInfoList list = QDir::drives();
        QFileInfoList::const_iterator listDisk = list.begin();
        int amount = list.count();

        for(int i = 0; i < amount; ++i)
        {
            model->appendRow(new QStandardItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_DriveHDIcon)), listDisk->path()));
            listDisk++;
        }
        setNewModelList(model);
    }
    else
    {
        QStandardItemModel* model = new QStandardItemModel(this);

        QDir dir(str);
        dir.setFilter(QDir::Hidden | QDir::NoSymLinks | QDir::Dirs);

        QStringList list = dir.entryList();
        int amount = list.count();

        for(int i = 0; i < amount; ++i)
        {
            if("." == list.at(i) || ".." == list.at(i))
            {
                continue;
            }

            QStandardItem* f = new QStandardItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_DirIcon)), list.at(i));
            model->appendRow(f);
        }

        dir.setFilter(QDir::Hidden | QDir::NoSymLinks | QDir::Files);
        amount = list.count();

        for(int i = 0; i < amount; ++i)
        {
            if("." == list.at(i) || ".." == list.at(i))
            {
                continue;
            }

            QStandardItem* f = new QStandardItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon)), list.at(i));
            model->appendRow(f);
        }

        setNewModelList(model);
    }
}

void FileViewer::openResoursec(const QModelIndex &index)
{
    QDir dir = currentPathText->toPlainText();
    if(dir.cd(index.data().toString()))
    {
        currentPathText->setText(dir.path());
        rebuildModelList(currentPathText->toPlainText());
    }
}

void FileViewer::upLevel()
{
    QString out = currentPathText->toPlainText();

    while(true)
    {
        if(out.isEmpty())
        {
            currentPathText->setText("This Computer\\");
            rebuildModelList(currentPathText->toPlainText());
            return;
        }
        else if('/' == out.at(out.size() - 1))
        {
            out.chop(1);
            currentPathText->setText(out);
            rebuildModelList(currentPathText->toPlainText());
            return;
        }
        else
        {
            out.chop(1);
        }
    }
}

void FileViewer::rebuildModelTree()
{
    QStandardItemModel *model = new QStandardItemModel(this);

    QFileInfoList list = QDir::drives();
    QFileInfoList::const_iterator listDisk = list.begin();
    int amountDisk = list.count();

    for(int i = 0; i < amountDisk; ++i)
    {
        QList<QStandardItem*> items;
        items.append(new QStandardItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_DriveHDIcon)), listDisk->path()));
        model->appendRow(items);

        items.at(0)->appendRows(getAllDirs(listDisk->path()));

        ++listDisk;
    }


    setNewModelTree(model);
}

QList<QStandardItem*> FileViewer::getAllDirs(QString dir)
{
    QDir directory(dir);
    directory.setFilter(QDir::Hidden | QDir::NoSymLinks | QDir::Dirs | QDir::NoDotAndDotDot);

    QStringList list = directory.entryList();
    int amount = list.count();

    QList<QStandardItem*>folders;
    for(int i = 0; i < amount; i++)
    {
        if(haveDirectory(dir + list.at(i)))
        {
            folders.append(new QStandardItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_DirIcon)), list.at(i)));
            folders.at(i)->appendRows(getAllDirs(dir + list.at(i) + "/"));
        }
        else
        {
            folders.append(new QStandardItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_DirIcon)), list.at(i)));
        }
    }

    return folders;
}

bool FileViewer::haveDirectory(QString in)
{
    QDir directory(in);
    directory.setFilter(QDir::Hidden | QDir::NoSymLinks | QDir::Dirs | QDir::NoDotAndDotDot);

    if(directory.entryList().isEmpty())
    {
        return false;
    }
    else
    {
        return true;
    }
}

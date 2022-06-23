#ifndef FILEVIEWER_H
#define FILEVIEWER_H

#include <QWidget>
#include <QGridLayout>
#include <QStandardItemModel>
#include <QTextEdit>
#include <QPushButton>
#include <QTreeView>
#include <QListView>
#include <QDir>

class FileViewer : public QWidget
{
    Q_OBJECT
public:
    explicit FileViewer(QWidget *parent = nullptr);
    ~FileViewer();

private slots:
    void openResourses(const QModelIndex &index);
    void upLevel();
    void openDirectoryFromNavigation(const QModelIndex &index);

private:
    void rebuildModelList(QString str);
    void rebuildModelTree();
    QList<QStandardItem*> getAllDirs(QString dir);
    bool haveDirectory(QString in);
    void setNewModelTree(QStandardItemModel* newmodel);
    void setNewModelList(QStandardItemModel* newmodel);
    QDir getPath(const QModelIndex &index);

    QGridLayout* gridLay;
    QStandardItemModel *modelTree;
    QStandardItemModel *modelList;
    QTextEdit* currentPathText;
    QPushButton* backPath;
    QTreeView* treePath;
    QListView* listPath;
};

#endif // FILEVIEWER_H

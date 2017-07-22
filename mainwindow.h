#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
 #include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QTextEdit>
#include <QFile>
#include <QLineEdit>
#include <QLabel>
 #include <QTime>
#include<QTimer>

#define MaxRecentFiles 5

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);  //每个头文件，都有这行，是什么意思？

signals:
    
public slots:

private slots:
    bool New();
    int Open();
    int Save();
    bool SaveAs();
    bool exit();
    void documentWasModified();
    void UpdateTime();
    void OpenRecentFile();

private:
     QMenu *pFileMenu ;  //禁止在定义时，初始化指针的值为0，编译不通过；
     QMenu *pEditMenu;
     QMenu *pToolsMenu;
     QMenu *pOptionsMenu;
     QMenu *pHelpMenu;

     QToolBar *pToolBar;

     QAction *pNewAction;
     QAction *pOpenAction;
     QAction *pSaveAction;
     QAction *pSaveAsAction;
     QAction *pRecentFileAction[MaxRecentFiles];
     QAction *pExitAction;


     //QACtion *pCutAction;

      int CreatMenu();
      int CreatToolbar();
      int CreatStatusbar();
      int CreatActions();

      bool saveFile(const QString &filename);
      void ShowName(const QString &filename);
      bool maySave();
      void setCurrentFile(const QString &filename);
      void UpdateRecentFile(const QString &filename);
      void ReadSetting();
      void SaveSetting();

      QDockWidget *dockWidget;

      //test，增加文本框用于显示打开的文件。

      int i;
      QTextEdit *pTextEdit;
      QString fileName;
      QString curFile;
      QLineEdit *pLineEdit;
      QDateTime *pDateTime;
      QLabel *pPathLabel;
      QLabel *pTimeLabel;
      QTime m_time;
      QTimer *ptimerobj;
      QStringList RecentFiles;
};

#endif // MAINWINDOW_H

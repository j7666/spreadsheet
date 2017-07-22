#include "mainwindow.h"
#include <QDebug>
#include <QDockWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QSettings>

//构造函数
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

//疑问：在头文件中定义的指针，需要在*.c文件中进行初始化吗,哪些需要初始化。
//是否所有的指针都按流程在构造函数中首先定义为NULL？？
//例如QACtion类对象的指针等。

    pLineEdit = NULL;
    pFileMenu = NULL;
    pEditMenu = NULL;
    pToolsMenu = NULL;
    pOptionsMenu = NULL;
    pHelpMenu = NULL;
    pToolBar = NULL;
    pNewAction = NULL;
    pOpenAction = NULL;
    pSaveAction = NULL;
    pSaveAsAction = NULL;
    pExitAction = NULL;
    dockWidget = NULL;
     pTextEdit = NULL;
     pLineEdit = NULL;
     pDateTime = NULL;
     pPathLabel = NULL;
     pTimeLabel = NULL;
     ptimerobj = NULL;

     pTextEdit = new QTextEdit("",this);
     this->setCentralWidget(pTextEdit);


 if(CreatMenu())          //创建菜单
    {
        qDebug()<<"creat Menu function OK!!!!"<<endl;
    }
    else qDebug()<<"creat Menu function failure!!!!"<<endl;

    if(CreatActions())           //创建Actions
    {
        qDebug()<<"creat Actions function OK!!!!"<<endl;
    }
    else qDebug()<<"creat Actions function failure!!!!"<<endl;

    if(CreatToolbar())         //创建工具栏
    {
        qDebug()<<"creat ToolBar function OK!!!!"<<endl;
    }
    else qDebug()<<"creat ToolBar function failure!!!!"<<endl;

    if(CreatStatusbar())        //创建状态栏
    {
        qDebug()<<"creat StatusBar function OK!!!!"<<endl;
    }
    else qDebug()<<"creat StatusBar function failure!!!!"<<endl;



    //设置初始大小为800*600
    this->resize(800,600);

    //文本框中有内容修改，就调用 setWindowModified(true)
    connect(pTextEdit->document(), SIGNAL(contentsChanged()),
              this, SLOT(documentWasModified()));
    setCurrentFile("");  //刚建立pTextEdit，把其fileName设置为empty。

    //用于显示时间【可以仔细研究下】
    ptimerobj = new QTimer(this);  //计时器
    connect(ptimerobj,SIGNAL(timeout()),this,SLOT(UpdateTime()));   //时间到了调用UpdateTime()
    ptimerobj->start(100);  //计时,【单位是什么】

}

//创建菜单栏done
int MainWindow::CreatMenu()
{
     //QMenu类的用法，可以参照帮助文档。
    pFileMenu = new QMenu(tr("&File"),this);
    pEditMenu = new QMenu(tr("&Edit"),this);
    pToolsMenu = new QMenu(tr("&Tools"),this);
    pOptionsMenu = new QMenu(tr("&Options"),this);
    pHelpMenu = new QMenu(tr("&Help"),this);

    //memuBar()第一次调用，之前不存在会建立。
    menuBar()->addMenu(pFileMenu);
    menuBar()->addMenu(pEditMenu);
    menuBar()->addMenu(pToolsMenu);
    menuBar()->addMenu(pOptionsMenu);
    menuBar()->addMenu(pHelpMenu);
    return true;     //判断此函数是否执行成功。
}

//创建工具栏done
int MainWindow::CreatToolbar()
{
    pToolBar = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea,pToolBar);
    pToolBar->addAction(pNewAction);
    pToolBar->addAction(pOpenAction);
    pToolBar->addAction(pSaveAction);
    pToolBar->addAction(pExitAction);
    return true;

    //如果按照每个菜单栏返回的QAction来进行设置的话，就要设置多个。
    //可以使用已经返回的QAction值。
    //当然也可以单独定义一个QAction，大家都可以调用。
}

//创建状态栏done
int MainWindow::CreatStatusbar()
{

    statusBar()->showMessage("status created");
    statusBar()->setStatusTip(tr("status tips"));

    //显示时间
    pTimeLabel = new QLabel(this);
    statusBar()->addPermanentWidget(pTimeLabel,0);
    pTimeLabel->setVisible(true);

    //显示路径
    pPathLabel = new QLabel(this);
    statusBar()->addPermanentWidget(pPathLabel,0);
    pPathLabel->setText("TextNum:");
    pPathLabel->setVisible(true);

    //显示编辑框，用于显示文件名称
    pLineEdit = new QLineEdit(this);
    statusBar()->addPermanentWidget(pLineEdit,0);
    pLineEdit->setFixedWidth(100);
    pLineEdit->setVisible(true);
    pLineEdit->setDisabled(true);

    return true;
}

//创建菜单项,done tmep
int MainWindow::CreatActions()
{

    //add New
    pNewAction = pFileMenu->addAction(QIcon(":/images/new.ico"),tr("New"));
    //上面的Icon文件的名称，要和实际的名称大小写一致。
    pNewAction->setShortcuts(QKeySequence::New);
    pNewAction->setStatusTip(tr("New status tips"));
    pNewAction->setWhatsThis(tr("new's WhatsTip"));
    pNewAction->connect(pNewAction,SIGNAL(triggered()),this,SLOT(New()));

    //add Open
    pOpenAction = pFileMenu->addAction(QIcon(":/images/open.ico"),tr("Open"));
    pOpenAction->setShortcuts(QKeySequence::Open);
    pOpenAction->setStatusTip(tr("Open status tips"));
    pOpenAction->setWhatsThis(tr("Open's WhatsTip"));
    pOpenAction->connect(pOpenAction,SIGNAL(triggered()),this,SLOT(Open()));

    //add Save
    pSaveAction = pFileMenu->addAction(QIcon(":/images/save.ico"),tr("Save"));
    pSaveAction->setShortcuts(QKeySequence::Save);
    pSaveAction->setStatusTip(tr("Save status tips"));
    pSaveAction->setWhatsThis(tr("Save's WhatsTip"));
    pSaveAction->connect(pSaveAction,SIGNAL(triggered()),this,SLOT(Save()));

    //add SaveAs
    pSaveAsAction = pFileMenu->addAction(QIcon(":/images/SaveAs.ico"),tr("SaveAs"));
    pSaveAsAction->setShortcuts(QKeySequence::SaveAs);
    pSaveAsAction->setStatusTip(tr("SaveAs status tips"));
    pSaveAsAction->setWhatsThis(tr("SaveAs's WhatsTip"));
    pSaveAsAction->connect(pSaveAsAction,SIGNAL(triggered()),this,SLOT(SaveAs()));

    //add seperator
     pFileMenu->addSeparator();

    //add RecentFile
     for(int j=0;j<MaxRecentFiles;j++)
     {

         pRecentFileAction[j]=new QAction(this);
         pRecentFileAction[j]->setVisible(false);
         pRecentFileAction[j]->setText("");
         pFileMenu->addAction(pRecentFileAction[j]);
         //QString tmp = pRecentFileAction[j]->data().toString();
         pRecentFileAction[j]->connect(pRecentFileAction[j],SIGNAL(triggered()),
                                       this,SLOT( OpenRecentFile())
                                       );
         //qDebug()<<pRecentFileAction[j]->text();

         //疑问：为什么不能直接使用connect，关联pRecentAction[j]的值。判断选择的是哪个最近打开的文件。
         //二是需要使用sender()函数来判断是哪个Action的。

     };


     //add seperator
      pFileMenu->addSeparator();

    //add Exit
    pExitAction = pFileMenu->addAction(QIcon(":/images/exit.ico"),tr("Exit"));
//    pExitAction->setShortcuts(QKeySequence:);
    pExitAction->setStatusTip(tr("Exit status tips"));
    pExitAction->setWhatsThis(tr("Exit's WhatsTip"));
    pExitAction->connect(pExitAction,SIGNAL(triggered()),this,SLOT(exit()));

    return true;


}

//新建一个文件
bool MainWindow::New()

{
    //判断是否当前文件有没有保存。
    //情况一：当前文件没有内容。为空的。这种情况可以直接新建。
    //情况二：当前文件未保存过，无名称，有内容。这种情况，内容非空，需要弹窗由用户判断。
    //情况三：当前文件有保存路径及名称，有内容，内容可能修改。。内容有修改需要保存，否则不需要。
    //用户判断决定后，做新建工作。即清空原有内容，设置文件名称为空。
    if(maySave())          //如果为真，即已经保存或放弃。
    {
        pTextEdit->clear();
        setCurrentFile("");  //新建立的文件的文件名为empty。
    }
    //放弃，直接返回什么都不做。
    return true;
}



//打开文件函数
    int MainWindow::Open()
{

     fileName = QFileDialog::getOpenFileName(this,tr("Open text!"),"/C/",tr("text files(*.txt)"));


    //下面为读入数据
     if(!fileName.isEmpty())
     {
         QFile *file=new QFile;
         file->setFileName(fileName);
         bool ok=file->open(QIODevice::ReadWrite | QIODevice::Text);
         if(ok)
         {
            QTextStream in(file);
            pTextEdit->setText(in.readAll());
            file->close();
            delete file;
         }
         else
         {
             QMessageBox::information(this,"Error Message","Open File:"+file->errorString());
             return 0;
         }

      }
      //读入数据结束。


     //显示最近打开的菜单。【存在bug。多次打开同一个文件，最近打开都会变成同一个名称。】
     UpdateRecentFile(fileName); //更新最近打开文件
     setCurrentFile(fileName); //更新窗口标题和pTextEdit的内容。
     return true;
}

//保存文件函数
int MainWindow::Save()
{
    if(!fileName.isEmpty())   //判断文件名称fileName非空。
        {
            if(saveFile(fileName))  //调用saveFile函数保存文件
            return 1;
        }
      else
         {
            return SaveAs();   //文件名为空，另存为。
         }
    return true;
}

//另存为函数
bool MainWindow::SaveAs()
{
    fileName = QFileDialog::getSaveFileName(this,tr("save text!"),"/C/",tr("text files(*.txt)"));

    if(saveFile(fileName))
    {
        return 1;
    }
    else return 0;
}

//退出
bool MainWindow::exit()
{
    QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    switch (ret) {
      case QMessageBox::Save:
          // Save was clicked
          MainWindow::Save();
          break;
      case QMessageBox::Discard:
          // Don't Save was clicked
          break;
      case QMessageBox::Cancel:
          // Cancel was clicked
          return 0;
      default:
          // should never be reached
          break;
    }
    SaveSetting();
    close();
    return 1;
}

//保存文件
bool MainWindow::saveFile(const QString &filename)
{
   if(!filename.isEmpty())
    {
        QFile file(filename);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))  //一定要open才行。
            {
                qDebug()<<"open"<< filename <<"failed"<<endl;
            }
        QTextStream out(&file);
        out << pTextEdit->toPlainText();
        setCurrentFile(fileName); //更新窗口标题和pTextEdit的内容。
        UpdateRecentFile(filename);//更新最近打开文件列表
        return 1;
     }
   else
   {
        return 0;
   }
}

//文档有更改
void MainWindow::documentWasModified()
{
     setWindowModified(pTextEdit->document()->isModified());
}

//判断新建之前文本是否有修改。
//如果有修改，就弹窗选择是否保存。选择是：保存。选择放弃，不保存并清空。选择取消，什么都不做。
//情况二：当前文件未保存过，无名称，有内容。这种情况，内容非空，即有更改。需要弹窗由用户判断。
//情况三：当前文件有保存路径及名称，有内容，内容可能修改。。内容有修改需要保存，否则不需要。
bool MainWindow::maySave()
{
    //判断当前的文档是否有更改。
    if(pTextEdit->document()->isModified())
     {
         QMessageBox msgBox;
         msgBox.setText("The document has been modified.");
         msgBox.setInformativeText("Do you want to save your changes?");
         msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
         msgBox.setDefaultButton(QMessageBox::Save);
         int ret = msgBox.exec();
         switch (ret) {
           case QMessageBox::Save:
                 return  Save();
           case QMessageBox::Cancel:
                 return false;
           default:
              // Discard was clicked
               return true;
             }
     }

    return true;
}


//此函数用于传入一个文件名称。来显示文件的名称。
//设置编辑区域和窗口为“未变化”

void MainWindow::setCurrentFile(const QString &filename)
{
    fileName = filename;
    pTextEdit->document()->setModified(false); //刚打开文件，肯定没有更改。
    setWindowModified(false);

    QString shownName = fileName;
    if (fileName.isEmpty())
        shownName = "untitled";

     setWindowFilePath(shownName);  //【到底如何使用，让标题增加*】
     pLineEdit->setText(QFileInfo(shownName).fileName()); //用于在LineEdit中进行显示。
     //UpdateRecentFile(filename);
}




void MainWindow::UpdateTime()
{
    pTimeLabel->setText(m_time.currentTime().toString("hh:mm:ss"));
}


//更新最近打开的文件。
//1、通过open按钮打开的文件。2、通过New保存的文件。3、通过RecentFile打开的文件。
//
//传入：本次打开的文件的名称。
//判断：此文件是否在RecentFile中。不在，放到第一位置。原来的依次后移。
//在。本文件名之后的顺序不动，本文件名之前的部分，本文件名放到第一位，其他的依次后移。

void MainWindow::UpdateRecentFile(const QString &filename)
{
    if(filename !="")
    {
        QString tmp1 = filename;  //保存前一个名称。
        QString tmp2= "";         //保存本次的名称。
        for( int j=0; j < MaxRecentFiles ; ++j)
        {
            //qDebug()<<j<<endl;
            if( pRecentFileAction[j]->text() != filename )  //判断之前列表是否与本次的文件名相同
            {
                tmp2= pRecentFileAction[j]->text();   //记录本位置的文件名。
                pRecentFileAction[j]->setText(tmp1);  //本位置值名称成上一个名称。
                if(pRecentFileAction[j]->text()!="")  //更改后，判断此值是否为空，是否显示。
                {
                    pRecentFileAction[j]->setVisible(true);
                }

                    tmp1 = tmp2;                    //迭代
            }
            else
            {
                pRecentFileAction[j]->setText(tmp1);
                break;

                //找到j点，与本次打开文件名相同，则停止继续下移，并且
                //把这个点的名称修改为上一个点的名称（上一个保存在tmp1）

            }




        }

   }

    //下面为更新pRecentAction[]数组。
//    if(filename != "") //打开的肯定有文件名称。但是增加一个保险。
//    {
//        if ( i >= MaxRecentFiles)
//            {
//                i = 0;
//                pRecentFileAction[i]->setText(filename);
//                pRecentFileAction[i]->setVisible(true);
//                i = 1;
//            }
//        else
//           {
//                pRecentFileAction[i]->setText(filename);
//                pRecentFileAction[i]->setVisible(true);
//                i++;
//           }
//      }

//   else
//        qDebug()<<"filename is null"<<endl;
   //更新pRecentAction[]数组结束。


}


void MainWindow::OpenRecentFile()
{

    if(maySave())
    {

    QAction *pTmp  = qobject_cast<QAction *> (sender()); //qobject_cast<QAction *>做强制转换。sender()函数整体要括号起来。
    QString filename = pTmp->text();

   //下面为读入数据
    if(!filename.isEmpty())
    {
        QFile *file=new QFile;
        file->setFileName(filename);
        bool ok=file->open(QIODevice::ReadWrite | QIODevice::Text);
        if(ok)
        {
           QTextStream in(file);
           pTextEdit->setText(in.readAll());
           file->close();
           delete file;
        }
        else
        {
            QMessageBox::information(this,"Error Message","Open File:"+file->errorString());
            //return 0;
        }

     }
     //读入数据结束。




    //显示最近打开的菜单。【存在bug。多次打开同一个文件，最近打开都会变成同一个名称。】
    UpdateRecentFile(filename); //更新最近打开文件
    setCurrentFile(filename); //更新窗口标题和pTextEdit的内容。
    //return true;
    }
}

//void MainWindow::ReadSetting()
//{

//}


void MainWindow::SaveSetting()

{
    QSettings settings("JLW", "SpreadSheet");
    settings.setValue("RecentFiles",RecentFiles);

//    QFile *pfile = new QFile("D:/test/Spreadsheet_new/Spreadsheet/config.txt");

//    if(!pfile->open(QIODevice::WriteOnly | QIODevice::Text))  //一定要open才行。
//        {
//            qDebug()<<"SaveSetting failed"<<endl;
//        }

//    for(int j =0 ; j <MaxRecentFiles ; j++)
//    {
//             QTextStream out(pfile);
//             out <<pRecentFileAction[j]->text();
//             out <<"\n";

//    }

}


void MainWindow::ReadSetting()
{


     qDebug()<<"ReadSetting"<<endl;

    //第一次尝试。
//    QFile *pfile = new QFile("D:/test/Spreadsheet_new/Spreadsheet/config.txt");

//    if(!pfile->open(QIODevice::WriteOnly | QIODevice::Text))  //一定要open才行。
//        {
//            qDebug()<<"SaveSetting failed"<<endl;
//        }

//    for(int j =0 ; j <MaxRecentFiles ; j++)
//    {
//             QTextStream in(pfile);
//             in >>pRecentFileAction[j]->text();

//    }

}


//        QMessageBox::warning(this,tr("warning"),"I'm testing",QMessageBox::Yes | QMessageBox::No);
//        QMessageBox::information(this,tr("information"),"I'm testing",QMessageBox::Yes | QMessageBox::No);
//        QMessageBox::question(this,tr("question"),"I'm testing",QMessageBox::Yes | QMessageBox::No);
//        QMessageBox::critical(this,tr("critical"),"I'm testing",QMessageBox::Yes | QMessageBox::No);


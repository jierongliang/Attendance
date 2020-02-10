#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QObject>
#include    <QFileDialog>
#include    <QMessageBox>
#include<QSqlQuery>
#include<QDebug>
#include<QList>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->ll->hide();
    ui->edit->hide();
    this->setCentralWidget(ui->groupBox);


    //   tableView显示属性设置
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);


}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::getFieldNames_1()
{
    ui->c1->clear();
    QSqlRecord  emptyRec=tabModel->record();//获取空记录，只有字段名
    for (int i=0;i<emptyRec.count();i++)
        ui->c1->addItem(emptyRec.fieldName(i));

}
void MainWindow::getFieldNames_2()
{
    ui->c2->clear();
    QSqlRecord  emptyRec=tabModel->record();//获取空记录，只有字段名
    for (int i=0;i<emptyRec.count();i++)
        ui->c2->addItem(emptyRec.fieldName(i));

}
void MainWindow::openTable()
{
        //打开数据表
        tabModel=new QSqlTableModel(this,DB);//数据表
        if(flag){
            tabModel->setTable("StudengMessage"); //设置数据表
            //字段显示名
            tabModel->setHeaderData(tabModel->fieldIndex("学号"),Qt::Horizontal,"学号");
            tabModel->setHeaderData(tabModel->fieldIndex("姓名"),Qt::Horizontal,"姓名");
            tabModel->setHeaderData(tabModel->fieldIndex("性别"),Qt::Horizontal,"性别");
            tabModel->setHeaderData(tabModel->fieldIndex("年龄"),Qt::Horizontal,"年龄");
            tabModel->setHeaderData(tabModel->fieldIndex("班级"),Qt::Horizontal,"班级");
            tabModel->setSort(tabModel->fieldIndex("学号"),Qt::AscendingOrder);

        }
        else{
            tabModel->setTable("DailyTime"); //设置数据表
            //字段显示名
            tabModel->setHeaderData(tabModel->fieldIndex("姓名"),Qt::Horizontal,"姓名");
            tabModel->setHeaderData(tabModel->fieldIndex("课程名称"),Qt::Horizontal,"课程名称");
            tabModel->setHeaderData(tabModel->fieldIndex("缺课日期"),Qt::Horizontal,"缺课日期");
            tabModel->setHeaderData(tabModel->fieldIndex("第几节课"),Qt::Horizontal,"第几节课");
            tabModel->setHeaderData(tabModel->fieldIndex("缺课类型"),Qt::Horizontal,"缺课类型");
            tabModel->setSort(tabModel->fieldIndex("姓名"),Qt::AscendingOrder);
        }
        tabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);//数据保存方式，OnManualSubmit , OnRowChange
          //排序
        if (!(tabModel->select()))//查询数据
        {
            QMessageBox::critical(this, "错误信息","打开数据表错误,错误信息\n"+tabModel->lastError().text(),QMessageBox::Ok,QMessageBox::NoButton);
            return;
        }




        theSelection=new QItemSelectionModel(tabModel);//关联选择模型
        //theSelection当前项变化时触发currentChanged信号
        connect(theSelection,SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(on_currentChanged(QModelIndex,QModelIndex)));
        //选择行变化时
        connect(theSelection,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(on_currentRowChanged(QModelIndex,QModelIndex)));

        ui->tableView->setModel(tabModel);//设置数据模型
        ui->tableView->setSelectionModel(theSelection); //设置选择模型



        //创建界面组件与数据模型的字段之间的数据映射
        dataMapper= new QDataWidgetMapper();
        dataMapper->setModel(tabModel);//设置数据模型
        dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);//
        if(flag){
        //界面组件与tabModel的具体字段之间的联系
        dataMapper->addMapping(ui->e1,tabModel->fieldIndex("学号"));
        dataMapper->addMapping(ui->e2,tabModel->fieldIndex("姓名"));
        dataMapper->addMapping(ui->e3,tabModel->fieldIndex("性别"));

        dataMapper->addMapping(ui->e4,tabModel->fieldIndex("年龄"));
        dataMapper->addMapping(ui->e5,tabModel->fieldIndex("班级"));
        }
        else{
            //界面组件与tabModel的具体字段之间的联系
            dataMapper->addMapping(ui->e1,tabModel->fieldIndex("姓名"));
            dataMapper->addMapping(ui->e2,tabModel->fieldIndex("课程名称"));
            dataMapper->addMapping(ui->e3,tabModel->fieldIndex("缺课日期"));

            dataMapper->addMapping(ui->e4,tabModel->fieldIndex("第几节课"));
            dataMapper->addMapping(ui->e5,tabModel->fieldIndex("缺课类型"));
        }


        dataMapper->toFirst();//移动到首记录

          getFieldNames_2();
        getFieldNames_1();//获取字段名称列表，填充ui->groupBoxSort组件

        //更新按钮和界面组件的使能状态
//        if(flag){
//            ui->btn1->setEnabled(false);
//            ui->btn2->setEnabled(true);
//        }
//        else{
//            ui->btn2->setEnabled(false);
//            ui->btn1->setEnabled(true);
//            }
        calculate();

}

void MainWindow::on_btn1_clicked()
{
//    if(DB.isOpen()){
//        flag = true ;
//        openTable();

//        ui->l1->setText("学号");
//        ui->l2->setText("姓名");
//        ui->l3->setText("性别");
//        ui->l4->setText("年龄");
//        ui->l5->setText("班级");
//        return ;
//    }
    QString aFile=QFileDialog::getOpenFileName(this,"选择数据库文件","","SQL Lite数据库(*.db *.db3)");    //打开数据表，选择SQL Lite数据库文件
    if (aFile.isEmpty())
        return;

    //打开数据库
    DB=QSqlDatabase::addDatabase("QSQLITE"); //添加 SQL LITE数据库驱动
    DB.setDatabaseName(aFile); //设置数据库名称

    if (!DB.open())   //打开数据库
    {
        QMessageBox::warning(this, "错误", "打开数据库失败",QMessageBox::Ok,QMessageBox::NoButton);
        return;
    }

    //打开数据表
    flag = true;
    openTable();
    ui->ll->hide();
    ui->edit->hide();
    ui->l1->setText("学号");
    ui->l2->setText("姓名");
    ui->l3->setText("性别");
    ui->l4->setText("年龄");
    ui->l5->setText("班级");

}
void MainWindow::on_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{//更新pushButton_3和pushButton_4 的状态
    Q_UNUSED(current);
    Q_UNUSED(previous);
//    ui->pushButton_3->setEnabled(tabModel_1->isDirty()); //有未保存修改时可用
//    ui->pushButton_4->setEnabled(tabModel_1->isDirty());

}
void MainWindow::on_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    // 行切换时的状态控制
//    ui->pushButton_4->setEnabled(current.isValid());

    dataMapper->setCurrentIndex(current.row()); //更新数据映射的行号


}

void MainWindow::on_btn2_clicked()
{
//    if(DB.isOpen()){
//         flag = false ;
//        openTable();

//        ui->l1->setText("姓名");
//        ui->l2->setText("课程名称");
//        ui->l3->setText("缺课日期");
//        ui->l4->setText("第几节课");
//        ui->l5->setText("缺课类型");
//        return ;
//    }
    QString aFile=QFileDialog::getOpenFileName(this,"选择数据库文件","","SQL Lite数据库(*.db *.db3)");    //打开数据表，选择SQL Lite数据库文件
    if (aFile.isEmpty())
        return;
    if(aFile.isDetached())
    //打开数据库
    DB=QSqlDatabase::addDatabase("QSQLITE"); //添加 SQL LITE数据库驱动
    DB.setDatabaseName(aFile); //设置数据库名称

    if (!DB.open())   //打开数据库
    {
        QMessageBox::warning(this, "错误", "打开数据库失败",QMessageBox::Ok,QMessageBox::NoButton);
        return;
    }

    //打开数据表
     flag = false ;
    openTable();
    ui->ll->show();
    ui->edit->show();
    ui->l1->setText("姓名");
    ui->l2->setText("课程名称");
    ui->l3->setText("缺课日期");
    ui->l4->setText("第几节课");
    ui->l5->setText("缺课类型");
}

void MainWindow::on_btn6_clicked()
{
    QString w=ui->c1->currentText();
    QString m=ui->find->text();
    tabModel->setFilter(QObject::tr("%1 = %2").arg(w).arg(m));
    tabModel->select();

    if(tabModel->rowCount()==0)
        QMessageBox::information(this, "消息", "找不到相应的信息",QMessageBox::Ok,QMessageBox::NoButton);
    else{
     dataMapper->toFirst();//移动到首记录
    }
//    ui->edit->setText(QString::number(map.value(m)));
    showresult();
}

void MainWindow::on_c2_currentIndexChanged(int index)
{
    if (ui->r1->isChecked())
        tabModel->setSort(index,Qt::AscendingOrder);
    else
        tabModel->setSort(index,Qt::DescendingOrder);

    tabModel->select();
}

void MainWindow::on_r1_clicked()
{
    tabModel->setSort(ui->c2->currentIndex(),Qt::AscendingOrder);
    tabModel->select();
}


void MainWindow::on_r2_clicked()
{
    tabModel->setSort(ui->c2->currentIndex(),Qt::DescendingOrder);
    tabModel->select();
}

void MainWindow::on_btn7_clicked()
{
    if(flag)
        tabModel->setTable("StudengMessage");
    else
        tabModel->setTable("DailyTime");
    tabModel->select();
    showresult();
}
//添加
void MainWindow::on_btn3_clicked()
{
    tabModel->insertRow(tabModel->rowCount(),QModelIndex()); //在末尾添加一个记录

    QModelIndex curIndex=tabModel->index(tabModel->rowCount()-1,1);//创建最后一行的ModelIndex
    theSelection->clearSelection();//清空选择项
    theSelection->setCurrentIndex(curIndex,QItemSelectionModel::Select);//设置刚插入的行为当前选择行

   // int currow=curIndex.row(); //获得当前行
}
//保存
void MainWindow::on_btn4_clicked()
{
    bool res=tabModel->submitAll();

    QString t = tabModel->lastError().text();
    int i = QString::compare(tabModel->lastError().text(),QObject::tr("UNIQUE constraint failed: StudengMessage.\u5B66\u53F7 Unable to fetch row"));
    if (!res){
        if(i==0)
            QMessageBox::information(this, "保存失败", "添加重复学号",QMessageBox::Ok,QMessageBox::NoButton);
        else QMessageBox::information(this, "保存失败", "数据保存错误,错误信息\n"+t,QMessageBox::Ok,QMessageBox::NoButton);

    }
//    else
//    {
//        ui->btn4->setEnabled(false);

//    }
}
//撤销
void MainWindow::on_btn5_clicked()
{
    tabModel->revertAll();
}
//删除
void MainWindow::on_btn8_clicked()
{
    QModelIndex curIndex=theSelection->currentIndex();//获取当前选择单元格的模型索引
    tabModel->removeRow(curIndex.row()); //删除最后一行
    bool res=tabModel->submitAll();
    if (!res)
        QMessageBox::information(this, "消息", "数据保存错误,错误信息\n"+tabModel->lastError().text(),QMessageBox::Ok,QMessageBox::NoButton);
//    else
//    {
//        ui->btn8->setEnabled(false);

//    }

}
void MainWindow::calculate()
{

    int rowNum = tabModel->rowCount();
    int res = 100;
    QString type,name ;
    QModelIndex index;
    for(int i=0;i<rowNum;i++){
        index = tabModel->index(i,4);
        type = tabModel->data(index).toString();
        index = tabModel->index(i,0);
        name = tabModel->data(index).toString();
        if(map.find(name)==map.end())
            map[name]=100;

        if(QString::compare(type,QObject::tr("请假"))==0)
            map[name]-=1;
        else if(QString::compare(type,QObject::tr("迟到"))==0)
            map[name]-=2;
        else if(QString::compare(type,QObject::tr("早退"))==0)
            map[name]-=3;
        else if(QString::compare(type,QObject::tr("旷课"))==0)
            map[name]-=4;


    }
//    QString str,temp;
//    QMap<QString,int>::iterator it ;
//    for(it = map.begin();it!=map.end();it++){
//        temp = it.key()+QString::number(it.value());
//        str+=temp;
//    }
    showresult();

}
void MainWindow::showresult(){
    int rowNum = tabModel->rowCount();
     QModelIndex index;
    QList<QString> list ;
    QString name,temp,res ;
    for(int i=0;i<rowNum;i++){
        index = tabModel->index(i,0);
        name = tabModel->data(index).toString();
        if(!list.contains(name))
            list.append(name);

    }
    for(int i=0;i<list.size();i++){
        temp = list.at(i)+ QString::number(map.value(list.at(i)));
        res += temp;
    }
    ui->edit->setText(res);
}

#ifndef NCLASSES_H
#define NCLASSES_H

#include <QWidget>
#include"paint_box.h"

class paint_box;
class MainWindow_fusion;
namespace Ui {
class nclasses;
}

class nclasses : public QWidget
{
    Q_OBJECT
    
public:
    explicit nclasses(std::string fname1, int row,int col,int **labels1, MainWindow_fusion *tempParent,QWidget *parent = 0);
    ~nclasses();
    std::string fname1_;
  //  std::string fname2_;
    int **labels1_;
  //  int **labels2_;
    int row_,col_;

    MainWindow_fusion *tempParent ;
    paint_box *pbox;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::nclasses *ui;
};

#endif // NCLASSES_H

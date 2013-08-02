#ifndef CLASSIFYATTRIBUTES_H
#define CLASSIFYATTRIBUTES_H

#include <QWidget>
#include"nclasses.h"

class nclasses;
class MainWindow;

namespace Ui {
class ClassifyAttributes;
}

class ClassifyAttributes : public QWidget
{
    Q_OBJECT
    
public:
    explicit ClassifyAttributes( int row,int col,int **labels1, QDialog *tempParent,QWidget *parent = 0);
    ~ClassifyAttributes();
    int classify[20];
    nclasses *ncls;

    int Row;
    int Col;
    int **labels1_;
    QDialog *Parent;
    
private slots:
    void on_checkBox_clicked();

    void on_checkBox_6_clicked();

    void on_checkBox_area_clicked();

    void on_checkBox_round_clicked();

    void on_checkBox_round_clicked(bool checked);

    void on_checkBox_peri_clicked(bool checked);

    void on_checkBox_compact_clicked(bool checked);

    void on_checkBox_contrast_clicked(bool checked);

    void on_checkBox_rough_clicked(bool checked);

    void on_checkBox_coarse_clicked(bool checked);

    void on_checkBox_dir_clicked(bool checked);

    void on_checkBox_mr_clicked(bool checked);

    void on_checkBox_mg_clicked(bool checked);

    void on_checkBox_mb_clicked(bool checked);

    void on_checkBox_sr_clicked(bool checked);

    void on_checkBox_sg_clicked(bool checked);

    void on_checkBox_sb_clicked(bool checked);

    void on_pushButton_clicked();

    void on_radioButton_mss_clicked(bool checked);

    void on_radioButton_fused_clicked();

    void on_checkBox_mr_2_clicked(bool checked);

    void on_checkBox_mg_2_clicked(bool checked);

    void on_checkBox_mb_2_clicked(bool checked);

    void on_checkBox_sr_2_clicked(bool checked);

    void on_checkBox_sg_2_clicked(bool checked);

    void on_checkBox_sb_2_clicked(bool checked);


private:
    Ui::ClassifyAttributes *ui;
};

#endif // CLASSIFYATTRIBUTES_H

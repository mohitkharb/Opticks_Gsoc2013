#include "classifyattributes.h"
#include "ui_classifyattributes.h"

ClassifyAttributes::ClassifyAttributes(int row,int col,int **labels1, MainWindow_fusion *tempParent,QWidget *parent) :
    Row(row),Col(col),labels1_(labels1),Parent(tempParent),
    QWidget(parent),
    ui(new Ui::ClassifyAttributes)
{
    ui->setupUi(this);
    for(int i=0;i<4;i++)
        classify[i]=0;
    for(int i=4;i<8;i++)
        classify[i]=1;
    for(int i=8;i<14;i++)
        classify[i]=0;
    for(int i=14;i<17;i++)
        classify[i]=1;
    for(int i=17;i<20;i++)
        classify[i]=0;
    this->setWindowTitle("Feature selection");
}

ClassifyAttributes::~ClassifyAttributes()
{
    delete ui;
}

void ClassifyAttributes::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked()){
        ui->checkBox_area->setChecked(true);
        ui->checkBox_area->setEnabled(true);
        classify[0]=1;
        ui->checkBox_peri->setChecked(true);
        ui->checkBox_peri->setEnabled(true);
        classify[1]=1;
        ui->checkBox_round->setChecked(true);
        ui->checkBox_round->setEnabled(true);
        classify[2]=1;
        ui->checkBox_compact->setChecked(true);
        ui->checkBox_compact->setEnabled(true);
        classify[3]=1;
    }
    else{
        ui->checkBox_area->setChecked(false);
        ui->checkBox_area->setDisabled(true);
        classify[0]=0;
        ui->checkBox_peri->setChecked(false);
        ui->checkBox_peri->setDisabled(true);
        classify[1]=0;
        ui->checkBox_round->setChecked(false);
        ui->checkBox_round->setDisabled(true);
        classify[2]=0;
        ui->checkBox_compact->setChecked(false);
        ui->checkBox_compact->setDisabled(true);
        classify[3]=0;
    }

}

void ClassifyAttributes::on_checkBox_6_clicked()
{
    if(ui->checkBox_6->isChecked()){
        ui->checkBox_contrast->setChecked(true);
        ui->checkBox_contrast->setEnabled(true);
        classify[4]=1;
        ui->checkBox_rough->setChecked(true);
        ui->checkBox_rough->setEnabled(true);
        classify[5]=1;
        ui->checkBox_coarse->setChecked(true);
        ui->checkBox_coarse->setEnabled(true);
        classify[6]=1;
        ui->checkBox_dir->setChecked(true);
        ui->checkBox_dir->setEnabled(true);
        classify[7]=1;
    }
    else{
        ui->checkBox_contrast->setChecked(false);
        ui->checkBox_contrast->setDisabled(true);
        classify[4]=0;
        ui->checkBox_rough->setChecked(false);
        ui->checkBox_rough->setDisabled(true);
        classify[5]=0;
        ui->checkBox_coarse->setChecked(false);
        ui->checkBox_coarse->setDisabled(true);
        classify[6]=0;
        ui->checkBox_dir->setChecked(false);
        ui->checkBox_dir->setDisabled(true);
        classify[7]=0;
    }
}



void ClassifyAttributes::on_checkBox_area_clicked()
{
    if(ui->checkBox_area->isChecked()){
        classify[0]=1;
    }
    else{
        classify[0]=0;
    }
}



void ClassifyAttributes::on_checkBox_round_clicked()
{

}

void ClassifyAttributes::on_checkBox_round_clicked(bool checked)
{
    if(checked==true){
        classify[2]=1;
    }
    else{
        classify[2]=0;
    }
}

void ClassifyAttributes::on_checkBox_peri_clicked(bool checked)
{
    if(checked==true){
        classify[1]=1;
    }
    else{
        classify[1]=0;
    }
}

void ClassifyAttributes::on_checkBox_compact_clicked(bool checked)
{
    if(checked==true){
        classify[3]=1;
    }
    else{
        classify[3]=0;
    }
}

void ClassifyAttributes::on_checkBox_contrast_clicked(bool checked)
{
    if(checked==true){
        classify[4]=1;
    }
    else{
        classify[4]=0;
    }
}

void ClassifyAttributes::on_checkBox_rough_clicked(bool checked)
{
    if(checked==true){
        classify[5]=1;
    }
    else{
        classify[5]=0;
    }
}

void ClassifyAttributes::on_checkBox_coarse_clicked(bool checked)
{
    if(checked==true){
        classify[6]=1;
    }
    else{
        classify[6]=0;
    }
}

void ClassifyAttributes::on_checkBox_dir_clicked(bool checked)
{
    if(checked==true){
        classify[7]=1;
    }
    else{
        classify[7]=0;
    }
}

void ClassifyAttributes::on_checkBox_mr_clicked(bool checked)
{
    if(checked==true){
        classify[8]=1;
    }
    else{
        classify[8]=0;
    }
}

void ClassifyAttributes::on_checkBox_mg_clicked(bool checked)
{
    if(checked==true){
        classify[9]=1;
    }
    else{
        classify[9]=0;
    }
}

void ClassifyAttributes::on_checkBox_mb_clicked(bool checked)
{
    if(checked==true){
        classify[10]=1;
    }
    else{
        classify[10]=0;
    }
}

void ClassifyAttributes::on_checkBox_sr_clicked(bool checked)
{
    if(checked==true){
        classify[11]=1;
    }
    else{
        classify[11]=0;
    }
}

void ClassifyAttributes::on_checkBox_sg_clicked(bool checked)
{
    if(checked==true){
        classify[12]=1;
    }
    else{
        classify[12]=0;
    }
}

void ClassifyAttributes::on_checkBox_sb_clicked(bool checked)
{
    if(checked==true){
        classify[13]=1;
    }
    else{
        classify[13]=0;
    }
}

void ClassifyAttributes::on_pushButton_clicked()
{
    std::string file1 = "C:/Windows/Temp/pan.ppm";
    ncls = new nclasses(file1,Row,Col,labels1_,Parent);
    ncls->update();
    ncls->show();

//    std::cout<<"classify array"<<std::endl;
//    for(int i=0;i<20;i++)
//    {
//        std::cout<<i<<" " <<classify[i]<<std::endl;
//    }

    this->close();
}

void ClassifyAttributes::on_radioButton_mss_clicked(bool checked)
{

        ui->checkBox_mr->setChecked(true);
        ui->checkBox_mr->setEnabled(true);
        classify[8]=1;
        ui->checkBox_mg->setChecked(true);
        ui->checkBox_mg->setEnabled(true);
        classify[9]=1;
        ui->checkBox_mb->setChecked(true);
        ui->checkBox_mb->setEnabled(true);
        classify[10]=1;
        ui->checkBox_sr->setChecked(true);
        ui->checkBox_sr->setEnabled(true);
        classify[11]=1;
        ui->checkBox_sg->setChecked(true);
        ui->checkBox_sg->setEnabled(true);
        classify[12]=1;
        ui->checkBox_sb->setChecked(true);
        ui->checkBox_sb->setEnabled(true);
        classify[13]=1;

        ui->checkBox_mr_2->setChecked(false);
        ui->checkBox_mr_2->setEnabled(false);
        classify[14]=0;
        ui->checkBox_mg_2->setChecked(false);
        ui->checkBox_mg_2->setEnabled(false);
        classify[15]=0;
        ui->checkBox_mb_2->setChecked(false);
        ui->checkBox_mb_2->setEnabled(false);
        classify[16]=0;
        ui->checkBox_sr_2->setChecked(false);
        ui->checkBox_sr_2->setEnabled(false);
        classify[17]=0;
        ui->checkBox_sg_2->setChecked(false);
        ui->checkBox_sg_2->setEnabled(false);
        classify[18]=0;
        ui->checkBox_sb_2->setChecked(false);
        ui->checkBox_sb_2->setEnabled(false);
        classify[19]=0;

}

void ClassifyAttributes::on_radioButton_fused_clicked()
{
    ui->checkBox_mr_2->setChecked(true);
    ui->checkBox_mr_2->setEnabled(true);
    classify[14]=1;
    ui->checkBox_mg_2->setChecked(true);
    ui->checkBox_mg_2->setEnabled(true);
    classify[15]=1;
    ui->checkBox_mb_2->setChecked(true);
    ui->checkBox_mb_2->setEnabled(true);
    classify[16]=1;
    ui->checkBox_sr_2->setChecked(true);
    ui->checkBox_sr_2->setEnabled(true);
    classify[17]=1;
    ui->checkBox_sg_2->setChecked(true);
    ui->checkBox_sg_2->setEnabled(true);
    classify[18]=1;
    ui->checkBox_sb_2->setChecked(true);
    ui->checkBox_sb_2->setEnabled(true);
    classify[19]=1;

    ui->checkBox_mr->setChecked(false);
    ui->checkBox_mr->setEnabled(false);
    classify[8]=0;
    ui->checkBox_mg->setChecked(false);
    ui->checkBox_mg->setEnabled(false);
    classify[9]=0;
    ui->checkBox_mb->setChecked(false);
    ui->checkBox_mb->setEnabled(false);
    classify[10]=0;
    ui->checkBox_sr->setChecked(false);
    ui->checkBox_sr->setEnabled(false);
    classify[11]=0;
    ui->checkBox_sg->setChecked(false);
    ui->checkBox_sg->setEnabled(false);
    classify[12]=0;
    ui->checkBox_sb->setChecked(false);
    ui->checkBox_sb->setEnabled(false);
    classify[13]=0;
}

void ClassifyAttributes::on_checkBox_mr_2_clicked(bool checked)
{
    if(checked==true){
        classify[14]=1;
    }
    else{
        classify[14]=0;
    }
}

void ClassifyAttributes::on_checkBox_mg_2_clicked(bool checked)
{
    if(checked==true){
        classify[15]=1;
    }
    else{
        classify[15]=0;
    }
}

void ClassifyAttributes::on_checkBox_mb_2_clicked(bool checked)
{
    if(checked==true){
        classify[16]=1;
    }
    else{
        classify[16]=0;
    }
}

void ClassifyAttributes::on_checkBox_sr_2_clicked(bool checked)
{
    if(checked==true){
        classify[17]=1;
    }
    else{
        classify[17]=0;
    }
}

void ClassifyAttributes::on_checkBox_sg_2_clicked(bool checked)
{
    if(checked==true){
        classify[18]=1;
    }
    else{
        classify[18]=0;
    }
}

void ClassifyAttributes::on_checkBox_sb_2_clicked(bool checked)
{
    if(checked==true){
        classify[19]=1;
    }
    else{
        classify[19]=0;
    }
}



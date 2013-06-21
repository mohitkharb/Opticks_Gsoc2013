#include "attributebased.h"
#include "ui_attributebased.h"
#include<iostream>
#include<math.h>
AttributeBased::AttributeBased(int row,int col,int **labels1,int ***data1,Image i1,int num_band,int i, QWidget *parent) :
    row(row),col(col),labels1(labels1),data1(data1),i1(i1),num_band(num_band),flag(i),
    QWidget(parent),
    ui(new Ui::AttributeBased)
{
    ui->setupUi(this);
    this->setWindowTitle("Attribute Based Analysis");
    attribute ="";
    att=0;
    flag1=flag2=0;
    maxMeanRed = INT_MIN; maxMeanGreen = INT_MIN; maxMeanBlue = INT_MIN;
    minMeanRed = INT_MAX; minMeanGreen = INT_MAX; minMeanBlue = INT_MAX;
    maxStdRed = maxStdGreen = maxStdBlue = (float) INT_MIN;
    minStdRed = minStdGreen = minStdBlue= (float) INT_MAX;
    maxArea = INT_MIN ,minArea = INT_MAX;maxPerimeter = INT_MIN;minPerimeter=INT_MAX;
    maxRoundness = (float) INT_MIN;	minRoundness = (float) INT_MAX;	maxCompactness = (float) INT_MIN;minCompactness = (float) INT_MAX;
    maxRoughness = (float) INT_MIN; minRoughness = (float) INT_MAX;maxCoarseness = (float) INT_MIN;minCoarseness = (float) INT_MAX;
    maxDirection = (float) INT_MIN; minDirection = (float) INT_MAX;maxContrast = (float) INT_MIN;minContrast = (float) INT_MAX;
    classmin= INT_MAX; classmax = INT_MIN;


    int tArea,tPerimeter,tMeanRed,tMeanGreen,tMeanBlue,tclass;
    float tStdRed,tStdGreen,tStdBlue,tRoundness,tCompactness,tDirection,tCoarseness,tRoughness,tContrast;


    for(int i = 0 ; i < i1.NO ;i++){


        tArea=i1.objectArray[i].fVector.area;
        if(tArea>maxArea)maxArea=tArea;
        if(tArea<minArea)minArea=tArea;

        tPerimeter=i1.objectArray[i].fVector.perimeter;
        if(tPerimeter>maxPerimeter)maxPerimeter=tPerimeter;
        if(tPerimeter<minPerimeter)minPerimeter=tPerimeter;

        tMeanRed=i1.objectArray[i].fVector.meanRed;
        if(tMeanRed>maxMeanRed)maxMeanRed=tMeanRed;
        if(tMeanRed<minMeanRed)minMeanRed=tMeanRed;

        tMeanGreen=i1.objectArray[i].fVector.meanGreen;
        if(tMeanGreen>maxMeanGreen)maxMeanGreen=tMeanGreen;
        if(tMeanGreen<minMeanGreen)minMeanGreen=tMeanGreen;

        tMeanBlue=i1.objectArray[i].fVector.meanBlue;
        if(tMeanBlue>maxMeanBlue)maxMeanBlue=tMeanBlue;
        if(tMeanBlue<minMeanBlue)minMeanBlue=tMeanBlue;

        tStdRed=i1.objectArray[i].fVector.stdRed;
        if(tStdRed>maxStdRed)maxStdRed=tStdRed;
        if(tStdRed<minStdRed)minStdRed=tStdRed;

        tStdGreen=i1.objectArray[i].fVector.stdGreen;
        if(tStdGreen>maxStdGreen)maxStdGreen=tStdGreen;
        if(tStdGreen<minStdGreen)minStdGreen=tStdGreen;

        tStdBlue=i1.objectArray[i].fVector.stdBlue;
        if(tStdBlue>maxStdBlue)maxStdBlue=tStdBlue;
        if(tStdBlue<minStdBlue)minStdBlue=tStdBlue;

        tCompactness=i1.objectArray[i].fVector.compactness;
        if(tCompactness>maxCompactness)maxCompactness=tCompactness;
        if(tCompactness<minCompactness)minCompactness=tCompactness;

        tRoundness=i1.objectArray[i].fVector.roundness;
        if(tRoundness>maxRoundness)maxRoundness=tRoundness;
        if(tRoundness<minRoundness)minRoundness=tRoundness;

        tDirection=i1.objectArray[i].fVector.direction;
        if(tDirection>maxDirection)maxDirection=tDirection;
        if(tDirection<minDirection)minDirection=tDirection;

        tRoughness=i1.objectArray[i].fVector.roughness;
        if(tRoughness>maxRoughness)maxRoughness=tRoughness;
        if(tRoughness<minRoughness)minRoughness=tRoughness;

        tContrast=i1.objectArray[i].fVector.contrast;
        if(tContrast>maxContrast)maxContrast=tContrast;
        if(tContrast<minContrast)minContrast=tContrast;

        tCoarseness=i1.objectArray[i].fVector.coarseness;
        if(tCoarseness>maxCoarseness)maxCoarseness=tCoarseness;
        if(tCoarseness<minCoarseness)minCoarseness=tCoarseness;

        if(flag==1){
            tclass=i1.objectArray[i].fVector.classNumber;
            if(tclass>classmax) classmax=tclass;
            if(tclass<classmin) classmin=tclass;
        }


    }


    QString text_number;

    text_number = text_number.setNum(maxArea);
    ui->lineEdit_2->setText(text_number);

    text_number = text_number.setNum(minArea);
    ui->lineEdit->setText(text_number);

    text_number = text_number.setNum(maxRoundness);
    ui->lineEdit_4->setText(text_number);


    text_number = text_number.setNum(minRoundness);
    ui->lineEdit_3->setText(text_number);


    text_number = text_number.setNum(maxCompactness);
    ui->lineEdit_6->setText(text_number);

    text_number = text_number.setNum(minCompactness);
    ui->lineEdit_5->setText(text_number);

    text_number = text_number.setNum(maxMeanRed);
    ui->lineEdit_8->setText(text_number);

    text_number = text_number.setNum(minMeanRed);
    ui->lineEdit_7->setText(text_number);

    text_number = text_number.setNum(maxMeanGreen);
    ui->lineEdit_10->setText(text_number);

    text_number = text_number.setNum(minMeanGreen);
    ui->lineEdit_9->setText(text_number);

    text_number = text_number.setNum(maxMeanBlue);
    ui->lineEdit_12->setText(text_number);

    text_number = text_number.setNum(minMeanBlue);
    ui->lineEdit_11->setText(text_number);

    text_number = text_number.setNum(maxStdRed);
    ui->lineEdit_14->setText(text_number);

    text_number = text_number.setNum(minStdRed);
    ui->lineEdit_13->setText(text_number);

    text_number = text_number.setNum(maxStdGreen);
    ui->lineEdit_16->setText(text_number);

    text_number = text_number.setNum(minStdGreen);
    ui->lineEdit_15->setText(text_number);

    text_number = text_number.setNum(maxStdBlue);
    ui->lineEdit_18->setText(text_number);

    text_number = text_number.setNum(minStdBlue);
    ui->lineEdit_17->setText(text_number);

    text_number = text_number.setNum(maxPerimeter);
    ui->lineEdit_20->setText(text_number);

    text_number = text_number.setNum(minPerimeter);
    ui->lineEdit_19->setText(text_number);


    text_number = text_number.setNum(maxContrast);
    ui->lineEdit_22->setText(text_number);

    text_number = text_number.setNum(minContrast);
    ui->lineEdit_21->setText(text_number);


    text_number = text_number.setNum(maxRoughness);
    ui->lineEdit_24->setText(text_number);

    text_number = text_number.setNum(minRoughness);
    ui->lineEdit_23->setText(text_number);

    text_number = text_number.setNum(maxCoarseness);
    ui->lineEdit_26->setText(text_number);

    text_number = text_number.setNum(minCoarseness);
    ui->lineEdit_25->setText(text_number);

    text_number = text_number.setNum(maxDirection);
    ui->lineEdit_28->setText(text_number);

    text_number = text_number.setNum(minDirection);
    ui->lineEdit_27->setText(text_number);

    text_number = text_number.setNum(classmin);
    ui->lineEdit_29->setText(text_number);

    text_number = text_number.setNum(classmax);
    ui->lineEdit_30->setText(text_number);

    if(flag==0){
        ui->lineEdit_29->setHidden(true);
        ui->lineEdit_30->setHidden(true);
        ui->radioButton_17->setHidden(true);
        ui->pushButton_17->setHidden(true);
    }

}

AttributeBased::~AttributeBased()
{
    delete ui;
    objs.clear();
    objsnew.clear();
    pixels.clear();
    pixelsnew.clear();
}

void AttributeBased::on_lineEdit_2_returnPressed()
{
    rmaxArea_= ui->lineEdit_2->text().toInt();
}

void AttributeBased::on_lineEdit_returnPressed()
{
    rminArea_=ui->lineEdit->text().toInt();

}

void AttributeBased::on_radioButton_3_clicked()
{
    attribute="area";
    att=6;
    int t;
    int max=INT_MIN;
    int min=INT_MAX;
    for(int i = 0 ; i < i1.NO ;i++){
        t=i1.objectArray[i].fVector.area;
        if(t>max)max=t;
        if(t<min)min=t;
    }


    QString qStr = QString::number(max);
    QString qStr1 = QString::number(min);
    ui->label_max->setText(qStr.toUtf8().data());
    ui->label_min->setText(qStr1.toUtf8().data());

}


void AttributeBased::on_radioButton_4_clicked()
{
    attribute="roundness";
    att=10;
    float t;
    float max= (float) INT_MIN;
    float min= (float) INT_MAX;
    for(int i = 0 ; i < i1.NO ;i++){
        t=i1.objectArray[i].fVector.roundness;
        if(t>max)max=t;
        if(t<min)min=t;
    }


    QString qStr = QString::number(max);
    QString qStr1 = QString::number(min);
    ui->label_max->setText(qStr.toUtf8().data());
    ui->label_min->setText(qStr1.toUtf8().data());
}

void AttributeBased::on_radioButton_5_clicked()
{
    attribute="compactness";
    att=11;
    float t;
    float max= (float) INT_MIN;
    float min= (float) INT_MAX;
    for(int i = 0 ; i < i1.NO ;i++){
        t=i1.objectArray[i].fVector.compactness;
        if(t>max)max=t;
        if(t<min)min=t;
    }


    QString qStr = QString::number(max);
    QString qStr1 = QString::number(min);
    ui->label_max->setText(qStr.toUtf8().data());
    ui->label_min->setText(qStr1.toUtf8().data());
}



void AttributeBased::on_pushButton_clicked()
{

	objs.erase(objs.begin(),objs.end());
	pixels.erase(pixels.begin(),pixels.end());

    int index = 0;
    std::cout<<"here1"<<std::endl;

    rmaxArea_= ui->lineEdit_2->text().toInt();
    rminArea_=ui->lineEdit->text().toInt();

    rmaxRoundness_= ui->lineEdit_4->text().toFloat();
    rminRoundness_= ui->lineEdit_3->text().toFloat();

    rminCompactness_= ui->lineEdit_5->text().toFloat();
    rmaxCompactness_= ui->lineEdit_6->text().toFloat();

    rminMeanRed_= ui->lineEdit_7->text().toInt();
    rmaxMeanRed_= ui->lineEdit_8->text().toInt();

    rminMeanGreen_= ui->lineEdit_9->text().toInt();
    rmaxMeanGreen_= ui->lineEdit_10->text().toInt();

    rminMeanBlue_= ui->lineEdit_11->text().toInt();
    rmaxMeanBlue_= ui->lineEdit_12->text().toInt();

    rminStdRed_ = ui->lineEdit_13->text().toFloat();
    rmaxStdRed_ = ui->lineEdit_14->text().toFloat();

    rminStdGreen_ = ui->lineEdit_15->text().toFloat();
    rmaxStdGreen_ = ui->lineEdit_16->text().toFloat();

    rminStdBlue_ = ui->lineEdit_17->text().toFloat();
    rmaxStdBlue_ = ui->lineEdit_18->text().toFloat();


    rminPerimeter_ = ui->lineEdit_19->text().toInt();
    rmaxPerimeter_ = ui->lineEdit_20->text().toInt();

    rminContrast_ = ui->lineEdit_21->text().toFloat();
    rmaxContrast_ = ui->lineEdit_22->text().toFloat();

    rminRoughness_ = ui->lineEdit_23->text().toFloat();
    rmaxRoughness_ = ui->lineEdit_24->text().toFloat();

    rminCoarseness_ = ui->lineEdit_25->text().toFloat();
    rmaxCoarseness_ = ui->lineEdit_26->text().toFloat();

    rminDirection_ = ui->lineEdit_27->text().toFloat();
    rmaxDirection_ = ui->lineEdit_28->text().toFloat();
    if(flag==1){
        rminClass_=ui->lineEdit_29->text().toInt();
        rmaxClass_=ui->lineEdit_30->text().toInt();
    }
    std::cout<<"here2"<<std::endl;




        for(int i = 0 ; i < i1.NO ;i++){

            if(i1.objectArray[i].fVector.area >= rminArea_ && i1.objectArray[i].fVector.area <= rmaxArea_ &&
                    i1.objectArray[i].fVector.roundness >= rminRoundness_ && i1.objectArray[i].fVector.roundness <= rmaxRoundness_ &&
                    i1.objectArray[i].fVector.meanRed >= rminMeanRed_ && i1.objectArray[i].fVector.meanRed <= rmaxMeanRed_ &&
                    i1.objectArray[i].fVector.meanGreen >= rminMeanGreen_ && i1.objectArray[i].fVector.meanGreen <= rmaxMeanGreen_ &&
                    i1.objectArray[i].fVector.meanBlue >= rminMeanBlue_ && i1.objectArray[i].fVector.meanBlue <= rmaxMeanBlue_ &&
                    i1.objectArray[i].fVector.stdRed >= rminStdRed_ && i1.objectArray[i].fVector.stdRed <= rmaxStdRed_ &&
                    i1.objectArray[i].fVector.stdGreen >= rminStdGreen_ && i1.objectArray[i].fVector.stdGreen <= rmaxStdGreen_ &&
                    i1.objectArray[i].fVector.stdBlue >= rminStdBlue_ && i1.objectArray[i].fVector.stdBlue <= rmaxStdBlue_ &&
                    i1.objectArray[i].fVector.perimeter >= rminPerimeter_ && i1.objectArray[i].fVector.perimeter <= rmaxPerimeter_ &&
                    i1.objectArray[i].fVector.direction >= rminDirection_ && i1.objectArray[i].fVector.direction <= rmaxDirection_ &&
                    i1.objectArray[i].fVector.contrast >= rminContrast_ && i1.objectArray[i].fVector.contrast <= rmaxContrast_ &&
                    i1.objectArray[i].fVector.coarseness >= rminCoarseness_ && i1.objectArray[i].fVector.coarseness <= rmaxCoarseness_ &&
                    i1.objectArray[i].fVector.roughness >= rminRoughness_ && i1.objectArray[i].fVector.roughness <= rmaxRoughness_ &&
                    i1.objectArray[i].fVector.compactness >= rminCompactness_ && i1.objectArray[i].fVector.compactness <= rmaxCompactness_ ){
                if(flag==1 && i1.objectArray[i].fVector.classNumber >= rminClass_ && i1.objectArray[i].fVector.classNumber <= rmaxClass_)
                    objs.push_back(i);
                else if(flag==0)
                    objs.push_back(i);
            }

        }
        std::cout<<"here3"<<std::endl;

        if(objs.size() == 0 ){
            ui->textBrowser->setText("No Object with given attributes. Select different parameters.");
            std::cout << " no obj" << std::endl;
        }
        else{

            for(int x = 0; x < row ; x++){
                for(int y = 0 ; y < col ; y++){
                    int tx = labels1[x][y];
                    if(objs.size() == 1){
                        int tsv = objs[0];
                        if(tsv == tx)
                            pixels.push_back(index);
                    }
                    else{
                        for(std::vector<int>::iterator it = objs.begin();it!=objs.end()-1;++it){
                            int tsv = *it;
                            if(tsv ==tx ){
                                pixels.push_back(index);

                            }
                        }
                    }
                    index++;
                }
            }

            std::cout<<"here5"<<std::endl;
            ui->textBrowser->setText("Object Image Generated");
            pain2img *img1= new pain2img(pixels,data1,row,col,num_band,"Objet Image");
            img1->show();

        }
        std::cout<<"here6"<<std::endl;







}

void AttributeBased::on_lineEdit_3_returnPressed()
{
    rminRoundness_= ui->lineEdit_3->text().toFloat();
}

void AttributeBased::on_lineEdit_4_returnPressed()
{
    rmaxRoundness_= ui->lineEdit_4->text().toFloat();
}

void AttributeBased::on_lineEdit_5_returnPressed()
{
    rminCompactness_= ui->lineEdit_5->text().toFloat();
}

void AttributeBased::on_lineEdit_6_returnPressed()
{
    rmaxCompactness_= ui->lineEdit_6->text().toFloat();
}

void AttributeBased::on_radioButton_6_clicked()
{
    attribute="meanred";
    att=1;
    int t;
    int max=INT_MIN;
    int min=INT_MAX;
    for(int i = 0 ; i < i1.NO ;i++){
        t=i1.objectArray[i].fVector.meanRed;
        if(t>max)max=t;
        if(t<min)min=t;
    }
    

    QString qStr = QString::number(max);
    QString qStr1 = QString::number(min);
    ui->label_max->setText(qStr.toUtf8().data());
    ui->label_min->setText(qStr1.toUtf8().data());
}

void AttributeBased::on_lineEdit_7_returnPressed()
{
    rminMeanRed_= ui->lineEdit_7->text().toInt();
}

void AttributeBased::on_lineEdit_8_returnPressed()
{
    rmaxMeanRed_= ui->lineEdit_8->text().toInt();
}

void AttributeBased::on_radioButton_7_clicked()
{
    attribute="meangreen";
    att=2;
    int t;
    int max=INT_MIN;
    int min=INT_MAX;
    for(int i = 0 ; i < i1.NO ;i++){
        t=i1.objectArray[i].fVector.meanGreen;
        if(t>max)max=t;
        if(t<min)min=t;
    }


    QString qStr = QString::number(max);
    QString qStr1 = QString::number(min);
    ui->label_max->setText(qStr.toUtf8().data());
    ui->label_min->setText(qStr1.toUtf8().data());
}

void AttributeBased::on_radioButton_8_clicked()
{
    attribute="meanblue";
    att=3;
    int t;
    int max=INT_MIN;
    int min=INT_MAX;
    for(int i = 0 ; i < i1.NO ;i++){
        t=i1.objectArray[i].fVector.meanBlue;
        if(t>max)max=t;
        if(t<min)min=t;
    }


    QString qStr = QString::number(max);
    QString qStr1 = QString::number(min);
    ui->label_max->setText(qStr.toUtf8().data());
    ui->label_min->setText(qStr1.toUtf8().data());
}

void AttributeBased::on_lineEdit_9_returnPressed()
{
    rminMeanGreen_= ui->lineEdit_9->text().toInt();
}

void AttributeBased::on_lineEdit_10_returnPressed()
{
    rmaxMeanGreen_= ui->lineEdit_10->text().toInt();
}

void AttributeBased::on_lineEdit_11_returnPressed()
{
    rminMeanBlue_= ui->lineEdit_11->text().toInt();
}

void AttributeBased::on_lineEdit_12_returnPressed()
{
    rmaxMeanBlue_= ui->lineEdit_12->text().toInt();
}

void AttributeBased::on_radioButton_9_clicked()
{
    attribute="stdred";
    att=4;
    float t;
    float max= (float) INT_MIN;
    float min= (float) INT_MAX;
    for(int i = 0 ; i < i1.NO ;i++){
        t=i1.objectArray[i].fVector.stdRed;
        if(t>max)max=t;
        if(t<min)min=t;
    }


    QString qStr = QString::number(max);
    QString qStr1 = QString::number(min);
    ui->label_max->setText(qStr.toUtf8().data());
    ui->label_min->setText(qStr1.toUtf8().data());
}

void AttributeBased::on_radioButton_10_clicked()
{
    attribute="stdgreen";
    att=5;
    float t;
    float max= (float) INT_MIN;
    float min= (float) INT_MAX;
    for(int i = 0 ; i < i1.NO ;i++){
        t=i1.objectArray[i].fVector.stdGreen;
        if(t>max)max=t;
        if(t<min)min=t;
    }


    QString qStr = QString::number(max);
    QString qStr1 = QString::number(min);
    ui->label_max->setText(qStr.toUtf8().data());
    ui->label_min->setText(qStr1.toUtf8().data());
}

void AttributeBased::on_radioButton_11_clicked()
{
    attribute="stdblue";
    att=6;
    float t;
    float max= (float) INT_MIN;
    float min= (float) INT_MAX;
    for(int i = 0 ; i < i1.NO ;i++){
        t=i1.objectArray[i].fVector.stdBlue;
        if(t>max)max=t;
        if(t<min)min=t;
    }


    QString qStr = QString::number(max);
    QString qStr1 = QString::number(min);
    ui->label_max->setText(qStr.toUtf8().data());
    ui->label_min->setText(qStr1.toUtf8().data());
}

void AttributeBased::on_lineEdit_13_returnPressed()
{
    rminStdRed_= ui->lineEdit_13->text().toFloat();
}

void AttributeBased::on_lineEdit_14_returnPressed()
{
    rmaxStdRed_= ui->lineEdit_14->text().toFloat();
}

void AttributeBased::on_lineEdit_15_returnPressed()
{
    rminStdGreen_= ui->lineEdit_15->text().toFloat();
}

void AttributeBased::on_lineEdit_16_returnPressed()
{
    rmaxStdGreen_= ui->lineEdit_16->text().toFloat();
}

void AttributeBased::on_lineEdit_17_returnPressed()
{
    rminStdBlue_= ui->lineEdit_17->text().toFloat();
}

void AttributeBased::on_lineEdit_18_returnPressed()
{
    rmaxStdBlue_= ui->lineEdit_18->text().toFloat();
}

void AttributeBased::on_lineEdit_19_returnPressed()
{
    rminPerimeter_= ui->lineEdit_19->text().toInt();
}

void AttributeBased::on_lineEdit_20_returnPressed()
{
    rmaxPerimeter_= ui->lineEdit_20->text().toInt();
}

void AttributeBased::on_radioButton_12_clicked()
{
    attribute="perimeter";
    att=8;
    int t;
    int max=  INT_MIN;
    int min=  INT_MAX;
    for(int i = 0 ; i < i1.NO ;i++){
        t=i1.objectArray[i].fVector.perimeter;
        if(t>max)max=t;
        if(t<min)min=t;
    }


    QString qStr = QString::number(max);
    QString qStr1 = QString::number(min);
    ui->label_max->setText(qStr.toUtf8().data());
    ui->label_min->setText(qStr1.toUtf8().data());
}

void AttributeBased::on_radioButton_13_clicked()
{
    attribute="contrast";
    att=6;
    float t;
    float max= (float) INT_MIN;
    float min= (float) INT_MAX;
    for(int i = 0 ; i < i1.NO ;i++){
        t=i1.objectArray[i].fVector.contrast;
        if(t>max)max=t;
        if(t<min)min=t;
    }


    QString qStr = QString::number(max);
    QString qStr1 = QString::number(min);
    ui->label_max->setText(qStr.toUtf8().data());
    ui->label_min->setText(qStr1.toUtf8().data());
}

void AttributeBased::on_radioButton_14_clicked()
{
    attribute="roughness";
    att=6;
    float t;
    float max= (float) INT_MIN;
    float min= (float) INT_MAX;
    for(int i = 0 ; i < i1.NO ;i++){
        t=i1.objectArray[i].fVector.roughness;
        if(t>max)max=t;
        if(t<min)min=t;
    }


    QString qStr = QString::number(max);
    QString qStr1 = QString::number(min);
    ui->label_max->setText(qStr.toUtf8().data());
    ui->label_min->setText(qStr1.toUtf8().data());
}


void AttributeBased::on_radioButton_15_clicked()
{
    attribute="Coarseness";
    att=6;
    float t;
    float max= (float) INT_MIN;
    float min= (float) INT_MAX;
    for(int i = 0 ; i < i1.NO ;i++){
        t=i1.objectArray[i].fVector.coarseness;
        if(t>max)max=t;
        if(t<min)min=t;
    }


    QString qStr = QString::number(max);
    QString qStr1 = QString::number(min);
    ui->label_max->setText(qStr.toUtf8().data());
    ui->label_min->setText(qStr1.toUtf8().data());
}

void AttributeBased::on_radioButton_16_clicked()
{
    attribute="direction";
    att=6;
    float t;
    float max= (float) INT_MIN;
    float min= (float) INT_MAX;
    for(int i = 0 ; i < i1.NO ;i++){
        t=i1.objectArray[i].fVector.direction;
        if(t>max)max=t;
        if(t<min)min=t;
    }


    QString qStr = QString::number(max);
    QString qStr1 = QString::number(min);
    ui->label_max->setText(qStr.toUtf8().data());
    ui->label_min->setText(qStr1.toUtf8().data());
}

void AttributeBased::on_lineEdit_21_returnPressed()
{

    rminContrast_= ui->lineEdit_21->text().toFloat();
}

void AttributeBased::on_lineEdit_22_returnPressed()
{
    rmaxContrast_= ui->lineEdit_22->text().toFloat();
}

void AttributeBased::on_lineEdit_23_returnPressed()
{
    rminRoughness_= ui->lineEdit_23->text().toFloat();
}

void AttributeBased::on_lineEdit_24_returnPressed()
{
    rmaxRoughness_= ui->lineEdit_24->text().toFloat();
}

void AttributeBased::on_lineEdit_25_returnPressed()
{
    rminCoarseness_= ui->lineEdit_25->text().toFloat();
}

void AttributeBased::on_lineEdit_26_returnPressed()
{
    rmaxCoarseness_= ui->lineEdit_26->text().toFloat();
}

void AttributeBased::on_lineEdit_27_returnPressed()
{
    rminDirection_= ui->lineEdit_27->text().toFloat();
}

void AttributeBased::on_lineEdit_28_returnPressed()
{
    rmaxDirection_= ui->lineEdit_28->text().toFloat();
}

void AttributeBased::on_pushButton_2_clicked()
{
    QString text_number;
    text_number = text_number.setNum(maxDirection);
    ui->lineEdit_28->setText(text_number);

    text_number = text_number.setNum(minDirection);
    ui->lineEdit_27->setText(text_number);
}

void AttributeBased::on_pushButton_3_clicked()
{
    QString text_number;
    text_number = text_number.setNum(maxCoarseness);
    ui->lineEdit_26->setText(text_number);

    text_number = text_number.setNum(minCoarseness);
    ui->lineEdit_25->setText(text_number);
}

void AttributeBased::on_pushButton_4_clicked()
{
    QString text_number;
    text_number = text_number.setNum(maxRoughness);
    ui->lineEdit_24->setText(text_number);

    text_number = text_number.setNum(minRoughness);
    ui->lineEdit_23->setText(text_number);
}

void AttributeBased::on_pushButton_5_clicked()
{
    QString text_number;

    text_number = text_number.setNum(maxContrast);
    ui->lineEdit_22->setText(text_number);

    text_number = text_number.setNum(minContrast);
    ui->lineEdit_21->setText(text_number);
}

void AttributeBased::on_pushButton_6_clicked()
{
    QString text_number;

    text_number = text_number.setNum(maxPerimeter);
    ui->lineEdit_20->setText(text_number);

    text_number = text_number.setNum(minPerimeter);
    ui->lineEdit_19->setText(text_number);


}

void AttributeBased::on_pushButton_7_clicked()
{
    QString text_number;

    text_number = text_number.setNum(maxStdBlue);
    ui->lineEdit_18->setText(text_number);

    text_number = text_number.setNum(minStdBlue);
    ui->lineEdit_17->setText(text_number);
}

void AttributeBased::on_pushButton_8_clicked()
{
    QString text_number;
    text_number = text_number.setNum(maxStdGreen);
    ui->lineEdit_16->setText(text_number);

    text_number = text_number.setNum(minStdGreen);
    ui->lineEdit_15->setText(text_number);

}

void AttributeBased::on_pushButton_9_clicked()
{
    QString text_number;
    text_number = text_number.setNum(maxStdRed);
    ui->lineEdit_14->setText(text_number);

    text_number = text_number.setNum(minStdRed);
    ui->lineEdit_13->setText(text_number);

}

void AttributeBased::on_pushButton_10_clicked()
{
    QString text_number;
    text_number = text_number.setNum(maxMeanBlue);
    ui->lineEdit_12->setText(text_number);

    text_number = text_number.setNum(minMeanBlue);
    ui->lineEdit_11->setText(text_number);

}

void AttributeBased::on_pushButton_11_clicked()
{
    QString text_number;

    text_number = text_number.setNum(maxMeanGreen);
    ui->lineEdit_10->setText(text_number);

    text_number = text_number.setNum(minMeanGreen);
    ui->lineEdit_9->setText(text_number);

}

void AttributeBased::on_pushButton_12_clicked()
{
    QString text_number;
    text_number = text_number.setNum(maxMeanRed);
    ui->lineEdit_8->setText(text_number);

    text_number = text_number.setNum(minMeanRed);
    ui->lineEdit_7->setText(text_number);
}

void AttributeBased::on_pushButton_13_clicked()
{
    QString text_number;
    text_number = text_number.setNum(maxCompactness);
    ui->lineEdit_6->setText(text_number);

    text_number = text_number.setNum(minCompactness);
    ui->lineEdit_5->setText(text_number);
}

void AttributeBased::on_pushButton_14_clicked()
{
    QString text_number;

    text_number = text_number.setNum(maxRoundness);
    ui->lineEdit_4->setText(text_number);


    text_number = text_number.setNum(minRoundness);
    ui->lineEdit_3->setText(text_number);
}

void AttributeBased::on_pushButton_15_clicked()
{
    QString text_number;
    text_number = text_number.setNum(maxArea);
    ui->lineEdit_2->setText(text_number);

    text_number = text_number.setNum(minArea);
    ui->lineEdit->setText(text_number);
}

void AttributeBased::on_pushButton_16_clicked()
{
    QString text_number;

    text_number = text_number.setNum(maxArea);
    ui->lineEdit_2->setText(text_number);

    text_number = text_number.setNum(minArea);
    ui->lineEdit->setText(text_number);

    text_number = text_number.setNum(maxRoundness);
    ui->lineEdit_4->setText(text_number);


    text_number = text_number.setNum(minRoundness);
    ui->lineEdit_3->setText(text_number);


    text_number = text_number.setNum(maxCompactness);
    ui->lineEdit_6->setText(text_number);

    text_number = text_number.setNum(minCompactness);
    ui->lineEdit_5->setText(text_number);

    text_number = text_number.setNum(maxMeanRed);
    ui->lineEdit_8->setText(text_number);

    text_number = text_number.setNum(minMeanRed);
    ui->lineEdit_7->setText(text_number);

    text_number = text_number.setNum(maxMeanGreen);
    ui->lineEdit_10->setText(text_number);

    text_number = text_number.setNum(minMeanGreen);
    ui->lineEdit_9->setText(text_number);

    text_number = text_number.setNum(maxMeanBlue);
    ui->lineEdit_12->setText(text_number);

    text_number = text_number.setNum(minMeanBlue);
    ui->lineEdit_11->setText(text_number);

    text_number = text_number.setNum(maxStdRed);
    ui->lineEdit_14->setText(text_number);

    text_number = text_number.setNum(minStdRed);
    ui->lineEdit_13->setText(text_number);

    text_number = text_number.setNum(maxStdGreen);
    ui->lineEdit_16->setText(text_number);

    text_number = text_number.setNum(minStdGreen);
    ui->lineEdit_15->setText(text_number);

    text_number = text_number.setNum(maxStdBlue);
    ui->lineEdit_18->setText(text_number);

    text_number = text_number.setNum(minStdBlue);
    ui->lineEdit_17->setText(text_number);

    text_number = text_number.setNum(maxPerimeter);
    ui->lineEdit_20->setText(text_number);

    text_number = text_number.setNum(minPerimeter);
    ui->lineEdit_19->setText(text_number);


    text_number = text_number.setNum(maxContrast);
    ui->lineEdit_22->setText(text_number);

    text_number = text_number.setNum(minContrast);
    ui->lineEdit_21->setText(text_number);


    text_number = text_number.setNum(maxRoughness);
    ui->lineEdit_24->setText(text_number);

    text_number = text_number.setNum(minRoughness);
    ui->lineEdit_23->setText(text_number);

    text_number = text_number.setNum(maxCoarseness);
    ui->lineEdit_26->setText(text_number);

    text_number = text_number.setNum(minCoarseness);
    ui->lineEdit_25->setText(text_number);

    text_number = text_number.setNum(maxDirection);
    ui->lineEdit_28->setText(text_number);

    text_number = text_number.setNum(minDirection);
    ui->lineEdit_27->setText(text_number);

    text_number = text_number.setNum(classmax);
    ui->lineEdit_30->setText(text_number);

    text_number = text_number.setNum(classmin);
    ui->lineEdit_29->setText(text_number);

}

void AttributeBased::on_radioButton_17_clicked()
{
    attribute="classNumber";
    att=15;
    float t;
    int max=INT_MIN;
    int min=INT_MAX;
    for(int i = 0 ; i < i1.NO ;i++){
        t=i1.objectArray[i].fVector.classNumber;
        if(t>max)max=t;
        if(t<min)min=t;
    }


    QString qStr = QString::number(max);
    QString qStr1 = QString::number(min);
    ui->label_max->setText(qStr.toUtf8().data());
    ui->label_min->setText(qStr1.toUtf8().data());
}

void AttributeBased::on_lineEdit_29_returnPressed()
{
    rminClass_ = ui->lineEdit_29->text().toInt();
}

void AttributeBased::on_lineEdit_30_returnPressed()
{
    rmaxClass_ = ui->lineEdit_30->text().toInt();
}

void AttributeBased::on_pushButton_17_clicked()
{
    QString text_number;
    text_number = text_number.setNum(classmax);
    ui->lineEdit_30->setText(text_number);

    text_number = text_number.setNum(classmin);
    ui->lineEdit_29->setText(text_number);
}

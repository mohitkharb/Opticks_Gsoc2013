#ifndef PAIN2IMG_H
#define PAIN2IMG_H

#include <QWidget>

class pain2img : public QWidget
{
    Q_OBJECT
public:

    explicit pain2img(std::vector<int> ts2,int ***data,int row,int col,int band,char *str, QWidget *parent = 0);
    std::vector<int> pix_;
    ~pain2img();
    char* file_1;
    char* file_2;
    int row,col;
    int ***data;
    int band;
signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);

};

#endif // PAIN2IMG_H

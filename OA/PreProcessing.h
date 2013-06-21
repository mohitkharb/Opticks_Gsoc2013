

#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <QtCore/QObject>

#include "ViewerShell.h"

class MainWindow;

class PreProcessing : public QObject, public ViewerShell
{
	Q_OBJECT
public:
	int Rows;
	int Cols;
	std::vector<double> dataR;
	std::vector<double> dataG;
	std::vector<double> dataB;
	
	
	MainWindow * w;
	
   PreProcessing();
   ~PreProcessing();
   
   bool getInputSpecification(PlugInArgList*& pInArgList);
   bool getOutputSpecification(PlugInArgList*& pOutArgList);
   bool execute(PlugInArgList* pInArgList, PlugInArgList* pOutArgList);

   protected:
   QWidget* getWidget() const;

   protected slots:
   void dialogClosed();



};

#endif

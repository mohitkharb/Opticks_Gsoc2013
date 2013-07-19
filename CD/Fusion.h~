

#ifndef CHANGEDETECTION_H
#define CHANGEDETECTION_H

#include <QtCore/QObject>

#include "ViewerShell.h"

class MainWindow_cd;

class changeDetection : public QObject, public ViewerShell
{
	Q_OBJECT
public:

	
	MainWindow_cd* w;
	
   changeDetection();
   ~changeDetection();
   

   bool execute(PlugInArgList* pInArgList, PlugInArgList* pOutArgList);

   protected:
   QWidget* getWidget() const;

   protected slots:
   void dialogClosed();



};

#endif
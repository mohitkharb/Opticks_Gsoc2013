

#ifndef MISSINGDATA_H
#define MISSINGDATA_H

#include <QtCore/QObject>

#include "ViewerShell.h"

class MainWindow_cr;

class missing_data : public QObject, public ViewerShell
{
	Q_OBJECT
public:

	
	MainWindow_cr * w;
	
   missing_data();
   ~missing_data();
   

   bool execute(PlugInArgList* pInArgList, PlugInArgList* pOutArgList);

   protected:
   QWidget* getWidget() const;

   protected slots:
   void dialogClosed();



};

#endif

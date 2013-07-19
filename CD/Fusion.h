

#ifndef FUSION_H
#define FUSION_H

#include <QtCore/QObject>

#include "ViewerShell.h"

class MainWindow_fusion;

class Fusion : public QObject, public ViewerShell
{
	Q_OBJECT
public:

	
	MainWindow_fusion * w;
	
   Fusion();
   ~Fusion();
   

   bool execute(PlugInArgList* pInArgList, PlugInArgList* pOutArgList);

   protected:
   QWidget* getWidget() const;

   protected slots:
   void dialogClosed();



};

#endif

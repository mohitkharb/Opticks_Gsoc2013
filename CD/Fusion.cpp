
/*
 * The information in this file is
 * subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#include "AppConfig.h"
#include "AppVerify.h"
#include "DataAccessor.h"
#include "DataAccessorImpl.h"
#include "DataRequest.h"
#include "MessageLogResource.h"
#include "ObjectResource.h"
#include "PlugInArgList.h"
#include "PlugInManagerServices.h"
#include "PlugInRegistration.h"
#include "PlugInResource.h"
#include "Progress.h"
#include <vector>
#include "RasterDataDescriptor.h"
#include "RasterElement.h"
#include "StringUtilities.h"
#include "switchOnEncoding.h"
#include <limits>
#include "Fusion.h"
#include "mainwindow_fusion.h"
#include "DesktopServices.h"
#include "qt4\QtCore\qfile.h"
#include <qt4\QtCore\qtextstream.h>
#include <Qt\qdir.h>
#include <QtGui>
#include <Qt\qprocess.h>
#include <QDir>
#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include <string>
#include<sstream>
#include <QDebug>
#include<QList>

REGISTER_PLUGIN_BASIC(OpticksCDFPlugin, Fusion);

Fusion::Fusion() 
	:w(NULL)
{
	
 
   ViewerShell::setName("Image data fusion");
   setCreator("Mohit Kumar");
   setType("Sample");
   setCopyright("Copyright (C) 2012, Lab For Spatial Informatics");
   setProductionStatus(false);
   setDescription("Verifies Requirement 324.");
   setShortDescription("Verifies Requirement 324.");
   setMenuLocation("[CDF]\\Image Fusion");
	setDescriptorId("{DF7B5AEF-B156-40ae-8DF6-6B3D5B6B3D5BC964E6}");
   setWizardSupported(false);
   
}

Fusion::~Fusion()
{
}



bool Fusion::execute(PlugInArgList* pInArgList, PlugInArgList* pOutArgList)
{   
  if (w == NULL)
  {   
	  Service<DesktopServices> pDesktop;
	  w = new MainWindow_fusion(pDesktop->getMainWidget());
	  w->connect(w,SIGNAL(finished(int)),SLOT(dialogClosed()));
	  
  }
   //   VERIFYNR(w, SIGNAL(finished(int)), this, SLOT(dialogClosed()));
   

   	   w->show();
	   

	

  

   return true;

}


QWidget* Fusion::getWidget() const
{
   return w;
	
}

void Fusion::dialogClosed()
{
   abort();
}

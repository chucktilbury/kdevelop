/***************************************************************************

                    kappwizard.cpp - the kde-application-wizard
                             -------------------                                         

    begin                : 9 Sept 1998                                        
    copyright            : (C) 1998 by Stefan Heidrich                         
    email                : sheidric@rz.uni-potsdam.de                                     
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "ckappwizard.h"
#include "debug.h"
#include <kmsgbox.h>

CKAppWizard::CKAppWizard(QWidget* parent,const char* name,QString author_name,QString author_email) : KWizard(parent,name,true){
  q = new KShellProcess();
  gen_prj = false;
  setCaption(i18n("Application Wizard"));
  init();
  initPages();
  m_author_email = author_email;
  m_author_name = author_name;
  cerr << ":" << m_author_name << ":";
  cerr << ":" << m_author_email << ":";
  
  slotDefaultClicked();    
}

CKAppWizard::~CKAppWizard () {
  
}

void CKAppWizard::init(){
  
  setFixedSize(515,530);
  setCancelButton();
  cancelButton = new QButton();
  cancelButton = getCancelButton();
  QToolTip::add(cancelButton,i18n("exit the CKAppWizard"));
  connect(this,SIGNAL(cancelclicked()),SLOT(slotAppEnd()));
  setOkButton(i18n("Create"));
  okButton = new QButton();
  okButton = getOkButton();
  QToolTip::add(okButton,i18n("creating the project"));
  connect(this,SIGNAL(okclicked()),SLOT(slotOkClicked()));
  setDefaultButton();
  defaultButton = new QButton();
  defaultButton = getDefaultButton();
  QToolTip::add(defaultButton,i18n("set all changes back"));
  connect(this,SIGNAL(defaultclicked(int)),SLOT(slotDefaultClicked()));
  setEnableArrowButtons(true);
}

void CKAppWizard::initPages(){
  
  // create the first page
  page0 = new KWizardPage;
  widget0 = new QWidget(this);
  page0->w = widget0;
  page0->title = (i18n("Applications"));
  page0->enabled = true;
  addPage(page0);
  
  QPixmap iconpm;    
  QPixmap minipm;
  QPixmap pm;
  pm.load(KApplication::kde_datadir() + "/kdevelop/pics/kAppWizard.bmp");
  
  // create a widget and paint a picture on it
  widget1a = new QWidget( widget0, "widget1a" );
  widget1a->setGeometry( 0, 0, 500, 160 );
  widget1a->setMinimumSize( 0, 0 );
  widget1a->setMaximumSize( 32767, 32767 );
  widget1a->setFocusPolicy( QWidget::NoFocus );
  widget1a->setBackgroundMode( QWidget::PaletteBackground );
  widget1a->setFontPropagation( QWidget::NoChildren );
  widget1a->setPalettePropagation( QWidget::NoChildren );
  widget1a->setBackgroundPixmap (pm);
  
  applications = new QListView( widget0, "applications" );
  applications->setGeometry( 20, 170, 170, 250 );
  applications->setMinimumSize( 0, 0 );
  applications->setMaximumSize( 32767, 32767 );
  applications->setFocusPolicy( QWidget::TabFocus );
  applications->setBackgroundMode( QWidget::PaletteBackground );
  applications->setFontPropagation( QWidget::NoChildren );
  applications->setPalettePropagation( QWidget::NoChildren );
  applications->setResizePolicy( QScrollView::Manual );
  applications->setVScrollBarMode( QScrollView::Auto );
  applications->setHScrollBarMode( QScrollView::Auto );
  applications->setTreeStepSize( 20 );
  applications->setMultiSelection( FALSE );
  applications->setAllColumnsShowFocus( FALSE );
  applications->setItemMargin( 1 );
  applications->setRootIsDecorated( TRUE );
  applications->addColumn( "Applications", -1 );
  applications->setColumnWidthMode( 0, QListView::Maximum );
  applications->setColumnAlignment( 0, 1 );
  applications->setSorting (-1,FALSE);
  applications->header()->hide();
  
  // create another widget for a picture
  widget1b = new QWidget( widget0, "widget1b" );
  widget1b->setGeometry( 255, 180, 190, 140 );
  widget1b->setMinimumSize( 0, 0 );
  widget1b->setMaximumSize( 32767, 32767 );
  widget1b->setFocusPolicy( QWidget::NoFocus );
  widget1b->setBackgroundMode( QWidget::PaletteBackground );
  widget1b->setFontPropagation( QWidget::NoChildren );
  widget1b->setPalettePropagation( QWidget::NoChildren );
  
  apphelp = new QLabel( widget0, "apphelp" );
  apphelp->setGeometry( 230, 330, 240, 90 );
  apphelp->setMinimumSize( 0, 0 );
  apphelp->setMaximumSize( 32767, 32767 );
  apphelp->setFocusPolicy( QWidget::NoFocus );
  apphelp->setBackgroundMode( QWidget::PaletteBackground );
  apphelp->setFontPropagation( QWidget::NoChildren );
  apphelp->setPalettePropagation( QWidget::NoChildren );
  apphelp->setText( "Label:" );
  apphelp->setAlignment( 289 );
  apphelp->setMargin( -1 );
  
  othersentry = new QListViewItem (applications, "others");
  othersentry->setExpandable (true);
  othersentry->setOpen (TRUE);
  othersentry->sortChildItems (0,FALSE);
  customprojitem = new QListViewItem (othersentry,"custom project");
  
  /*	gtkentry = new QListViewItem (applications, "GTK");
	gtkentry->setExpandable (true);
	gtkentry->setOpen (TRUE);
	gtkentry->sortChildItems (0,FALSE);	
	gtkminiitem = new QListViewItem (gtkentry,"Mini");
	gtknormalitem = new QListViewItem (gtkentry,"Normal");
  */
  ccppentry = new QListViewItem (applications, "Terminal");
  ccppentry->setExpandable (true);
  ccppentry->setOpen (TRUE);
  ccppentry->sortChildItems (0,FALSE);
  cppitem = new QListViewItem (ccppentry,"C++");
  //citem = new QListViewItem (ccppentry,"C");

  qtentry = new QListViewItem (applications, "QT");
  qtentry->setExpandable (true);
  qtentry->setOpen (TRUE);
  qtentry->sortChildItems (0,FALSE);
  //qtminiitem = new QListViewItem (qtentry,"Mini");
  qtnormalitem = new QListViewItem (qtentry,"Normal");
  
  kdeentry = new QListViewItem (applications,"KDE");
  kdeentry->setExpandable (true);
  kdeentry->setOpen (TRUE);
  kdeentry->sortChildItems (0,FALSE);
  //komitem = new QListViewItem (kdeentry,"KOM");
  //corbaitem = new QListViewItem (kdeentry,"Corba");
  kdeminiitem = new QListViewItem (kdeentry,"Mini");
  kdenormalitem = new QListViewItem (kdeentry,"Normal");
  
  applications->setFrameStyle( QListView::Panel | QListView::Sunken );
  applications->setLineWidth( 2 );
  
  separator0 = new KSeparator (widget0);
  separator0->setGeometry(0,160,515,5);
  
  connect (applications,SIGNAL(selectionChanged ()),SLOT(slotApplicationClicked()));
  
  /************************************************************/
  
  // create the second page
  page1 = new KWizardPage;
  widget1 = new QWidget(this);
  page1->w = widget1;
  page1->title = (i18n("Generate settings"));
  page1->enabled = true;
  addPage(page1);  
  
  name = new QLabel( widget1, "name" );
  name->setGeometry( 30, 10, 100, 30 );
  name->setMinimumSize( 0, 0 );
  name->setMaximumSize( 32767, 32767 );
  name->setFocusPolicy( QWidget::NoFocus );
  name->setBackgroundMode( QWidget::PaletteBackground );
  name->setFontPropagation( QWidget::NoChildren );
  name->setPalettePropagation( QWidget::NoChildren );
  name->setText( i18n("Projectname:") );
  name->setAlignment( 289 );
  name->setMargin( -1 );

  nameline = new QLineEdit( widget1, "nameline" );
  nameline->setGeometry( 140, 10, 290, 30 );
  nameline->setMinimumSize( 0, 0 );
  nameline->setMaximumSize( 32767, 32767 );
  nameline->setFocusPolicy( QWidget::StrongFocus );
  nameline->setBackgroundMode( QWidget::PaletteBase );
  nameline->setFontPropagation( QWidget::NoChildren );
  nameline->setPalettePropagation( QWidget::NoChildren );
  nameline->setText( "" );
  nameline->setMaxLength( 32767 );
  nameline->setEchoMode( QLineEdit::Normal );
  nameline->setFrame( TRUE );

  directory = new QLabel( widget1, "directory" );
  directory->setGeometry( 30, 50, 100, 30 );
  directory->setMinimumSize( 0, 0 );
  directory->setMaximumSize( 32767, 32767 );
  directory->setFocusPolicy( QWidget::NoFocus );
  directory->setBackgroundMode( QWidget::PaletteBackground );
  directory->setFontPropagation( QWidget::NoChildren );
  directory->setPalettePropagation( QWidget::NoChildren );
  directory->setText( i18n("Projectdirectory:") );
  directory->setAlignment( 289 );
  directory->setMargin( -1 );

  directoryline = new QLineEdit( widget1, "directoryline" );
  directoryline->setGeometry( 140, 50, 290, 30 );
  directoryline->setMinimumSize( 0, 0 );
  directoryline->setMaximumSize( 32767, 32767 );
  directoryline->setFocusPolicy( QWidget::StrongFocus );
  directoryline->setBackgroundMode( QWidget::PaletteBase );
  directoryline->setFontPropagation( QWidget::NoChildren );
  directoryline->setPalettePropagation( QWidget::NoChildren );
  directoryline->setText( "" );
  directoryline->setMaxLength( 32767 );
  directoryline->setEchoMode( QLineEdit::Normal );
  directoryline->setFrame( TRUE );

  directoryload = new QPushButton( widget1, "directoryload" );
  directoryload->setGeometry( 440, 50, 30, 30 );
  directoryload->setMinimumSize( 0, 0 );
  directoryload->setMaximumSize( 32767, 32767 );
  directoryload->setFocusPolicy( QWidget::TabFocus );
  directoryload->setBackgroundMode( QWidget::PaletteBackground );
  directoryload->setFontPropagation( QWidget::NoChildren );
  directoryload->setPalettePropagation( QWidget::NoChildren );
	QPixmap pix;
  pix.load(KApplication::kde_datadir() + "/kdevelop/toolbar/open.xpm");
  directoryload->setPixmap(pix);
  directoryload->setAutoRepeat( FALSE );
  directoryload->setAutoResize( FALSE );
  
  versionnumber = new QLabel( widget1, "versionnumber" );
  versionnumber->setGeometry( 30, 90, 100, 30 );
  versionnumber->setMinimumSize( 0, 0 );
  versionnumber->setMaximumSize( 32767, 32767 );
  versionnumber->setFocusPolicy( QWidget::NoFocus );
  versionnumber->setBackgroundMode( QWidget::PaletteBackground );
  versionnumber->setFontPropagation( QWidget::NoChildren );
  versionnumber->setPalettePropagation( QWidget::NoChildren );
  versionnumber->setText( i18n("Versionnumber:") );
  versionnumber->setAlignment( 289 );
  versionnumber->setMargin( -1 );

  versionline = new QLineEdit( widget1, "versionline" );
  versionline->setGeometry( 140, 90, 290, 30 );
  versionline->setMinimumSize( 0, 0 );
  versionline->setMaximumSize( 32767, 32767 );
  versionline->setFocusPolicy( QWidget::StrongFocus );
  versionline->setBackgroundMode( QWidget::PaletteBase );
  versionline->setFontPropagation( QWidget::NoChildren );
  versionline->setPalettePropagation( QWidget::NoChildren );
  versionline->setText( "" );
  versionline->setMaxLength( 32767 );
  versionline->setEchoMode( QLineEdit::Normal );
  versionline->setFrame( TRUE );

  authorname = new QLabel( widget1, "authorname" );
  authorname->setGeometry( 30, 130, 100, 30 );
  authorname->setMinimumSize( 0, 0 );
  authorname->setMaximumSize( 32767, 32767 );
  authorname->setFocusPolicy( QWidget::NoFocus );
  authorname->setBackgroundMode( QWidget::PaletteBackground );
  authorname->setFontPropagation( QWidget::NoChildren );
  authorname->setPalettePropagation( QWidget::NoChildren );
  authorname->setText( i18n("Author:") );
  authorname->setAlignment( 289 );
  authorname->setMargin( -1 );

  authorline = new QLineEdit( widget1, "authorline" );
  authorline->setGeometry( 140, 130, 290, 30 );
  authorline->setMinimumSize( 0, 0 );
  authorline->setMaximumSize( 32767, 32767 );
  authorline->setFocusPolicy( QWidget::StrongFocus );
  authorline->setBackgroundMode( QWidget::PaletteBase );
  authorline->setFontPropagation( QWidget::NoChildren );
  authorline->setPalettePropagation( QWidget::NoChildren );
  authorline->setText( "" );
  authorline->setMaxLength( 32767 );
  authorline->setEchoMode( QLineEdit::Normal );
  authorline->setFrame( TRUE );
  
  email = new QLabel( widget1, "email" );
  email->setGeometry( 30, 170, 100, 30 );
  email->setMinimumSize( 0, 0 );
  email->setMaximumSize( 32767, 32767 );
  email->setFocusPolicy( QWidget::NoFocus );
  email->setBackgroundMode( QWidget::PaletteBackground );
  email->setFontPropagation( QWidget::NoChildren );
  email->setPalettePropagation( QWidget::NoChildren );
  email->setText( i18n("Email:") );
  email->setAlignment( 289 );
  email->setMargin( -1 );

  emailline = new QLineEdit( widget1, "emailline" );
  emailline->setGeometry( 140, 170, 290, 30 );
  emailline->setMinimumSize( 0, 0 );
  emailline->setMaximumSize( 32767, 32767 );
  emailline->setFocusPolicy( QWidget::StrongFocus );
  emailline->setBackgroundMode( QWidget::PaletteBase );
  emailline->setFontPropagation( QWidget::NoChildren );
  emailline->setPalettePropagation( QWidget::NoChildren );
  emailline->setText( "" );
  emailline->setMaxLength( 32767 );
  emailline->setEchoMode( QLineEdit::Normal );
  emailline->setFrame( TRUE );

  generatesource = new QCheckBox( widget1, "generatesource" );
  generatesource->setGeometry( 30, 220, 440, 30 );
  generatesource->setMinimumSize( 0, 0 );
  generatesource->setMaximumSize( 32767, 32767 );
  generatesource->setFocusPolicy( QWidget::TabFocus );
  generatesource->setBackgroundMode( QWidget::PaletteBackground );
  generatesource->setFontPropagation( QWidget::NoChildren );
  generatesource->setPalettePropagation( QWidget::NoChildren );
  generatesource->setText( i18n("generate sources and headers") );
  generatesource->setAutoRepeat( FALSE );
  generatesource->setAutoResize( FALSE );

  gnufiles = new QCheckBox( widget1, "gnufiles" );
  gnufiles->setGeometry( 30, 270, 440, 30 );
  gnufiles->setMinimumSize( 0, 0 );
  gnufiles->setMaximumSize( 32767, 32767 );
  gnufiles->setFocusPolicy( QWidget::TabFocus );
  gnufiles->setBackgroundMode( QWidget::PaletteBackground );
  gnufiles->setFontPropagation( QWidget::NoChildren );
  gnufiles->setPalettePropagation( QWidget::NoChildren );
  gnufiles->setText( i18n("GNU-Standard-Files (INSTALL,README,COPYING...)" ));
  gnufiles->setAutoRepeat( FALSE );
  gnufiles->setAutoResize( FALSE );

  userdoc = new QCheckBox( widget1, "userdoc" );
  userdoc->setGeometry( 30, 300, 330, 30 );
  userdoc->setMinimumSize( 0, 0 );
  userdoc->setMaximumSize( 32767, 32767 );
  userdoc->setFocusPolicy( QWidget::TabFocus );
  userdoc->setBackgroundMode( QWidget::PaletteBackground );
  userdoc->setFontPropagation( QWidget::NoChildren );
  userdoc->setPalettePropagation( QWidget::NoChildren );
  userdoc->setText( i18n("User-Documentation") );
  userdoc->setAutoRepeat( FALSE );
  userdoc->setAutoResize( FALSE );

  apidoc = new QCheckBox( widget1, "apidoc" );
  apidoc->setGeometry( 30, 330, 200, 30 );
  apidoc->setMinimumSize( 0, 0 );
  apidoc->setMaximumSize( 32767, 32767 );
  apidoc->setFocusPolicy( QWidget::TabFocus );
  apidoc->setBackgroundMode( QWidget::PaletteBackground );
  apidoc->setFontPropagation( QWidget::NoChildren );
  apidoc->setPalettePropagation( QWidget::NoChildren );
  apidoc->setText( i18n("API-Documentation") );
  apidoc->setAutoRepeat( FALSE );
  apidoc->setAutoResize( FALSE );

  lsmfile = new QCheckBox( widget1, "lsmfile" );
  lsmfile->setGeometry( 30, 360, 340, 30 );
  lsmfile->setMinimumSize( 0, 0 );
  lsmfile->setMaximumSize( 32767, 32767 );
  lsmfile->setFocusPolicy( QWidget::TabFocus );
  lsmfile->setBackgroundMode( QWidget::PaletteBackground );
  lsmfile->setFontPropagation( QWidget::NoChildren );
  lsmfile->setPalettePropagation( QWidget::NoChildren );
  lsmfile->setText( i18n("lsm-File - Linux Software Map") );
  lsmfile->setAutoRepeat( FALSE );
  lsmfile->setAutoResize( FALSE );

  datalink = new QCheckBox( widget1, "datalink" );
  datalink->setGeometry( 30, 390, 200, 30 );
  datalink->setMinimumSize( 0, 0 );
  datalink->setMaximumSize( 32767, 32767 );
  datalink->setFocusPolicy( QWidget::TabFocus );
  datalink->setBackgroundMode( QWidget::PaletteBackground );
  datalink->setFontPropagation( QWidget::NoChildren );
  datalink->setPalettePropagation( QWidget::NoChildren );
  datalink->setText( i18n(".kdelnk-File") );
  datalink->setAutoRepeat( FALSE );
  datalink->setAutoResize( FALSE );
  
  progicon = new QCheckBox( widget1, "progicon" );
  progicon->setGeometry( 290, 330, 110, 30 );
  progicon->setMinimumSize( 0, 0 );
  progicon->setMaximumSize( 32767, 32767 );
  progicon->setFocusPolicy( QWidget::TabFocus );
  progicon->setBackgroundMode( QWidget::PaletteBackground );
  progicon->setFontPropagation( QWidget::NoChildren );
  progicon->setPalettePropagation( QWidget::NoChildren );
  progicon->setText( i18n("Program-Icon") );
  progicon->setAutoRepeat( FALSE );
  progicon->setAutoResize( FALSE );

  iconload = new QPushButton( widget1, "iconload" );
  iconload->setGeometry( 410, 310, 60, 60 );
  iconload->setMinimumSize( 0, 0 );
  iconload->setMaximumSize( 32767, 32767 );
  iconload->setFocusPolicy( QWidget::TabFocus );
  iconload->setBackgroundMode( QWidget::PaletteBackground );
  iconload->setFontPropagation( QWidget::NoChildren );
  iconload->setPalettePropagation( QWidget::NoChildren );
  iconload->setText( "" );
  iconload->setAutoRepeat( FALSE );
  iconload->setAutoResize( FALSE );

  miniicon = new QCheckBox( widget1, "miniicon" );
  miniicon->setGeometry( 290, 390, 110, 30 );
  miniicon->setMinimumSize( 0, 0 );
  miniicon->setMaximumSize( 32767, 32767 );
  miniicon->setFocusPolicy( QWidget::TabFocus );
  miniicon->setBackgroundMode( QWidget::PaletteBackground );
  miniicon->setFontPropagation( QWidget::NoChildren );
  miniicon->setPalettePropagation( QWidget::NoChildren );
  miniicon->setText( i18n("Mini-Icon") );
  miniicon->setAutoRepeat( FALSE );
  miniicon->setAutoResize( FALSE );
  
  miniload = new QPushButton( widget1, "miniload" );
  miniload->setGeometry( 440, 390, 30, 30 );
  miniload->setMinimumSize( 0, 0 );
  miniload->setMaximumSize( 32767, 32767 );
  miniload->setFocusPolicy( QWidget::TabFocus );
  miniload->setBackgroundMode( QWidget::PaletteBackground );
  miniload->setFontPropagation( QWidget::NoChildren );
  miniload->setPalettePropagation( QWidget::NoChildren );
  miniload->setText( "" );
  miniload->setAutoRepeat( FALSE );
  miniload->setAutoResize( FALSE );
  
  separator1 = new KSeparator (widget1);
  separator1->setGeometry(0,210,515,5);
  
  separator2 = new KSeparator (widget1);
  separator2->setGeometry(0,255,515,5);
  
  KQuickHelp::add(name,
		  KQuickHelp::add(nameline,
				  i18n("Enter the Project's name here. This is\n"
				       "also the directory name under which your Project\n"
				       "will be created.")));
  KQuickHelp::add(directory,
		  KQuickHelp::add(directoryline,
				  KQuickHelp::add(directoryload,
						  i18n("Enter the toplevel-directory the project\n"
						       "will be stored. This <b>must</b> be an\n"
						       "<b>existing directory !!"))));
  KQuickHelp::add(versionnumber,
		  KQuickHelp::add(versionline,
				  i18n("Enter the Version number here.\n"
				       "The number will be used in the about-dialog\n"
				       "and in the main widget's topbar.")));
  KQuickHelp::add(authorname,
		  KQuickHelp::add(authorline,
				  i18n("Enter your name here for the about dialog.\n"
				       "You may also consider using the Team-name.")));
  KQuickHelp::add(email,
		  KQuickHelp::add(emailline,
				  i18n("Enter your email adress here, so users\n"
				       "can send you bug reports easiely ;-))")));
  
  KQuickHelp::add(apidoc, i18n("Check this if you wish to have your API\n"
			       "Documentation generated automatically"));
  KQuickHelp::add(userdoc, i18n("Check this if you wish a general User\n"
				"Documentation to be generated automatically."));
  KQuickHelp::add(lsmfile, i18n("Check this to create a lsm-File"));
  KQuickHelp::add(gnufiles, i18n("Check this if you wish to have\n"
				"the GNU-Standard files created like\n"
				 "INSTALL and README"));
  KQuickHelp::add(progicon, i18n("Check this to create a sample icon for\n"
				 "your program. We suggest to do so and just\n"
				 "change it's view by using KIconEdit."));
  KQuickHelp::add(miniicon, i18n("Check this to create a sample mini-icon\n"
				 "for your program. We suggest to do so and just\n"
				 "change it's view by using KIconEdit."));
  KQuickHelp::add(datalink, i18n("Check this if you wish to have\n"
				 "a .kdelnk for you application in the\n"
				 "K-Menu.  The default is the Applications-\n"
				 "submenu. You can change this to other menus\n"
				 "by changing the file properties afterwards."));
  KQuickHelp::add(iconload, i18n("Select this if you wish another\n"
				 "already created icon to be used."));
  KQuickHelp::add(miniload, i18n("Select this if you wish another\n"
				 "already created mini-icon to be used."));
  
  
  

  connect(nameline,SIGNAL(textChanged(const char*)),SLOT(slotProjectnameEntry()));
  connect(directoryload,SIGNAL(clicked()),SLOT(slotDirDialogClicked()));
  connect(miniload,SIGNAL(clicked()),SLOT(slotMiniIconButtonClicked()));
  connect(progicon,SIGNAL(clicked()),SLOT(slotProgIconClicked()));   
  connect(miniicon,SIGNAL(clicked()),SLOT(slotMiniIconClicked()));   
  connect(iconload,SIGNAL(clicked()),SLOT(slotIconButtonClicked()));
					  
  /************************************************************/
  
  // create the thirth page
  page2 = new KWizardPage;
  widget2 = new QWidget(this);
  page2->w = widget2;
  page2->title = (i18n("Headertemplate for .h-files"));
  page2->enabled = true;
  addPage(page2);
  
  hheader = new QCheckBox( widget2, "hheader" );
  hheader->setGeometry( 20, 20, 230, 30 );
  hheader->setMinimumSize( 0, 0 );
  hheader->setMaximumSize( 32767, 32767 );
  hheader->setFocusPolicy( QWidget::TabFocus );
  hheader->setBackgroundMode( QWidget::PaletteBackground );
  hheader->setFontPropagation( QWidget::NoChildren );
  hheader->setPalettePropagation( QWidget::NoChildren );
  hheader->setText( i18n("headertemplate for .h-files") );
  hheader->setAutoRepeat( FALSE );
  hheader->setAutoResize( FALSE );

  hload = new QPushButton( widget2, "hload" );
  hload->setGeometry( 260, 20, 100, 30 );
  hload->setMinimumSize( 0, 0 );
  hload->setMaximumSize( 32767, 32767 );
  hload->setFocusPolicy( QWidget::TabFocus );
  hload->setBackgroundMode( QWidget::PaletteBackground );
  hload->setFontPropagation( QWidget::NoChildren );
  hload->setPalettePropagation( QWidget::NoChildren );
  hload->setText(i18n( "Load..." ));
  hload->setAutoRepeat( FALSE );
  hload->setAutoResize( FALSE );
  
  hnew = new QPushButton( widget2, "hnew" );
  hnew->setGeometry( 380, 20, 100, 30 );
  hnew->setMinimumSize( 0, 0 );
  hnew->setMaximumSize( 32767, 32767 );
  hnew->setFocusPolicy( QWidget::TabFocus );
  hnew->setBackgroundMode( QWidget::PaletteBackground );
  hnew->setFontPropagation( QWidget::NoChildren );
  hnew->setPalettePropagation( QWidget::NoChildren );
  hnew->setText( i18n("New" ));
  hnew->setAutoRepeat( FALSE );
  hnew->setAutoResize( FALSE );
  
  hedit = new KEdit( kapp,widget2 );
  QFont f("fixed",10);
  hedit->setFont(f);
  hedit->setGeometry( 20, 70, 460, 350 );
  hedit->setMinimumSize( 0, 0 );
  hedit->setMaximumSize( 32767, 32767 );
  hedit->setFocusPolicy( QWidget::StrongFocus );
  hedit->setBackgroundMode( QWidget::PaletteBase );
  hedit->setFontPropagation( QWidget::SameFont );
  hedit->setPalettePropagation( QWidget::SameFont );
  hedit->insertLine( "" );
  hedit->setReadOnly( FALSE );
  hedit->setOverwriteMode( FALSE );
  
  QToolTip::add(hload,i18n("you can load another headertemplate here"));
  QToolTip::add(hnew,i18n("you can clear the headertemplate here"));
  QToolTip::add(hedit,i18n("you can edit your headertemplate here"));
  
  KQuickHelp::add(hheader, i18n("Check this if you want a standard\n"
				"headertemplate for your headerfiles"));
  
  
  connect(hheader,SIGNAL(clicked()),SLOT(slotHeaderHeaderClicked()));   
  connect(hload,SIGNAL(clicked()),SLOT(slotHeaderDialogClicked()));
  connect(hnew,SIGNAL(clicked()),SLOT(slotNewHeaderButtonClicked()));

  /************************************************************/
  
  // create the fourth page
  page3 = new KWizardPage;
  widget3 = new QWidget(this);
  page3->w = widget3;
  page3->title = (i18n("Headertemplate for .cpp-files"));
  page3->enabled = true;
  addPage(page3);
  
  cppheader = new QCheckBox( widget3, "cppheader" );
  cppheader->setGeometry( 20, 20, 230, 30 );
  cppheader->setMinimumSize( 0, 0 );
  cppheader->setMaximumSize( 32767, 32767 );
  cppheader->setFocusPolicy( QWidget::TabFocus );
  cppheader->setBackgroundMode( QWidget::PaletteBackground );
  cppheader->setFontPropagation( QWidget::NoChildren );
  cppheader->setPalettePropagation( QWidget::NoChildren );
  cppheader->setText( i18n("headertemplate for .cpp-files") );
  cppheader->setAutoRepeat( FALSE );
  cppheader->setAutoResize( FALSE );

  cppload = new QPushButton( widget3, "cppload" );
  cppload->setGeometry( 260, 20, 100, 30 );
  cppload->setMinimumSize( 0, 0 );
  cppload->setMaximumSize( 32767, 32767 );
  cppload->setFocusPolicy( QWidget::TabFocus );
  cppload->setBackgroundMode( QWidget::PaletteBackground );
  cppload->setFontPropagation( QWidget::NoChildren );
  cppload->setPalettePropagation( QWidget::NoChildren );
  cppload->setText( i18n("Load...") );
  cppload->setAutoRepeat( FALSE );
  cppload->setAutoResize( FALSE );
  
  cppnew = new QPushButton( widget3, "cppnew" );
  cppnew->setGeometry( 380, 20, 100, 30 );
  cppnew->setMinimumSize( 0, 0 );
  cppnew->setMaximumSize( 32767, 32767 );
  cppnew->setFocusPolicy( QWidget::TabFocus );
  cppnew->setBackgroundMode( QWidget::PaletteBackground );
  cppnew->setFontPropagation( QWidget::NoChildren );
  cppnew->setPalettePropagation( QWidget::NoChildren );
  cppnew->setText( i18n("New") );
  cppnew->setAutoRepeat( FALSE );
  cppnew->setAutoResize( FALSE );

  cppedit = new KEdit(kapp,widget3);
  cppedit->setFont(f);
  cppedit->setGeometry( 20, 70, 460, 350 );
  cppedit->setMinimumSize( 0, 0 );
  cppedit->setMaximumSize( 32767, 32767 );
  cppedit->setFocusPolicy( QWidget::StrongFocus );
  cppedit->setBackgroundMode( QWidget::PaletteBase );
  cppedit->setFontPropagation( QWidget::SameFont );
  cppedit->setPalettePropagation( QWidget::SameFont );
  cppedit->insertLine( "" );
  cppedit->setReadOnly( FALSE );
  cppedit->setOverwriteMode( FALSE );
  
  KQuickHelp::add(cppheader, i18n("Check this if you want a standard\n"
				  "cpp-Template for your cpp-files."));
  
  QToolTip::add(cppload,i18n("you can load another cpptemplate here"));
  QToolTip::add(cppnew,i18n("you can clear the cpptemplate here"));
  QToolTip::add(cppedit,i18n("you can edit your cpptemplate here"));
  
  connect(cppheader,SIGNAL(clicked()),SLOT(slotCppHeaderClicked()));   
  connect(cppload,SIGNAL(clicked()),SLOT(slotCppDialogClicked()));
  connect(cppnew,SIGNAL(clicked()),SLOT(slotNewCppButtonClicked()));   

  /************************************************************/
  
  // create the fifth page
  page4 = new KWizardPage;
  widget4 = new QWidget(this);
  page4->w = widget4;
  page4->title = (i18n("Processes"));
  page4->enabled = true;
  addPage(page4);
  
  // create a MultiLineEdit for the processes of kAppWizard
  output = new QMultiLineEdit( widget4, "output" );
  output->setGeometry( 10, 10, 480, 330 );
  output->setMinimumSize( 0, 0 );
  output->setMaximumSize( 32767, 32767 );
  output->setFocusPolicy( QWidget::StrongFocus );
  output->setBackgroundMode( QWidget::PaletteBase );
  output->setFontPropagation( QWidget::SameFont );
  output->setPalettePropagation( QWidget::SameFont );
  output->insertLine( "" );
  output->setReadOnly( FALSE );
  output->setOverwriteMode( FALSE );
  
  errOutput = new QMultiLineEdit( widget4, "errOutput" );
  errOutput->setGeometry( 10, 340, 480, 80 );
  errOutput->setMinimumSize( 0, 0 );
  errOutput->setMaximumSize( 32767, 32767 );
  errOutput->setFocusPolicy( QWidget::StrongFocus );
  errOutput->setBackgroundMode( QWidget::PaletteBase );
  errOutput->setFontPropagation( QWidget::SameFont );
  errOutput->setPalettePropagation( QWidget::SameFont );
  errOutput->insertLine( "" );
  errOutput->setReadOnly( FALSE );
  errOutput->setOverwriteMode( FALSE );
  QFont font("helvetica",10);
  output->setFont(font);
  QToolTip::add(output,i18n("you can see the normal outputs here"));
  errOutput->setFont(font);
  QToolTip::add(errOutput,i18n("you can see all warnings and errormessages here"));
  // go to page 2 then to page 1
  gotoPage(1);  
  gotoPage(0);    
}

// connection to directoryload
void CKAppWizard::slotDirDialogClicked() {
  QString projname;
  dirdialog = new KDirDialog(dir,this,"Directory");
  dirdialog->setCaption (i18n("Directory"));
  dirdialog->show();
  projname = nameline->text();
  dir = dirdialog->dirPath() + projname.lower();
  directoryline->setText(dir);
  dir = dirdialog->dirPath();
  delete (dirdialog);
}

// connection of hload
void CKAppWizard::slotHeaderDialogClicked() {
  headerdialog = new KFileDialog(QDir::homeDirPath(),"*",this,"Headertemplate",true,true);
  headerdialog->setCaption (i18n("Select your template for Header-file headers"));
  if(headerdialog->exec()){
    hedit->loadFile(headerdialog->selectedFile(),cppedit->OPEN_READWRITE);
  }
  delete (headerdialog);
}

// connection of cppload
void CKAppWizard::slotCppDialogClicked() {
  cppdialog = new KFileDialog(QDir::homeDirPath(),"*",this,"Cpptemplate",true,true);
  cppdialog->setCaption(i18n("Select your template for Cpp-file headers"));
  if(cppdialog->exec()){
    cppedit->loadFile(cppdialog->selectedFile(),cppedit->OPEN_READWRITE);
  }
  delete (cppdialog);
}

// connection of hnew
void CKAppWizard::slotNewHeaderButtonClicked() {
  hedit->clear();
}

// connection of cppnew
void CKAppWizard::slotNewCppButtonClicked() {
  cppedit->clear();
}

// connection of this (defaultButton)
void CKAppWizard::slotOkClicked() {
  QDir dir;
  KShellProcess p;
  dir.setPath(directoryline->text());
  if (dir.exists()) {
    if(KMsgBox::yesNo(0,i18n("Directory exists!"),i18n("WARNING!!! If you click 'OK', all files and subdirs will
removed."),KMsgBox::EXCLAMATION,i18n("Ok"),i18n("Cancel"))==2) {       return;
    }
    else {
      p.clearArguments();
      QString copydes = (QString) "rm -r -f " + directoryline->text() + QString ("/");
      p << copydes;
      p.start(KProcess::Block,KProcess::AllOutput);
      okPermited();
    }
  }
  else {
    p.clearArguments();
    QString copydes = (QString ) "rm -r -f " + directoryline->text() + QString ("/");
    p << copydes;
    p.start(KProcess::Block,KProcess::AllOutput);
    okPermited();
  }
}

void CKAppWizard::okPermited() {
  cancelButton->setFixedWidth(75);
  cancelButton->setEnabled(false);
  defaultButton->setEnabled(false);
  setCancelButton(i18n("Exit"));
  errOutput->clear();
  output->clear();
  QDir kdevelop;
  kdevelop.mkdir(QDir::homeDirPath() + "/.kde/share/apps/kdevelop");
  cppedit->setName(QDir::homeDirPath() + "/.kde/share/apps/kdevelop/cpp");
  cppedit->toggleModified(true);
  cppedit->doSave();
  hedit->setName(QDir::homeDirPath() + "/.kde/share/apps/kdevelop/header");
  hedit->toggleModified(true);
  hedit->doSave();
  ofstream entries (QDir::homeDirPath() + "/.kde/share/apps/kdevelop/entries");
  entries << "APPLICATION\n";
  if (kdenormalitem->isSelected()) {
    entries << "kdenormal\n";
  }
  else if (kdeminiitem->isSelected()) {
    entries << "kdemini\n";
  }
  /*  else if (corbaitem->isSelected()) {
      entries << "corba\n";
      }
      else if (komitem->isSelected()) {
      entries << "kom\n";
      }*/
  else if (qtnormalitem->isSelected()) {
    entries << "qtnormal\n";
  }
  /* else if (qtminiitem->isSelected()) {
     entries << "qtmini\n";
     }*/
  else if (cppitem->isSelected()) {
    entries << "cpp\n";
  }
  /*else if (citem->isSelected()) {
    entries << "c\n";
    }
    else if (gtknormalitem->isSelected()) {
    entries << "gtknormal\n";
    }
    else if (gtkminiitem->isSelected()) {
    entries << "gtkmini\n";
    }*/
  else if (customprojitem->isSelected()) {
    entries << "customproj\n";
  }
  entries << "NAME\n";
  entries << nameline->text() << "\n";
  entries << "DIRECTORY\n";
  if(QString(directoryline->text()).right(1) == "/"){
    entries << directoryline->text() << "\n";
  }
  else{
    entries << directoryline->text() << "/" << "\n";
  }
  
  
  entries << "AUTHOR\n";
  entries << authorline->text() << "\n";
  entries << "EMAIL\n";
  entries << emailline->text() << "\n";
  entries << "API\n";
  if (apidoc->isChecked())
    entries << "yes\n";
  else entries << "no\n";
  entries << "USER\n";
  if (userdoc->isChecked())
    entries << "yes\n";
  else entries << "no\n";
  entries << "LSM\n";
  if (lsmfile->isChecked())
    entries << "yes\n";
  else entries << "no\n";
  entries << "GNU\n";
  if (gnufiles->isChecked())
    entries << "yes\n";
  else entries << "no\n";
  entries << "PROGICON\n";
  if (progicon->isChecked()) {
    entries << name1 << "\n";
  }
  else entries << "no\n";
  entries << "MINIICON\n";
  if (miniicon->isChecked())
    entries << name2 << "\n";
  else entries << "no\n";
  entries << "KDELNK\n";
  if (datalink->isChecked())
    entries << "yes\n";
  else entries << "no\n";
  entries << "HEADER\n";
  if (hheader->isChecked())
    entries << "yes\n";
  else entries << "no\n";
  entries << "CPP\n";
  if (cppheader->isChecked())
    entries << "yes\n";
  else entries << "no\n";
  entries << "VERSION\n";
  entries << versionline->text() << "\n";
  
  namelow = nameline->text();
  namelow = namelow.lower();
  QDir directory;
  directory.mkdir(directoryline->text() + QString("/"));
  KShellProcess p;
  QString copysrc;
  QString copydes = directoryline->text() + QString ("/");
  if (kdeminiitem->isSelected()) { 
    copysrc = KApplication::kde_datadir() + "/kdevelop/templates/mini.tar.gz";
    p << "cp " + copysrc + (QString) " " + copydes;
    p.start(KProcess::Block,KProcess::AllOutput);
  }  
  else if (kdenormalitem->isSelected()) {
    copysrc = KApplication::kde_datadir() + "/kdevelop/templates/normal.tar.gz";
    p << "cp " + copysrc + (QString) " " + copydes;
    p.start(KProcess::Block,KProcess::AllOutput);
  } 
  else if (qtnormalitem->isSelected()) {
    copysrc = KApplication::kde_datadir() + "/kdevelop/templates/qt.tar.gz";
    p << "cp " + copysrc + (QString) " " + copydes;
    p.start(KProcess::Block,KProcess::AllOutput);
  } 
  else if (cppitem->isSelected()) {
    copysrc = KApplication::kde_datadir() + "/kdevelop/templates/cpp.tar.gz";
    p << "cp " + copysrc + (QString) " " + copydes;
    p.start(KProcess::Block,KProcess::AllOutput);
  } 
  
  q->clearArguments();
  connect(q,SIGNAL(processExited(KProcess *)),this,SLOT(slotProcessExited()));
  connect(q,SIGNAL(receivedStdout(KProcess *, char *, int)),
	  this,SLOT(slotPerlOut(KProcess *, char *, int)));
  connect(q,SIGNAL(receivedStderr(KProcess *, char *, int)),
	  this,SLOT(slotPerlErr(KProcess *, char *, int)));
  QString path = kapp->kde_datadir()+"/kdevelop/tools/";
  *q << "perl" << path + "processes.pl";
  q->start(KProcess::NotifyOnExit, KProcess::AllOutput);
  okButton->setEnabled(false);
  gotoPage(4);
  int i;
  for (i=0;i<5;i++) {
    setPageEnabled(i,false);
  }
  apidoc->setEnabled(false);
  lsmfile->setEnabled(false);
  cppheader->setEnabled(false);
  hheader->setEnabled(false);
  datalink->setEnabled(false);
  miniicon->setEnabled(false);
  progicon->setEnabled(false);
  gnufiles->setEnabled(false);
  userdoc->setEnabled(false);
  directoryline->setEnabled(false);
  nameline->setEnabled(false);
  okButton->setEnabled(false);
  miniload->setEnabled(false);
  iconload->setEnabled(false);
  cppedit->setEnabled(false);
  hedit->setEnabled(false);
  authorline->setEnabled(false);
  emailline->setEnabled(false);  
  versionline->setEnabled(false);
  hnew->setEnabled(false);
  hload->setEnabled(false);
  cppnew->setEnabled(false);
  cppload->setEnabled(false);
  applications->setEnabled(false);
  generatesource->setEnabled(false);
  apphelp->setEnabled(false);
}


// connection of this (cancelButton)
void CKAppWizard::slotAppEnd() {
  nametext = nameline->text();
  m_author_name = authorline->text();
  m_author_email = emailline->text();

  if ((!(okButton->isEnabled())) && (nametext.length() >= 1)) {

    delete (project);
  }
  delete (errOutput);
  delete (output);
  delete (cppedit);
  delete (cppnew);
  delete (cppload);
  delete (cppheader);
  delete (hedit);
  delete (hnew);
  delete (hload);
  delete (hheader);
  delete (miniload);
  delete (iconload);
  delete (datalink);
  delete (miniicon);
  delete (progicon);
  delete (separator1);
  delete (gnufiles);
  delete (lsmfile);
  delete (userdoc);
  delete (apidoc);
  delete (emailline);
  delete (authorline);
  delete (versionline);
  delete (directoryload);
  delete (email);
  delete (authorname);
  delete (separator2);
  delete (versionnumber);
  delete (directory);
  delete (name);
  delete (separator0);
  delete (widget1b);
  delete (widget1a);
  delete (apphelp);
  delete (generatesource);
  kdeentry->removeItem (kdenormalitem);
  kdeentry->removeItem (kdeminiitem);
  delete (kdenormalitem);
  delete (kdeminiitem);
  //delete (corbaitem);
  //delete (komitem);
  delete (kdeentry);
  qtentry->removeItem (qtnormalitem);
  delete (qtnormalitem);
  //delete (qtminiitem);
  delete (qtentry);
  //delete (citem);
  ccppentry->removeItem (cppitem);
  delete (cppitem);
  delete (ccppentry);
  //delete (gtknormalitem);
  //delete (gtkminiitem);
  //delete (gtkentry);
  othersentry->removeItem (customprojitem);
  delete (customprojitem);
  delete (othersentry);
  delete (applications);
  reject();
}

// connection of this (okButton)
void CKAppWizard::slotPerlOut(KProcess*,char* buffer,int buflen) {
  QString str(buffer,buflen);
  output->append(str);
  output->setCursorPosition(output->numLines(),0);
}

// connection of this (okButton)
void CKAppWizard::slotPerlErr(KProcess*,char* buffer,int buflen) {
  QString str(buffer,buflen);
  errOutput->append(str);
  errOutput->setCursorPosition(errOutput->numLines(),0);
}

void CKAppWizard::slotApplicationClicked() {
  if (kdenormalitem->isSelected() && strcmp (cancelButton->text(), i18n("Exit"))) {
    pm.load(KApplication::kde_datadir() +"/kdevelop/pics/normalApp.bmp");
    widget1b->setBackgroundPixmap(pm);
    apidoc->setEnabled(true);
    apidoc->setChecked(true);
    datalink->setEnabled(true);
    datalink->setChecked(true);
    progicon->setEnabled(true);
    progicon->setChecked(true);
    miniicon->setEnabled(true);
    miniicon->setChecked(true);
    miniload->setEnabled(true);
    iconload->setEnabled(true);
    lsmfile->setChecked(true);
    gnufiles->setChecked(true);
    userdoc->setChecked(true);
    generatesource->setChecked(true);
    generatesource->setEnabled(true);
    if (strcmp(nameline->text(), "") && strcmp (cancelButton->text(), i18n("Exit"))) {
      okButton->setEnabled(true);
    }
    apphelp->setText (i18n("With this framework you can\n"
			   "generate a normal kde-program with\n"
			   "toolsbar and main menus."));
  }
  else if (kdeminiitem->isSelected() && strcmp (cancelButton->text(), i18n("Exit"))) {
    pm.load(KApplication::kde_datadir() + "/kdevelop/pics/miniApp.bmp");
    widget1b->setBackgroundPixmap(pm);
    apidoc->setEnabled(true);
    apidoc->setChecked(true);
    datalink->setEnabled(true);
    datalink->setChecked(true);
    progicon->setEnabled(true);
    progicon->setChecked(true);
    miniicon->setEnabled(true);
    miniicon->setChecked(true);
    miniload->setEnabled(true);
    iconload->setEnabled(true);
    lsmfile->setChecked(true);
    gnufiles->setChecked(true);
    userdoc->setChecked(true);
    generatesource->setChecked(true);
    generatesource->setEnabled(true);
    if (strcmp(nameline->text(), "") && strcmp (cancelButton->text(), i18n("Exit"))) {
      okButton->setEnabled(true);
    }
    apphelp->setText (i18n("With this framework you can\n"
			   "generate a kde-mini-program with only\n"
			   "an empty widget."));
  }
  else if (qtnormalitem->isSelected() && strcmp (cancelButton->text(), i18n("Exit"))) {
    pm.load(KApplication::kde_datadir() +"/kdevelop/pics/qtApp.bmp");
    widget1b->setBackgroundPixmap(pm);
    apidoc->setEnabled(false);
    apidoc->setChecked(false);
    datalink->setEnabled(false);
    datalink->setChecked(false);
    progicon->setEnabled(true);
    progicon->setChecked(true);
    miniicon->setEnabled(true);
    miniicon->setChecked(true);
    miniload->setEnabled(true);
    iconload->setEnabled(true);
    lsmfile->setChecked(true);
    gnufiles->setChecked(true);
    userdoc->setChecked(true);
    generatesource->setChecked(true);
    generatesource->setEnabled(true);
    if (strcmp(nameline->text(), "") && strcmp (cancelButton->text(), i18n("Exit"))) {
      okButton->setEnabled(true);
    }
    apphelp->setText (i18n("With this framework you can\n"
			   "generate a qt-program with toolsbar\n"
			   "and main menus"));
  }
  else if (cppitem->isSelected() && strcmp (cancelButton->text(), i18n("Exit"))) {
    pm.load(KApplication::kde_datadir() + "/kdevelop/pics/terminalApp.bmp");
    widget1b->setBackgroundPixmap(pm);
    apidoc->setEnabled(false);
    apidoc->setChecked(false);
    datalink->setEnabled(false);
    datalink->setChecked(false);
    progicon->setEnabled(false);
    progicon->setChecked(false);
    miniicon->setEnabled(false);
    miniicon->setChecked(false);
    miniload->setEnabled(false);
    iconload->setEnabled(false);
    lsmfile->setChecked(true);
    gnufiles->setChecked(true);
    userdoc->setChecked(true);
    generatesource->setChecked(true);
    generatesource->setEnabled(true);
    if (strcmp(nameline->text(), "") && strcmp (cancelButton->text(), i18n("Exit"))) {
      okButton->setEnabled(true);
    }
    apphelp->setText (i18n("With this framework you can\n"
			   "generate a c++-program with write\n"
			   "`Hallo World` on the terminal.\n"));
  }
  /*  else if (citem->isSelected() && strcmp (cancelButton->text(), i18n("Exit"))) {
      if (strcmp(nameline->text(), "") && strcmp (cancelButton->text(), i18n("Exit"))) {
      okButton->setEnabled(true);
      }
      apphelp->setText (i18n("With this framework you can\n"
			   "generate a program with toolsbar\n"
			   "and mainmenus"));
      }
      else if (corbaitem->isSelected() && strcmp (cancelButton->text(), i18n("Exit"))) {
      if (strcmp(nameline->text(), "") & strcmp (cancelButton->text(), i18n("Exit"))) {
      okButton->setEnabled(true);
      }
      apphelp->setText (i18n("With this framework you can\n"
			   "generate a program with toolsbar\n"
			   "and mainmenus"));
      }
      else if (komitem->isSelected() && strcmp (cancelButton->text(), i18n("Exit"))) {
      if (strcmp(nameline->text(), "") & strcmp (cancelButton->text(), i18n("Exit"))) {
      okButton->setEnabled(true);
      }
      apphelp->setText (i18n("With this framework you can\n"
			   "generate a program with toolsbar\n"
			   "and mainmenus"));
      }*/
  else if (customprojitem->isSelected() && strcmp (cancelButton->text(), i18n("Exit"))) {
    pm.load(KApplication::kde_datadir() + "/kdevelop/pics/customApp.bmp");
    widget1b->setBackgroundPixmap(pm);
    apidoc->setEnabled(false);
    apidoc->setChecked(false);
    datalink->setEnabled(false);
    datalink->setChecked(false);
    progicon->setEnabled(false);
    progicon->setChecked(false);
    miniicon->setEnabled(false);
    miniicon->setChecked(false);
    miniload->setEnabled(false);
    iconload->setEnabled(false);
    lsmfile->setChecked(true);
    gnufiles->setChecked(true);
    userdoc->setChecked(true);
    generatesource->setChecked(false);
    generatesource->setEnabled(false);
    if (strcmp(nameline->text(), "") && strcmp (cancelButton->text(), i18n("Exit"))) {
      okButton->setEnabled(true);
    }
    apphelp->setText (i18n("With this program-typ you can\n"
			   "generate an empty project.\n"
			   "Here you can use your own makefiles and\n"
			   "kdevelop would not edit your makefiles."));
  }
  /*  else if (gtknormalitem->isSelected() && strcmp (cancelButton->text(), i18n("Exit"))) {
      if (strcmp(nameline->text(), "") && strcmp (cancelButton->text(), i18n("Exit"))) {
      okButton->setEnabled(true);
      }
      apphelp->setText (i18n("With this framework you can\n"
			   "generate a program with toolsbar\n"
			   "and mainmenus"));
      }
      else if (gtkminiitem->isSelected() && strcmp (cancelButton->text(), i18n("Exit"))) {
      if (strcmp(nameline->text(), "") && strcmp (cancelButton->text(), i18n("Exit"))) {
      okButton->setEnabled(true);
      }
      apphelp->setText (i18n("With this framework you can\n"
			   "generate a program with toolsbar\n"
			   "and mainmenus"));
      }
      else if (qtminiitem->isSelected() && strcmp (cancelButton->text(), i18n("Exit"))) {
      if (strcmp(nameline->text(), "") && strcmp (cancelButton->text(), i18n("Exit"))) {
      okButton->setEnabled(true);
      }
      apphelp->setText (i18n("With this framework you can\n"
			   "generate a program with toolsbar\n"
			   "and mainmenus"));
      }*/
  else if (kdeentry->isSelected()) {
    okButton->setEnabled(false);
    apphelp->setText (i18n("Under this group all kde-releated\n"
			   "programs are listed."));
  }
  else if (qtentry->isSelected()) {
    okButton->setEnabled(false);
    apphelp->setText (i18n("Under this group all qt-releated\n"
			   "programs are listed."));
  }
  else if (ccppentry->isSelected()) {
    okButton->setEnabled(false);
    apphelp->setText (i18n("Under this group all c/c++-terminal\n"
			   "releated programs are listed."));
  }
  /*  else if (gtkentry->isSelected()) {
      okButton->setEnabled(false);
      apphelp->setText (i18n("With this framework you can\n"
			   "generate a program with toolsbar\n"
			   "and mainmenus"));
      }*/
  else if (othersentry->isSelected()) {
    okButton->setEnabled(false);
    apphelp->setText (i18n("Under this group are listed all\n"
			   "program-typs, which can not be list\n"
			   "in the other groups."));
  }
}

// connection of this
void CKAppWizard::slotDefaultClicked() {
  pm.load(KApplication::kde_datadir() +"/kdevelop/pics/normalApp.bmp");
  widget1b->setBackgroundPixmap(pm);
  applications->setSelected(kdenormalitem,true);
  apidoc->setChecked(true);
  lsmfile->setChecked(true);
  cppheader->setChecked(true);
  hheader->setChecked(true);
  datalink->setChecked(true);
  miniicon->setChecked(true);
  progicon->setChecked(true);
  gnufiles->setChecked(true);
  userdoc->setChecked(true);
  miniload->setEnabled(true);
  iconload->setEnabled(true);
  generatesource->setChecked(true);  
  directoryline->setText(QDir::homeDirPath()+ QString("/"));
  dir = QDir::homeDirPath()+ QString("/");
  nameline->setText(0);
  okButton->setEnabled(false);
  miniload->setPixmap(QPixmap(KApplication::kde_icondir() + "/mini/application_settings.xpm"));
  iconload->setPixmap(QPixmap(KApplication::kde_icondir() + "/edit.xpm"));
  cppedit->loadFile(KApplication::kde_datadir() + "/kdevelop/templates/cpp_template",cppedit->OPEN_READWRITE);
  hedit->loadFile(KApplication::kde_datadir() + "/kdevelop/templates/header_template",hedit->OPEN_READWRITE);
  authorline->setText(m_author_name);
  emailline->setText(m_author_email);
  versionline->setText("0.1");
}

// connection of nameline
void CKAppWizard::slotProjectnameEntry() {
  nametext = nameline->text();
  nametext = nametext.stripWhiteSpace();
  if (nametext.length() == 1) {
    QRegExp regexp ("[a-zA-Z0-9]");
    if (regexp.match(nametext) == -1) {
      nametext = "";
    }
    else {
      nametext = nametext.upper();
    }
  }
  nameline->setText(nametext);
  directoryline->setText(dir + nametext.lower());
  if (nametext == "" || kdeentry->isSelected() || qtentry->isSelected() || 
      ccppentry->isSelected() || othersentry->isSelected()) {
    okButton->setEnabled(false);
  }
  else {
    okButton->setEnabled(true);
  }
}

// connection of iconload
void CKAppWizard::slotIconButtonClicked() {
  QStrList iconlist;
  KIconLoaderDialog iload;
  iconlist.append (KApplication::kde_icondir());
  iconlist.append (KApplication::localkdedir()+"/share/icons");
  iload.setDir(&iconlist);
  iload.selectIcon(name1,"*");
  if (!name1.isNull() )   
    iconload->setPixmap(kapp->getIconLoader()->loadIcon(name1));
}

// connection of miniload
void CKAppWizard::slotMiniIconButtonClicked() {
  QStrList miniiconlist;
  KIconLoaderDialog  mload;
  miniiconlist.append (KApplication::kde_icondir()+"/mini");
  miniiconlist.append (KApplication::localkdedir()+"/share/icons/mini");
  mload.setDir(&miniiconlist);
  mload.selectIcon(name2,"*");
  if (!name2.isNull() )     
    miniload->setPixmap(kapp->getIconLoader()->loadMiniIcon(name2));
}

// activate and deactivate the iconbutton
void CKAppWizard::slotProgIconClicked() {
  if (progicon->isChecked()) {
    iconload->setEnabled(true);
  }
  else {
    iconload->setEnabled(false);
  }
}

// activate and deactivate the miniiconbutton
void CKAppWizard::slotMiniIconClicked() {
  if (miniicon->isChecked()) {
    miniload->setEnabled(true);
  }
  else {
    miniload->setEnabled(false);
  }
}

// activate and deactivate the headerloadbutton, headernewbutton, headeredit
void CKAppWizard::slotHeaderHeaderClicked() {
  if (hheader->isChecked()) {
    hload->setEnabled(true);
    hnew->setEnabled(true);
    hedit->setEnabled(true);
  }
  else {
    hload->setEnabled(false);
    hnew->setEnabled(false);
    hedit->setEnabled(false);
  }
}

// activate and deactivate the cpploadbutton, cppnewbutton, cppedit
void CKAppWizard::slotCppHeaderClicked() {
  if (cppheader->isChecked()) {
    cppload->setEnabled(true);
    cppnew->setEnabled(true);
    cppedit->setEnabled(true);
  }
  else {
    cppload->setEnabled(false);
    cppnew->setEnabled(false);
    cppedit->setEnabled(false);
  }
}

void CKAppWizard::slotProcessExited() {

  QString directory = directoryline->text();
  QString prj_str = directory + "/" + namelow + ".kdevprj";
  project = new CProject(prj_str);
  project->readProject();
  project->setKDevPrjVersion("0.3");
  if (cppitem->isSelected()) {
    project->setProjectType("normal_cpp");
  } 
  else if (kdeminiitem->isSelected()) {
    project->setProjectType("mini_kde");  
  } 
  else if (kdenormalitem->isSelected()) {
    project->setProjectType("normal_kde");
  } 
  else if (qtnormalitem->isSelected()) {
    project->setProjectType("normal_qt");
  } 
  else if (customprojitem->isSelected()) {
    project->setProjectType("normal_empty");
  }
  
  project->setProjectName (nameline->text());
  project->setSubDir (namelow + "/");
  project->setAuthor (authorline->text());
  project->setEmail (emailline->text());
  project->setVersion (versionline->text());
  if (userdoc->isChecked()) {
    project->setSGMLFile ("index.sgml");
  }
  project->setBinPROGRAM (namelow);
  project->setLDFLAGS (" ");
  project->setCXXFLAGS ("-O0 -g3 -Wall");
  
  if (kdenormalitem->isSelected()) {
    project->setLDADD (" -lkfile -lkfm -lkdeui -lkdecore -lqt -lXext -lX11");
  }
  else if (kdeminiitem->isSelected()) {
    project->setLDADD (" -lkdeui -lkdecore -lqt -lXext -lX11");
  }
  else if (qtnormalitem->isSelected()) {
    project->setLDADD (" -lqt -lXext -lX11");
  }
  
  QStrList sub_dir_list;
  TMakefileAmInfo makeAmInfo;
  makeAmInfo.rel_name = "Makefile.am";
  KDEBUG1(KDEBUG_INFO,CKAPPWIZARD,"%s",makeAmInfo.rel_name.data());
  makeAmInfo.type = "normal";
  KDEBUG1(KDEBUG_INFO,CKAPPWIZARD,"%s",makeAmInfo.type.data());
  sub_dir_list.append(namelow);
  if (kdenormalitem->isSelected() || kdeminiitem->isSelected()) {
    sub_dir_list.append("po");
  }
  makeAmInfo.sub_dirs = sub_dir_list;
  project->addMakefileAmToProject (makeAmInfo.rel_name,makeAmInfo);

  makeAmInfo.rel_name =  namelow + "/Makefile.am";
  KDEBUG1(KDEBUG_INFO,CKAPPWIZARD,"%s",makeAmInfo.rel_name.data());
  makeAmInfo.type = "prog_main";
  KDEBUG1(KDEBUG_INFO,CKAPPWIZARD,"%s",makeAmInfo.type.data());
  sub_dir_list.clear();
  if (userdoc->isChecked()) {
    //    sub_dir_list.append("docs");
  }
  makeAmInfo.sub_dirs = sub_dir_list;
  project->addMakefileAmToProject (makeAmInfo.rel_name,makeAmInfo);
  
  makeAmInfo.rel_name =  namelow + "/docs/Makefile.am";
  KDEBUG1(KDEBUG_INFO,CKAPPWIZARD,"%s",makeAmInfo.rel_name.data());
  makeAmInfo.type = "normal";
  KDEBUG1(KDEBUG_INFO,CKAPPWIZARD,"%s",makeAmInfo.type.data());
  sub_dir_list.clear();
  //  sub_dir_list.append("en");
  makeAmInfo.sub_dirs = sub_dir_list;
  project->addMakefileAmToProject (makeAmInfo.rel_name,makeAmInfo);

  makeAmInfo.rel_name =  namelow + "/docs/en/Makefile.am";
  KDEBUG1(KDEBUG_INFO,CKAPPWIZARD,"%s",makeAmInfo.rel_name.data());
  makeAmInfo.type = "normal";
  KDEBUG1(KDEBUG_INFO,CKAPPWIZARD,"%s",makeAmInfo.type.data());
  sub_dir_list.clear();
  makeAmInfo.sub_dirs = sub_dir_list;
  project->addMakefileAmToProject (makeAmInfo.rel_name,makeAmInfo);
  
  if (!(cppitem->isSelected() || qtnormalitem->isSelected())) {
    makeAmInfo.rel_name = "po/Makefile.am";
    KDEBUG1(KDEBUG_INFO,CKAPPWIZARD,"%s",makeAmInfo.rel_name.data());
    makeAmInfo.type = "po";
    KDEBUG1(KDEBUG_INFO,CKAPPWIZARD,"%s",makeAmInfo.type.data());
    sub_dir_list.clear();
    makeAmInfo.sub_dirs = sub_dir_list;
    project->addMakefileAmToProject (makeAmInfo.rel_name,makeAmInfo);
  }
  
  TFileInfo fileInfo;
  fileInfo.rel_name = namelow + ".kdevprj";
  fileInfo.type = DATA;
  fileInfo.dist = true;
  fileInfo.install = false;
  fileInfo.install_location = "";
  project->addFileToProject (namelow + ".kdevprj",fileInfo);

  if (gnufiles->isChecked()) {
    fileInfo.rel_name = "AUTHORS";
    fileInfo.type = DATA;
    fileInfo.dist = true;
    fileInfo.install = false;
    fileInfo.install_location = "";
    project->addFileToProject ("AUTHORS",fileInfo);
    
    fileInfo.rel_name = "COPYING";
    fileInfo.type = DATA;
    fileInfo.dist = true;
    fileInfo.install = false;
    fileInfo.install_location = "";
    project->addFileToProject ("COPYING",fileInfo);
    
    fileInfo.rel_name = "ChangeLog";
    fileInfo.type = DATA;
    fileInfo.dist = true;
    fileInfo.install = false;
    fileInfo.install_location = "";
    project->addFileToProject ("ChangeLog",fileInfo);
    
    fileInfo.rel_name = "INSTALL";
    fileInfo.type = DATA;
    fileInfo.dist = true;
    fileInfo.install = false;
    fileInfo.install_location = "";
    project->addFileToProject ("INSTALL",fileInfo);
    
    fileInfo.rel_name = "README";
    fileInfo.type = DATA;
    fileInfo.dist = true;
    fileInfo.install = false;
    fileInfo.install_location = "";
    project->addFileToProject ("README",fileInfo);
    
    fileInfo.rel_name = "TODO";
    fileInfo.type = DATA;
    fileInfo.dist = true;
    fileInfo.install = false;
    fileInfo.install_location = "";
    project->addFileToProject ("TODO",fileInfo);
    
  } 

  if (lsmfile->isChecked()) {
    fileInfo.rel_name = namelow + ".lsm";
    fileInfo.type = DATA;
    fileInfo.dist = true;
    fileInfo.install = false;
    fileInfo.install_location = "";
    project->addFileToProject (namelow + ".lsm",fileInfo);
  }

  if (generatesource->isChecked()) {
    fileInfo.rel_name = namelow + "/main.cpp";
    fileInfo.type = CPP_SOURCE;
    fileInfo.dist = true;
    fileInfo.install = false;
    fileInfo.install_location = "";
    project->addFileToProject (namelow + "/main.cpp",fileInfo);
  }
  
  if (!(cppitem->isSelected())) {
    if (generatesource->isChecked()) {
      fileInfo.rel_name = namelow + "/" + namelow + ".cpp";
      fileInfo.type = CPP_SOURCE;
      fileInfo.dist = true;
      fileInfo.install = false;
      fileInfo.install_location = "";
      project->addFileToProject (namelow + "/" + namelow + ".cpp",fileInfo);
      
      fileInfo.rel_name = namelow + "/" + namelow + ".h";
      fileInfo.type = CPP_HEADER;
      fileInfo.dist = true;
      fileInfo.install = false;
      fileInfo.install_location = "";
      project->addFileToProject (namelow + "/" + namelow + ".h",fileInfo);
    }
  }
  
  if (kdenormalitem->isSelected() || qtnormalitem->isSelected()) {
    if (generatesource->isChecked()) {
      fileInfo.rel_name = namelow + "/" + namelow + "doc.cpp";
      fileInfo.type = CPP_SOURCE;
      fileInfo.dist = true;
      fileInfo.install = false;
      fileInfo.install_location = "";
      project->addFileToProject (namelow + "/" + namelow + "doc.cpp",fileInfo);
      
      fileInfo.rel_name = namelow + "/" + namelow + "doc.h";
      fileInfo.type = CPP_HEADER;
      fileInfo.dist = true;
      fileInfo.install = false;
      fileInfo.install_location = "";
      project->addFileToProject (namelow + "/" + namelow + "doc.h",fileInfo);
      
      fileInfo.rel_name = namelow + "/" + namelow + "view.cpp";
      fileInfo.type = CPP_SOURCE;
      fileInfo.dist = true;
      fileInfo.install = false;
      fileInfo.install_location = "";
      project->addFileToProject (namelow + "/" + namelow + "view.cpp",fileInfo);
      
      fileInfo.rel_name = namelow + "/" + namelow + "view.h";
      fileInfo.type = CPP_HEADER;
      fileInfo.dist = true;
      fileInfo.install = false;
      fileInfo.install_location = "";
      project->addFileToProject (namelow + "/" + namelow + "view.h",fileInfo);
      
      fileInfo.rel_name = namelow + "/resource.h";
      fileInfo.type = CPP_HEADER;
      fileInfo.dist = true;
      fileInfo.install = false;
      fileInfo.install_location = "";
      project->addFileToProject (namelow + "/resource.h",fileInfo);
    }  
  }
  
  if (datalink->isChecked()) {
    fileInfo.rel_name = namelow + "/" + namelow + ".kdelnk";
    fileInfo.type = DATA;
    fileInfo.dist = true;
    fileInfo.install = true;
    fileInfo.install_location = "$(kde_appsdir)/Applications/" + namelow + ".kdelnk";
    project->addFileToProject (namelow + "/" + namelow + ".kdelnk",fileInfo);
  } 
  
  if (progicon->isChecked()) {
    fileInfo.rel_name = namelow + "/" + namelow + ".xpm";
    fileInfo.type = DATA;
    fileInfo.dist = true;
    if (!qtnormalitem->isSelected()) {
      fileInfo.install = true;
      fileInfo.install_location = "$(kde_icondir)/" + namelow + ".xpm";
    }
    else {
      fileInfo.install = false;
      fileInfo.install_location = "";
    }
    project->addFileToProject (namelow + "/" + namelow + ".xpm",fileInfo);
  }
  
  if (miniicon->isChecked()) {
    fileInfo.rel_name = namelow + "/mini-" + namelow + ".xpm";
    fileInfo.type = DATA;
    fileInfo.dist = true;
    if (!qtnormalitem->isSelected()) {
      fileInfo.install = true;
      fileInfo.install_location = "$(kde_icondir)/" + namelow + ".xpm";
    }
    else {
      fileInfo.install = false;
      fileInfo.install_location = "";
    }
    project->addFileToProject (namelow + "/mini-" + namelow + ".xpm",fileInfo);
  }
  
  if (qtnormalitem->isSelected()) {
    fileInfo.rel_name = namelow + "/filenew.xpm";
    fileInfo.type = DATA;
    fileInfo.dist = true;
    fileInfo.install = false;
    fileInfo.install_location = "";
    project->addFileToProject (namelow + "/filenew.xpm",fileInfo);

    fileInfo.rel_name = namelow + "/filesave.xpm";
    fileInfo.type = DATA;
    fileInfo.dist = true;
    fileInfo.install = false;
    fileInfo.install_location = "";
    project->addFileToProject (namelow + "/filesave.xpm",fileInfo);

    fileInfo.rel_name = namelow + "/fileopen.xpm";
    fileInfo.type = DATA;
    fileInfo.dist = true;
    fileInfo.install = false;
    fileInfo.install_location = "";
    project->addFileToProject (namelow + "/fileopen.xpm",fileInfo);
  } 
  
  if (userdoc->isChecked()) {
    fileInfo.rel_name = namelow + "/docs/en/index-1.html";
    fileInfo.type = DATA;
    fileInfo.dist = true;
    if (!(cppitem->isSelected())) {
      fileInfo.install = true;
      if (qtnormalitem->isSelected()) {
	fileInfo.install_location = "$(prefix)/doc/" + namelow+ "/index-1.html";
      } 
      else 
	fileInfo.install_location = "$(kde_htmldir)/en/" + namelow+ "/index-1.html";
    } 
    
    project->addFileToProject (namelow + "/docs/en/index-1.html",fileInfo);
    
    fileInfo.rel_name = namelow + "/docs/en/index-2.html";
    fileInfo.type = DATA;
    fileInfo.dist = true;
    if (!(cppitem->isSelected())) {
      fileInfo.install = true;
      if (qtnormalitem->isSelected()) {
	fileInfo.install_location = "$(prefix)/doc/" + namelow+ "/index-2.html";
      } 
      else 
	fileInfo.install_location = "$(kde_htmldir)/en/" + namelow+ "/index-2.html";
    } 
    project->addFileToProject (namelow + "/docs/en/index-2.html",fileInfo);
    
    fileInfo.rel_name = namelow + "/docs/en/index-3.html";
    fileInfo.type = DATA;
    fileInfo.dist = true;
    if (!(cppitem->isSelected())) {
      fileInfo.install = true;
      if (qtnormalitem->isSelected()) {
	fileInfo.install_location = "$(prefix)/doc/" + namelow+ "/index-3.html";
      } 
      else 
	fileInfo.install_location = "$(kde_htmldir)/en/" + namelow+ "/index-3.html";
    } 
    project->addFileToProject (namelow + "/docs/en/index-3.html",fileInfo);
    
    fileInfo.rel_name = namelow + "/docs/en/index-4.html";
    fileInfo.type = DATA;
    fileInfo.dist = true;
    if (!(cppitem->isSelected())) {
      fileInfo.install = true;
      if (qtnormalitem->isSelected()) {
	fileInfo.install_location = "$(prefix)/doc/" + namelow+ "/index-4.html";
      } 
      else 
	fileInfo.install_location = "$(kde_htmldir)/en/" + namelow+ "/index-4.html";
    } 
    project->addFileToProject (namelow + "/docs/en/index-4.html",fileInfo);
    
    fileInfo.rel_name = namelow + "/docs/en/index-5.html";
    fileInfo.type = DATA;
    fileInfo.dist = true;
    if (!(cppitem->isSelected())) { 
      fileInfo.install = true;
      if (qtnormalitem->isSelected()) {
	fileInfo.install_location = "$(prefix)/doc/" + namelow+ "/index-5.html";
      } 
      else 
	fileInfo.install_location = "$(kde_htmldir)/en/" + namelow+ "/index-5.html";
    }
    project->addFileToProject (namelow + "/docs/en/index-5.html",fileInfo);
    
    fileInfo.rel_name = namelow + "/docs/en/index-6.html";
    fileInfo.type = DATA;
    fileInfo.dist = true;
    if (!(cppitem->isSelected())) {
      fileInfo.install = true;
      if (qtnormalitem->isSelected()) {
	fileInfo.install_location = "$(prefix)/doc/" + namelow+ "/index-6.html";
      } 
      else 
	fileInfo.install_location = "$(kde_htmldir)/en/" + namelow+ "/index-6.html";
    } 
    project->addFileToProject (namelow + "/docs/en/index-6.html",fileInfo);
    
    fileInfo.rel_name = namelow + "/docs/en/index.html";
    fileInfo.type = DATA;
    fileInfo.dist = true;
    if (!(cppitem->isSelected())) {
      fileInfo.install = true;
      if (qtnormalitem->isSelected()) {
	fileInfo.install_location = "$(prefix)/doc/" + namelow+ "/index.html";
      } 
      else 
	fileInfo.install_location = "$(kde_htmldir)/en/" + namelow+ "/index.html";
    } 
    project->addFileToProject (namelow + "/docs/en/index.html",fileInfo);
    
  }
  
  
  QStrList group_filters;
  group_filters.append("*");
  project->addLFVGroup ("Others","");
  project->setFilters("Others",group_filters);
  
  
  
  if (gnufiles->isChecked()) {
    group_filters.clear();
    group_filters.append("AUTHORS");
    group_filters.append("COPYING");
    group_filters.append("ChangeLog");
    group_filters.append("INSTALL");
    group_filters.append("README");
    group_filters.append("TODO");
    group_filters.append("NEWS");
    project->addLFVGroup ("GNU","");
    project->setFilters("GNU",group_filters);
  }
  
  if (!(cppitem->isSelected() || qtnormalitem->isSelected())) {
    group_filters.clear();
    group_filters.append("*.po");
    project->addLFVGroup ("Translations","");
    project->setFilters("Translations",group_filters);
  } 
  if (!cppitem->isSelected()) {
    group_filters.clear();
    group_filters.append("*.kdevdlg");
    project->addLFVGroup ("Dialogs","");
    project->setFilters("Dialogs",group_filters);
  } 
  
  
  group_filters.clear();
  group_filters.append("*.cpp");
  group_filters.append("*.c");
  group_filters.append("*.cc");
  group_filters.append("*.C");
  group_filters.append("*.cxx");
  group_filters.append("*.ec");
  group_filters.append("*.ecpp");
  project->addLFVGroup ("Sources","");
  project->setFilters("Sources",group_filters);
  
  group_filters.clear();
  group_filters.append("*.h");
  group_filters.append("*.hxx");
  project->addLFVGroup ("Header","");
  project->setFilters("Header",group_filters);
  
  project->writeProject ();
  project->updateMakefilesAm ();

  disconnect(q,SIGNAL(processExited(KProcess *)),this,SLOT(slotProcessExited()));
  connect(q,SIGNAL(processExited(KProcess *)),this,SLOT(slotMakeEnd()));
  
  QString path1 = kapp->kde_datadir()+"/kdevelop/tools/";
  q->clearArguments();
  *q << "perl" << path1 + "processesend.pl";
  q->start(KProcess::NotifyOnExit, KProcess::AllOutput);
  
}

// enable cancelbutton if everything is done
void CKAppWizard::slotMakeEnd() {
  if (!generatesource->isChecked()) {
    QFile file;
    q->clearArguments();
    directorytext = directoryline->text();
    nametext = nameline->text();
    nametext = nametext.lower();
    file.remove (directorytext + "/" + nametext + "/main.cpp");
    file.remove (directorytext + "/" + nametext + "/" + nametext + ".cpp");
    file.remove (directorytext + "/" + nametext + "/" + nametext + ".h");
    if (kdenormalitem->isSelected() || qtnormalitem->isSelected()) {
      file.remove (directorytext + "/" + nametext + "/" + nametext + "doc.cpp");
      file.remove (directorytext + "/" + nametext + "/" + nametext + "doc.h");
      file.remove (directorytext + "/" + nametext + "/" + nametext + "view.cpp");
      file.remove (directorytext + "/" + nametext + "/" + nametext + "view.h");
    }
  }
  cancelButton->setEnabled(true);
  gen_prj = true;
}

// return the directory with the projectfile
QString CKAppWizard::getProjectFile() {
  nametext = nameline->text();
  nametext = nametext.lower();
  directorytext = directoryline->text();
  if(QString(directoryline->text()).right(1) == "/"){
    directorytext = directorytext + nametext + ".kdevprj";
  }
  else{
    directorytext = directorytext + "/" + nametext + ".kdevprj";
  }
  delete (directoryline);
  delete (nameline);
  delete (defaultButton);
  delete (okButton);
  delete (cancelButton);
  delete (widget0);
  delete (widget1);
  delete (widget2);
  delete (widget3);
  delete (widget4);
  return directorytext;
}

// return TRUE if a poject is generated
bool CKAppWizard::generatedProject(){
  return gen_prj;
}



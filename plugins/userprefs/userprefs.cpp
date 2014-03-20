/*
 Copyright (C) 2011 Georgia Institute of Technology, University of Utah, Weill Cornell Medical College

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

/*
 * The settings are only written when the panel is closed and the QSettings
 * is destructed. So all Reset, Apply, and Cancel buttons all close the panel.
 */

#include <debug.h>
#include <main_window.h>
#include <userprefs.h>
#include <QtGui>
#include <QFileDialog>

UserPrefs::Panel::Panel(QWidget *parent) : QWidget(parent) {

	setAttribute(Qt::WA_DeleteOnClose);

	// Preferences structure
	QSettings userprefs;
	userprefs.setPath(QSettings::NativeFormat, QSettings::UserScope, "RTXI");

	// Main layout
	QVBoxLayout *layout = new QVBoxLayout;

	// Create child widget and layout
  dirGroup = new QGroupBox;
	QGridLayout *dirLayout = new QGridLayout;

	// Create elements for directory paths
  settingsDirEdit = new QLineEdit(dirGroup);
  settingsDirEdit->setText(userprefs.value("/dirs/setfiles", getenv("HOME")).toString());
  dirLayout->addWidget(new QLabel(tr("Default settings directory:")), 1, 0);
  dirLayout->addWidget(settingsDirEdit, 1, 1);
  QPushButton *chooseSettingsDirButton = new QPushButton("Browse", this);
  dirLayout->addWidget(chooseSettingsDirButton, 1, 2);
  QObject::connect(chooseSettingsDirButton,SIGNAL(clicked(void)),this,SLOT(chooseSettingsDir(void)));

  dynamoDirEdit = new QLineEdit(dirGroup);
  dynamoDirEdit->setText(userprefs.value("/dirs/dynamomodels", getenv("HOME")).toString());
	dirLayout->addWidget(new QLabel(tr("Default DYNAMO models directory:")), 2, 0);
	dirLayout->addWidget(dynamoDirEdit, 2, 1);
  QPushButton *chooseDynamoDirButton = new QPushButton("Browse", this);
  dirLayout->addWidget(chooseDynamoDirButton, 2, 2);
  QObject::connect(chooseDynamoDirButton,SIGNAL(clicked(void)),this,SLOT(chooseDynamoDir(void)));

  dataDirEdit = new QLineEdit(dirGroup);
  dataDirEdit->setText(userprefs.value("/dirs/data", getenv("HOME")).toString());
	dirLayout->addWidget(new QLabel(tr("Default HDF5 data directory:")), 3, 0);
	dirLayout->addWidget(dataDirEdit, 3, 1);
  QPushButton *chooseDataDirButton = new QPushButton("Browse", this);
  dirLayout->addWidget(chooseDataDirButton, 3, 2);
  QObject::connect(chooseDataDirButton,SIGNAL(clicked(void)),this,SLOT(chooseDataDir(void)));

	// Attach layout to group
	dirGroup->setLayout(dirLayout);

	// Create new child widget and layout
	HDF = new QGroupBox;
	QGridLayout *hdfLayout = new QGridLayout;

	// Create elements for child widget
  HDFBufferEdit = new QLineEdit(HDF);
	hdfLayout->addWidget(new QLabel(tr("HDF Data Recorder Buffer Size (MB):")), 1, 0);
	hdfLayout->addWidget(HDFBufferEdit, 1, 1);
  HDFBufferEdit->setText(QString::number(userprefs.value("/system/HDFbuffer", 10).toInt()));

	// Attach child to parent
	HDF->setLayout(hdfLayout);

	// Create new child widget
  buttons = new QGroupBox;
	QHBoxLayout *buttonLayout = new QHBoxLayout;

	// Create elements for child widget
  QPushButton *resetButton = new QPushButton("Reset and Close");
  QObject::connect(resetButton,SIGNAL(clicked(void)),this,SLOT(reset(void)));
  QPushButton *applyButton = new QPushButton("Save and Close");
  QObject::connect(applyButton,SIGNAL(clicked(void)),this,SLOT(apply(void)));
  QPushButton *cancelButton = new QPushButton("Close without Saving");
  QObject::connect(cancelButton,SIGNAL(clicked(void)),this,SLOT(cancel(void)));

	// Assign elements to layout in child widget
	buttonLayout->addWidget(resetButton);
	buttonLayout->addWidget(applyButton);
	buttonLayout->addWidget(cancelButton);

	// Assign layout to child widget
	buttons->setLayout(buttonLayout);

	// Attach child widget to parent widget
	layout->addWidget(dirGroup);
	layout->addWidget(HDF);
  layout->addWidget(buttons);

  // Attach layout to widget
  setLayout(layout);
  setWindowTitle("User Preferences");
  show();
}

UserPrefs::Panel::~Panel(void) {
  Prefs::getInstance()->panel = 0;
}

void UserPrefs::Panel::reset(void) {
  settingsDirEdit->setText(getenv("HOME"));
  dynamoDirEdit->setText(getenv("HOME"));
  dataDirEdit->setText(getenv("HOME"));
  HDFBufferEdit->setText(QString::number(10));
  apply();
}

void UserPrefs::Panel::apply(void) {
  userprefs.setValue("/dirs/setfiles", settingsDirEdit->text());
  userprefs.setValue("/dirs/dynamomodels", dynamoDirEdit->text());
  userprefs.setValue("/dirs/data", dataDirEdit->text());
  bool ok;
  QString buffer = HDFBufferEdit->text();
  userprefs.setValue("/system/HDFbuffer", buffer.toInt(&ok));
  this->close();
}

void UserPrefs::Panel::cancel(void) {
  this->close();
}

void UserPrefs::Panel::chooseSettingsDir(void) {
  QString dir_name = QFileDialog::getExistingDirectory(this, tr("Choose default directory for settings files"), 
  		userprefs.value("/dirs/setfiles", getenv("HOME")).toString(),
  		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  settingsDirEdit->setText(dir_name);
}

void UserPrefs::Panel::chooseDynamoDir(void) {
  QString dir_name = QFileDialog::getExistingDirectory(this, tr("Choose default directory for DYNAMO models"), 
  		userprefs.value("/dirs/setfiles", getenv("HOME")).toString(),
  		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  dynamoDirEdit->setText(dir_name);
}

void UserPrefs::Panel::chooseDataDir(void) {
  QString dir_name = QFileDialog::getExistingDirectory(this, tr("Choose default directory for HDF5 data files"), 
  		userprefs.value("/dirs/setfiles", getenv("HOME")).toString(),
  		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  dataDirEdit->setText(dir_name);
}

extern "C" Plugin::Object * createRTXIPlugin(void *) {
  return UserPrefs::Prefs::getInstance();
}

UserPrefs::Prefs::Prefs(void) : panel(0) {
	MainWindow::getInstance()->createSystemMenuItem("Preferences",this,SLOT(createPrefsPanel(void)));
}

UserPrefs::Prefs::~Prefs(void) {
  MainWindow::getInstance()->removeSystemMenuItem(menuID);
  if (panel)
    delete panel;
  instance = 0;
  panel = 0;
}

void UserPrefs::Prefs::createPrefsPanel(void) {
  if (!panel)
    panel = new Panel(MainWindow::getInstance()->centralWidget());
  panel->show();
}

static Mutex mutex;
UserPrefs::Prefs *UserPrefs::Prefs::instance = 0;

UserPrefs::Prefs *
UserPrefs::Prefs::getInstance(void) {
  if (instance)
    return instance;

  /*************************************************************************
   * Seems like alot of hoops to jump through, but allocation isn't        *
   *   thread-safe. So effort must be taken to ensure mutual exclusion.    *
   *************************************************************************/

  Mutex::Locker lock(&::mutex);
  if (!instance)
    instance = new Prefs();

  return instance;
}

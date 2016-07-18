/*********
*
* In the name of the Father, and of the Son, and of the Holy Spirit.
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2016 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef INSTBACKEND_H
#define INSTBACKEND_H

#include <QDir>
#include <QString>
#include <swordxx/installmgr.h>
#include "managers/cswordbackend.h"


class CSwordModuleInfo;

namespace BtInstallBackend {

/** Adds the source to the backend. */
bool addSource(swordxx::InstallSource& source);

/** Returns the source struct. */
swordxx::InstallSource source(const QString &name);

/** Deletes the source. */
bool deleteSource(const QString &name);

/** Returns the moduleinfo list for the source. */
QList<CSwordModuleInfo*> moduleList(const QString &name);

/** Tells if the source is remote or local. */
bool isRemote(const swordxx::InstallSource& source);

/** Returns the list of available install target paths. */
QStringList targetList();

/** Saves the list of available install target paths to the Sword++ config. Return success indicator.*/
bool setTargetList( const QStringList& targets );

QStringList sourceNameList();

/** Returns the path of the Sword++ installer configuration file. */
QString configPath();

/** Returns the name of the Sword++ installer configuration file. */
QString configFilename();

/** Sets the passive mode for as default.
* \todo see if we can en/disable this per source.
*/
void initPassiveFtpMode();

/** Returns the file name for the Sword++ config file. */
QString swordConfigFilename();

/** Returns the Sword++ directory ($HOME/.swordxx/) as a QDir, created with absolute path (not canonical).
*/
QDir swordDir();

/** Returns backend Sword++ manager for the source. */
CSwordBackend * backend(const swordxx::InstallSource & is);

} // namespace BtInstallBackend

#endif

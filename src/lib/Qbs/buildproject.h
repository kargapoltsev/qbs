/**************************************************************************
**
** This file is part of the Qt Build Suite
**
** Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (info@qt.nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file.
** Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**************************************************************************/


#ifndef QBS_QBSBUILDPROJECT_H
#define QBS_QBSBUILDPROJECT_H

#include "buildproduct.h"

#include <QSharedPointer>

namespace qbs {
    class BuildProject;
}

namespace Qbs {

class SourceProject;

class BuildProject
{
    friend class SourceProject;
public:
    BuildProject();
    ~BuildProject();
    BuildProject(const BuildProject &other);
    BuildProject &operator =(const BuildProject &other);

    QVector<BuildProduct> buildProducts() const;
    BuildProduct buildProductForName(const QString &name) const;
    bool hasBuildProductForName(const QString &name) const;

    QString buildDirectory() const;
    QString displayName() const;

    void storeBuildGraph();

    bool isValid() const;

    QString qtInstallPath() const;

    QSharedPointer<qbs::BuildProject> internalBuildProject() const;

    bool removeBuildDirectory();


private:  // functions
    BuildProject(const QSharedPointer<qbs::BuildProject> &internalBuildProject);

private:  // variables
    QSharedPointer<qbs::BuildProject> m_internalBuildProject;
};

} // namespace Qbs

#endif // QBS_QBSBUILDPROJECT_H

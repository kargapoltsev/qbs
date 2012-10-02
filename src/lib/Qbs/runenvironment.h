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

#ifndef QBS_RUNENVIRONMENT_H
#define QBS_RUNENVIRONMENT_H

#include "buildproduct.h"

#include "private/resolvedproduct.h"

#include <QSharedPointer>



namespace Qbs {

class RunEnvironment
{
public:
    RunEnvironment(const Qbs::Private::ResolvedProduct &resolvedproduct);
    RunEnvironment(const Qbs::BuildProduct &buildProduct);
    ~RunEnvironment();
    RunEnvironment(const RunEnvironment &other);
    RunEnvironment &operator =(const RunEnvironment &other);

    int runShell();
    int runTarget(const QString &targetBin, const QStringList &arguments);

private:
    Private::ResolvedProduct m_resolvedProduct;
};

} // namespace Qbs

#endif // QBS_RUNENVIRONMENT_H

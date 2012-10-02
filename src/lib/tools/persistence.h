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

#ifndef QBS_PERSISTENCE
#define QBS_PERSISTENCE

#include <tools/error.h>

#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <QMap>
#include <QSharedPointer>
#include <QString>
#include <QVariantMap>

namespace qbs {

typedef int PersistentObjectId;
class PersistentPool;

class PersistentObject
{
public:
    virtual ~PersistentObject() {}
    virtual void load(PersistentPool &, QDataStream &) {}
    virtual void store(PersistentPool &, QDataStream &) const {}
};

class PersistentPool
{
public:
    PersistentPool();
    ~PersistentPool();

    struct HeadData
    {
        QVariantMap projectConfig;
    };

    bool load(const QString &filePath);
    bool setupWriteStream(const QString &filePath);
    void closeStream();
    void clear();
    QDataStream &stream();

    template <typename T>
    inline T *idLoad(QDataStream &s)
    {
        PersistentObjectId id;
        s >> id;
        return loadRaw<T>(id);
    }

    template <class T>
    inline T *loadRaw(PersistentObjectId id)
    {
        if (id < 0)
            return 0;

        PersistentObject *obj = 0;
        if (id < m_loadedRaw.count()) {
            obj = m_loadedRaw.value(id);
        } else {
            int i = m_loadedRaw.count();
            m_loadedRaw.resize(id + 1);
            for (; i < m_loadedRaw.count(); ++i)
                m_loadedRaw[i] = 0;

            obj = new T;
            m_loadedRaw[id] = obj;
            obj->load(*this, m_stream);
        }

        return static_cast<T*>(obj);
    }

    template <class T>
    inline typename T::Ptr idLoadS(QDataStream &s)
    {
        PersistentObjectId id;
        s >> id;
        return load<T>(id);
    }

    template <class T>
    inline QSharedPointer<T> load(PersistentObjectId id)
    {
        if (id < 0)
            return QSharedPointer<T>();

        QSharedPointer<PersistentObject> obj;
        if (id < m_loaded.count()) {
            obj = m_loaded.value(id);
        } else {
            m_loaded.resize(id + 1);
            T *t = new T;
            obj = QSharedPointer<PersistentObject>(t);
            m_loaded[id] = obj;
            obj->load(*this, m_stream);
        }

        return obj.staticCast<T>();
    }

    template <class T>
    void store(QSharedPointer<T> ptr)
    {
        store(ptr.data());
    }

    void store(PersistentObject *object);

    void storeString(const QString &t);
    QString loadString(int id);
    QString idLoadString();

    void storeStringSet(const QSet<QString> &t);
    QSet<QString> loadStringSet(const QList<int> &id);
    QSet<QString> idLoadStringSet();

    void storeStringList(const QStringList &t);
    QStringList loadStringList(const QList<int> &ids);
    QStringList idLoadStringList();

    const HeadData &headData() const { return m_headData; }
    void setHeadData(const HeadData &hd) { m_headData = hd; }

private:
    QDataStream m_stream;
    HeadData m_headData;
    QVector<PersistentObject *> m_loadedRaw;
    QVector<QSharedPointer<PersistentObject> > m_loaded;
    QHash<PersistentObject *, int> m_storageIndices;
    PersistentObjectId m_lastStoredObjectId;

    QVector<QString> m_stringStorage;
    QHash<QString, int> m_inverseStringStorage;
    PersistentObjectId m_lastStoredStringId;
};

template<typename T> struct RemovePointer { typedef T Type; };
template<typename T> struct RemovePointer<T*> { typedef T Type; };

template <typename T>
void loadContainerS(T &container, QDataStream &s, qbs::PersistentPool &pool)
{
    int count;
    s >> count;
    container.clear();
    container.reserve(count);
    for (int i = count; --i >= 0;)
        container += pool.idLoadS<typename T::value_type::Type>(s);
}

template <typename T>
void loadContainer(T &container, QDataStream &s, qbs::PersistentPool &pool)
{
    int count;
    s >> count;
    container.clear();
    container.reserve(count);
    for (int i = count; --i >= 0;)
        container += pool.idLoad<typename RemovePointer<typename T::value_type>::Type>(s);
}

template <typename T>
void storeContainer(T &container, QDataStream &s, qbs::PersistentPool &pool)
{
    s << container.count();
    typename T::const_iterator it = container.constBegin();
    const typename T::const_iterator itEnd = container.constEnd();
    for (; it != itEnd; ++it)
        pool.store(*it);
}

template <typename T>
void storeHashContainer(T &container, QDataStream &s, qbs::PersistentPool &pool)
{
    s << container.count();
    foreach (const typename T::mapped_type &item, container)
        pool.store(item);
}

} // namespace qbs

#endif

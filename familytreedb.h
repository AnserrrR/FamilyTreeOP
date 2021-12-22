#ifndef FAMILYTREEDB_H
#define FAMILYTREEDB_H

#include <QHash>
#include <QFile>
#include "familytreerecord.h"

struct BrowserField
{
    QString fio;
    QDate dateOfBirth;
    QDate dateOfDeath;
    Parents parents;
    unsigned int id;
};

class FamilyTreeDB
{
private:
    static bool modifiedFlag;
    unsigned int idCounter = 1;
    QHash<unsigned int, FamilyTreeRecord> DB;
    unsigned int getRecordByFio(const QString& fio) const;

public:
    FamilyTreeDB();
    int count() const;
    int append(FamilyTreeRecord& record);
    void remove(unsigned int id);
    int update(const FamilyTreeRecord& record);
    void record(unsigned int id, FamilyTreeRecord& record) const;
    const QVector<BrowserField> records() const;
    bool save(QString filename) const;
    bool load(QString filename);
    void clear();
    bool isModified();
    const QString recordVerification(const FamilyTreeRecord &verifiableRecord, bool newRecord = false);
};

#endif // FAMILYTREEDB_H

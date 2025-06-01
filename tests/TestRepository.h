#ifndef TESTREPOSITORY_H
#define TESTREPOSITORY_H

#include <QtTest>
#include "../include/repository/CSVRepository.h"
#include "../include/repository/JSONRepository.h"
#include <QDir>
#include <QFile>

class TestRepository : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void testCSVRepositoryAddGetRemove();
    void testCSVRepositoryUpdate();
    void testCSVRepositoryGetAll();
    void testJSONRepositoryAddGetRemove();
    void testJSONRepositoryUpdate();
    void testJSONRepositoryGetAll();

private:
    QString testCSVFile;
    QString testJSONFile;
    std::unique_ptr<Artifact> createTestArtifact(const QString& id);
};

#endif // TESTREPOSITORY_H

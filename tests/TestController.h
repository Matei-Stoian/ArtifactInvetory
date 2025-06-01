#ifndef TESTCONTROLLER_H
#define TESTCONTROLLER_H

#include <QtTest>
#include "../include/controller/ArtifactController.h"
#include "../include/repository/CSVRepository.h"
#include "../include/filters/TypeFilter.h"
#include "../include/filters/PeriodFilter.h"
#include "../include/filters/AndFilter.h"
#include <memory>

class MockRepository; // Forward declaration of the MockRepository class

class TestController : public QObject
{
    Q_OBJECT

private slots:
    void testAddArtifact();
    void testRemoveArtifact();
    void testUpdateArtifact();
    void testFindArtifact();
    void testGetAllArtifacts();
    void testFilterArtifacts();
    void testUndoRedo();
    void testSwitchRepository();

private:
    std::unique_ptr<Artifact> createTestArtifact(const QString& id);
};

#endif // TESTCONTROLLER_H

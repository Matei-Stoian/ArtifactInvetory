#ifndef TESTARTIFACT_H
#define TESTARTIFACT_H

#include <QtTest>
#include "../include/domain/Artifact.h"

class TestArtifact : public QObject
{
    Q_OBJECT

private slots:
    void testConstructor();
    void testSettersGetters();
    void testToCSV();
    void testFromCSV();
    void testToJson();
    void testFromJson();
    void testComparison();
    void testTypeConversion();
    void testConditionConversion();
};

#endif // TESTARTIFACT_H

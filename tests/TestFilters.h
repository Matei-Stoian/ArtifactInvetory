#ifndef TESTFILTERS_H
#define TESTFILTERS_H

#include <QtTest>
#include "../include/filters/TypeFilter.h"
#include "../include/filters/PeriodFilter.h"
#include "../include/filters/LocationFilter.h"
#include "../include/filters/MaterialFilter.h"
#include "../include/filters/AndFilter.h"
#include "../include/filters/OrFilter.h"

class TestFilters : public QObject
{
    Q_OBJECT

private slots:
    void testTypeFilter();
    void testPeriodFilter();
    void testLocationFilter();
    void testMaterialFilter();
    void testAndFilter();
    void testOrFilter();
    void testComplexFilters();

private:
    Artifact createTestArtifact();
};

#endif // TESTFILTERS_H

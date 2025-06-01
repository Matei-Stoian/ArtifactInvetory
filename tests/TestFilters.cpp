#include <QtTest>
#include "TestFilters.h" // Added this line
#include "../include/filters/TypeFilter.h"
#include "../include/filters/PeriodFilter.h"
#include "../include/filters/LocationFilter.h"
#include "../include/filters/MaterialFilter.h"
#include "../include/filters/AndFilter.h"
#include "../include/filters/OrFilter.h"

Artifact TestFilters::createTestArtifact()
{
    return Artifact(
        "TEST123",
        "Test Artifact",
        ArtifactType::Pottery,
        "Clay",
        "Roman",
        "1st century CE",
        "Pompeii",
        "40.7484° N, 14.4848° E",
        QDate(1950, 3, 15),
        ArtifactCondition::Good
    );
}

void TestFilters::testTypeFilter()
{
    TypeFilter potteryFilter(ArtifactType::Pottery);
    TypeFilter weaponFilter(ArtifactType::Weapon);

    Artifact artifact = createTestArtifact();

    // Should match the pottery filter but not the weapon filter
    QVERIFY(potteryFilter.matches(artifact));
    QVERIFY(!weaponFilter.matches(artifact));

    // Change the artifact type and verify the results are reversed
    artifact.setType(ArtifactType::Weapon);
    QVERIFY(!potteryFilter.matches(artifact));
    QVERIFY(weaponFilter.matches(artifact));

    // Test clone
    auto clonedFilter = potteryFilter.clone();
    QVERIFY(clonedFilter);

    artifact.setType(ArtifactType::Pottery);
    QVERIFY(clonedFilter->matches(artifact));
}

void TestFilters::testPeriodFilter()
{
    PeriodFilter romanFilter("Roman");
    PeriodFilter greekFilter("Greek");
    PeriodFilter centuryFilter("century");  // Partial match

    Artifact artifact = createTestArtifact();
    artifact.setPeriod("1st century CE");

    // Test exact and partial matches
    QVERIFY(!romanFilter.matches(artifact));  // Roman is not in "1st century CE"
    QVERIFY(!greekFilter.matches(artifact));
    QVERIFY(centuryFilter.matches(artifact));  // "century" is in "1st century CE"

    // Change the period and verify
    artifact.setPeriod("Roman Republic");
    QVERIFY(romanFilter.matches(artifact));
    QVERIFY(!greekFilter.matches(artifact));
    QVERIFY(!centuryFilter.matches(artifact));

    // Test clone
    auto clonedFilter = romanFilter.clone();
    QVERIFY(clonedFilter);
    QVERIFY(clonedFilter->matches(artifact));
}

void TestFilters::testLocationFilter()
{
    LocationFilter pompeiiFilter("Pompeii");
    LocationFilter romeFilter("Rome");

    Artifact artifact = createTestArtifact();

    // Test exact location matches
    QVERIFY(pompeiiFilter.matches(artifact));
    QVERIFY(!romeFilter.matches(artifact));

    // Change the site and verify
    artifact.setSite("Rome");
    QVERIFY(!pompeiiFilter.matches(artifact));
    QVERIFY(romeFilter.matches(artifact));

    // Test clone
    auto clonedFilter = romeFilter.clone();
    QVERIFY(clonedFilter);
    QVERIFY(clonedFilter->matches(artifact));
}

void TestFilters::testMaterialFilter()
{
    MaterialFilter clayFilter("Clay");
    MaterialFilter goldFilter("Gold");
    MaterialFilter partialFilter("ay");  // Partial match for "Clay"

    Artifact artifact = createTestArtifact();

    // Test exact and partial matches
    QVERIFY(clayFilter.matches(artifact));
    QVERIFY(!goldFilter.matches(artifact));
    QVERIFY(partialFilter.matches(artifact));

    // Change the material and verify
    artifact.setMaterial("Gold");
    QVERIFY(!clayFilter.matches(artifact));
    QVERIFY(goldFilter.matches(artifact));
    QVERIFY(!partialFilter.matches(artifact));

    // Test clone
    auto clonedFilter = goldFilter.clone();
    QVERIFY(clonedFilter);
    QVERIFY(clonedFilter->matches(artifact));
}

void TestFilters::testAndFilter()
{
    auto typeFilter = std::make_unique<TypeFilter>(ArtifactType::Pottery);
    auto materialFilter = std::make_unique<MaterialFilter>("Clay");

    AndFilter filter(std::move(typeFilter), std::move(materialFilter));

    Artifact artifact = createTestArtifact();

    // Should match both criteria
    QVERIFY(filter.matches(artifact));

    // Change one property to make it not match
    artifact.setMaterial("Stone");
    QVERIFY(!filter.matches(artifact));

    // Change back to match material but change type to not match
    artifact.setMaterial("Clay");
    artifact.setType(ArtifactType::Weapon);
    QVERIFY(!filter.matches(artifact));

    // Test clone
    auto clonedFilter = filter.clone();
    QVERIFY(clonedFilter);

    // Reset artifact to match again
    artifact.setType(ArtifactType::Pottery);
    QVERIFY(clonedFilter->matches(artifact));
}

void TestFilters::testOrFilter()
{
    auto typeFilter = std::make_unique<TypeFilter>(ArtifactType::Pottery);
    auto materialFilter = std::make_unique<MaterialFilter>("Gold");

    OrFilter filter(std::move(typeFilter), std::move(materialFilter));

    Artifact artifact = createTestArtifact();

    // Should match the type criteria but not material
    QVERIFY(filter.matches(artifact));

    // Change type to not match but keep material not matching
    artifact.setType(ArtifactType::Weapon);
    QVERIFY(!filter.matches(artifact));

    // Change material to match
    artifact.setMaterial("Gold");
    QVERIFY(filter.matches(artifact));

    // Test clone
    auto clonedFilter = filter.clone();
    QVERIFY(clonedFilter);
    QVERIFY(clonedFilter->matches(artifact));
}

void TestFilters::testComplexFilters()
{
    // Create a complex filter: (Pottery OR Sculpture) AND (Roman OR Greek)

    // First part: Pottery OR Sculpture
    auto potteryFilter = std::make_unique<TypeFilter>(ArtifactType::Pottery);
    auto sculptureFilter = std::make_unique<TypeFilter>(ArtifactType::Sculpture);
    auto typeFilter = std::make_unique<OrFilter>(std::move(potteryFilter), std::move(sculptureFilter));

    // Second part: Roman OR Greek
    auto romanFilter = std::make_unique<PeriodFilter>("Roman");
    auto greekFilter = std::make_unique<PeriodFilter>("Greek");
    auto periodFilter = std::make_unique<OrFilter>(std::move(romanFilter), std::move(greekFilter));

    // Combined filter
    AndFilter complexFilter(std::move(typeFilter), std::move(periodFilter));

    // Test various combinations
    Artifact artifact = createTestArtifact();

    // Initially it's Pottery but not Roman or Greek period
    QVERIFY(!complexFilter.matches(artifact));

    // Make it Roman period
    artifact.setPeriod("Roman Imperial");
    QVERIFY(complexFilter.matches(artifact));

    // Change to Sculpture but keep Roman period
    artifact.setType(ArtifactType::Sculpture);
    QVERIFY(complexFilter.matches(artifact));

    // Change to Greek period
    artifact.setPeriod("Greek Classical");
    QVERIFY(complexFilter.matches(artifact));

    // Change to non-matching type and period
    artifact.setType(ArtifactType::Weapon);
    artifact.setPeriod("Medieval");
    QVERIFY(!complexFilter.matches(artifact));
}


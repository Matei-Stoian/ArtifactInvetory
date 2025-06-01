#include <QtTest>
#include "TestRepository.h"
#include "../include/repository/CSVRepository.h"
#include "../include/repository/JSONRepository.h"
#include <QDir>
#include <QFile>

void TestRepository::init()
{
    // Create temporary test files
    testCSVFile = "test_artifacts.csv";
    testJSONFile = "test_artifacts.json";

    // Make sure test files don't already exist
    QFile csvFile(testCSVFile);
    if (csvFile.exists()) {
        csvFile.remove();
    }

    QFile jsonFile(testJSONFile);
    if (jsonFile.exists()) {
        jsonFile.remove();
    }
}

void TestRepository::cleanup()
{
    // Clean up test files
    QFile csvFile(testCSVFile);
    if (csvFile.exists()) {
        csvFile.remove();
    }

    QFile jsonFile(testJSONFile);
    if (jsonFile.exists()) {
        jsonFile.remove();
    }
}

std::unique_ptr<Artifact> TestRepository::createTestArtifact(const QString& id)
{
    return std::make_unique<Artifact>(
        id,
        "Test Artifact " + id,
        ArtifactType::Pottery,
        "Clay",
        "Test Culture",
        "Test Period",
        "Test Site",
        "0.0, 0.0",
        QDate::currentDate(),
        ArtifactCondition::Good,
        "Test Description",
        10.0, 5.0, 3.0, 200.0,
        "Test Location",
        false
    );
}

void TestRepository::testCSVRepositoryAddGetRemove()
{
    CSVRepository repo(testCSVFile);

    // Test adding an artifact
    auto artifact1 = createTestArtifact("CSV1");
    QVERIFY(repo.addArtifact(std::move(artifact1)));

    // Test retrieving the artifact
    auto retrieved = repo.getByCatalogNumber("CSV1");
    QVERIFY(retrieved != nullptr);
    QCOMPARE(retrieved->getCatalogNumber(), "CSV1");
    QCOMPARE(retrieved->getName(), "Test Artifact CSV1");

    // Test removing the artifact
    QVERIFY(repo.removeArtifact("CSV1"));

    // Verify it was removed
    auto shouldBeNull = repo.getByCatalogNumber("CSV1");
    QVERIFY(shouldBeNull == nullptr);
}

void TestRepository::testCSVRepositoryUpdate()
{
    CSVRepository repo(testCSVFile);

    // Add an artifact
    auto artifact = createTestArtifact("CSV2");
    QVERIFY(repo.addArtifact(std::move(artifact)));

    // Create updated version
    auto updatedArtifact = std::make_unique<Artifact>(
        "CSV2",                // Same catalog number
        "Updated Artifact",    // Updated name
        ArtifactType::Sculpture, // Updated type
        "Marble",              // Updated material
        "Test Culture",
        "Test Period",
        "Test Site",
        "0.0, 0.0",
        QDate::currentDate(),
        ArtifactCondition::Good
    );

    // Update the artifact
    QVERIFY(repo.updateArtifact(*updatedArtifact));

    // Verify the update
    auto retrieved = repo.getByCatalogNumber("CSV2");
    QVERIFY(retrieved != nullptr);
    QCOMPARE(retrieved->getName(), "Updated Artifact");
    QCOMPARE(retrieved->getType(), ArtifactType::Sculpture);
    QCOMPARE(retrieved->getMaterial(), "Marble");
}

void TestRepository::testCSVRepositoryGetAll()
{
    CSVRepository repo(testCSVFile);

    // Initially should be empty
    auto artifacts = repo.getAll();
    QVERIFY(artifacts.empty());

    // Add multiple artifacts
    repo.addArtifact(createTestArtifact("CSV3"));
    repo.addArtifact(createTestArtifact("CSV4"));
    repo.addArtifact(createTestArtifact("CSV5"));

    // Get all artifacts
    artifacts = repo.getAll();
    QCOMPARE(artifacts.size(), 3);

    // Check that we have the right artifacts
    QStringList ids;
    for (const auto& artifact : artifacts) {
        ids << artifact->getCatalogNumber();
    }

    QVERIFY(ids.contains("CSV3"));
    QVERIFY(ids.contains("CSV4"));
    QVERIFY(ids.contains("CSV5"));
}

void TestRepository::testJSONRepositoryAddGetRemove()
{
    JSONRepository repo(testJSONFile);

    // Test adding an artifact
    auto artifact1 = createTestArtifact("JSON1");
    QVERIFY(repo.addArtifact(std::move(artifact1)));

    // Test retrieving the artifact
    auto retrieved = repo.getByCatalogNumber("JSON1");
    QVERIFY(retrieved != nullptr);
    QCOMPARE(retrieved->getCatalogNumber(), "JSON1");
    QCOMPARE(retrieved->getName(), "Test Artifact JSON1");

    // Test removing the artifact
    QVERIFY(repo.removeArtifact("JSON1"));

    // Verify it was removed
    auto shouldBeNull = repo.getByCatalogNumber("JSON1");
    QVERIFY(shouldBeNull == nullptr);
}

void TestRepository::testJSONRepositoryUpdate()
{
    JSONRepository repo(testJSONFile);

    // Add an artifact
    auto artifact = createTestArtifact("JSON2");
    QVERIFY(repo.addArtifact(std::move(artifact)));

    // Create updated version
    auto updatedArtifact = std::make_unique<Artifact>(
        "JSON2",               // Same catalog number
        "Updated JSON Artifact", // Updated name
        ArtifactType::Coin,    // Updated type
        "Gold",                // Updated material
        "Test Culture",
        "Test Period",
        "Test Site",
        "0.0, 0.0",
        QDate::currentDate(),
        ArtifactCondition::Excellent
    );

    // Update the artifact
    QVERIFY(repo.updateArtifact(*updatedArtifact));

    // Verify the update
    auto retrieved = repo.getByCatalogNumber("JSON2");
    QVERIFY(retrieved != nullptr);
    QCOMPARE(retrieved->getName(), "Updated JSON Artifact");
    QCOMPARE(retrieved->getType(), ArtifactType::Coin);
    QCOMPARE(retrieved->getMaterial(), "Gold");
    QCOMPARE(retrieved->getCondition(), ArtifactCondition::Excellent);
}

void TestRepository::testJSONRepositoryGetAll()
{
    JSONRepository repo(testJSONFile);

    // Initially should be empty
    auto artifacts = repo.getAll();
    QVERIFY(artifacts.empty());

    // Add multiple artifacts
    repo.addArtifact(createTestArtifact("JSON3"));
    repo.addArtifact(createTestArtifact("JSON4"));
    repo.addArtifact(createTestArtifact("JSON5"));

    // Get all artifacts
    artifacts = repo.getAll();
    QCOMPARE(artifacts.size(), 3);

    // Check that we have the right artifacts
    QStringList ids;
    for (const auto& artifact : artifacts) {
        ids << artifact->getCatalogNumber();
    }

    QVERIFY(ids.contains("JSON3"));
    QVERIFY(ids.contains("JSON4"));
    QVERIFY(ids.contains("JSON5"));
}


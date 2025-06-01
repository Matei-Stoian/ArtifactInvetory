#include <QtTest>
#include "TestController.h" // Added this line
#include "../include/controller/ArtifactController.h"
#include "../include/repository/CSVRepository.h"
#include "../include/filters/TypeFilter.h"
#include "../include/filters/PeriodFilter.h"
#include "../include/filters/AndFilter.h"
#include <memory>

class MockRepository : public BaseRepository {
public:
    MockRepository() = default;
    bool addArtifact(std::unique_ptr<Artifact> artifact) override {
        if (artifact) {
            QString id = artifact->getCatalogNumber();
            artifacts[id] = std::make_unique<Artifact>(*artifact);
            return true;
        }
        return false;
    }

    bool removeArtifact(const QString& catalogNumber) override {
        return artifacts.erase(catalogNumber) > 0;
    }

    bool updateArtifact(const Artifact& artifact) override {
        QString id = artifact.getCatalogNumber();
        if (artifacts.find(id) != artifacts.end()) {
            artifacts[id] = std::make_unique<Artifact>(artifact);
            return true;
        }
        return false;
    }

    std::unique_ptr<Artifact> getByCatalogNumber(const QString& catalogNumber) override {
        auto it = artifacts.find(catalogNumber);
        if (it != artifacts.end()) {
            return std::make_unique<Artifact>(*(it->second));
        }
        return nullptr;
    }

    std::vector<std::unique_ptr<Artifact>> getAll() override {
        std::vector<std::unique_ptr<Artifact>> result;
        for (const auto& pair : artifacts) {
            result.push_back(std::make_unique<Artifact>(*(pair.second)));
        }
        return result;
    }

    // Implementing the missing pure virtual functions
    bool save(const std::vector<std::unique_ptr<Artifact>>& artifacts) override {
        // In this mock implementation, we just return true
        return true;
    }

    std::vector<std::unique_ptr<Artifact>> load() override {
        // Return a copy of our current artifacts
        return getAll();
    }

private:
    std::map<QString, std::unique_ptr<Artifact>> artifacts;
};

std::unique_ptr<Artifact> TestController::createTestArtifact(const QString& id)
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
        ArtifactCondition::Good
    );
}

void TestController::testAddArtifact()
{
    auto mockRepo = std::make_unique<MockRepository>();
    ArtifactController controller(std::move(mockRepo));

    // Add a test artifact
    auto artifact = createTestArtifact("C1");
    QVERIFY(controller.addArtifact(std::move(artifact)));

    // Verify it was added correctly
    auto retrieved = controller.findArtifact("C1");
    QVERIFY(retrieved != nullptr);
    QCOMPARE(retrieved->getCatalogNumber(), "C1");
    QCOMPARE(retrieved->getName(), "Test Artifact C1");
}

void TestController::testRemoveArtifact()
{
    auto mockRepo = std::make_unique<MockRepository>();
    ArtifactController controller(std::move(mockRepo));

    // Add an artifact then remove it
    controller.addArtifact(createTestArtifact("C2"));
    QVERIFY(controller.removeArtifact("C2"));

    // Verify it was removed
    auto shouldBeNull = controller.findArtifact("C2");
    QVERIFY(shouldBeNull == nullptr);
}

void TestController::testUpdateArtifact()
{
    auto mockRepo = std::make_unique<MockRepository>();
    ArtifactController controller(std::move(mockRepo));

    // Add an artifact
    controller.addArtifact(createTestArtifact("C3"));

    // Create updated version
    auto updatedArtifact = std::make_unique<Artifact>(
        "C3",                 // Same ID
        "Updated Artifact",   // Changed name
        ArtifactType::Jewelry, // Changed type
        "Gold",               // Changed material
        "Updated Culture",
        "Updated Period",
        "Test Site",
        "0.0, 0.0",
        QDate::currentDate(),
        ArtifactCondition::Excellent
    );

    // Update the artifact
    QVERIFY(controller.updateArtifact(std::move(updatedArtifact)));

    // Verify the update
    auto retrieved = controller.findArtifact("C3");
    QVERIFY(retrieved != nullptr);
    QCOMPARE(retrieved->getName(), "Updated Artifact");
    QCOMPARE(retrieved->getType(), ArtifactType::Jewelry);
    QCOMPARE(retrieved->getMaterial(), "Gold");
}

void TestController::testFindArtifact()
{
    auto mockRepo = std::make_unique<MockRepository>();
    ArtifactController controller(std::move(mockRepo));

    // Add several artifacts
    controller.addArtifact(createTestArtifact("C4"));
    controller.addArtifact(createTestArtifact("C5"));
    controller.addArtifact(createTestArtifact("C6"));

    // Test finding one that exists
    auto found = controller.findArtifact("C5");
    QVERIFY(found != nullptr);
    QCOMPARE(found->getCatalogNumber(), "C5");

    // Test finding one that doesn't exist
    auto notFound = controller.findArtifact("NonExistent");
    QVERIFY(notFound == nullptr);
}

void TestController::testGetAllArtifacts()
{
    auto mockRepo = std::make_unique<MockRepository>();
    ArtifactController controller(std::move(mockRepo));

    // Add several artifacts
    controller.addArtifact(createTestArtifact("C7"));
    controller.addArtifact(createTestArtifact("C8"));
    controller.addArtifact(createTestArtifact("C9"));

    // Get all artifacts
    auto artifacts = controller.getAllArtifacts();
    QCOMPARE(artifacts.size(), 3);

    // Check that all expected artifacts are present
    QStringList ids;
    for (const auto& artifact : artifacts) {
        ids << artifact->getCatalogNumber();
    }

    QVERIFY(ids.contains("C7"));
    QVERIFY(ids.contains("C8"));
    QVERIFY(ids.contains("C9"));
}

void TestController::testFilterArtifacts()
{
    auto mockRepo = std::make_unique<MockRepository>();
    ArtifactController controller(std::move(mockRepo));

    // Add artifacts with different types and periods
    auto artifact1 = std::make_unique<Artifact>(
        "CF1", "Vase", ArtifactType::Pottery, "Clay", "Greek",
        "Classical", "Athens", "0.0,0.0", QDate::currentDate()
    );

    auto artifact2 = std::make_unique<Artifact>(
        "CF2", "Sword", ArtifactType::Weapon, "Iron", "Roman",
        "Roman Imperial", "Rome", "0.0,0.0", QDate::currentDate()
    );

    auto artifact3 = std::make_unique<Artifact>(
        "CF3", "Bowl", ArtifactType::Pottery, "Clay", "Roman",
        "Roman Republican", "Pompeii", "0.0,0.0", QDate::currentDate()
    );

    controller.addArtifact(std::move(artifact1));
    controller.addArtifact(std::move(artifact2));
    controller.addArtifact(std::move(artifact3));

    // Test filtering by type
    TypeFilter potteryFilter(ArtifactType::Pottery);
    auto potteryArtifacts = controller.filterArtifacts(potteryFilter);
    QCOMPARE(potteryArtifacts.size(), 2);  // Should find 2 pottery artifacts

    // Test filtering by period
    PeriodFilter romanFilter("Roman"); // This will match "Roman Imperial" and "Roman Republican"
    auto romanArtifacts = controller.filterArtifacts(romanFilter);
    QCOMPARE(romanArtifacts.size(), 2);  // Should find 2 Roman artifacts

    // Test combined filter (Pottery AND Roman Republican)
    auto typeFilterForAnd = std::make_unique<TypeFilter>(ArtifactType::Pottery);
    auto periodFilterForAnd = std::make_unique<PeriodFilter>("Roman Republican");
    AndFilter andFilter{std::move(typeFilterForAnd), std::move(periodFilterForAnd)}; // Corrected initialization

    auto filteredArtifacts = controller.filterArtifacts(andFilter);
    QCOMPARE(filteredArtifacts.size(), 1);  // Should find 1 Roman Republican pottery artifact
    QCOMPARE(filteredArtifacts[0]->getCatalogNumber(), "CF3");
}

void TestController::testUndoRedo()
{
    auto mockRepo = std::make_unique<MockRepository>();
    ArtifactController controller(std::move(mockRepo));

    // Initially can't undo or redo
    QVERIFY(!controller.canUndo());
    QVERIFY(!controller.canRedo());

    // Add an artifact
    auto artifact = createTestArtifact("UR1");
    controller.addArtifact(std::move(artifact));
    QVERIFY(controller.canUndo());
    QVERIFY(!controller.canRedo());

    // Undo the add
    controller.undo();
    QVERIFY(!controller.canUndo());
    QVERIFY(controller.canRedo());



    // Redo the add
    controller.redo();
    QVERIFY(controller.canUndo());
    QVERIFY(!controller.canRedo());

    // Verify the artifact was added back
    auto shouldExist = controller.findArtifact("UR1");
    QVERIFY(shouldExist != nullptr);
}

void TestController::testSwitchRepository()
{
    // Create a controller with a mock repository and add an artifact
    auto mockRepo1 = std::make_unique<MockRepository>();
    ArtifactController controller(std::move(mockRepo1));
    controller.addArtifact(createTestArtifact("SR1"));

    // Switch to a new empty repository
    auto mockRepo2 = std::make_unique<MockRepository>();
    controller.switchRepository(std::move(mockRepo2));

    // Verify the artifact from the first repo is no longer accessible
    auto shouldBeNull = controller.findArtifact("SR1");
    QVERIFY(shouldBeNull == nullptr);

    // Add a new artifact to the second repo
    controller.addArtifact(createTestArtifact("SR2"));

    // Verify the new artifact exists
    auto shouldExist = controller.findArtifact("SR2");
    QVERIFY(shouldExist != nullptr);
}


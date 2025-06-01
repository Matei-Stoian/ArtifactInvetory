#include <QtTest>
#include "TestArtifact.h"
#include "../include/domain/Artifact.h"

void TestArtifact::testConstructor()
{
    // Test basic constructor
    Artifact artifact("A123", "Ancient Vase", ArtifactType::Pottery,
                     "Clay", "Greek", "Classical", "Athens", "37.9838° N, 23.7275° E",
                     QDate(1950, 5, 15), ArtifactCondition::Good);

    QCOMPARE(artifact.getCatalogNumber(), "A123");
    QCOMPARE(artifact.getName(), "Ancient Vase");
    QCOMPARE(artifact.getType(), ArtifactType::Pottery);
    QCOMPARE(artifact.getMaterial(), "Clay");
    QCOMPARE(artifact.getCulture(), "Greek");
    QCOMPARE(artifact.getPeriod(), "Classical");
    QCOMPARE(artifact.getSite(), "Athens");
    QCOMPARE(artifact.getCoordinates(), "37.9838° N, 23.7275° E");
    QCOMPARE(artifact.getDiscoveryDate(), QDate(1950, 5, 15));
    QCOMPARE(artifact.getCondition(), ArtifactCondition::Good);

    // Test full constructor
    Artifact fullArtifact("B456", "Gold Coin", ArtifactType::Coin,
                         "Gold", "Roman", "Imperial", "Rome", "41.9028° N, 12.4964° E",
                         QDate(1970, 3, 10), ArtifactCondition::Excellent,
                         "A well-preserved gold coin from the reign of Augustus", 2.5, 2.5, 0.2, 8.5,
                         "Cabinet 3, Drawer 2", true);

    QCOMPARE(fullArtifact.getCatalogNumber(), "B456");
    QCOMPARE(fullArtifact.getName(), "Gold Coin");
    QCOMPARE(fullArtifact.getDescription(), "A well-preserved gold coin from the reign of Augustus");
    QCOMPARE(fullArtifact.getLength(), 2.5);
    QCOMPARE(fullArtifact.getWidth(), 2.5);
    QCOMPARE(fullArtifact.getHeight(), 0.2);
    QCOMPARE(fullArtifact.getWeight(), 8.5);
    QCOMPARE(fullArtifact.getStorageLocation(), "Cabinet 3, Drawer 2");
    QVERIFY(fullArtifact.isOnDisplay());
}

void TestArtifact::testSettersGetters()
{
    Artifact artifact;

    artifact.setCatalogNumber("C789");
    QCOMPARE(artifact.getCatalogNumber(), "C789");

    artifact.setName("Bronze Statue");
    QCOMPARE(artifact.getName(), "Bronze Statue");

    artifact.setType(ArtifactType::Sculpture);
    QCOMPARE(artifact.getType(), ArtifactType::Sculpture);

    artifact.setMaterial("Bronze");
    QCOMPARE(artifact.getMaterial(), "Bronze");

    artifact.setCulture("Etruscan");
    QCOMPARE(artifact.getCulture(), "Etruscan");

    artifact.setPeriod("6th century BCE");
    QCOMPARE(artifact.getPeriod(), "6th century BCE");

    artifact.setSite("Vulci");
    QCOMPARE(artifact.getSite(), "Vulci");

    artifact.setCoordinates("42.4189° N, 11.6297° E");
    QCOMPARE(artifact.getCoordinates(), "42.4189° N, 11.6297° E");

    QDate discoveryDate(1990, 7, 22);
    artifact.setDiscoveryDate(discoveryDate);
    QCOMPARE(artifact.getDiscoveryDate(), discoveryDate);

    artifact.setCondition(ArtifactCondition::Fair);
    QCOMPARE(artifact.getCondition(), ArtifactCondition::Fair);

    artifact.setDescription("Bronze statue depicting a warrior");
    QCOMPARE(artifact.getDescription(), "Bronze statue depicting a warrior");

    artifact.setLength(45.0);
    QCOMPARE(artifact.getLength(), 45.0);

    artifact.setWidth(15.0);
    QCOMPARE(artifact.getWidth(), 15.0);

    artifact.setHeight(120.0);
    QCOMPARE(artifact.getHeight(), 120.0);

    artifact.setWeight(5000.0);
    QCOMPARE(artifact.getWeight(), 5000.0);

    artifact.setStorageLocation("Exhibition Hall B");
    QCOMPARE(artifact.getStorageLocation(), "Exhibition Hall B");

    artifact.setOnDisplay(true);
    QVERIFY(artifact.isOnDisplay());
}

void TestArtifact::testToCSV()
{
    Artifact artifact("D123", "Stone Tablet", ArtifactType::Tablet,
                     "Stone", "Sumerian", "3rd millennium BCE", "Ur",
                     "30.9626N 46.1062E", QDate(1926, 11, 5), // Changed coordinates
                     ArtifactCondition::Good, "Cuneiform tablet", 15.0, 10.0,
                     2.0, 750.0, "Archive Room", false);

    QString csv = artifact.toCSV();

    // Verify that the CSV contains all the necessary fields
    QStringList fields = csv.split(',');
    QCOMPARE(fields.size(), 17);
    QCOMPARE(fields[0], "D123");
    QCOMPARE(fields[1], "Stone Tablet");
    QCOMPARE(fields[2], "Tablet");
    QCOMPARE(fields[3], "Stone");
    QCOMPARE(fields[4], "Sumerian");
    QCOMPARE(fields[9], "Good");
    QCOMPARE(fields[16], "0");  // onDisplay = false
}

void TestArtifact::testFromCSV()
{
    QString csvLine = "E456,Bronze Dagger,Weapon,Bronze,Celtic,Iron Age,Hallstatt,47.5622N 13.6493E,1880-06-15,Fair,Ceremonial bronze dagger with decorated handle,25.0,5.0,1.5,350.0,Storage Box 12,1"; // Changed coordinates

    auto artifact = Artifact::fromCSV(csvLine);

    QVERIFY(artifact != nullptr);
    QCOMPARE(artifact->getCatalogNumber(), "E456");
    QCOMPARE(artifact->getName(), "Bronze Dagger");
    QCOMPARE(artifact->getType(), ArtifactType::Weapon);
    QCOMPARE(artifact->getMaterial(), "Bronze");
    QCOMPARE(artifact->getCulture(), "Celtic");
    QCOMPARE(artifact->getPeriod(), "Iron Age");
    QCOMPARE(artifact->getSite(), "Hallstatt"); // Added check for site
    QCOMPARE(artifact->getCoordinates(), "47.5622N 13.6493E"); // Added check for coordinates
    QCOMPARE(artifact->getDiscoveryDate(), QDate(1880, 6, 15));
    QCOMPARE(artifact->getCondition(), ArtifactCondition::Fair);
    QCOMPARE(artifact->getStorageLocation(), "Storage Box 12");
    QVERIFY(artifact->isOnDisplay());
}

void TestArtifact::testToJson()
{
    Artifact artifact("F789", "Silver Chalice", ArtifactType::Jewelry,
                    "Silver", "Byzantine", "6th century CE", "Constantinople",
                    "41.0082° N, 28.9784° E", QDate(1912, 3, 20),
                    ArtifactCondition::Excellent, "Ornate silver chalice with engravings",
                    12.0, 8.0, 15.0, 600.0, "Secure Vault", true);

    QJsonObject json = artifact.toJson();

    QCOMPARE(json["catalogNumber"].toString(), "F789");
    QCOMPARE(json["name"].toString(), "Silver Chalice");
    QCOMPARE(json["type"].toString(), "Jewelry");
    QCOMPARE(json["material"].toString(), "Silver");
    QCOMPARE(json["culture"].toString(), "Byzantine");
    QCOMPARE(json["period"].toString(), "6th century CE");
    QCOMPARE(json["condition"].toString(), "Excellent");
    QCOMPARE(json["description"].toString(), "Ornate silver chalice with engravings");
    QCOMPARE(json["length"].toDouble(), 12.0);
    QCOMPARE(json["onDisplay"].toBool(), true);
}

void TestArtifact::testFromJson()
{
    QJsonObject json;
    json["catalogNumber"] = "G123";
    json["name"] = "Bone Comb";
    json["type"] = "Bone";
    json["material"] = "Bone";
    json["culture"] = "Viking";
    json["period"] = "10th century CE";
    json["site"] = "Jorvik";
    json["coordinates"] = "53.9599° N, 1.0873° W";
    json["discoveryDate"] = "1980-05-22";
    json["condition"] = "Good";
    json["description"] = "Decorated bone comb with animal motifs";
    json["length"] = 10.0;
    json["width"] = 5.0;
    json["height"] = 1.0;
    json["weight"] = 30.0;
    json["storageLocation"] = "Display Case 7";
    json["onDisplay"] = true;

    auto artifact = Artifact::fromJson(json);

    QVERIFY(artifact != nullptr);
    QCOMPARE(artifact->getCatalogNumber(), "G123");
    QCOMPARE(artifact->getName(), "Bone Comb");
    QCOMPARE(artifact->getType(), ArtifactType::Bone);
    QCOMPARE(artifact->getMaterial(), "Bone");
    QCOMPARE(artifact->getCulture(), "Viking");
    QCOMPARE(artifact->getPeriod(), "10th century CE");
    QCOMPARE(artifact->getDiscoveryDate(), QDate(1980, 5, 22));
    QVERIFY(artifact->isOnDisplay());
}

void TestArtifact::testComparison()
{
    Artifact artifact1("H123", "Clay Lamp", ArtifactType::Pottery,
                     "Clay", "Roman", "1st century CE", "Pompeii",
                     "40.7484° N, 14.4848° E", QDate(1920, 6, 5));

    Artifact artifact2("H123", "Clay Lamp", ArtifactType::Pottery,
                     "Clay", "Roman", "1st century CE", "Pompeii",
                     "40.7484° N, 14.4848° E", QDate(1920, 6, 5));

    Artifact artifact3("I456", "Gold Ring", ArtifactType::Jewelry,
                     "Gold", "Anglo-Saxon", "7th century CE", "Sutton Hoo",
                     "52.0889° N, 1.3414° E", QDate(1939, 5, 10));

    // Test operator==
    QVERIFY(artifact1 == artifact2);
    QVERIFY(!(artifact1 == artifact3));
}

void TestArtifact::testTypeConversion()
{
    // Test typeToString
    QCOMPARE(Artifact::typeToString(ArtifactType::Pottery), "Pottery");
    QCOMPARE(Artifact::typeToString(ArtifactType::Weapon), "Weapon");
    QCOMPARE(Artifact::typeToString(ArtifactType::Unknown), "Unknown");

    // Test stringToType
    QCOMPARE(Artifact::stringToType("Pottery"), ArtifactType::Pottery);
    QCOMPARE(Artifact::stringToType("Weapon"), ArtifactType::Weapon);
    QCOMPARE(Artifact::stringToType("Unknown"), ArtifactType::Unknown);
    QCOMPARE(Artifact::stringToType("InvalidType"), ArtifactType::Unknown);
}

void TestArtifact::testConditionConversion()
{
    // Test conditionToString
    QCOMPARE(Artifact::conditionToString(ArtifactCondition::Excellent), "Excellent");
    QCOMPARE(Artifact::conditionToString(ArtifactCondition::Fair), "Fair");
    QCOMPARE(Artifact::conditionToString(ArtifactCondition::Fragments), "Fragments");

    // Test stringToCondition
    QCOMPARE(Artifact::stringToCondition("Excellent"), ArtifactCondition::Excellent);
    QCOMPARE(Artifact::stringToCondition("Fair"), ArtifactCondition::Fair);
    QCOMPARE(Artifact::stringToCondition("Fragments"), ArtifactCondition::Fragments);
    QCOMPARE(Artifact::stringToCondition("InvalidCondition"), ArtifactCondition::Good); // Default is Good
}


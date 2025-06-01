//
// Created by matei on 26/05/2025.
//

#include "../../include/domain/Artifact.h"

Artifact::Artifact(const QString& catalogNumber, const QString& name, ArtifactType type,
                   const QString& material, const QString& culture, const QString& period,
                   const QString& site, const QString& coordinates, const QDate& discoveryDate,
                   ArtifactCondition condition)
    : catalogNumber_(catalogNumber), name_(name), type_(type), material_(material),
      culture_(culture), period_(period), site_(site), coordinates_(coordinates),
      discoveryDate_(discoveryDate), condition_(condition) {}

Artifact::Artifact(const QString &catalogNumber,
                   const QString &name,
                   ArtifactType type,
                   const QString &material,
                   const QString &culture,
                   const QString &period,
                   const QString &site,
                   const QString &coordinates,
                   const QDate &discoveryDate,
                   ArtifactCondition condition,
                   const QString &description,
                   double length,
                   double width,
                   double height,
                   double weight,
                   const QString &storageLocation,
                   bool onDisplay)
  : catalogNumber_(catalogNumber),
    name_(name),
    type_(type),
    material_(material),
    culture_(culture),
    period_(period),
    site_(site),
    coordinates_(coordinates),
    discoveryDate_(discoveryDate),
    condition_(condition),
    description_(description),
    length_(length),
    width_(width),
    height_(height),
    weight_(weight),
    storageLocation_(storageLocation),
    onDisplay_(onDisplay)
{}

QString Artifact::typeToString(ArtifactType type) {
    switch (type) {
        case ArtifactType::Pottery: return "Pottery";
        case ArtifactType::Tool: return "Tool";
        case ArtifactType::Weapon: return "Weapon";
        case ArtifactType::Jewelry: return "Jewelry";
        case ArtifactType::Sculpture: return "Sculpture";
        case ArtifactType::Coin: return "Coin";
        case ArtifactType::Tablet: return "Tablet";
        case ArtifactType::Bone: return "Bone";
        case ArtifactType::Textile: return "Textile";
        default: return "Unknown";
    }
}

ArtifactType Artifact::stringToType(const QString& type) {
    if (type == "Pottery") return ArtifactType::Pottery;
    if (type == "Tool") return ArtifactType::Tool;
    if (type == "Weapon") return ArtifactType::Weapon;
    if (type == "Jewelry") return ArtifactType::Jewelry;
    if (type == "Sculpture") return ArtifactType::Sculpture;
    if (type == "Coin") return ArtifactType::Coin;
    if (type == "Tablet") return ArtifactType::Tablet;
    if (type == "Bone") return ArtifactType::Bone;
    if (type == "Textile") return ArtifactType::Textile;
    return ArtifactType::Unknown;
}

QString Artifact::conditionToString(ArtifactCondition condition) {
    switch (condition) {
        case ArtifactCondition::Excellent: return "Excellent";
        case ArtifactCondition::Good: return "Good";
        case ArtifactCondition::Fair: return "Fair";
        case ArtifactCondition::Poor: return "Poor";
        case ArtifactCondition::Fragments: return "Fragments";
        default: return "Unknown";
    }
}

ArtifactCondition Artifact::stringToCondition(const QString& condition) {
    if (condition == "Excellent") return ArtifactCondition::Excellent;
    if (condition == "Good") return ArtifactCondition::Good;
    if (condition == "Fair") return ArtifactCondition::Fair;
    if (condition == "Poor") return ArtifactCondition::Poor;
    if (condition == "Fragments") return ArtifactCondition::Fragments;
    return ArtifactCondition::Good;
}

QString Artifact::toCSV() const {
    return QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17")
        .arg(catalogNumber_)
        .arg(name_)
        .arg(typeToString(type_))
        .arg(material_)
        .arg(culture_)
        .arg(period_)
        .arg(site_)
        .arg(coordinates_)
        .arg(discoveryDate_.toString("yyyy-MM-dd"))
        .arg(conditionToString(condition_))
        .arg(description_)
        .arg(length_)
        .arg(width_)
        .arg(height_)
        .arg(weight_)
        .arg(storageLocation_)
        .arg(onDisplay_ ? "1" : "0");
}

std::unique_ptr<Artifact> Artifact::fromCSV(const QString& csvLine) {
    QStringList fields = csvLine.split(',');
    if (fields.size() < 17) return nullptr; // Not enough fields

    Artifact artifact;
    artifact.catalogNumber_ = fields[0];
    artifact.name_ = fields[1];
    artifact.type_ = stringToType(fields[2]);
    artifact.material_ = fields[3];
    artifact.culture_ = fields[4];
    artifact.period_ = fields[5];
    artifact.site_ = fields[6];
    artifact.coordinates_ = fields[7];
    artifact.discoveryDate_ = QDate::fromString(fields[8], "yyyy-MM-dd");
    artifact.condition_ = stringToCondition(fields[9]);
    artifact.description_ = fields[10];
    artifact.length_ = fields[11].toDouble();
    artifact.width_ = fields[12].toDouble();
    artifact.height_ = fields[13].toDouble();
    artifact.weight_ = fields[14].toDouble();
    artifact.storageLocation_ = fields[15];
    artifact.onDisplay_ = (fields[16] == "1");

    return std::make_unique<Artifact>(artifact);
}

QJsonObject Artifact::toJson() const {
    QJsonObject json;
    json["catalogNumber"] = catalogNumber_;
    json["name"] = name_;
    json["type"] = typeToString(type_);
    json["material"] = material_;
    json["culture"] = culture_;
    json["period"] = period_;
    json["site"] = site_;
    json["coordinates"] = coordinates_;
    json["discoveryDate"] = discoveryDate_.toString("yyyy-MM-dd");
    json["condition"] = conditionToString(condition_);
    json["description"] = description_;
    json["length"] = length_;
    json["width"] = width_;
    json["height"] = height_;
    json["weight"] = weight_;
    json["storageLocation"] = storageLocation_;
    json["onDisplay"] = onDisplay_;

    return json;
}

std::unique_ptr<Artifact> Artifact::fromJson(const QJsonObject &json) {
    auto artifact = std::make_unique<Artifact>();
    artifact->setCatalogNumber(json["catalogNumber"].toString());
    artifact->setName(json["name"].toString());
    artifact->setType(stringToType(json["type"].toString()));
    artifact->setMaterial(json["material"].toString());
    artifact->setCulture(json["culture"].toString());
    artifact->setPeriod(json["period"].toString());
    artifact->setSite(json["site"].toString());
    artifact->setCoordinates(json["coordinates"].toString());
    artifact->setDiscoveryDate(QDate::fromString(json["discoveryDate"].toString(), "yyyy-MM-dd"));
    artifact->setCondition(stringToCondition(json["condition"].toString()));
    artifact->setDescription(json["description"].toString());
    artifact->setLength(json["length"].toDouble());
    artifact->setWidth(json["width"].toDouble());
    artifact->setHeight(json["height"].toDouble());
    artifact->setWeight(json["weight"].toDouble());
    artifact->setStorageLocation(json["storageLocation"].toString());
    artifact->setOnDisplay(json["onDisplay"].toBool());
    return artifact;
}

bool Artifact::operator==(const Artifact& other) const {
    return catalogNumber_ == other.catalogNumber_;
}


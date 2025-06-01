//
// Created by matei on 26/05/2025.
//

#ifndef ARTIFACT_H
#define ARTIFACT_H
#include <QString>
#include <QDate>
#include <QJsonObject>
enum class ArtifactType {
    Pottery,
    Tool,
    Weapon,
    Jewelry,
    Sculpture,
    Coin,
    Tablet,
    Bone,
    Textile,
    Unknown
};

enum class ArtifactCondition {
    Excellent,
    Good,
    Fair,
    Poor,
    Fragments
};

class Artifact {
public:
    Artifact() = default;
    Artifact(const QString& catalogNumber, const QString& name, ArtifactType type,
             const QString& material, const QString& culture, const QString& period,
             const QString& site, const QString& coordinates, const QDate& discoveryDate,
             ArtifactCondition condition = ArtifactCondition::Good);
    Artifact(const QString &catalogNumber,
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
            bool onDisplay);

    const QString& getCatalogNumber() const { return catalogNumber_; }
    const QString& getName() const { return name_; }
    ArtifactType getType() const { return type_; }
    const QString& getMaterial() const { return material_; }
    const QString& getCulture() const { return culture_; }
    const QString& getPeriod() const { return period_; }
    const QString& getSite() const { return site_; }
    const QString& getCoordinates() const { return coordinates_; }
    const QDate& getDiscoveryDate() const { return discoveryDate_; }
    ArtifactCondition getCondition() const { return condition_; }
    const QString& getDescription() const { return description_; }
    double getLength() const { return length_; }
    double getWidth() const { return width_; }
    double getHeight() const { return height_; }
    double getWeight() const { return weight_; }
    const QString& getStorageLocation() const { return storageLocation_; }
    bool isOnDisplay() const { return onDisplay_; }

    // Setters
    void setCatalogNumber(const QString& catalogNumber) { catalogNumber_ = catalogNumber; }
    void setName(const QString& name) { name_ = name; }
    void setType(ArtifactType type) { type_ = type; }
    void setMaterial(const QString& material) { material_ = material; }
    void setCulture(const QString& culture) { culture_ = culture; }
    void setPeriod(const QString& period) { period_ = period; }
    void setSite(const QString& site) { site_ = site; }
    void setCoordinates(const QString& coordinates) { coordinates_ = coordinates; }
    void setDiscoveryDate(const QDate& date) { discoveryDate_ = date; }
    void setCondition(ArtifactCondition condition) { condition_ = condition; }
    void setDescription(const QString& description) { description_ = description; }
    void setLength(double length) { length_ = length; }
    void setWidth(double width) { width_ = width; }
    void setHeight(double height) { height_ = height; }
    void setWeight(double weight) { weight_ = weight; }
    void setStorageLocation(const QString& location) { storageLocation_ = location; }
    void setOnDisplay(bool onDisplay) { onDisplay_ = onDisplay; }

    // Utility methods
    static QString typeToString(ArtifactType type);
    static ArtifactType stringToType(const QString& typeStr);
    static QString conditionToString(ArtifactCondition condition);
    static ArtifactCondition stringToCondition(const QString& conditionStr);

    // Serialization
    QString toCSV() const;
    static std::unique_ptr<Artifact> fromCSV(const QString& csvLine);

    QJsonObject toJson() const;
    static std::unique_ptr<Artifact> fromJson(const QJsonObject& json);

    // Comparison
    bool operator==(const Artifact& other) const;
private:
    QString catalogNumber_;
    QString name_;
    ArtifactType type_ = ArtifactType::Unknown;
    QString material_;
    QString culture_;
    QString period_;
    QString site_;
    QString coordinates_;
    QDate discoveryDate_;
    ArtifactCondition condition_ = ArtifactCondition::Good;
    QString description_;
    double length_ = 0.0;  // in cm
    double width_ = 0.0;   // in cm
    double height_ = 0.0;  // in cm
    double weight_ = 0.0;  // in grams
    QString storageLocation_;
    bool onDisplay_ = false;

};

#endif //ARTIFACT_H

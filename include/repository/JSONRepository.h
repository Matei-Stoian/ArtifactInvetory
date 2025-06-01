//
// Created by matei on 31/05/2025.
//

#ifndef JSONREPOSITORY_H
#define JSONREPOSITORY_H
#include "BaseRepository.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "BaseRepository.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

class JSONRepository : public BaseRepository {
public:
    JSONRepository(const QString& filename);
    ~JSONRepository() override = default;

    bool save(const std::vector<std::unique_ptr<Artifact>>& artifacts) override;
    std::vector<std::unique_ptr<Artifact>> load() override;
    bool addArtifact(std::unique_ptr<Artifact> artifact) override;
    bool removeArtifact(const QString& catalogNumber) override;
    bool updateArtifact(const Artifact& artifact) override;
    std::unique_ptr<Artifact> getByCatalogNumber(const QString& catalogNumber) override;
    std::vector<std::unique_ptr<Artifact>> getAll() override;

private:
    QString filename_;
    void ensureFileExists() const;
};



#endif //JSONREPOSITORY_H

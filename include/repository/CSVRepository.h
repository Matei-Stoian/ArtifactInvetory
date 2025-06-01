//
// Created by matei on 26/05/2025.
//

#ifndef CSVREPOSITORY_H
#define CSVREPOSITORY_H
#include "BaseRepository.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <algorithm>

class CSVRepository : public BaseRepository {
public:
    CSVRepository(const QString& filename);
    ~CSVRepository() override = default;

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



#endif //CSVREPOSITORY_H

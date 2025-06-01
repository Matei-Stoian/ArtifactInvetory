//
// Created by matei on 26/05/2025.
//

#ifndef BASEREPOSITORY_H
#define BASEREPOSITORY_H

#include <vector>

#include "../domain/Artifact.h"

class BaseRepository {
public:
    virtual ~BaseRepository() = default;
    virtual bool save(const std::vector<std::unique_ptr<Artifact>>& artifacts) = 0;
    virtual std::vector<std::unique_ptr<Artifact>> load() = 0;
    virtual bool addArtifact(const std::unique_ptr<Artifact> artifact) = 0;
    virtual bool removeArtifact(const QString& calatogNumber) = 0;
    virtual bool updateArtifact(const Artifact& artifact) = 0;
    virtual std::unique_ptr<Artifact> getByCatalogNumber(const QString& catalogNumber) = 0;
    virtual std::vector<std::unique_ptr<Artifact>> getAll() = 0;
protected:
    QString filename_;
};



#endif //BASEREPOSITORY_H

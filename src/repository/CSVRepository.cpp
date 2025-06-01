//
// Created by matei on 26/05/2025.
//

#include "../../include/repository/CSVRepository.h"


CSVRepository::CSVRepository(const QString& filename)
    : filename_(filename) {
    ensureFileExists();
}

void CSVRepository::ensureFileExists() const {
    QDir dir = QFileInfo(filename_).absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    QFile file(filename_);
    if (!file.exists()) {
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);
        out << "CatalogNumber,Name,Type,Material,Culture,Period,Site,DiscoveryDate,Condition,StorageLocation,OnDisplay\n";
        file.close();
    }
}

bool CSVRepository::save(const std::vector<std::unique_ptr<Artifact>>& artifacts) {
    QFile file(filename_);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream out(&file);
    for (const auto& artifact : artifacts) {
        out << artifact->toCSV() << "\n";
    }
    file.close();
    return true;
}

std::vector<std::unique_ptr<Artifact>> CSVRepository::load() {
    QFile file(filename_);
    std::vector<std::unique_ptr<Artifact>> artifacts;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return artifacts;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        auto artifact = Artifact::fromCSV(line);
        if (artifact) {
            artifacts.push_back(std::move(artifact));
        }
    }
    file.close();
    return artifacts;
}

bool CSVRepository::addArtifact(std::unique_ptr<Artifact> artifact) {
    auto artifacts = load();
    artifacts.push_back(std::move(artifact));  // <-- always std::move
    return save(artifacts);
}

bool CSVRepository::removeArtifact(const QString& catalogNumber) {
    auto artifacts = load();
    const auto it = std::remove_if(artifacts.begin(), artifacts.end(),
        [&catalogNumber](const std::unique_ptr<Artifact>& artifact) {
            return artifact->getCatalogNumber() == catalogNumber;
        });

    if (it != artifacts.end()) {
        artifacts.erase(it, artifacts.end());
        return save(artifacts);
    }
    return false;
}

bool CSVRepository::updateArtifact(const Artifact& updatedArtifact) {
    auto artifacts = load();
    for (auto& artifact : artifacts) {
        if (artifact->getCatalogNumber() == updatedArtifact.getCatalogNumber()) {
            *artifact = updatedArtifact;
            return save(artifacts);
        }
    }
    return false;
}

std::unique_ptr<Artifact> CSVRepository::getByCatalogNumber(const QString& catalogNumber) {
    auto artifacts = load();
    for (const auto& artifact : artifacts) {
        if (artifact->getCatalogNumber() == catalogNumber) {
            return std::make_unique<Artifact>(*artifact);
        }
    }
    return nullptr;
}

std::vector<std::unique_ptr<Artifact>> CSVRepository::getAll() {
    return load();
}
//
// Created by matei on 31/05/2025.
//

#include "../../include/repository/JSONRepository.h"

JSONRepository::JSONRepository(const QString& filename) {
    filename_ = filename;
    ensureFileExists();
}

void JSONRepository::ensureFileExists() const {
    if (!QFile::exists(filename_)) {
        QFile file(filename_);
        file.open(QIODevice::WriteOnly);
        file.write("[]");
        file.close();
    }
}
bool JSONRepository::save(const std::vector<std::unique_ptr<Artifact>>& artifacts) {
    QJsonArray jsonArray;
    for (const auto& artifact : artifacts) {
        jsonArray.append(artifact->toJson());
    }

    QJsonDocument doc(jsonArray);
    QFile file(filename_);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.write(doc.toJson());
    return true;
}

std::vector<std::unique_ptr<Artifact>> JSONRepository::load() {
    std::vector<std::unique_ptr<Artifact>> artifacts;
    QFile file(filename_);

    if (!file.open(QIODevice::ReadOnly)) {
        return artifacts;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray jsonArray = doc.array();

    for (const auto& value : jsonArray) {
        auto artifact = Artifact::fromJson(value.toObject());
        if (artifact) {
            artifacts.push_back(std::move(artifact));
        }
    }

    return artifacts;
}

bool JSONRepository::addArtifact(std::unique_ptr<Artifact> artifact) {
    auto artifacts = load();
    artifacts.push_back(std::move(artifact));
    return save(artifacts);
}

bool JSONRepository::removeArtifact(const QString& catalogNumber) {
    auto artifacts = load();
    auto it = std::remove_if(artifacts.begin(), artifacts.end(),
        [&catalogNumber](const std::unique_ptr<Artifact>& artifact) {
            return artifact->getCatalogNumber() == catalogNumber;
        });

    if (it != artifacts.end()) {
        artifacts.erase(it, artifacts.end());
        return save(artifacts);
    }
    return false;
}

bool JSONRepository::updateArtifact(const Artifact& updatedArtifact) {
    auto artifacts = load();
    for (auto& artifact : artifacts) {
        if (artifact->getCatalogNumber() == updatedArtifact.getCatalogNumber()) {
            *artifact = updatedArtifact;
            return save(artifacts);
        }
    }
    return false;
}

std::unique_ptr<Artifact> JSONRepository::getByCatalogNumber(const QString& catalogNumber) {
    auto artifacts = load();
    for (const auto& artifact : artifacts) {
        if (artifact->getCatalogNumber() == catalogNumber) {
            return std::make_unique<Artifact>(*artifact);
        }
    }
    return nullptr;
}

std::vector<std::unique_ptr<Artifact>> JSONRepository::getAll() {
    return load();
}
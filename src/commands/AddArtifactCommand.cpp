//
// Created by matei on 31/05/2025.
//

#include "../../include/commands/AddArtifactCommand.h"


AddArtifactCommand::AddArtifactCommand(BaseRepository *repository, const std::unique_ptr<Artifact> &artifact)
    : repository_(repository), artifact_(std::make_unique<Artifact>(*artifact)), wasExectuted_(false) {}


void AddArtifactCommand::execute() {
    if (repository_ && artifact_) {
         repository_->addArtifact(std::move(artifact_));
        wasExectuted_ = true;
    }
}

void AddArtifactCommand::undo() {
    if (repository_ && artifact_) {
        repository_->removeArtifact(artifact_->getCatalogNumber());
        wasExectuted_ = false;
    }
}



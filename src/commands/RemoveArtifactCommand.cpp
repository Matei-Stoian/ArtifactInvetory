//
// Created by matei on 31/05/2025.
//

#include "../../include/commands/RemoveArtifactCommand.h"
RemoveArtifactCommand::RemoveArtifactCommand(BaseRepository* repository, const QString& catalogNumber)
    : repository_(repository), catalogNumber_(catalogNumber), wasExecuted_(false) {}

void RemoveArtifactCommand::execute() {
    if (repository_) {
        removedArtifact_ = repository_->getByCatalogNumber(catalogNumber_);
        if (removedArtifact_) {
            repository_->removeArtifact(catalogNumber_);
            wasExecuted_ = true;
        }
    }
}

void RemoveArtifactCommand::undo() {
    if (wasExecuted_ && repository_ && removedArtifact_) {
        repository_->addArtifact(std::make_unique<Artifact>(*removedArtifact_));
        wasExecuted_ = false;
    }
}
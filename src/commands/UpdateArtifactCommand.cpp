//
// Created by matei on 31/05/2025.
//

#include "../../include/commands/UpdateArtifactCommand.h"

UpdateArtifactCommand::UpdateArtifactCommand(BaseRepository* repository, std::unique_ptr<Artifact> newArtifact)
    : repository_(repository), newArtifact_(std::move(newArtifact)), wasExecuted_(false) {}

void UpdateArtifactCommand::execute() {
    if (repository_ && newArtifact_) {
        oldArtifact_ = repository_->getByCatalogNumber(newArtifact_->getCatalogNumber());
        if (oldArtifact_) {
            repository_->updateArtifact(*newArtifact_);
            wasExecuted_ = true;
        }
    }
}

void UpdateArtifactCommand::undo() {
    if (wasExecuted_ && repository_ && oldArtifact_) {
        repository_->updateArtifact(*oldArtifact_);
        wasExecuted_ = false;
    }
}
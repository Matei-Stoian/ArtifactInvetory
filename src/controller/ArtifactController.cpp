//
// Created by matei on 31/05/2025.
//

#include "../../include/controller/ArtifactController.h"

#include "../../include/commands/AddArtifactCommand.h"
#include "../../include/commands/RemoveArtifactCommand.h"
#include "../../include/commands/UpdateArtifactCommand.h"

ArtifactController::ArtifactController(std::unique_ptr<BaseRepository> repository)
    : repository_(std::move(repository)) {}

bool ArtifactController::addArtifact(std::unique_ptr<Artifact> artifact) {
    auto command = std::make_unique<AddArtifactCommand>(repository_.get(), std::move(artifact));
    executeCommand(std::move(command));
    return true;
}

bool ArtifactController::removeArtifact(const QString& catalogNumber) {
    auto command = std::make_unique<RemoveArtifactCommand>(repository_.get(), catalogNumber);
    executeCommand(std::move(command));
    return true;
}

bool ArtifactController::updateArtifact(std::unique_ptr<Artifact> artifact) {
    auto command = std::make_unique<UpdateArtifactCommand>(repository_.get(), std::move(artifact));
    executeCommand(std::move(command));
    return true;
}

std::unique_ptr<Artifact> ArtifactController::findArtifact(const QString& catalogNumber) {
    return repository_->getByCatalogNumber(catalogNumber);
}

std::vector<std::unique_ptr<Artifact>> ArtifactController::getAllArtifacts() {
    return repository_->getAll();
}

std::vector<std::unique_ptr<Artifact>> ArtifactController::filterArtifacts(const ArtifactFilter& filter) {
    auto allArtifacts = repository_->getAll();
    std::vector<std::unique_ptr<Artifact>> filteredArtifacts;

    for (auto& artifact : allArtifacts) {
        if (filter.matches(*artifact)) {
            filteredArtifacts.push_back(std::move(artifact));
        }
    }

    return filteredArtifacts;
}

void ArtifactController::undo() {
    if (!undoStack_.empty()) {
        auto command = std::move(undoStack_.top());
        undoStack_.pop();
        command->undo();
        redoStack_.push(std::move(command));
    }
}

void ArtifactController::redo() {
    if (!redoStack_.empty()) {
        auto command = std::move(redoStack_.top());
        redoStack_.pop();
        command->execute();
        undoStack_.push(std::move(command));
    }
}

bool ArtifactController::canUndo() const {
    return !undoStack_.empty();
}

bool ArtifactController::canRedo() const {
    return !redoStack_.empty();
}

void ArtifactController::switchRepository(std::unique_ptr<BaseRepository> newRepository) {
    repository_ = std::move(newRepository);
    // Clear undo/redo stacks when switching repositories
    while (!undoStack_.empty()) undoStack_.pop();
    while (!redoStack_.empty()) redoStack_.pop();
}

void ArtifactController::executeCommand(std::unique_ptr<Command> command) {
    command->execute();
    undoStack_.push(std::move(command));
    clearRedoStack();
}

void ArtifactController::clearRedoStack() {
    while (!redoStack_.empty()) {
        redoStack_.pop();
    }
}


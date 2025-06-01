//
// Created by matei on 31/05/2025.
//

#ifndef ARTIFACTCONTROLLER_H
#define ARTIFACTCONTROLLER_H



#include "../domain//Artifact.h"
#include "../repository/BaseRepository.h"
#include "../commands/Command.h"
#include "../filters/ArtifactFilter.h"
#include <memory>
#include <vector>
#include <stack>

class ArtifactController {
public:
    explicit ArtifactController(std::unique_ptr<BaseRepository> repository);
    ~ArtifactController() = default;

    // CRUD operations
    bool addArtifact(std::unique_ptr<Artifact> artifact);
    bool removeArtifact(const QString& catalogNumber);
    bool updateArtifact(std::unique_ptr<Artifact> artifact);
    std::unique_ptr<Artifact> findArtifact(const QString& catalogNumber);
    std::vector<std::unique_ptr<Artifact>> getAllArtifacts();

    // Filtering
    std::vector<std::unique_ptr<Artifact>> filterArtifacts(const ArtifactFilter& filter);

    // Undo/Redo
    void undo();
    void redo();
    bool canUndo() const;
    bool canRedo() const;

    // Repository management
    void switchRepository(std::unique_ptr<BaseRepository> newRepository);

private:
    std::unique_ptr<BaseRepository> repository_;
    std::stack<std::unique_ptr<Command>> undoStack_;
    std::stack<std::unique_ptr<Command>> redoStack_;

    void executeCommand(std::unique_ptr<Command> command);
    void clearRedoStack();
};



#endif //ARTIFACTCONTROLLER_H

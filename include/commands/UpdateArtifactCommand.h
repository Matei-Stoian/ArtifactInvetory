//
// Created by matei on 31/05/2025.
//

#ifndef UPDATEARTIFACTCOMMAND_H
#define UPDATEARTIFACTCOMMAND_H
#include "../domain//Artifact.h"
#include "../repository/BaseRepository.h"
#include "Command.h"
class UpdateArtifactCommand : public Command {
public:
    UpdateArtifactCommand(BaseRepository* repository, std::unique_ptr<Artifact> newArtifact);

    void execute() override;
    void undo() override;

private:
    BaseRepository* repository_;
    std::unique_ptr<Artifact> newArtifact_;
    std::unique_ptr<Artifact> oldArtifact_;
    bool wasExecuted_;
};


#endif //UPDATEARTIFACTCOMMAND_H

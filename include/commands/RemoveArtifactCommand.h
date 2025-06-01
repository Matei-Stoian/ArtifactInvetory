//
// Created by matei on 31/05/2025.
//

#ifndef REMOVEARTIFACTCOMMAND_H
#define REMOVEARTIFACTCOMMAND_H
#include "Command.h"
#include "../domain/Artifact.h"
#include "../repository/BaseRepository.h"

class RemoveArtifactCommand : public Command {
public:
    RemoveArtifactCommand(BaseRepository* repository, const QString& catalogNumber);

    void execute() override;
    void undo() override;

private:
    BaseRepository* repository_;
    QString catalogNumber_;
    std::unique_ptr<Artifact> removedArtifact_;
    bool wasExecuted_;
};


#endif //REMOVEARTIFACTCOMMAND_H

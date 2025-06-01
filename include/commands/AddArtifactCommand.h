//
// Created by matei on 31/05/2025.
//

#ifndef ADDARTIFACTCOMMAND_H
#define ADDARTIFACTCOMMAND_H
#include "Command.h"
#include "../../include/repository/BaseRepository.h"

class AddArtifactCommand:public Command {
public:
     AddArtifactCommand(BaseRepository *repository, const std::unique_ptr<Artifact> &artifact);

     void execute() override;
     void undo() override;
private:
     BaseRepository* repository_;
     std::unique_ptr<Artifact> artifact_;
     bool wasExectuted_;
};



#endif //ADDARTIFACTCOMMAND_H

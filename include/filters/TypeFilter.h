//
// Created by matei on 31/05/2025.
//

#ifndef TYPEFILTER_H
#define TYPEFILTER_H


#include "ArtifactFilter.h"

class TypeFilter : public ArtifactFilter {
public:
    explicit TypeFilter(ArtifactType type);

    bool matches(const Artifact& artifact) const override;
    std::unique_ptr<ArtifactFilter> clone() const override;

private:
    ArtifactType type_;
};



#endif //TYPEFILTER_H

//
// Created by matei on 31/05/2025.
//

#ifndef ARTIFACTFILTER_H
#define ARTIFACTFILTER_H
#include <memory>
#include "../domain/Artifact.h"

class ArtifactFilter {
public:
    virtual ~ArtifactFilter() = default;
    virtual bool matches(const Artifact& artifact) const = 0;
    virtual std::unique_ptr<ArtifactFilter> clone() const = 0;
};



#endif //ARTIFACTFILTER_H

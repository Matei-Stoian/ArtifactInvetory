//
// Created by matei on 31/05/2025.
//

#ifndef ANDFILTER_H
#define ANDFILTER_H



#include "ArtifactFilter.h"
#include <vector>

class AndFilter : public ArtifactFilter {
public:
    AndFilter(std::unique_ptr<ArtifactFilter> left, std::unique_ptr<ArtifactFilter> right);

    bool matches(const Artifact& artifact) const override;
    std::unique_ptr<ArtifactFilter> clone() const override;

private:
    std::unique_ptr<ArtifactFilter> left_;
    std::unique_ptr<ArtifactFilter> right_;
};



#endif //ANDFILTER_H

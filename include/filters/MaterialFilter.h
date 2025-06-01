//
// Created by matei on 31/05/2025.
//

#ifndef MATERIALFILTER_H
#define MATERIALFILTER_H


#include "ArtifactFilter.h"

class MaterialFilter : public ArtifactFilter {
public:
    explicit MaterialFilter(const QString& material);

    bool matches(const Artifact& artifact) const override;
    std::unique_ptr<ArtifactFilter> clone() const override;

private:
    QString material_;
};




#endif //MATERIALFILTER_H

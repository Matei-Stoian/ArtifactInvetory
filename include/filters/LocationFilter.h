//
// Created by matei on 31/05/2025.
//

#ifndef LOCATIONFILTER_H
#define LOCATIONFILTER_H

#include "ArtifactFilter.h"

class LocationFilter : public ArtifactFilter {
public:
    explicit LocationFilter(const QString& site);

    bool matches(const Artifact& artifact) const override;
    std::unique_ptr<ArtifactFilter> clone() const override;

private:
    QString site_;
};



#endif //LOCATIONFILTER_H

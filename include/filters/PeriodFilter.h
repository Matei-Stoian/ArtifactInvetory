//
// Created by matei on 31/05/2025.
//

#ifndef PERIODFILTER_H
#define PERIODFILTER_H


#include "ArtifactFilter.h"

class PeriodFilter : public ArtifactFilter {
public:
    explicit PeriodFilter(const QString& period);

    bool matches(const Artifact& artifact) const override;
    std::unique_ptr<ArtifactFilter> clone() const override;

private:
    QString period_;
};


#endif //PERIODFILTER_H

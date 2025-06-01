//
// Created by matei on 31/05/2025.
//

#include "../../include/filters/PeriodFilter.h"

PeriodFilter::PeriodFilter(const QString& period) : period_(period) {}

bool PeriodFilter::matches(const Artifact& artifact) const {
    return artifact.getPeriod().contains(period_, Qt::CaseInsensitive);
}

std::unique_ptr<ArtifactFilter> PeriodFilter::clone() const {
    return std::make_unique<PeriodFilter>(period_);
}
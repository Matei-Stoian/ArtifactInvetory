//
// Created by matei on 31/05/2025.
//

#include "../../include/filters/LocationFilter.h"

LocationFilter::LocationFilter(const QString& site) : site_(site) {}

bool LocationFilter::matches(const Artifact& artifact) const {
    return artifact.getSite().contains(site_, Qt::CaseInsensitive);
}

std::unique_ptr<ArtifactFilter> LocationFilter::clone() const {
    return std::make_unique<LocationFilter>(site_);
}
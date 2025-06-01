//
// Created by matei on 01/06/2025.
//

#include "../../include/filters/NameFilter.h"

NameFilter::NameFilter(const QString& name)
    : name_(name) {}

bool NameFilter::matches(const Artifact& artifact) const {
    return artifact.getName().contains(name_, Qt::CaseInsensitive);
}

std::unique_ptr<ArtifactFilter> NameFilter::clone() const {
    return std::make_unique<NameFilter>(name_);
}
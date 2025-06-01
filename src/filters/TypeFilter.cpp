//
// Created by matei on 31/05/2025.
//

#include "../../include/filters/TypeFilter.h"


TypeFilter::TypeFilter(ArtifactType type) : type_(type) {}

bool TypeFilter::matches(const Artifact& artifact) const {
    return artifact.getType() == type_;
}

std::unique_ptr<ArtifactFilter> TypeFilter::clone() const {
    return std::make_unique<TypeFilter>(type_);
}
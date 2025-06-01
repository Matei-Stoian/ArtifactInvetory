//
// Created by matei on 31/05/2025.
//

#include "../../include/filters/OrFilter.h"

OrFilter::OrFilter(std::unique_ptr<ArtifactFilter> left, std::unique_ptr<ArtifactFilter> right)
    : left_(std::move(left)), right_(std::move(right)) {}

bool OrFilter::matches(const Artifact& artifact) const {
    return left_->matches(artifact) || right_->matches(artifact);
}

std::unique_ptr<ArtifactFilter> OrFilter::clone() const {
    return std::make_unique<OrFilter>(left_->clone(), right_->clone());
}

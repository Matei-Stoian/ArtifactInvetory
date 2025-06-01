//
// Created by matei on 31/05/2025.
//

#include "../../include/filters/AndFilter.h"

AndFilter::AndFilter(std::unique_ptr<ArtifactFilter> left, std::unique_ptr<ArtifactFilter> right)
    : left_(std::move(left)), right_(std::move(right)) {}

bool AndFilter::matches(const Artifact& artifact) const {
    return left_->matches(artifact) && right_->matches(artifact);
}

std::unique_ptr<ArtifactFilter> AndFilter::clone() const {
    return std::make_unique<AndFilter>(left_->clone(), right_->clone());
}
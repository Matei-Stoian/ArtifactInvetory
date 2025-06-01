//
// Created by matei on 31/05/2025.
//

#include "../../include/filters/MaterialFilter.h"



MaterialFilter::MaterialFilter(const QString& material) : material_(material) {}

bool MaterialFilter::matches(const Artifact& artifact) const {
    return artifact.getMaterial().contains(material_, Qt::CaseInsensitive);
}

std::unique_ptr<ArtifactFilter> MaterialFilter::clone() const {
    return std::make_unique<MaterialFilter>(material_);
}
//
// Created by matei on 01/06/2025.
//

#ifndef NAMEFILTER_H
#define NAMEFILTER_H
#include "ArtifactFilter.h"



class NameFilter : public ArtifactFilter{
public:
    explicit NameFilter(const QString& name);
    bool matches(const Artifact& artifact) const override;
    std::unique_ptr<ArtifactFilter> clone() const override;
private:
    QString name_;
};



#endif //NAMEFILTER_H

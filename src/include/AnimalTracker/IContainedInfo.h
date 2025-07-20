#pragma once

#include "IInfo.h"
#include "QualityFilter.h"

#include <vector>

class IContainingInfo;
class ContainingInfoAccess;

class IContainedInfo : virtual public IInfo {
public:
	virtual ~IContainedInfo() = default;

	virtual void SetContainingItem(IContainingInfo& parent, ContainingInfoAccess const&) = 0;
	virtual void SetQuality(const QualityFilter& quality, std::vector<const IInfo*>& affectedInfos, ContainingInfoAccess const&) = 0;
};


class ContainedInfoAccess {
	friend class IContainedInfo;
private:
	ContainedInfoAccess() = default;
};
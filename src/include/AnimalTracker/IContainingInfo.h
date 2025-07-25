#pragma once

#include "IInfo.h"

#include <memory>
#include <vector>

class IContainedInfo;
class ContainedInfoAccess;

class ContainingInfoAccess {
	friend class IContainingInfo;
private:
	ContainingInfoAccess() = default;
};

class IContainingInfo : virtual public IInfo {
public:
	virtual ~IContainingInfo() = default;

	virtual void AddContainedItem(std::unique_ptr<IContainedInfo> child) = 0;
	virtual void UnsetQuality(std::vector<const IInfo*> & affectedInfos, ContainedInfoAccess const&) = 0;
protected:
	static constexpr const ContainingInfoAccess CHILD_ACCESS{};
};

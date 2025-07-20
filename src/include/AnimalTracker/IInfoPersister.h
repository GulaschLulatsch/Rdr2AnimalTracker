#pragma once

#include "IInfo.h"

#include <vector>

class IInfoPersister {
public:
	virtual ~IInfoPersister() = default;

	virtual void StoreInfos(std::vector<const IInfo*> infos) const = 0;
};
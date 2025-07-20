#pragma

#include "IContainedInfo.h"
#include "IContainingInfo.h"

class ISandwichedInfo : public IContainedInfo, public IContainingInfo {
public:
	virtual ~ISandwichedInfo() = default;
};
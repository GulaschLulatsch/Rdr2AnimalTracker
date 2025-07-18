#pragma once

#include <RDR2ScriptHook/enums.h>

#include <string>


class IInfo {
public:
	virtual ~IInfo() = default;

	virtual const std::string& GetName() const = 0;
	virtual int GetQualityBitmask() const = 0;
	virtual bool IsQualitySet() const = 0;
	virtual bool QualityMatches(ePedQuality quality) const = 0;

	virtual void RotateQuality() = 0;
};
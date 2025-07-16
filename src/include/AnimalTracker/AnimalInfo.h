#pragma once

#include "QualityFilter.h"

#include <RDR2ScriptHook/enums.h>
#include <RDR2ScriptHook/types.h>

#include <string>

class AnimalInfo {
public:
	AnimalInfo(
		Hash hash, 
		std::string const& name, 
		bool isFish,
		QualityFilter filter = QualityFilter{ true, true, true }
	);

	Hash GetHash() const;
	const std::string& GetName() const;
	int GetQualityBitmask() const;

	bool IsFish() const;

	bool QualityMatches(ePedQuality quality) const;
	void RotateQuality();

private:
	const Hash m_hash;
	const std::string m_name;
	const bool m_isFish;
	QualityFilter m_filter;
};
#pragma once

#include <ScriptHookRDR2/enums.h>

class QualityFilter {
public:
	QualityFilter(int filter);
	QualityFilter(const QualityFilter& other);

	bool IsSet() const;
	bool Matches(ePedQuality quality) const;

	int GetBitMask() const;
	
	void Unset();
	void Rotate();

	static constexpr const int POOR		{ 0b0001 };
	static constexpr const int GOOD		{ 0b0010 };
	static constexpr const int PERFECT	{ 0b0100 };
	static constexpr const int NOT_SET	{ 0b1000 };

private:

	int m_filter;
};
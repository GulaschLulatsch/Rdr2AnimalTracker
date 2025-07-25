#include "QualityFilter.h"

#include <ScriptHookRDR2/enums.h>

QualityFilter::QualityFilter(int filter) :
	m_filter{ ((filter & NOT_SET) > 0) ? NOT_SET : (filter & (POOR | GOOD | PERFECT )) }
{}

QualityFilter::QualityFilter(const QualityFilter & other):
	m_filter{ other.m_filter }
{}

bool QualityFilter::IsSet() const
{
	return (m_filter & NOT_SET) == 0;
}

bool QualityFilter::Matches(ePedQuality quality) const
{
	if (!IsSet()) {
		return false;
	}
	if (quality < PQ_MEDIUM) {
		return (m_filter & POOR) > 0;
	}
	if (quality > PQ_MEDIUM) {
		return (m_filter & PERFECT) > 0;
	}
	return (m_filter & GOOD) > 0;
}

int QualityFilter::GetBitMask() const
{
	return m_filter;
}

void QualityFilter::Unset()
{
	m_filter = NOT_SET;
}

void QualityFilter::Rotate(){
	if (m_filter == (POOR | GOOD | PERFECT) || !IsSet()) {
		m_filter = 0;
	}
	else {
		m_filter += 1;
	}
}

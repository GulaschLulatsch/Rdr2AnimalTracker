#include "QualityFilter.h"

#include <RDR2ScriptHook/enums.h>

QualityFilter::QualityFilter(int filter) :
	m_filter{ filter & (POOR | GOOD | PERFECT) }
{}

QualityFilter::QualityFilter(bool poor, bool good, bool perfect) :
	m_filter{ 
		(poor ? POOR : 0) |
		(good ? GOOD : 0) |
		(perfect ? PERFECT : 0)
	}
{}

bool QualityFilter::Matches(ePedQuality quality) const
{
	if (quality < PQ_MEDIUM) {
		return (m_filter & POOR) > 0;
	}
	if (quality > PQ_MEDIUM) {
		return (m_filter & PERFECT) > 0;
	}
	return (m_filter & GOOD) > 0;
}

void QualityFilter::Rotate(){
	if (m_filter == (POOR | GOOD | PERFECT)) {
		m_filter = 0;
	}
	else {
		m_filter += 1;
	}
}

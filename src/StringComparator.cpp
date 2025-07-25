#include "StringComparator.h"

#include <unicode/coll.h>
#include <unicode/locid.h>
#include <unicode/ucol.h>
#include <unicode/unistr.h>
#include <unicode/utypes.h>

#include <spdlog/spdlog.h>

#include <string>

StringComparator::StringComparator(const char* localeName) :
	m_collator{ nullptr }
{
	icu::Locale locale{ icu::Locale::createFromName(localeName) };

	UErrorCode status{ U_ZERO_ERROR };
	m_collator.reset(icu::Collator::createInstance(locale, status));
	if (U_FAILURE(status) || !m_collator) {
		spdlog::error("ICU collator creation faild for locale \"{}\"", localeName);
	}
}

bool StringComparator::operator()(const std::string& a, const std::string& b)
{
	icu::UnicodeString ua{ icu::UnicodeString::fromUTF8(a) };
	icu::UnicodeString ub{ icu::UnicodeString::fromUTF8(b) };

	UErrorCode status{ U_ZERO_ERROR };
	UCollationResult result = m_collator->compare(ua, ub, status);
	if (U_FAILURE(status)) {
		spdlog::error("ICU comparison of UTF-8 unicode strings \"{}\" and \"{}\" failed", a, b);
	}
	return result == UCOL_LESS;
}

#pragma once

#include <memory>
#include <string>

#include <unicode/coll.h>

class StringComparator {
public:

	explicit StringComparator(const char* localeName);

	bool operator()(const std::string& select, const std::string& cancel);
private:

	std::unique_ptr<icu::Collator> m_collator;
};
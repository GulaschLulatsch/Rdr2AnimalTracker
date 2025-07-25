#pragma once

#include <memory>
#include <string>

#include <unicode/coll.h>

class StringComparator {
public:

	explicit StringComparator(const char* localeName);

	bool operator()(const std::string& a, const std::string& b);
private:

	std::unique_ptr<icu::Collator> m_collator;
};
#ifndef SUPERFLAPPY_UNLOCK_MAP_HPP
#define SUPERFLAPPY_UNLOCK_MAP_HPP

#include <vector>
#include <algorithm>
#include <type_traits>

#include <cereal/types/vector.hpp>

template<typename T>
class unlock_map {
	static_assert(std::is_enum_v<T>, "T must be an enum type");
	using U = std::underlying_type_t<T>;

	std::vector<U> m_map; // pairs of [start, end)

public:
	unlock_map() = default;
	unlock_map(std::vector<U> vec) : m_map(std::move(vec)) {};
	~unlock_map() = default;

	void push(const T& item) {
		U entry = static_cast<U>(item);
		std::size_t i = 0;

		for (; i < m_map.size(); ++i) {
			if (m_map[i] > entry) break;
		}

		if (i % 2 == 1) return; // already in an interval

		if (i == m_map.size()) {
			m_map.push_back(entry);
			m_map.push_back(entry + 1);
			return;
		}

		if (m_map[i] == entry + 1) {
			m_map[i]--;
			return;
		}

		if (i > 0 && m_map[i - 1] == entry) {
			m_map[i - 1]++;
			return;
		}

		m_map.insert(m_map.begin() + i, {entry, entry + 1});
	}

	bool check(const T& item) const {
		U entry = static_cast<U>(item);
		for (std::size_t i = 0; i < m_map.size(); ++i) {
			if (m_map[i] > entry) {
				return (i % 2) == 1;
			}
		}
		return false;
	}

	template <class Archive>
	void serialize(Archive& ar) {
		ar(m_map);
	}
};

#endif // SUPERFLAPPY_UNLOCK_MAP_HPP

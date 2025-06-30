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
public:
	unlock_map() = default;
	unlock_map(std::vector<U> vec) : _data(std::move(vec)) {};
	~unlock_map() = default;


	std::vector<U> _data; // pairs of [start, end)

	void push(const T& item) {
		U entry = static_cast<U>(item);
		std::size_t i = 0;

		for (; i < _data.size(); ++i) {
			if (_data[i] > entry) break;
		}

		if (i % 2 == 1) return; // already in an interval

		if (i == _data.size()) {
			_data.push_back(entry);
			_data.push_back(entry + 1);
			return;
		}

		if (_data[i] == entry + 1) {
			_data[i]--;
			return;
		}

		if (i > 0 && _data[i - 1] == entry) {
			_data[i - 1]++;
			return;
		}

		_data.insert(_data.begin() + i, {entry, entry + 1});
	}

	bool check(const T& item) const {
		U entry = static_cast<U>(item);
		for (std::size_t i = 0; i < _data.size(); ++i) {
			if (_data[i] > entry) {
				return (i % 2) == 1;
			}
		}
		return false;
	}

    // (De)Serialization
	template <class Archive>
	void serialize(Archive& ar) {
		ar(_data);
	}
};

#endif // SUPERFLAPPY_UNLOCK_MAP_HPP

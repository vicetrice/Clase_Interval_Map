#include <map>
#include <iostream>
#include <limits>
#include <stdexcept>

template <typename K, typename V>
class interval_map
{
	V m_valBegin;
	std::map<K, V> m_map;
	K fina;

public:
	// constructor associates whole range of K with val, be careful ,if you dont want to lose the values of val just type val in the constructor
	// However that will make the assignation more expensive(in efficiency terms).
	interval_map(V const &val)
		: m_valBegin(std::move(val))
	{
	}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign(K const &keyBegin, K const &keyEnd, V const &val)
	{
		fina = keyEnd;

		if (!(keyBegin < keyEnd))
			return;
		if (!m_map.empty())
		{
			const bool key_exist = m_map.find(keyBegin) != m_map.end();
			const bool is_canonic = m_map.rbegin()->second != val;
			if (!is_canonic && !key_exist)
			{
				return;
			}
			else if (is_canonic && key_exist)
			{
				// The key exists, and the map is canonical, so we simply update the value associated with the key.
				m_map[keyBegin] = val;
			}
			else if (is_canonic)
			{
				// The map is canonical, but the key doesn't exist, so we can insert a new key-value pair into the map.
				m_map.emplace(keyBegin, val);
			}
			else if (key_exist)
			{
				// If the map is not canonical, but the key exists, we first remove it and then insert the new key-value pair.
				m_map.erase(keyBegin);
				m_map.emplace(keyBegin, val);
			}
			m_map.emplace(keyEnd, m_valBegin);

			// Throw an exception if necessary
		}
		else if (val != m_valBegin)
		{
			m_map.emplace(keyBegin, val);
			m_map.emplace(keyEnd, m_valBegin);
		}
	}
	// look-up of the value associated with key
	V const &operator[](K const &key) const
	{
		auto it = m_map.upper_bound(key);
		if (it == m_map.begin())
		{
			return m_valBegin;
		}
		else
		{
			return (--it)->second;
		}
	}

	friend std::ostream &operator<<(std::ostream &out, const interval_map<K, V> &sali)
	{

		for (const auto &entry : sali.m_map)
		{
			out << entry.first << "->" << entry.second << '\n';
		}
		return out;

		/*K fin;
		fin = sali.fina;
		const V prompt = sali.m_valBegin ? sali.m_valBegin : std::numeric_limits<V>::min();
		// this is really inefficient but is the only way to print the number from -infinity to +infinity, the best way would be put all the numbers in an array and put the output in a file. doing it this way for the review process.
		for (K i = 0; i < sali.m_map.begin()->first; ++i)
		{
			out << i << "->" << prompt << '\n';
		}
		for (auto it = sali.m_map.begin(); it != sali.m_map.end(); ++it)
		{
			const K startKey = it->first;
			const K endKey = std::next(it)->first;
			for (K key = startKey; key < endKey; ++key)
			{
				out << key << "->" << it->second << '\n';
			}
		}
		for (K key = sali.m_map.rbegin()->first; key < fin; ++key)
		{
			out << key << "->" << sali.m_map.rbegin()->second << '\n';
		}
		for (K i = fin; i <= 80; ++i)
		{
			out << i << "->" << sali.m_valBegin << '\n';
		}
		return out;*/
	}
};

int main()
{
	interval_map<int, char> prueba{'M'};
	prueba.assign(1, 6, 'B');
	prueba.assign(7, 9, 'K');
	prueba.assign(10, 15, 'A');
	std::cout << prueba;
}

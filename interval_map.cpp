#include <map>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <cassert>
#include <algorithm>

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
		: m_valBegin(val)
	{
	}
	// constructor especifico
	interval_map(const V &valbegin, const std::map<K, V> &other)
		: m_valBegin(valbegin), m_map{other}
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
			bool key_exist = m_map.find(keyBegin) != m_map.end();
			bool is_canonic = (m_map.rbegin()->second != val);
			V record = std::prev(m_map.lower_bound(keyBegin))->second;
			if (keyEnd > std::prev(m_map.lower_bound(keyBegin))->first)
			{
				m_map.erase(m_map.upper_bound(keyBegin), m_map.upper_bound(keyEnd));
			}

			if (!is_canonic && !key_exist)
			{
				if (m_map.empty())
				{
					m_map.emplace(keyBegin, val);
				}
				else
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
				std::cout << m_map.rbegin()->second << "\n";
				std::cout << !(m_map.rbegin()->second != val);
				if (!is_canonic)
				{
					m_map.erase(keyBegin);
				}
			}
			else if (key_exist)
			{
				// If the map is not canonical, but the key exists, we first remove it and then insert the new key-value pair.
				m_map.erase(keyBegin);
				m_map.emplace(keyBegin, val);
				if (!is_canonic)
				{
					m_map.erase(keyBegin);
				}
			}
			if (m_map.rbegin()->second != m_valBegin)
			{

				m_map.emplace(keyEnd, m_valBegin);
			}
			else if (keyEnd < m_map.rbegin()->first)
			{
				m_map.emplace(keyEnd, record);
				if ((m_map.upper_bound(keyEnd)->second == val) || (m_map.lower_bound(keyEnd)->second == val))
				{
					m_map.erase(keyEnd);
				}
			}
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
	bool empty() const
	{
		return m_map.empty();
	}

	bool operator==(const interval_map<K, V> &m) const
	{
		return ((m.m_map == m_map) && (m.m_valBegin == m_valBegin));
	}
};

void testign()
{
	int x = 0;
	do
	{
		x++;
		switch (x)
		{
		case 1:
			std::cout << "Testing "
					  << "EmptyRange" << '\n';
			{
				interval_map<int, char> m('A');
				m.assign(3, 3, 'B');
				m.assign(3, 2, 'B');
				assert(m.empty());
			}
			break;

		case 2:
			std::cout << "Testing "
					  << "TrivialRange" << '\n';
			{
				interval_map<int, char> m('A');
				interval_map<int, char> c('A', {{1, 'B'}, {10, 'A'}});
				m.assign(1, 10, 'B');
				assert(m == c);
			}
			break;

		case 3:
			std::cout << "Testing "
					  << "TrivialTwoRange" << '\n';
			{
				interval_map<int, char> m('A');
				interval_map<int, char> c('A', {{1, 'B'}, {3, 'A'}, {6, 'C'}, {8, 'A'}});
				m.assign(1, 3, 'B');
				m.assign(6, 8, 'C');
				assert(m == c);
				assert(m == c);
			}
			break;

		case 4:
			std::cout << "Testing "
					  << "OverwriteLowest" << '\n';
			{
				interval_map<int, char> m('A');
				m.assign(std::numeric_limits<int>::lowest(), 10000, 'B');
				interval_map<int, char> c('A', {{std::numeric_limits<int>::lowest(), 'B'}, {10000, 'A'}});
				assert(m == c);
			}
			break;

		case 5:
			/*std::cout << "Testing "
					  << "Merge" << '\n';
			{
				interval_map<int, char> m('A');
				interval_map<int, char> c('A', {{std::numeric_limits<int>::lowest(), 'B'}, {20, 'A'}});
				m.assign(std::numeric_limits<int>::lowest(), 10, 'B');
				m.assign(10, 20, 'B');
				assert(m == c);
			}*/
			break;

		case 6:
			std::cout << "Testing "
					  << "FloatKey" << '\n';
			{
				interval_map<float, char> m('A');
				interval_map<float, char> c('A', {{1, 'B'}, {5, 'A'}});
				m.assign(1.f, 5.f, 'B');
				assert(m == c);
			}
			break;

		case 7:
			std::cout << "Testing "
					  << "OverlappingRangeComplete" << '\n';
			{
				interval_map<int, char> m('A');
				interval_map<int, char> c('A', {{1, 'C'}, {6, 'A'}});
				m.assign(3, 5, 'B');
				m.assign(1, 6, 'C');
				assert(m == c);
			}
			break;

		case 8:
			std::cout << "Testing "
					  << "OverlappingRangeInner" << '\n';
			{
				interval_map<int, char> m('A');
				interval_map<int, char> c('A', {{1, 'C'}, {3, 'B'}, {5, 'C'}, {6, 'A'}});
				m.assign(1, 6, 'C');
				m.assign(3, 5, 'B');
				assert(m == c);
			}
			break;

		case 9:
			std::cout << "Testing "
					  << "OverlappingRangeSmallToLarge" << '\n';
			{
				interval_map<int, char> m('A');
				interval_map<int, char> c('A', {{1, 'B'}, {3, 'C'}, {6, 'A'}});
				m.assign(1, 5, 'B');
				m.assign(3, 6, 'C');
				assert(m == c);
			}
			break;

		case 10:
			std::cout << "Testing "
					  << "OverlappingRangeLargeToSmall" << '\n';
			{
				interval_map<int, char> m('A');
				interval_map<int, char> c('A', {{1, 'B'}, {5, 'C'}, {6, 'A'}});
				m.assign(3, 6, 'C');
				m.assign(1, 5, 'B');
				assert(m == c);
			}
			break;

		case 11:
			std::cout << "Testing "
					  << "ExtendingRangeBegin" << '\n';
			{
				interval_map<int, char> m('A');
				interval_map<int, char> c('A', {{1, 'B'}, {5, 'A'}});
				m.assign(3, 5, 'B');
				m.assign(1, 4, 'B');
				assert(m == c);
			}
			break;

		case 12:
			std::cout << "Testing "
					  << "ExtendingRangeEnd" << '\n';
			{
				interval_map<int, char> m('A');
				interval_map<int, char> c('A', {{1, 'B'}, {6, 'A'}});
				m.assign(1, 5, 'B');
				std::cout << m << "-*-\n";
				m.assign(3, 6, 'B');
				std::cout << m << "---\n";
				assert(m == c);
			}
			break;

		case 13:
			std::cout << "Testing "
					  << "ExtendingRangeBothBeginEnd" << '\n';
			{
				interval_map<int, char> m('A');
				interval_map<int, char> c('A', {{1, 'B'}, {5, 'A'}});
				m.assign(2, 3, 'B');
				m.assign(1, 5, 'B');
				assert(m == c);
			}
			break;

		case 14:
			std::cout << "Testing "
					  << "OverwriteEndValueSafety" << '\n';
			{
				interval_map<int, char> m('A');
				interval_map<int, char> c('A', {{2, 'B'}, {4, 'A'}, {5, 'C'}, {8, 'A'}});
				m.assign(2, 5, 'B');
				m.assign(5, 8, 'C');
				m.assign(4, 5, 'A');
				assert(m == c);
			}
			break;

		case 15:
			std::cout << "Testing "
					  << "ReusingExistingRangeBothBeginEnd" << '\n';
			{
				interval_map<int, char> m('A');
				interval_map<int, char> c('A', {{1, 'B'}, {5, 'A'}});
				m.assign(1, 5, 'B');
				m.assign(2, 3, 'B');
				assert(m == c);
			}
			break;

		case 16:
			std::cout << "Testing "
					  << "ReusingEnd" << '\n';
			{
				interval_map<int, char> m('A');
				interval_map<int, char> c('A', {{1, 'B'}, {4, 'A'}});
				m.assign(1, 5, 'B');
				m.assign(4, 6, 'A');
				assert(m == c);
			}
			break;

		case 17:
			std::cout << "Testing "
					  << "RestoringInitial" << '\n';
			{
				interval_map<int, char> m('A');
				m.assign(1, 5, 'B');
				m.assign(1, 5, 'A');
				assert(m.empty());
			}
			break;

		case 18:
			std::cout << "Testing "
					  << "RestoringInitial2" << '\n';
			{
				interval_map<int, char> m('A');
				m.assign(1, 5, 'B');
				m.assign(0, 7, 'A');
				assert(m.empty());
			}
			break;

		case 19:
			std::cout << "Testing "
					  << "VeryComplex" << '\n';
			{
				interval_map<int, char> m('A');
				interval_map<int, char> c('A', {{2, 'C'}, {4, 'A'}});
				m.assign(3, 6, 'B');
				m.assign(2, 5, 'C');
				m.assign(4, 7, 'A');
				assert(m == c);
			}
			break;
		default:
			std::cout << "Test not available" << 'n';
			break;
		}
	} while (x < 19);
}

int main()
{
	testign();
}

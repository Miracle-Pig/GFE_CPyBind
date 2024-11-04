#pragma once

#include <list>
#include <string>
#include <vector>

std::vector<int> deduplication(const std::vector<std::vector<int>>& input, int size);

std::list<std::string> split(const std::string& str, const std::string& delim, bool deduplicate = true);

// Comparator for case-insensitive comparison in STL assos. containers
struct ci_less
{
  // case-independent (ci) compare_less binary function
  struct nocase_compare
  {
    bool operator()(const unsigned char &c1, const unsigned char &c2) const
    {
      return tolower(c1) < tolower(c2);
    }
  };
  bool operator()(const std::string &s1, const std::string &s2) const
  {
    return std::lexicographical_compare(s1.begin(), s1.end(), // source range
                                        s2.begin(), s2.end(), // dest range
                                        nocase_compare());    // comparison
  }
};

template <class T>
std::vector<char> ToByteArray(const std::vector<T>& in)
{
    return std::vector<char>((char*)&in.front(), (char*)&in.front()+in.size()*sizeof(T));
}

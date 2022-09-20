#ifndef _VFRU_GET_LINE_H_
#define _VFRU_GET_LINE_H_

#include <iostream>
#include <string>
#include <vector>
#include <string.h>


namespace VFR {
  inline void GetLine(std::istream& is, std::string& buff) {
    char c;
    const char dcs[] = "\n\r";
    buff = "";
    while ( is.get(c) && ! strchr(dcs, c) )
      buff.push_back(c);
    while ( is.get(c) && strchr(dcs, c) )
      ;
    if ( is.good() ) is.putback(c);
  }

  inline void GetLine(std::istream& is, char* p, const int n) {
    char c;
    const char dcs[] = "\n\r";
    register size_t i = 0;
    if ( ! p || n < 1 ) return;
    memset(p, 0, n);
    while ( i < n-1 && is.get(c) && ! strchr(dcs, c) ) {
      *p++ = c;
      i++;
    }
    while ( is.get(c) && strchr(dcs, c) )
      ;
    if ( is.good() ) is.putback(c);    
  }

  inline std::string
  TrimString(const std::string& str, const std::string& trim) {
    std::string result;
    std::string::size_type left = str.find_first_not_of(trim);
    if ( left != std::string::npos ) {
      std::string::size_type right = str.find_last_not_of(trim);
      result = str.substr(left, right - left + 1);
    }
    return result;
  }
  
  inline std::vector<std::string>
  SplitString(const std::string& str, const std::string& delim) {
    std::vector<std::string> res;
    size_t current = 0, found;
    while ((found = str.find_first_of(delim, current)) != std::string::npos ) {
      res.push_back(std::string(str, current, found - current));
      current = str.find_first_not_of(delim, found);
      if ( current == std::string::npos ) break;
    }
    if ( current != std::string::npos )
      res.push_back(std::string(str, current, std::string::npos));
    return res;
  }
};

#endif // _VFRU_GET_LINE_H_

#ifndef __MIND_ERRORBUF__
#define __MIND_ERRORBUF__

#include "location.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace mind {

class ErrorBuffer {
  public:
    typedef std::pair<Location, std::string> ErrorMsg;

    ErrorBuffer(std::ostream &);
    void flush(void);
    void add(const Location *, const std::string &);
    ~ErrorBuffer();

  private:
    std::vector<ErrorMsg> _buf;
    std::ostream &_os;
};

ErrorBuffer::ErrorBuffer(std::ostream &os) : _os(os) {}

struct errmsg_less {
    bool operator()(const ErrorBuffer::ErrorMsg &e1,
                    const ErrorBuffer::ErrorMsg &e2) {
        if (e1.first.line != e2.first.line)
            return (e1.first.line < e2.first.line);
        else if (e1.first.col != e2.first.col)
            return (e1.first.col < e2.first.col);
        else
            return (e1.second < e2.second);
    }
};

void ErrorBuffer::flush(void) {
    std::sort(_buf.begin(), _buf.end(), errmsg_less());
    for (std::vector<ErrorMsg>::iterator it = _buf.begin(); it != _buf.end();
         ++it)
        _os << it->second << std::endl;
    _os.flush();
    _buf.clear();
}

ErrorBuffer::~ErrorBuffer() { flush(); }

void ErrorBuffer::add(const Location *l, const std::string &s) {
    if (NULL != l)
        _buf.push_back(make_pair(*l, s));
    else
        _buf.push_back(make_pair(Location(-1), s));
}

} // namespace mind

#endif // __MIND_ERRORBUF__

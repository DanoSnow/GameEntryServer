#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
using namespace std;

class NotOpen : public runtime_error{
public:
    NotOpen();
};
class NotAccess : public runtime_error{
public:
    NotAccess();
};
class NotInserted : public runtime_error{
public:
    NotInserted();
};
class NotModified : public runtime_error{
public:
    NotModified();
};

#endif // EXCEPTIONS_H

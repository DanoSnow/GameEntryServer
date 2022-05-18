#include "exceptions.h"

NotOpen::NotOpen() : runtime_error("Couldn't open the data base"){}
NotAccess::NotAccess() : runtime_error("Couldn't not create or access to the table"){}
NotInserted::NotInserted() : runtime_error("Couldn't insert the new player into the table"){}
NotModified::NotModified() : runtime_error("Couldn't modify the score"){}

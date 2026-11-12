#pragma once
#include "trainer/model.hpp"

using namespace std;

void addEvent(EventLog& log,
              const string& severity,
              const string& source,
              const string& message);

void printEventLog(const EventLog& log);

void checkParametersAndLogWarnings(const ParameterList& parameters,
                                   EventLog& log);
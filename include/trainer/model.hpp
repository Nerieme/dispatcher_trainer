#pragma once

#include <string>
#include <vector>
#include <array>

using namespace std;

struct ParameterRecord {
    string name;
    int value;
    int minValue;
    int maxValue;
    string unit;
};

struct EventRecord {
    string severity;
    string source;
    string message;
};

using ParameterList = vector<ParameterRecord>;
using EventLog = vector<EventRecord>;

const array<string, 5>& visualLayerNames();

const array<string, 3>& cameraStatus();

void printParameters(const ParameterList& parameters);

bool setParameter(ParameterList& parameters,
                  const std::string& name,
                  int newValue);

bool isInRange(const ParameterRecord& parameter);

void printVisualLayers(const std::array<std::string, 5>& layers);

void printCameraStatus(const ParameterList& parameters);
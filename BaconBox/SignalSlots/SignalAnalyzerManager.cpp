#include "BaconBox/SignalSlots/SignalAnalyzerManager.h"
#include "BaconBox/Core/Engine.h"

using namespace BaconBox;

BB_SINGLETON_IMPL(SignalAnalyzerManager);

void SignalAnalyzerManager::addSignalAnalyzer(const std::string& analyzerName, SignalAnalyzer* analyzer, bool overwrite) {
	std::pair<std::map<std::string, SignalAnalyzer*>::iterator, bool> insertResult = analyzers.insert(std::pair<std::string, SignalAnalyzer*>(analyzerName, analyzer));

	if(!insertResult.second && overwrite) {
		// We delete the previous signal analyzer.
		delete insertResult.first->second;
		analyzers[analyzerName] = analyzer;
	}
}

void SignalAnalyzerManager::removeSignalAnalyzer(const std::string& analyzerName) {
	std::map<std::string, SignalAnalyzer*>::iterator toRemove = analyzers.find(analyzerName);

	if(toRemove != analyzers.end()) {
		delete toRemove->second;
		analyzers.erase(analyzerName);
	}
}

SignalAnalyzerManager::SignalAnalyzerManager() {
}

SignalAnalyzerManager::~SignalAnalyzerManager() {
	// We make sure to delete all the signal analyzers.
	for(std::map<std::string, SignalAnalyzer*>::iterator i = analyzers.begin();
	    i != analyzers.end(); ++i) {

		if(i->second) {
			delete i->second;
		}
	}
}

/**
 * @file
 * @ingroup Events
 */
#ifndef BB_SIGNAL_ANALYZER_MANAGER_H
#define BB_SIGNAL_ANALYZER_MANAGER_H

#include <map>
#include <string>

#include "BaconBox/SignalSlots/SignalAnalyzer.h"
#include "BaconBox/Core/Singleton.h"

namespace BaconBox {
	/**
	 * Manages signal analyzers.
	 * @ingroup Events
	 */
	class SignalAnalyzerManager : public Singleton {
		BB_SINGLETON_DECL(SignalAnalyzerManager);
	public:
		/**
		 * Adds a signal analyzer to the manager. From the time the analyzer is
		 * passed to the manager, the manager takes care of freeing up the
		 * memory
		 * @param analyzerName Name to give to the analyzer.
		 * @param analyzer Pointer to the analyzer to add to the manager.
		 * @param overwrite Flag checked to know if the manager has to
		 * overwrite the signal analyzer if there is already an analyzer with
		 * the given name.
		 */
		void addSignalAnalyzer(const std::string& analyzerName,
		                       SignalAnalyzer* analyzer,
		                       bool overwrite = false);

		/**
		 * Removes the signal analyzer from the manager.
		 * @param analyzerName Name of the analyzer to delete and remove from
		 * the manager.
		 */
		void removeSignalAnalyzer(const std::string& analyzerName);

		/**
		 * Gets a signal analyzer.
		 * @param analyzerName Name of the analyzer to get.
		 * @return Pointer to the signal analyzer. Can be used to deactivate
		 * its slots temporarily.
		 */
		SignalAnalyzer* getSignalAnalyzer(const std::string& analyzerName);

	private:
		/// Analyzers and their name.
		std::map<std::string, SignalAnalyzer*> analyzers;

		/**
		 * Private default constructor.
		 */
		SignalAnalyzerManager();

		/**
		 * Private destructor.
		 */
		~SignalAnalyzerManager();
	};
}

#endif

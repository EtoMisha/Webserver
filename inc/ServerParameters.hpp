#ifndef SERVERPARAMETERS_HPP
#define SERVERPARAMETERS_HPP

#include <algorithm>
#include <iostream>
#include <vector>
#include <map>

namespace ft {
	struct ValidConfigKeys { //_validConfigParams
		std::vector<std::string>			servParams;
		std::vector<std::string>			locParams;
		std::vector<std::string>			servKeys;
		std::map<std::string, std::string>	errorsMap;
		std::vector<std::string>			errorPage;
		std::vector<std::string>			autoindexPage;

		ValidConfigKeys();
		// ValidConfigKeys(int responder);
		~ValidConfigKeys();
	};
}

#endif
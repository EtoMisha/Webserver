#include "../inc/ServerParameters.hpp"
#include "../inc/Parser.hpp"

ft::ValidConfigKeys::ValidConfigKeys() { 

	std::string validParams[] = { "location", "methods", "root", "redirection", "error_page", "bin_path_py", "bin_path_sh", "path_cgi", "index", "upload_path", "autoindex" };

	for (size_t i = 0; i < 11; i++) {
		locParams.push_back(validParams[i]);
	}

	std::string validServParams[] = { "listen", "server_name", "autoindex", "root", "index", "methods", "client_max_body_size", "error_page", "upload_path" };

	for (size_t i = 0; i < 9; i++) {
		servParams.push_back(validServParams[i]);
	}

	std::string allParams[] = { "listen", "server", "server_name", "location", "autoindex", "root", "index", "methods", "client_max_body_size", "error_page", "redirection", "path_cgi", "bin_path_py", "bin_path_sh", "upload_path", ";", "{", "}", "" };

	for (size_t i = 0; i < 19; i++) {
		servKeys.push_back(allParams[i]);
	}
}

// ft::ValidConfigKeys::ValidConfigKeys(int) {
// std::cout << "construktor \n";
// 	std::string errorPageStrings[] = {
// 		"<html>",
// 		"<head>",
// 		"<title>KEY</title>",
// 		"</head>",
// 		"<body>",
// 		"<center>",
// 		"<br></br>",
// 		"<h1 style=\"font-size:50px;\" >Error KEY</h1>",
// 		"<h2 style=\"font-size:30px;\" >VALUE</h2>",
// 		"</center>",
// 		"</body>",
// 		"</html>" };

// 	for (size_t i = 0; i < 12; i++) {
// 		errorPage.push_back(errorPageStrings[i]);
// 	}

//     errorsMap["400"] = "BadRequest";
//     errorsMap["403"] = "Forbidden";
//     errorsMap["404"] = "NotFound";
//     errorsMap["405"] = "MethodNotAllowed";
//     errorsMap["406"] = "NotAcceptable";
//     errorsMap["413"] = "ContentTooLarge";
//     errorsMap["415"] = "UnsupportedMediaType";
//     errorsMap["500"] = "InternalServerError";
//     errorsMap["505"] = "BadRequest";
//     errorsMap["409"] = "Conflipwct";

// }

ft::ValidConfigKeys::~ValidConfigKeys() {}
#pragma once

#include <vector>
#include <iostream>
#include <string>

class Location
{
	private:
		enum AutoindexParseState
		{
			AUTO_NOT_SET,
			AUTO_OK,
			AUTO_MISSING_VALUE,
			AUTO_INVALID_VALUE
		};

		std::vector<std::string> _allowedMethods;
		std::string _path;
		std::string _root;
		std::string _index;
		std::string _return;
		std::string _uploadPath;
		std::string _cgiExtensions;
		std::string _cgiPath;
		size_t _clientMaxBodySize;
		bool _autoindex;
		AutoindexParseState _autoindexState;

	public:
		Location();
		~Location();

		// Setters
		void setAllowedMethods(const std::vector<std::string>& allowedMethods);
		void addAllowedMethod(const std::string& method);
		void setPath(const std::string& path);
		void setRoot(const std::string& root);
		void setIndex(const std::string& index);
		void setReturn(const std::string& returnValue);
		void setUploadPath(const std::string& uploadPath);
		void setCgiExtensions(const std::string& cgiExtensions);
		void setCgiPath(const std::string& cgiPath);
		void setClientMaxBodySize(size_t size);
		void setAutoindex(bool autoindex);
		void setAutoindexMissingValue();
		void setAutoindexInvalidValue();

		// Getters
		const std::vector<std::string>& getAllowedMethods() const;
		const std::string& getPath() const;
		const std::string& getRoot() const;
		const std::string& getIndex() const;
		const std::string& getReturn() const;
		const std::string& getUploadPath() const;
		const std::string& getCgiExtensions() const;
		const std::string& getCgiPath() const;
		size_t getClientMaxBodySize() const;
		bool getAutoindex() const;

		// Convenience overload for legacy call sites.
		bool isDataValid() const;
		bool isDataValid(std::vector<std::string>& errs) const;
	
};
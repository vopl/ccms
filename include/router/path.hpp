#ifndef _ccms_path_hpp_
#define _ccms_path_hpp_

namespace ccms
{
	class Path
		: public std::deque<std::string>
	{
	private:
	public:
		Path(std::deque<std::string>::const_iterator begin, std::deque<std::string>::const_iterator end);
		Path();
		Path(const Path &from);
		Path(const char *from);
		Path(const std::string &from);

		~Path();

		Path &operator=(const Path &from);
		Path &operator=(const char *from);
		Path &operator=(const std::string &from);

		Path &operator/=(const Path &from);
		Path &operator/=(const char *from);
		Path &operator/=(const std::string &from);

		Path operator/(const Path &from)const;
		Path operator/(const char *from)const;
		Path operator/(const std::string &from)const;

		Path beforeBack() const;
		Path afterFront() const;

		std::string string() const;
		operator std::string()const;

		void normalize();

		bool eqSmaller(const Path &with) const;

		static bool isValidPathPart(const std::string &part);
	};

	typedef std::vector<Path> TVPaths;
	typedef std::set<Path> TSPaths;
}

#endif

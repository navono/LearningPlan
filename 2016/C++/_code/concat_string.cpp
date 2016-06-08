inline void build_string(std::ostream& o) {}

template<typename First, typename... Rest>
inline void build_string(std::ostream& o, const First& value, const Rest&... rest)
{
	o << value;
	build_string(o, rest...);
}

template<typename.. T>
std::string concat_string(const T&... value)
{
	std::ostringstream o;
	build_string(o, value...);
	
	return o.str();
}

// usage
std::string date_string = concat_string(year, '-', month, '-', day);
throw error(concat_string("Unable to open", path, ": ", errno));
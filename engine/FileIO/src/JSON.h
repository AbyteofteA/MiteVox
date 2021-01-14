
#ifndef JSON_H
#define JSON_H

#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

namespace fileio
{
	enum struct JSON_type { null = 0, number, string, boolean, array, object };
	
#define JSON_TAB_STR			"    "

#define JSON_REGEX_SPACE		"[\\s]*"
#define JSON_REGEX_NULL			"(null)"
#define JSON_REGEX_NUMBER		"(-?[\\d]+.?[\\d]*([eE]-?[\\d]+.?[\\d]*)?)"
#define JSON_REGEX_STRING		"\"([^\"]*)\""
#define JSON_REGEX_BOOLEAN		"(true|false)"
#define JSON_REGEX_ARRAY_START	"(\\[)"
#define JSON_REGEX_OBJECT_START	"(\\{)"

#define JSON_REGEX_FIELDNAME		\
	JSON_REGEX_STRING				\
	JSON_REGEX_SPACE				\
	":"

#define JSON_REGEX_FIELD_NULL		\
	JSON_REGEX_FIELDNAME			\
	JSON_REGEX_SPACE				\
	JSON_REGEX_NULL

#define JSON_REGEX_FIELD_NUMBER		\
	JSON_REGEX_FIELDNAME			\
	JSON_REGEX_SPACE				\
	JSON_REGEX_NUMBER

#define JSON_REGEX_FIELD_STRING		\
	JSON_REGEX_FIELDNAME			\
	JSON_REGEX_SPACE				\
	JSON_REGEX_STRING

#define JSON_REGEX_FIELD_BOOLEAN	\
	JSON_REGEX_FIELDNAME			\
	JSON_REGEX_SPACE				\
	JSON_REGEX_BOOLEAN

#define JSON_REGEX_FIELD_ARRAY		\
	JSON_REGEX_FIELDNAME			\
	JSON_REGEX_SPACE				\
	JSON_REGEX_ARRAY_START			\
	JSON_REGEX_SPACE

#define JSON_REGEX_FIELD_OBJECT		\
	JSON_REGEX_FIELDNAME			\
	JSON_REGEX_SPACE				\
	JSON_REGEX_OBJECT_START			\
	JSON_REGEX_SPACE

	// regex that matches any JSON key-value pair.
	const std::string JSON_regex_pair =
		JSON_REGEX_FIELD_NULL
		"|"
		JSON_REGEX_FIELD_NUMBER
		"|"
		JSON_REGEX_FIELD_STRING
		"|"
		JSON_REGEX_FIELD_BOOLEAN
		"|"
		JSON_REGEX_FIELD_ARRAY
		"|"
		JSON_REGEX_FIELD_OBJECT;

	// regex that matches any JSON value.
	const std::string JSON_regex_value =
		JSON_REGEX_NULL
		"|"
		JSON_REGEX_NUMBER
		"|"
		JSON_REGEX_STRING
		"|"
		JSON_REGEX_BOOLEAN
		"|"
		JSON_REGEX_ARRAY_START
		"|"
		JSON_REGEX_OBJECT_START;
	

	class JSON
	{
		std::string name = "";
		bool is_nameless = true;

		JSON_type type = JSON_type::object;

		double number = 0.0;
		std::string string = "";
		bool boolean = false;

		std::vector<JSON*> fields;

	public:

		JSON()
		{
			type = JSON_type::object;
			is_nameless = true;
		}
		JSON(std::string str)
		{
			parse_str(str);
		}
		~JSON()
		{
			fields.clear();
		}

		bool is_null()
		{
			if (type == JSON_type::null)
			{
				return true;
			}
			return false;
		}
		bool is_number()
		{
			if (type == JSON_type::number)
			{
				return true;
			}
			return false;
		}
		bool is_string()
		{
			if (type == JSON_type::string)
			{
				return true;
			}
			return false;
		}
		bool is_boolean()
		{
			if (type == JSON_type::boolean)
			{
				return true;
			}
			return false;
		}
		bool is_array()
		{
			if (type == JSON_type::array)
			{
				return true;
			}
			return false;
		}
		bool is_object()
		{
			if (type == JSON_type::object)
			{
				return true;
			}
			return false;
		}

		// field GET methods //
		
		JSON* get_field(std::string field_name)
		{
			for (auto field : fields)
			{
				if (field->name == field_name)
				{
					return field;
				}
			}
			return nullptr;
		}
		double get_number(std::string field_name)
		{
			JSON* field = get_field(field_name);
			if (field)
			{
				if (field->is_number())
				{
					return field->number;
				}
			}
			return -0.0;
		}
		std::string get_string(std::string field_name)
		{
			JSON* field = get_field(field_name);
			if (field)
			{
				if (field->is_string())
				{
					return field->string;
				}
			}
			return "";
		}
		bool get_boolean(std::string field_name)
		{
			JSON* field = get_field(field_name);
			if (field)
			{
				if (field->is_boolean())
				{
					return field->boolean;
				}
			}
			return false;
		}
		JSON* get_array(std::string field_name)
		{
			JSON* field = get_field(field_name);
			if (field)
			{
				if (field->is_array())
				{
					return field;
				}
			}
			return nullptr;
		}
		JSON* get_object(std::string field_name)
		{
			JSON* field = get_field(field_name);
			if (field)
			{
				if (field->is_object())
				{
					return field;
				}
			}
			return nullptr;
		}

		// field SET methods //

		JSON* set_field(std::string field_name, JSON_type type = JSON_type::null)
		{
			JSON* field = get_field(field_name);

			if (field == nullptr)
			{
				field = new JSON();
				fields.push_back(field);
				field->name = field_name;
			}
			else
			{
				field->number = 0.0;
				field->string = "";
				field->boolean = false;
				field->fields.clear();
			}
			field->type = type;
			field->is_nameless = false;

			return field;
		}
		JSON* set_field(std::string field_name, double value)
		{
			JSON* field = set_field(field_name, JSON_type::number);
			field->number = value;
			return field;
		}
		JSON* set_field(std::string field_name, std::string value)
		{
			JSON* field = set_field(field_name, JSON_type::string);
			field->string = value;
			return field;
		}
		JSON* set_field(std::string field_name, bool value)
		{
			JSON* field = set_field(field_name, JSON_type::boolean);
			field->boolean = value;
			return field;
		}

		// array methods //

		size_t add_array_item(std::string field_name, JSON_type type = JSON_type::null)
		{
			JSON* the_array = get_field(field_name);

			if (the_array)
			{
				JSON* new_item = new JSON();
				new_item->type = type;
				new_item->is_nameless = true;
				size_t index = the_array->fields.size();
				the_array->fields.push_back(new_item);
				return index;
			}
			return -1;
		}
		size_t add_array_item(JSON* the_array, JSON_type type = JSON_type::null)
		{
			if (the_array)
			{
				JSON* new_item = new JSON();
				new_item->type = type;
				new_item->is_nameless = true;
				size_t index = the_array->fields.size();
				the_array->fields.push_back(new_item);
				return index;
			}
			return -1;
		}

		std::string to_str(unsigned int level = 0)
		{
			std::string quote = "\"";
			std::string tabs = "";
			for (unsigned int i = 0; i < level; i++)
			{
				tabs += JSON_TAB_STR;
			}
			std::string name_str = tabs + quote + name + quote + ": ";
			std::string result;

			size_t amount_of_fields = fields.size();

			switch (type)
			{
			case JSON_type::number:
				if (is_nameless)
				{
					result = tabs + std::to_string(number);
				}
				else
				{
					result = name_str + std::to_string(number);
				}
				
				break;

			case JSON_type::string:
				if (is_nameless)
				{
					result = tabs + quote + string + quote;;
				}
				else
				{
					result = name_str + quote + string + quote;
				}
				break;

			case JSON_type::boolean:
				if (is_nameless)
				{
					result = tabs;
				}
				else
				{
					result = name_str;
				}
				if (boolean == true)
				{
					result += "true";
				}
				else
				{
					result += "false";
				}
				break;

			case JSON_type::array:
				if (is_nameless)
				{
					result = tabs + "[\n";
				}
				else
				{
					result = name_str + "\n" + tabs + "[\n";
				}

				for (size_t i = 0; i < amount_of_fields; i++)
				{
					result += fields[i]->to_str(level + 1);
					if (i != (amount_of_fields - 1))
					{
						result += ",\n";
					}
				}
				result += "\n" + tabs + "]";
				break;

			case JSON_type::object:
				if ((level == 0) || is_nameless)
				{
					result = tabs + "{\n";
				}
				else
				{
					result = name_str + "\n" + tabs + "{\n";
				}

				amount_of_fields = fields.size();
				for (size_t i = 0; i < amount_of_fields; i++)
				{
					result += fields[i]->to_str(level + 1);
					if (i != (amount_of_fields - 1))
					{
						result += ",\n";
					}
				}
				result += "\n" + tabs + "}";
				break;

			default:
				if (is_nameless)
				{
					result = tabs + "null";
				}
				else
				{
					result = name_str + "null";
				}
				break;
			}

			return result;
		}
		std::string parse_str(std::string str, JSON* obj = nullptr, unsigned int level = 0)
		{
			if (level == 0)
			{
				std::smatch match;
				std::regex_search(str, match, std::regex("[\\s]*\\{[\\s]*"));
				str = match.suffix().str();
				return parse_str(str, this, level + 1);
			}

			int null_regex_pos = 0;
			int number_regex_pos = 0;
			int string_regex_pos = 0;
			int boolean_regex_pos = 0;
			int array_regex_pos = 0;
			int object_regex_pos = 0;

			std::smatch match;
			std::regex regexp_field = std::regex(JSON_regex_pair);
			std::regex regexp_nameless = std::regex(JSON_regex_value);
			std::regex regexp_space = std::regex(JSON_REGEX_SPACE);
			std::regex regexp_end_of_object_or_array = std::regex("[\\s]*\\]|[\\s]*\\}");

			if (try_exit_object(str, regexp_space, match))
			{
				return str;
			}
			if (obj->type == JSON_type::array)
			{
				std::regex_search(str, match, regexp_nameless);
				null_regex_pos = 1;
				number_regex_pos = 2;
				string_regex_pos = 4;
				boolean_regex_pos = 5;
				array_regex_pos = 6;
				object_regex_pos = 7;
			}
			else
			{
				std::regex_search(str, match, regexp_field);
				null_regex_pos = 1;
				number_regex_pos = 3;
				string_regex_pos = 6;
				boolean_regex_pos = 8;
				array_regex_pos = 10;
				object_regex_pos = 12;
			}

			while (!match.empty())
			{
				// null mached.
				if (match[null_regex_pos].length() != 0)
				{
					JSON* new_item = obj->set_field(
						match[null_regex_pos].str(), JSON_type::null);
					if (obj->type == JSON_type::array)
					{
						new_item->is_nameless = true;
						new_item->name.clear();
					}
					str = match.suffix().str();
				}
				// number mached.
				else if (match[number_regex_pos].length() != 0)
				{
					char* end_ptr;
					if (obj->type == JSON_type::array)
					{
						double num = std::strtod(match[number_regex_pos].str().c_str(), &end_ptr);
						JSON* new_item = obj->set_field("", num);
						new_item->is_nameless = true;
						new_item->name.clear();
					}
					else
					{
						double num = std::strtod(match[number_regex_pos + 1].str().c_str(), &end_ptr);
						JSON* new_item = obj->set_field(
							match[number_regex_pos].str(), num);
					}
					str = match.suffix().str();
				}
				// string mached.
				else if (match[string_regex_pos].length() != 0)
				{
					if (obj->type == JSON_type::array)
					{
						JSON* new_item = obj->set_field("", match[string_regex_pos].str());
						new_item->is_nameless = true;
						new_item->name.clear();
					}
					else
					{
						JSON* new_item = obj->set_field(
							match[string_regex_pos].str(), match[string_regex_pos + 1].str());
					}
					str = match.suffix().str();
				}
				// boolean mached.
				else if (match[boolean_regex_pos].length() != 0)
				{
					JSON* new_item = nullptr;
					if (obj->type == JSON_type::array)
					{
						if (match[boolean_regex_pos].str() == "true")
						{
							new_item = obj->set_field("", true);
						}
						else
						{
							new_item = obj->set_field("", false);
						}
						new_item->is_nameless = true;
						new_item->name.clear();
					}
					else
					{
						if (match[boolean_regex_pos + 1].str() == "true")
						{
							new_item = obj->set_field(match[boolean_regex_pos].str(), true);
						}
						else
						{
							new_item = obj->set_field(match[boolean_regex_pos].str(), false);
						}
					}
					str = match.suffix().str();
				}
				// array mached.
				else if (match[array_regex_pos].length() != 0)
				{
					JSON* new_arr = obj->set_field(
						match[array_regex_pos].str(), JSON_type::array);
					if (obj->type == JSON_type::array)
					{
						new_arr->is_nameless = true;
						new_arr->name.clear();
					}
					str = parse_str(match.suffix().str(), new_arr, level + 1);
				}
				// object mached.
				else if (match[object_regex_pos].length() != 0)
				{
					JSON* new_obj = obj->set_field(
						match[object_regex_pos].str(), JSON_type::object);
					if (obj->type == JSON_type::array)
					{
						new_obj->is_nameless = true;
						new_obj->name.clear();
					}
					str = parse_str(match.suffix().str(), new_obj, level + 1);
				}

				if (try_exit_object(str, regexp_space, match))
				{
					return str;
				}
				if (obj->type == JSON_type::array)
				{
					std::regex_search(str, match, regexp_nameless);
				}
				else
				{
					std::regex_search(str, match, regexp_field);
				}
			}

			return "ok";
		}
		int to_file(std::string filename);
		int parse_file(std::string filename)
		{
			std::string json_data;

			std::fstream file;
			file.open(filename, std::ios::in);
			if (file.is_open())
			{
				std::string tmp_str;
				while (std::getline(file, tmp_str))
				{
					json_data += tmp_str + "\n";
				}
				file.close();
			}
			else
			{
				return -1;
			}

			parse_str(json_data);
			return 1;
		}
		void wipe();

	private:

		bool try_exit_object(std::string& str, std::regex& regexp_space, std::smatch& match)
		{
			std::regex_search(str, match, regexp_space);
			str = match.suffix().str();

			if ((str[0] == ']') || (str[0] == '}'))
			{
				str.erase(str.begin());
				return true;
			}
			return false;
		}
	};
}

#endif
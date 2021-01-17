
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
	enum struct JSONtype { NIL = 0, NUMBER, STRING, BOOLEAN, ARRAY, OBJECT };
	
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
	const std::string JSON_REGEX_PAIR =
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
	const std::string JSON_REGEX_VALUE =
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
		bool isNameless = true;

		JSONtype type = JSONtype::OBJECT;

		double number = 0.0;
		std::string string = "";
		bool boolean = false;

		std::vector<JSON*> fields;

	public:

		JSON()
		{
			type = JSONtype::OBJECT;
			isNameless = true;
		}
		JSON(std::string str)
		{
			parseStr(str);
		}
		~JSON()
		{
			fields.clear();
		}

		bool isNull()
		{
			if (type == JSONtype::NIL)
			{
				return true;
			}
			return false;
		}
		bool isNumber()
		{
			if (type == JSONtype::NUMBER)
			{
				return true;
			}
			return false;
		}
		bool isString()
		{
			if (type == JSONtype::STRING)
			{
				return true;
			}
			return false;
		}
		bool isBoolean()
		{
			if (type == JSONtype::BOOLEAN)
			{
				return true;
			}
			return false;
		}
		bool isArray()
		{
			if (type == JSONtype::ARRAY)
			{
				return true;
			}
			return false;
		}
		bool isObject()
		{
			if (type == JSONtype::OBJECT)
			{
				return true;
			}
			return false;
		}

		// field GET methods //
		
		JSON* getField(std::string fieldName)
		{
			for (auto field : fields)
			{
				if (field->name == fieldName)
				{
					return field;
				}
			}
			return nullptr;
		}
		double getNumber(std::string fieldName)
		{
			JSON* field = getField(fieldName);
			if (field)
			{
				if (field->isNumber())
				{
					return field->number;
				}
			}
			return -0.0;
		}
		std::string getString(std::string fieldName)
		{
			JSON* field = getField(fieldName);
			if (field)
			{
				if (field->isString())
				{
					return field->string;
				}
			}
			return "";
		}
		bool getBoolean(std::string fieldName)
		{
			JSON* field = getField(fieldName);
			if (field)
			{
				if (field->isBoolean())
				{
					return field->boolean;
				}
			}
			return false;
		}
		JSON* getArray(std::string fieldName)
		{
			JSON* field = getField(fieldName);
			if (field)
			{
				if (field->isArray())
				{
					return field;
				}
			}
			return nullptr;
		}
		JSON* getObject(std::string fieldName)
		{
			JSON* field = getField(fieldName);
			if (field)
			{
				if (field->isObject())
				{
					return field;
				}
			}
			return nullptr;
		}

		// field SET methods //

		JSON* setField(std::string fieldName, JSONtype type = JSONtype::NIL)
		{
			JSON* field = getField(fieldName);

			if (field == nullptr)
			{
				field = new JSON();
				fields.push_back(field);
				field->name = fieldName;
			}
			else
			{
				field->number = 0.0;
				field->string = "";
				field->boolean = false;
				field->fields.clear();
			}
			field->type = type;
			field->isNameless = false;

			return field;
		}
		JSON* setField(std::string fieldName, double value)
		{
			JSON* field = setField(fieldName, JSONtype::NUMBER);
			field->number = value;
			return field;
		}
		JSON* setField(std::string fieldName, std::string value)
		{
			JSON* field = setField(fieldName, JSONtype::STRING);
			field->string = value;
			return field;
		}
		JSON* setField(std::string fieldName, bool value)
		{
			JSON* field = setField(fieldName, JSONtype::BOOLEAN);
			field->boolean = value;
			return field;
		}

		// array methods //

		size_t addArrayItem(std::string fieldName, JSONtype type = JSONtype::NIL)
		{
			JSON* theArray = getField(fieldName);

			if (theArray)
			{
				JSON* newItem = new JSON();
				newItem->type = type;
				newItem->isNameless = true;
				size_t index = theArray->fields.size();
				theArray->fields.push_back(newItem);
				return index;
			}
			return -1;
		}
		size_t addArrayItem(JSON* theArray, JSONtype type = JSONtype::NIL)
		{
			if (theArray)
			{
				JSON* newItem = new JSON();
				newItem->type = type;
				newItem->isNameless = true;
				size_t index = theArray->fields.size();
				theArray->fields.push_back(newItem);
				return index;
			}
			return -1;
		}

		std::string toStr(unsigned int level = 0)
		{
			std::string quote = "\"";
			std::string tabs = "";
			for (unsigned int i = 0; i < level; i++)
			{
				tabs += JSON_TAB_STR;
			}
			std::string nameStr = tabs + quote + name + quote + ": ";
			std::string result;

			size_t amountOfFields = fields.size();

			switch (type)
			{
			case JSONtype::NUMBER:
				if (isNameless)
				{
					result = tabs + std::to_string(number);
				}
				else
				{
					result = nameStr + std::to_string(number);
				}
				
				break;

			case JSONtype::STRING:
				if (isNameless)
				{
					result = tabs + quote + string + quote;;
				}
				else
				{
					result = nameStr + quote + string + quote;
				}
				break;

			case JSONtype::BOOLEAN:
				if (isNameless)
				{
					result = tabs;
				}
				else
				{
					result = nameStr;
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

			case JSONtype::ARRAY:
				if (isNameless)
				{
					result = tabs + "[\n";
				}
				else
				{
					result = nameStr + "\n" + tabs + "[\n";
				}

				for (size_t i = 0; i < amountOfFields; i++)
				{
					result += fields[i]->toStr(level + 1);
					if (i != (amountOfFields - 1))
					{
						result += ",\n";
					}
				}
				result += "\n" + tabs + "]";
				break;

			case JSONtype::OBJECT:
				if ((level == 0) || isNameless)
				{
					result = tabs + "{\n";
				}
				else
				{
					result = nameStr + "\n" + tabs + "{\n";
				}

				amountOfFields = fields.size();
				for (size_t i = 0; i < amountOfFields; i++)
				{
					result += fields[i]->toStr(level + 1);
					if (i != (amountOfFields - 1))
					{
						result += ",\n";
					}
				}
				result += "\n" + tabs + "}";
				break;

			default:
				if (isNameless)
				{
					result = tabs + "null";
				}
				else
				{
					result = nameStr + "null";
				}
				break;
			}

			return result;
		}
		std::string parseStr(std::string str, JSON* obj = nullptr, unsigned int level = 0)
		{
			if (level == 0)
			{
				std::smatch match;
				std::regex_search(str, match, std::regex("[\\s]*\\{[\\s]*"));
				str = match.suffix().str();
				return parseStr(str, this, level + 1);
			}

			int null_regex_pos = 0;
			int number_regex_pos = 0;
			int string_regex_pos = 0;
			int boolean_regex_pos = 0;
			int array_regex_pos = 0;
			int object_regex_pos = 0;

			std::smatch match;
			std::regex regexp_field = std::regex(JSON_REGEX_PAIR);
			std::regex regexp_nameless = std::regex(JSON_REGEX_VALUE);
			std::regex regexp_space = std::regex(JSON_REGEX_SPACE);
			std::regex regexp_end_of_object_or_array = std::regex("[\\s]*\\]|[\\s]*\\}");

			if (tryExitObject(str, regexp_space, match))
			{
				return str;
			}
			if (obj->type == JSONtype::ARRAY)
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
					JSON* new_item = obj->setField(
						match[null_regex_pos].str(), JSONtype::NIL);
					if (obj->type == JSONtype::ARRAY)
					{
						new_item->isNameless = true;
						new_item->name.clear();
					}
					str = match.suffix().str();
				}
				// number mached.
				else if (match[number_regex_pos].length() != 0)
				{
					char* end_ptr;
					if (obj->type == JSONtype::ARRAY)
					{
						double num = std::strtod(match[number_regex_pos].str().c_str(), &end_ptr);
						JSON* new_item = obj->setField("", num);
						new_item->isNameless = true;
						new_item->name.clear();
					}
					else
					{
						double num = std::strtod(match[number_regex_pos + 1].str().c_str(), &end_ptr);
						JSON* new_item = obj->setField(
							match[number_regex_pos].str(), num);
					}
					str = match.suffix().str();
				}
				// string mached.
				else if (match[string_regex_pos].length() != 0)
				{
					if (obj->type == JSONtype::ARRAY)
					{
						JSON* new_item = obj->setField("", match[string_regex_pos].str());
						new_item->isNameless = true;
						new_item->name.clear();
					}
					else
					{
						JSON* new_item = obj->setField(
							match[string_regex_pos].str(), match[string_regex_pos + 1].str());
					}
					str = match.suffix().str();
				}
				// boolean mached.
				else if (match[boolean_regex_pos].length() != 0)
				{
					JSON* new_item = nullptr;
					if (obj->type == JSONtype::ARRAY)
					{
						if (match[boolean_regex_pos].str() == "true")
						{
							new_item = obj->setField("", true);
						}
						else
						{
							new_item = obj->setField("", false);
						}
						new_item->isNameless = true;
						new_item->name.clear();
					}
					else
					{
						if (match[boolean_regex_pos + 1].str() == "true")
						{
							new_item = obj->setField(match[boolean_regex_pos].str(), true);
						}
						else
						{
							new_item = obj->setField(match[boolean_regex_pos].str(), false);
						}
					}
					str = match.suffix().str();
				}
				// array mached.
				else if (match[array_regex_pos].length() != 0)
				{
					JSON* new_arr = obj->setField(
						match[array_regex_pos].str(), JSONtype::ARRAY);
					if (obj->type == JSONtype::ARRAY)
					{
						new_arr->isNameless = true;
						new_arr->name.clear();
					}
					str = parseStr(match.suffix().str(), new_arr, level + 1);
				}
				// object mached.
				else if (match[object_regex_pos].length() != 0)
				{
					JSON* new_obj = obj->setField(
						match[object_regex_pos].str(), JSONtype::OBJECT);
					if (obj->type == JSONtype::ARRAY)
					{
						new_obj->isNameless = true;
						new_obj->name.clear();
					}
					str = parseStr(match.suffix().str(), new_obj, level + 1);
				}

				if (tryExitObject(str, regexp_space, match))
				{
					return str;
				}
				if (obj->type == JSONtype::ARRAY)
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
		int toFile(std::string filename);
		int parseFile(std::string filename)
		{
			std::string jsonData;

			std::fstream file;
			file.open(filename, std::ios::in);
			if (file.is_open())
			{
				std::string tmp_str;
				while (std::getline(file, tmp_str))
				{
					jsonData += tmp_str + "\n";
				}
				file.close();
			}
			else
			{
				return -1;
			}

			parseStr(jsonData);
			return 1;
		}
		void wipe();

	private:

		bool tryExitObject(std::string& str, std::regex& regexpSpace, std::smatch& match)
		{
			std::regex_search(str, match, regexpSpace);
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
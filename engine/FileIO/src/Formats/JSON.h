
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

	// Regex that matches any JSON key-value pair.
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

	// Regex that matches any JSON value.
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
	public:

		inline JSON();
		inline JSON(std::string str);
		inline ~JSON();

		inline bool isNameless();
		inline bool isNull();
		inline bool isNumber();
		inline bool isString();
		inline bool isBoolean();
		inline bool isArray();
		inline bool isObject();

		inline void clear();
		inline void setType(JSONtype newType);
		inline std::string getName();
		inline void setName(std::string);
		inline void setNameless(bool nameless = true);

		inline void set(double value);
		inline void set(std::string value);
		inline void set(bool value);

		inline double getNumber();
		inline std::string getString();
		inline bool getBoolean();

		// Field methods //

		inline double getFieldNumber(std::string fieldName);
		inline std::string getFieldString(std::string fieldName);
		inline bool getFieldBoolean(std::string fieldName);
		inline JSON* getFieldArray(std::string fieldName);
		inline JSON* getFieldObject(std::string fieldName);

		inline JSON* setFieldType(std::string fieldName, JSONtype type);
		inline JSON* setField(std::string fieldName, double value);
		inline JSON* setField(std::string fieldName, std::string value);
		inline JSON* setField(std::string fieldName, bool value);
		inline JSON* setField(JSON* field);

		// Array methods //

		inline size_t getArraySize();
		inline size_t addArrayItem(JSONtype type = JSONtype::NIL);
		inline size_t addArrayItem(double value);
		inline size_t addArrayItem(std::string value);
		inline size_t addArrayItem(bool value);
		inline size_t addArrayItem(JSON* newItem);

		inline JSON* getArrayItem(size_t itemIndex);
		inline double getArrayItemNumber(size_t itemIndex);
		inline std::string getArrayItemString(size_t itemIndex);
		inline bool getArrayItemBool(size_t itemIndex);

		inline std::string toStr(unsigned int level = 0);
		inline std::string parseStr(std::string str, JSON* obj = nullptr, unsigned int level = 0);
		inline int toFile(std::string filename);
		inline int parseFile(std::string filename);

	private:

		JSONtype _type = JSONtype::OBJECT;
		bool _isNameless;
		std::string _name;

		union
		{
			bool boolean;
			double number;
		} _booleanOrNumber;

		std::string _string;

		std::vector<JSON*> _fields;

		inline JSON* getField(std::string fieldName);
		inline bool tryExitObject(std::string& str, std::regex& regexpSpace, std::smatch& match);
	};


	// IMPLEMENTATION BELOW //


	inline JSON::JSON()
	{
		_type = JSONtype::OBJECT;
		_isNameless = true;
		_name = "";
		
		_booleanOrNumber.number = 0.0;
		_string = "";
	}

	inline JSON::JSON(std::string str)
	{
		parseStr(str);
	}

	inline JSON::~JSON()
	{
		clear();
	}

	inline void JSON::clear()
	{
		for (auto field : _fields)
		{
			delete field;
		}
		_fields.clear();
		_type = JSONtype::NIL;
	}

	inline void JSON::setType(JSONtype newType)
	{
		if (_type != newType)
		{
			clear();
			_type = newType;
		}
	}

	inline std::string JSON::getName()
	{
		return _name;
	}

	inline void JSON::setName(std::string name)
	{
		_name = name;
		_isNameless = false;
	}

	inline void JSON::setNameless(bool isNameless)
	{
		_isNameless = isNameless;
	}

	inline void JSON::set(double value)
	{
		_booleanOrNumber.number = value;
	}

	inline void JSON::set(std::string value)
	{
		_string = value;
	}

	inline void JSON::set(bool value)
	{
		_booleanOrNumber.boolean = value;
	}

	inline double JSON::getNumber()
	{
		return _booleanOrNumber.number;
	}

	inline std::string JSON::getString()
	{
		return _string;
	}

	inline bool JSON::getBoolean()
	{
		return _booleanOrNumber.boolean;
	}

	inline bool JSON::isNameless()
	{
		return _isNameless;
	}

	inline bool JSON::isNull()
	{
		if (_type == JSONtype::NIL)
		{
			return true;
		}
		return false;
	}

	inline bool JSON::isNumber()
	{
		if (_type == JSONtype::NUMBER)
		{
			return true;
		}
		return false;
	}

	inline bool JSON::isString()
	{
		if (_type == JSONtype::STRING)
		{
			return true;
		}
		return false;
	}

	inline bool JSON::isBoolean()
	{
		if (_type == JSONtype::BOOLEAN)
		{
			return true;
		}
		return false;
	}

	inline bool JSON::isArray()
	{
		if (_type == JSONtype::ARRAY)
		{
			return true;
		}
		return false;
	}

	inline bool JSON::isObject()
	{
		if (_type == JSONtype::OBJECT)
		{
			return true;
		}
		return false;
	}

	inline double JSON::getFieldNumber(std::string fieldName)
	{
		JSON* field = getField(fieldName);
		if (field)
		{
			if (field->isNumber())
			{
				return field->_booleanOrNumber.number;
			}
		}
		return -0.0;
	}

	inline std::string JSON::getFieldString(std::string fieldName)
	{
		JSON* field = getField(fieldName);
		if (field)
		{
			if (field->isString())
			{
				return field->_string;
			}
		}
		return "";
	}

	inline bool JSON::getFieldBoolean(std::string fieldName)
	{
		JSON* field = getField(fieldName);
		if (field)
		{
			if (field->isBoolean())
			{
				return field->_booleanOrNumber.boolean;
			}
		}
		return false;
	}

	inline JSON* JSON::getFieldArray(std::string fieldName)
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

	inline JSON* JSON::getFieldObject(std::string fieldName)
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

	inline JSON* JSON::setFieldType(std::string fieldName, JSONtype type)
	{
		JSON* field = getField(fieldName);

		if (field == nullptr)
		{
			field = new JSON();
			_fields.push_back(field);
			field->_name = fieldName;
		}
		field->setType(type);
		field->setNameless(false);

		return field;
	}

	inline JSON* JSON::setField(std::string fieldName, double value)
	{
		JSON* field = setFieldType(fieldName, JSONtype::NUMBER);
		field->_booleanOrNumber.number = value;
		return field;
	}

	inline JSON* JSON::setField(std::string fieldName, std::string value)
	{
		JSON* field = setFieldType(fieldName, JSONtype::STRING);
		field->_string = value;
		return field;
	}

	inline JSON* JSON::setField(std::string fieldName, bool value)
	{
		JSON* field = setFieldType(fieldName, JSONtype::BOOLEAN);
		field->_booleanOrNumber.boolean = value;
		return field;
	}

	inline JSON* JSON::setField(JSON* newField)
	{
		if (newField->isNameless())
		{
			return nullptr;
		}
		std::string fieldName = newField->getName();
		JSON* field = getField(fieldName);

		if (field == nullptr)
		{
			_fields.push_back(newField);
		}
		else
		{
			delete field;
			field = newField;
		}

		return field;
	}

	inline size_t JSON::getArraySize()
	{
		if (_type != JSONtype::ARRAY)
		{
			return 0;
		}
		
		return _fields.size();
	}

	inline size_t JSON::addArrayItem(JSONtype type)
	{
		setType(JSONtype::ARRAY);

		JSON* newItem = new JSON();
		newItem->setType(type);
		newItem->setNameless(true);
		size_t index = _fields.size();
		_fields.push_back(newItem);
		return index;
	}

	inline size_t JSON::addArrayItem(double value)
	{
		size_t itemIndex = addArrayItem(JSONtype::NUMBER);
		JSON* newItem = getArrayItem(itemIndex);
		newItem->set(value);
		return itemIndex;
	}

	inline size_t JSON::addArrayItem(std::string value)
	{
		size_t itemIndex = addArrayItem(JSONtype::STRING);
		JSON* newItem = getArrayItem(itemIndex);
		newItem->set(value);
		return itemIndex;
	}

	inline size_t JSON::addArrayItem(bool value)
	{
		size_t itemIndex = addArrayItem(JSONtype::BOOLEAN);
		JSON* newItem = getArrayItem(itemIndex);
		newItem->set(value);
		return itemIndex;
	}

	inline size_t JSON::addArrayItem(JSON* newItem)
	{
		if (newItem->isNameless() == false)
		{
			newItem->setNameless(true);
		}

		size_t itemIndex = _fields.size();
		_fields.push_back(newItem);

		return itemIndex;
	}

	inline JSON* JSON::getArrayItem(size_t itemIndex)
	{
		if (_type != JSONtype::ARRAY)
		{
			return nullptr;
		}
		if (_fields.size() <= itemIndex)
		{
			return nullptr;
		}

		return _fields[itemIndex];
	}

	inline double JSON::getArrayItemNumber(size_t itemIndex)
	{
		JSON* numberJSON = getArrayItem(itemIndex);
		return numberJSON->getNumber();
	}

	inline std::string JSON::getArrayItemString(size_t itemIndex)
	{
		JSON* stringJSON = getArrayItem(itemIndex);
		return stringJSON->getString();
	}

	inline bool JSON::getArrayItemBool(size_t itemIndex)
	{
		JSON* boolJSON = getArrayItem(itemIndex);
		return boolJSON->getBoolean();
	}

	inline std::string JSON::toStr(unsigned int level)
	{
		std::string quote = "\"";
		std::string tabs = "";
		for (unsigned int i = 0; i < level; i++)
		{
			tabs += JSON_TAB_STR;
		}
		std::string nameStr = tabs + quote + _name + quote + ": ";
		std::string result;

		size_t amountOfFields = _fields.size();

		switch (_type)
		{
		case JSONtype::NUMBER:
			if (_isNameless)
			{
				result = tabs + std::to_string(_booleanOrNumber.number);
			}
			else
			{
				result = nameStr + std::to_string(_booleanOrNumber.number);
			}

			break;

		case JSONtype::STRING:
			if (_isNameless)
			{
				result = tabs + quote + _string + quote;
			}
			else
			{
				result = nameStr + quote + _string + quote;
			}
			break;

		case JSONtype::BOOLEAN:
			if (_isNameless)
			{
				result = tabs;
			}
			else
			{
				result = nameStr;
			}
			if (_booleanOrNumber.boolean == true)
			{
				result += "true";
			}
			else
			{
				result += "false";
			}
			break;

		case JSONtype::ARRAY:
			if (_isNameless)
			{
				result = tabs + "[\n";
			}
			else
			{
				result = nameStr + "\n" + tabs + "[\n";
			}

			for (size_t i = 0; i < amountOfFields; i++)
			{
				result += _fields[i]->toStr(level + 1);
				if (i != (amountOfFields - 1))
				{
					result += ",\n";
				}
			}
			result += "\n" + tabs + "]";
			break;

		case JSONtype::OBJECT:
			if ((level == 0) || _isNameless)
			{
				result = tabs + "{\n";
			}
			else
			{
				result = nameStr + "\n" + tabs + "{\n";
			}

			amountOfFields = _fields.size();
			for (size_t i = 0; i < amountOfFields; i++)
			{
				result += _fields[i]->toStr(level + 1);
				if (i != (amountOfFields - 1))
				{
					result += ",\n";
				}
			}
			result += "\n" + tabs + "}";
			break;

		default:
			if (_isNameless)
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

	inline std::string JSON::parseStr(std::string str, JSON* obj, unsigned int level)
	{
		if (level == 0)
		{
			std::smatch match;
			std::regex_search(str, match, std::regex("[\\s]*\\{[\\s]*"));
			str = match.suffix().str();
			return parseStr(str, this, 1);
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
		if (obj->isArray())
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
				if (obj->isArray())
				{
					obj->addArrayItem(JSONtype::NIL);
				}
				else
				{
					obj->setFieldType(match[null_regex_pos].str(), JSONtype::NIL);
				}

				str = match.suffix().str();
			}
			// number mached.
			else if (match[number_regex_pos].length() != 0)
			{
				char* end_ptr;
				if (obj->isArray())
				{
					double num = std::strtod(match[number_regex_pos].str().c_str(), &end_ptr);
					obj->addArrayItem(num);
				}
				else
				{
					double num = std::strtod(match[number_regex_pos + 1].str().c_str(), &end_ptr);
					obj->setField(match[number_regex_pos].str(), num);
				}
				str = match.suffix().str();
			}
			// string mached.
			else if (match[string_regex_pos].length() != 0)
			{
				if (obj->isArray())
				{
					obj->addArrayItem(match[string_regex_pos].str());
				}
				else
				{
					obj->setField(match[string_regex_pos].str(), match[string_regex_pos + 1].str());
				}
				str = match.suffix().str();
			}
			// boolean mached.
			else if (match[boolean_regex_pos].length() != 0)
			{
				if (obj->isArray())
				{
					if (match[boolean_regex_pos].str() == "true")
					{
						obj->addArrayItem(true);
					}
					else
					{
						obj->addArrayItem(false);
					}
				}
				else
				{
					if (match[boolean_regex_pos + 1].str() == "true")
					{
						obj->setField(match[boolean_regex_pos].str(), true);
					}
					else
					{
						obj->setField(match[boolean_regex_pos].str(), false);
					}
				}
				str = match.suffix().str();
			}
			// array mached.
			else if (match[array_regex_pos].length() != 0)
			{
				JSON* newArray = nullptr;
				if (obj->isArray())
				{
					size_t itemIndex = obj->addArrayItem(JSONtype::ARRAY);
					newArray = obj->getArrayItem(itemIndex);
				}
				else
				{
					newArray = obj->setFieldType(match[array_regex_pos].str(), JSONtype::ARRAY);
				}
				str = parseStr(match.suffix().str(), newArray, level + 1);
			}
			// object mached.
			else if (match[object_regex_pos].length() != 0)
			{
				JSON* newObject = nullptr;
				if (obj->isArray())
				{
					size_t itemIndex = obj->addArrayItem(JSONtype::OBJECT);
					newObject = obj->getArrayItem(itemIndex);
				}
				else
				{
					newObject = obj->setFieldType(match[object_regex_pos].str(), JSONtype::OBJECT);
				}
				str = parseStr(match.suffix().str(), newObject, level + 1);
			}

			if (tryExitObject(str, regexp_space, match))
			{
				return str;
			}
			if (obj->isArray())
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

	inline int JSON::parseFile(std::string filename)
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

	inline JSON* JSON::getField(std::string fieldName)
	{
		for (auto field : _fields)
		{
			if (field->_name == fieldName)
			{
				return field;
			}
		}
		return nullptr;
	}

	inline bool JSON::tryExitObject(std::string& str, std::regex& regexpSpace, std::smatch& match)
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
}

#endif
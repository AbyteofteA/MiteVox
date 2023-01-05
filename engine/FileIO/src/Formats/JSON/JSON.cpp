#include "JSON.h"

#include "JSONRegex.h"

#include "engine/CodeSafety/src/SafeArray.h"

#include <regex>
#include <iostream>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

namespace fileio
{
	JSON::JSON()
	{
		_type = JSONtype::OBJECT;
		_isNameless = true;
		_name = "";

		_booleanOrNumber.number = 0.0;
		_string = "";
	}

	JSON::~JSON()
	{
		clear();
	}

	void JSON::clear()
	{
		for (auto field : _fields)
		{
			delete field;
		}
		_fields.clear();
		_type = JSONtype::NIL;
	}

	void JSON::setType(JSONtype newType)
	{
		if (_type != newType)
		{
			clear();
			_type = newType;
		}
	}

	std::string JSON::getName()
	{
		return _name;
	}

	void JSON::setName(std::string name)
	{
		_name = name;
		_isNameless = false;
	}

	void JSON::setNameless(bool isNameless)
	{
		_isNameless = isNameless;
	}

	void JSON::set(double value)
	{
		_booleanOrNumber.number = value;
	}

	void JSON::set(std::string value)
	{
		_string = value;
	}

	void JSON::set(bool value)
	{
		_booleanOrNumber.boolean = value;
	}

	double JSON::getNumberOrDefault(double defaultValue)
	{
		if (isNumber())
		{
			return _booleanOrNumber.number;
		}
		return defaultValue;
	}

	std::string JSON::getStringOrDefault(std::string defaultValue)
	{
		if (isString())
		{
			return _string;
		}
		return defaultValue;
	}

	bool JSON::getBooleanOrDefault(bool defaultValue)
	{
		if (isBoolean())
		{
			return _booleanOrNumber.boolean;
		}
		return defaultValue;
	}

	void JSON::toStringArrayOrDefault(safety::SafeArray<std::string>* resultArray, std::string defaultValue)
	{
		if (_type != JSONtype::ARRAY)
		{
			return;
		}

		int64_t arraySize = (int64_t)_fields.size();
		resultArray->resize(arraySize);

		int64_t stringIndex = 0;
		for (int64_t index = 0; index < arraySize; ++index)
		{
			JSON* stringField = _fields[index];
			if (stringField->isString())
			{
				resultArray->setElement(stringIndex, stringField->getStringOrDefault(defaultValue));
				++stringIndex;
			}
		}
	}

	bool JSON::isNameless()
	{
		return _isNameless;
	}

	bool JSON::isNull()
	{
		if (_type == JSONtype::NIL)
		{
			return true;
		}
		return false;
	}

	bool JSON::isNumber()
	{
		if (_type == JSONtype::NUMBER)
		{
			return true;
		}
		return false;
	}

	bool JSON::isString()
	{
		if (_type == JSONtype::STRING)
		{
			return true;
		}
		return false;
	}

	bool JSON::isBoolean()
	{
		if (_type == JSONtype::BOOLEAN)
		{
			return true;
		}
		return false;
	}

	bool JSON::isArray()
	{
		if (_type == JSONtype::ARRAY)
		{
			return true;
		}
		return false;
	}

	bool JSON::isObject()
	{
		if (_type == JSONtype::OBJECT)
		{
			return true;
		}
		return false;
	}

	JSON* JSON::getField(std::string fieldName)
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

	std::vector<JSON*> JSON::getFields()
	{
		return _fields;
	}

	double JSON::getFieldNumberOrDefault(std::string fieldName, double defaultValue)
	{
		JSON* field = getField(fieldName);
		if (field)
		{
			return field->getNumberOrDefault(defaultValue);
		}
		return -0.0;
	}

	std::string JSON::getFieldStringOrDefault(std::string fieldName, std::string defaultValue)
	{
		JSON* field = getField(fieldName);
		if (field)
		{
			return field->getStringOrDefault(defaultValue);
		}
		return "";
	}

	bool JSON::getFieldBooleanOrDefault(std::string fieldName, bool defaultValue)
	{
		JSON* field = getField(fieldName);
		if (field)
		{
			return field->getBooleanOrDefault(defaultValue);
		}
		return false;
	}

	JSON* JSON::getFieldArray(std::string fieldName)
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

	JSON* JSON::getFieldObject(std::string fieldName)
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

	JSON* JSON::setFieldType(std::string fieldName, JSONtype type)
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

	JSON* JSON::setField(std::string fieldName, double value)
	{
		JSON* field = setFieldType(fieldName, JSONtype::NUMBER);
		field->_booleanOrNumber.number = value;
		return field;
	}

	JSON* JSON::setField(std::string fieldName, std::string value)
	{
		JSON* field = setFieldType(fieldName, JSONtype::STRING);
		field->_string = value;
		return field;
	}

	JSON* JSON::setField(std::string fieldName, bool value)
	{
		JSON* field = setFieldType(fieldName, JSONtype::BOOLEAN);
		field->_booleanOrNumber.boolean = value;
		return field;
	}

	JSON* JSON::setField(JSON* newField)
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

	size_t JSON::getArraySize()
	{
		if (_type != JSONtype::ARRAY)
		{
			return 0;
		}

		return _fields.size();
	}

	size_t JSON::addArrayItem(JSONtype type)
	{
		setType(JSONtype::ARRAY);

		JSON* newItem = new JSON();
		newItem->setType(type);
		newItem->setNameless(true);
		size_t index = _fields.size();
		_fields.push_back(newItem);
		return index;
	}

	size_t JSON::addArrayItem(double value)
	{
		size_t itemIndex = addArrayItem(JSONtype::NUMBER);
		JSON* newItem = getArrayItem(itemIndex);
		newItem->set(value);
		return itemIndex;
	}

	size_t JSON::addArrayItem(std::string value)
	{
		size_t itemIndex = addArrayItem(JSONtype::STRING);
		JSON* newItem = getArrayItem(itemIndex);
		newItem->set(value);
		return itemIndex;
	}

	size_t JSON::addArrayItem(bool value)
	{
		size_t itemIndex = addArrayItem(JSONtype::BOOLEAN);
		JSON* newItem = getArrayItem(itemIndex);
		newItem->set(value);
		return itemIndex;
	}

	size_t JSON::addArrayItem(JSON* newItem)
	{
		if (newItem->isNameless() == false)
		{
			newItem->setNameless(true);
		}

		size_t itemIndex = _fields.size();
		_fields.push_back(newItem);

		return itemIndex;
	}

	JSON* JSON::getArrayItem(size_t itemIndex)
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

	double JSON::getArrayItemNumberOrDefault(size_t itemIndex, double defaultValue)
	{
		JSON* numberJSON = getArrayItem(itemIndex);
		return numberJSON->getNumberOrDefault(defaultValue);
	}

	std::string JSON::getArrayItemStringOrDefault(size_t itemIndex, std::string defaultValue)
	{
		JSON* stringJSON = getArrayItem(itemIndex);
		return stringJSON->getStringOrDefault(defaultValue);
	}

	bool JSON::getArrayItemBoolOrDefault(size_t itemIndex, bool defaultValue)
	{
		JSON* boolJSON = getArrayItem(itemIndex);
		return boolJSON->getBooleanOrDefault(defaultValue);
	}

	SerializationStatus JSON::serialize(safety::SafeByteArray* fileData)
	{
		std::string fileDataStr = serializeHelper(0);
		fileData->fromASCII(fileDataStr);
		return SerializationStatus::COMPLETE;
	}

	DeserializationStatus JSON::deserialize(safety::SafeByteArray* fileData)
	{
		std::string fileDataStr = fileData->toASCII();
		deserializeHelper(fileDataStr, nullptr, 0);
		return DeserializationStatus::COMPLETE;
	}

	bool JSON::tryExitObject(std::string& str, std::regex& regexpSpace, std::smatch& match)
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

	std::string JSON::serializeHelper(unsigned int level)
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
				result += _fields[i]->serializeHelper(level + 1);
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
				result += _fields[i]->serializeHelper(level + 1);
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

	std::string JSON::deserializeHelper(std::string str, JSON* obj, unsigned int level)
	{
		if (level == 0)
		{
			std::smatch match;
			std::regex_search(str, match, std::regex("[\\s]*\\{[\\s]*"));
			str = match.suffix().str();
			return deserializeHelper(str, this, 1);
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

		while (match.empty() == false)
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
				str = deserializeHelper(match.suffix().str(), newArray, level + 1);
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
				str = deserializeHelper(match.suffix().str(), newObject, level + 1);
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
}
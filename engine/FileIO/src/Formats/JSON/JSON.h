#ifndef JSON_H
#define JSON_H

#include "engine/FileIO/src/Formats/FileFormatBase.h"
#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/FileIO/src/SerializationStatus.h"

#include <string>
#include <vector>
#include <regex>

#define JSON_TAB_STR "    "

namespace fileio
{
	enum struct JSONtype { NIL = 0, NUMBER, STRING, BOOLEAN, ARRAY, OBJECT };

	class JSON : public FileFormatBase
	{
	public:

		JSON();
		~JSON();

		bool isNameless();
		bool isNull();
		bool isNumber();
		bool isString();
		bool isBoolean();
		bool isArray();
		bool isObject();

		void clear();
		void setType(JSONtype newType);
		std::string getName();
		void setName(std::string);
		void setNameless(bool nameless = true);

		void set(double value);
		void set(std::string value);
		void set(bool value);

		double getNumberOrDefault(double defaultValue);
		std::string getStringOrDefault(std::string defaultValue);
		bool getBooleanOrDefault(bool defaultValue);

		template <typename T>
		inline void toNumberArray(safety::SafeArray<T>* resultArray);

		template <typename T>
		inline void fromNumberArray(safety::SafeArray<T>* numberArray);

		void toStringArrayOrDefault(safety::SafeArray<std::string>* resultArray, std::string defaultValue);

		// Field methods //

		JSON* getField(std::string fieldName);
		std::vector<JSON*> getFields();
		double getFieldNumberOrDefault(std::string fieldName, double defaultValue);
		std::string getFieldStringOrDefault(std::string fieldName, std::string defaultValue);
		bool getFieldBooleanOrDefault(std::string fieldName, bool defaultValue);
		JSON* getFieldArray(std::string fieldName);
		JSON* getFieldObject(std::string fieldName);

		JSON* setFieldType(std::string fieldName, JSONtype type);
		JSON* setField(std::string fieldName, double value);
		JSON* setField(std::string fieldName, std::string value);
		JSON* setField(std::string fieldName, bool value);
		JSON* setField(JSON* field);

		// Array methods //

		size_t getArraySize();
		size_t addArrayItem(JSONtype type = JSONtype::NIL);
		size_t addArrayItem(double value);
		size_t addArrayItem(std::string value);
		size_t addArrayItem(bool value);
		size_t addArrayItem(JSON* newItem);

		JSON* getArrayItem(size_t itemIndex);
		double getArrayItemNumberOrDefault(size_t itemIndex, double defaultValue);
		std::string getArrayItemStringOrDefault(size_t itemIndex, std::string defaultValue);
		bool getArrayItemBoolOrDefault(size_t itemIndex, bool defaultValue);

		SerializationStatus serialize(safety::SafeByteArray* fileData);
		DeserializationStatus deserialize(safety::SafeByteArray* fileData);

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

		bool tryExitObject(std::string& str, std::regex& regexpSpace, std::smatch& match);

		std::string serializeHelper(unsigned int level);
		std::string deserializeHelper(std::string str, JSON* obj, unsigned int level);
	};

	template <typename T>
	inline void JSON::toNumberArray(safety::SafeArray<T>* resultArray)
	{
		if (_type != JSONtype::ARRAY)
		{
			return;
		}

		int64_t arraySize = (int64_t)_fields.size();
		resultArray->resize(arraySize);

		int64_t numberIndex = 0;
		for (int64_t index = 0; index < arraySize; ++index)
		{
			JSON* numberField = _fields[index];
			if (numberField->isNumber())
			{
				resultArray->setElement(numberIndex, (T)numberField->getNumberOrDefault(0.0));
				++numberIndex;
			}
		}
	}

	template <typename T>
	inline void JSON::fromNumberArray(safety::SafeArray<T>* numberArray)
	{
		_type = JSONtype::ARRAY;
		_fields.clear();

		size_t arraySize = numberArray->getElementsCount();
		_fields.reserve(arraySize);

		for (int64_t index = 0; index < arraySize; ++index)
		{
			JSON* numberField = new JSON();
			numberField->setType(JSONtype::NUMBER);
			numberField->set((double)numberArray->getElement(index));
			_fields.push_back(numberField);
		}
	}
}

#endif
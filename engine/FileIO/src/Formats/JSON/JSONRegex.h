#ifndef JSONREGEX_H
#define JSONREGEX_H

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
#define JSON_REGEX_PAIR				\
	JSON_REGEX_FIELD_NULL			\
	"|"								\
	JSON_REGEX_FIELD_NUMBER			\
	"|"								\
	JSON_REGEX_FIELD_STRING			\
	"|"								\
	JSON_REGEX_FIELD_BOOLEAN		\
	"|"								\
	JSON_REGEX_FIELD_ARRAY			\
	"|"								\
	JSON_REGEX_FIELD_OBJECT

// Regex that matches any JSON value.
#define JSON_REGEX_VALUE			\
	JSON_REGEX_NULL					\
	"|"								\
	JSON_REGEX_NUMBER				\
	"|"								\
	JSON_REGEX_STRING				\
	"|"								\
	JSON_REGEX_BOOLEAN				\
	"|"								\
	JSON_REGEX_ARRAY_START			\
	"|"								\
	JSON_REGEX_OBJECT_START

#endif
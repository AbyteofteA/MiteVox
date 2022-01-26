#ifndef WAVEFRONTOBJREGEX_H
#define WAVEFRONTOBJREGEX_H

#define OBJ_REGEX_SPACE				"\\s*"
#define OBJ_REGEX_NAME				"([a-zA-z0-9_.]+)"
#define OBJ_REGEX_MANY_NAMES		"([a-zA-z0-9_.]+\\s*){2,}"
#define OBJ_REGEX_INT_NUMBER		"(-?\\d+)"
#define OBJ_REGEX_MANY_INT_NUMBERS	"(\\s+-?\\d+){2,}"
#define OBJ_REGEX_FLOAT_NUMBER		"(-?\\d+[.]\\d+)"
#define OBJ_REGEX_ON_OR_OFF			"(on|off)"

#define OBJ_REGEX_COMMENT_LINES		"(\\s*#[^\\n]*\\n?)*"

// Vertex data regular expresions

#define OBJ_REGEX_GEOMETRIC_VERTEX	\
	OBJ_REGEX_SPACE					\
	"v\\s+"							\
	OBJ_REGEX_FLOAT_NUMBER			\
	"\\s+"							\
	OBJ_REGEX_FLOAT_NUMBER			\
	"\\s+"							\
	OBJ_REGEX_FLOAT_NUMBER			\
	OBJ_REGEX_SPACE					\
	"\\n?"

#define OBJ_REGEX_TEXTURE_VERTEX	\
	OBJ_REGEX_SPACE					\
	"vt\\s+"						\
	OBJ_REGEX_FLOAT_NUMBER			\
	"\\s+"							\
	OBJ_REGEX_FLOAT_NUMBER			\
	OBJ_REGEX_SPACE					\
	"\\n?"

#define OBJ_REGEX_VERTEX_NORMAL		\
	OBJ_REGEX_SPACE					\
	"vn\\s+"						\
	OBJ_REGEX_FLOAT_NUMBER			\
	"\\s+"							\
	OBJ_REGEX_FLOAT_NUMBER			\
	"\\s+"							\
	OBJ_REGEX_FLOAT_NUMBER			\
	OBJ_REGEX_SPACE					\
	"\\n?"

// Elements regular expresions

#define OBJ_REGEX_POINT				\
	OBJ_REGEX_SPACE					\
	"p\\s+"							\
	OBJ_REGEX_INT_NUMBER			\
	OBJ_REGEX_SPACE					\
	"\\n?"

#define OBJ_REGEX_LINE				\
	OBJ_REGEX_SPACE					\
	"l\\s+"							\
	OBJ_REGEX_MANY_INT_NUMBERS		\
	"\\n?"

#define OBJ_REGEX_FACE					\
	OBJ_REGEX_SPACE						\
	"f"									\
	"(" OBJ_REGEX_MANY_INT_NUMBERS ")"	\
	OBJ_REGEX_SPACE						\
	"\\n?"

#define OBJ_REGEX_VERTEX_WITH_TEXTURE	\
	OBJ_REGEX_INT_NUMBER "\/" OBJ_REGEX_INT_NUMBER

#define OBJ_REGEX_FACE_WITH_TEXTURE		\
	OBJ_REGEX_SPACE						\
	"f"									\
	"((\\s+" OBJ_REGEX_VERTEX_WITH_TEXTURE "){2,})" \
	OBJ_REGEX_SPACE						\
	"\\n?"

#define OBJ_REGEX_VERTEX_WITH_NORMALS	\
	OBJ_REGEX_INT_NUMBER "\/\/" OBJ_REGEX_INT_NUMBER

#define OBJ_REGEX_FACE_WITH_NORMALS		\
	OBJ_REGEX_SPACE						\
	"f"									\
	"((\\s+" OBJ_REGEX_VERTEX_WITH_NORMALS "){2,})" \
	OBJ_REGEX_SPACE						\
	"\\n?"

#define OBJ_REGEX_VERTEX_COMPLETE	\
	OBJ_REGEX_INT_NUMBER "\/" OBJ_REGEX_INT_NUMBER "\/" OBJ_REGEX_INT_NUMBER

#define OBJ_REGEX_FACE_COMPLETE		\
	OBJ_REGEX_SPACE					\
	"f"							\
	"((\\s+" OBJ_REGEX_VERTEX_COMPLETE "){2,})" \
	OBJ_REGEX_SPACE					\
	"\\n?"

// Grouping regular expresions

#define OBJ_REGEX_GROUP					\
	OBJ_REGEX_SPACE						\
	"g\\s+"								\
	OBJ_REGEX_MANY_NAMES				\
	"\\n?"

#define OBJ_REGEX_SMOOTHING_GROUP		\
	OBJ_REGEX_SPACE						\
	"s\\s+"								\
	"(" OBJ_REGEX_INT_NUMBER "|off)"	\
	OBJ_REGEX_SPACE						\
	"\\n?"

#define OBJ_REGEX_OBJECT				\
	OBJ_REGEX_SPACE						\
	"o\\s+"								\
	OBJ_REGEX_NAME						\
	OBJ_REGEX_SPACE						\
	"\\n?"

// Display/render attributes regular expresions

#define OBJ_REGEX_BEVEL_INTERPOLATION		\
	OBJ_REGEX_SPACE							\
	"bevel\\s+"								\
	OBJ_REGEX_ON_OR_OFF						\
	OBJ_REGEX_SPACE							\
	"\\n?"

#define OBJ_REGEX_COLOR_INTERPOLATION		\
	OBJ_REGEX_SPACE							\
	"c_interp\\s+"							\
	OBJ_REGEX_ON_OR_OFF						\
	OBJ_REGEX_SPACE							\
	"\\n?"

#define OBJ_REGEX_DISSOLVE_INTERPOLATION	\
	OBJ_REGEX_SPACE							\
	"d_interp\\s+"							\
	OBJ_REGEX_ON_OR_OFF						\
	OBJ_REGEX_SPACE							\
	"\\n?"

#define OBJ_REGEX_LEVEL_OF_DETAIL			\
	OBJ_REGEX_SPACE							\
	"lod\\s+"								\
	OBJ_REGEX_INT_NUMBER					\
	OBJ_REGEX_SPACE							\
	"\\n?"

#define OBJ_REGEX_USE_MATERIAL				\
	OBJ_REGEX_SPACE							\
	"usemtl\\s+"							\
	OBJ_REGEX_NAME							\
	OBJ_REGEX_SPACE							\
	"\\n?"

#define OBJ_REGEX_MATERIAL_LIBRARY_FILE		\
	OBJ_REGEX_SPACE							\
	"mtllib\\s+"							\
	OBJ_REGEX_MANY_NAMES					\
	"\\n?"

#define OBJ_REGEX_SHADOW_OBJ_FILE			\
	OBJ_REGEX_SPACE							\
	"shadow_obj\\s+"						\
	OBJ_REGEX_NAME							\
	OBJ_REGEX_SPACE							\
	"\\n?"

#define OBJ_REGEX_RAY_TRACING_OBJ_FILE		\
	OBJ_REGEX_SPACE							\
	"trace_obj\\s+"							\
	OBJ_REGEX_NAME							\
	OBJ_REGEX_SPACE							\
	"\\n?"

#endif

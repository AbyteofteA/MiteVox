
#ifndef OBJ_H
#define OBJ_H

#include <string>

namespace fileio
{

#define COMMENTS		"(\n+ *#)*"
#define USE_MATERIAL	"(\n+ *usemtl +[a-zA-z0-9_]+)*"

	const std::string OBJ_regex =
		COMMENTS
		"(\n+ *mtllib( +[a-zA-z0-9_]+)+)*"
		COMMENTS
		"("
			USE_MATERIAL
			COMMENTS
			"(\n+ *vt( +-?[[:digit:]]+.?[[:digit:]]*){3})*"
			COMMENTS
			"(\n+ *vn( +-?[[:digit:]]+.?[[:digit:]]*){3})*"
			COMMENTS
			"(\n+ *p( +-?[[:digit:]]+){1,})*"
			COMMENTS
			"("
				USE_MATERIAL
				COMMENTS
				"(\n+ *l( +-?[[:digit:]]+/-?[[:digit:]]*){2,})*"
				COMMENTS
			")*"
			"("
				USE_MATERIAL
				COMMENTS
				"(\n+ *f( +-?[[:digit:]]+/-?[[:digit:]]*/-?[[:digit:]]*){3,})*"
				COMMENTS
			")*"
			COMMENTS
		")*";
}

#endif

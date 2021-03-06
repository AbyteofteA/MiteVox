
#ifndef GZ_FILE_IO_H
#define GZ_FILE_IO_H


/*-----------------------------------------------------------------------------------
	GZ(.gz) LABEL file structure:
-------------------------------------------------------------------------------------
		bytes		|		paddingType		|			description
--------------------+-------------------+--------------------------------------------
		  4			|		int			|	magic number (2049)
		  4			|		int			|	number of items
		  1			|	unsigned byte	|	label
		 ...		|		 ...		|	...
-------------------------------------------------------------------------------------
	GZ(.gz) IMAGE file structure:
-------------------------------------------------------------------------------------
		bytes		|		paddingType		|			description
--------------------+-------------------+--------------------------------------------
		  4			|		int			|	magic number (2051)
		  4			|		int			|	number of images
		  4			|		int			|	number of rows
		  4			|		int			|	number of columns
		  1			|	unsigned byte	|	pixel
		 ...		|		 ...		|	...
-------------------------------------------------------------------------------------
	NOTE:
-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------
	"IDX" file structure :
-------------------------------------------------------------------------------------
		bytes		|		paddingType		|			description
--------------------+-------------------+--------------------------------------------
		  2			|	unsigned byte	|	(zeros)
		  1			|	unsigned byte	|	X = the paddingType of the data
		  1			|	unsigned byte	|	N = the number of dimensions
		  4			|		int			|	size in dimension 0
		  4			|		int			|	size in dimension 1
		 ...		|		...			|	...
		  4			|		int			|	size in dimension N
	  sizeof(X)		|		 X			|	data
		 ...		|		...			|	...
-------------------------------------------------------------------------------------
NOTE:		X's possible values:
		---------------------------
		0x08 - unsigned	byte	|	1 byte
		0x09 - signed byte		|	1 byte
		0x0B - short			|	2 bytes
		0x0C - int				|	4 bytes
		0x0D - float			|	4 bytes
		0x0E - double			|	8 bytes
-----------------------------------------------------------------------------------*/

#endif
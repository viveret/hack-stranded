#ifndef SURVIVOR_ENGINE_FONT
#define SURVIVOR_ENGINE_FONT
					  			 
#include "../Core.hpp"

#include <string>
#include <vector>
						
#include <cstdlib>

#ifndef _ANDROID
#include <freetype2/ft2build.h>
#ifdef _WIN32
#include <freetype\freetype.h>
#else
#include <freetype2/freetype.h>
#endif
#endif

#include "../math/Math.hpp"
					



#define USE_SE_FONT

//#ifndef USE_SE_FONT
//#include "..\..\gl\libdraw\drawtext.h"
//#endif

namespace SE_Font
{	  			  
	void Register_Module();
	void Set_Default( const std::string& File );

	struct vertex
	{
		float x, y;
		float s, t;
	};

	struct quad
	{
		vertex v[6];
	};

	struct F_Glyphmap;		  

	struct F_Font
	{
		// freetype FT_Face
		void *face;

		// list of glyphmaps
		F_Glyphmap *gmaps;

		// last returned glyphmap (cache)						  
		F_Glyphmap *last_gmap;

		uint Size;
	};		

	struct F_Glyph
	{
		int code;
		float x, y, width, height;

		// normalized coords [0, 1] 
		float nx, ny, nwidth, nheight;
		float orig_x, orig_y;
		float advance;

		F_Glyph *next;
	};

	struct F_Glyphmap
	{
		uint ptsize;

		uint xsz, ysz;
		uchar *pixels;
		uint tex;

		int cstart, cend;	// character range 
		int crange;

		float line_advance;

		F_Glyph *glyphs;
		F_Glyphmap *next;
	};

	// draw buffering modes 
	enum {
		BUF_NONE = 0,	// unbuffered 
		BUF_LINE,		// line buffered 
		BUF_FULL		// fully buffered 
	};

	Vector2f Get_Str_Size( std::string Str );
				
		
#ifdef USE_SE_FONT
	/* Open a truetype/opentype/whatever font.
	 *
	 * If sz is non-zero, the default ASCII glyphmap at the requested point size is
	 * automatically created as well, and ready to use.
	 *
	 * To use other unicode ranges and different font sizes you must first call
	 * dtx_prepare or dtx_prepare_range before issuing any drawing calls, otherwise
	 * nothing will be rendered.
	 */																		
	uint Open_Font( std::string Name, uint Size = 0 );// 0 == default
	F_Font* Open_Font_Ptr( std::string Name, uint Size = 0 );// 0 == default
	//uint Load_Font( std::string Name, uint Size = 0 );// 0 == default
	uint Delete_Font( F_Font& Which );
	uint Delete_Font( uint Which );

	void Use_Font( F_Font* Which, uint Size = 0 );// 0 == default	   
	void Use_Font( uint Which, uint Size = 0 );// 0 == default	   

	void Draw_String( std::string Str, uchar Pos = 0 );

	void Flush();

	uint Add_Font( F_Font* nFont );
	uint Get_Font( F_Font* nFont );
#endif
	
				  
	namespace Font_Helper
	{		  		   
#ifdef USE_SE_FONT
		void Add_Glyph( F_Glyph *g, float x, float y);
		// HELPER FUNCTIONS
		void Prepare( F_Font* fnt, uint sz );
		// prepare an arbitrary unicode range glyphmap for the specified font size 
		void Prepare_Range( F_Font* fnt, uint sz, int cstart, int cend );

		/* Finds the glyphmap that contains the specified character code and matches the requested size
		 * Returns null if it hasn't been created (you should call dtx_prepare/dtx_prepare_range).
		 */
		F_Glyphmap* Get_Font_Glyphmap( F_Font* fnt, uint sz, int code );

		/* Finds the glyphmap that contains the specified unicode range and matches the requested font size
		 * Will automatically generate one if it can't find it.
		 */
		F_Glyphmap* Get_Font_Glyphmap_Range( F_Font* fnt, uint sz, int cstart, int cend );

		/* Creates and returns a glyphmap for a particular unicode range and font size.
		 * The generated glyphmap is added to the font's list of glyphmaps.
		 */
		F_Glyphmap* Create_Glyphmap_Range( F_Font* fnt, uint sz, int cstart, int cend );
		// free a glyphmap 
		void Free_Glyphmap( F_Glyphmap* gmap );

		// returns a pointer to the raster image of a glyphmap (1 byte per pixel grayscale) 
		uchar* Get_Glyphmap_Image( F_Glyphmap* gmap );
		// returns the width of the glyphmap image in pixels 
		uint Get_Glyphmap_Width( F_Glyphmap* gmap );
		// returns the height of the glyphmap image in pixels 
		uint Get_Glyphmap_Height( F_Glyphmap* gmap );

		// The following can be used even when the library is compiled without freetype support. (TODO)
		F_Glyphmap *Load_Glyphmap( std::string File );
		int Save_Glyphmap( std::string File, F_Glyphmap* gmap );

		
		/* ---- rendering ---- */

		/* sets the buffering mode
		 * - DTX_NBUF: every call to dtx_string gets rendered immediately.
		 * - DTX_LBUF: renders when the buffer is full or the string contains a newline.
		 * - DTX_FBUF: renders only when the buffer is full (you must call dtx_flush explicitly).
		 */
		void F_draw_buffering(int mode);

		/* Sets the vertex attribute indices to use for passing vertex and texture coordinate
		 * data. By default both are -1 which means you don't have to use a shader, and if you
		 * do they are accessible through gl_Vertex and gl_MultiTexCoord0, as usual.
		 *
		 * NOTE: If you are using OpenGL ES 2.x or OpenGL >= 3.1 pure (non-compatibility) context
		 * you must specify valid attribute indices.
		 */
		void F_vertex_attribs(int vert_attr, int tex_attr);

		/* draws a single glyph at the origin */
		void F_glyph(int code);
		/* draws a utf-8 string starting at the origin. \n \r and \t are handled appropriately. */
		void F_string(const char *str, uchar Pos = 0 );

		/* render any pending glyphs (see F_draw_buffering) */
		void F_flush(void);


		/* ---- encodings ---- */

		/* returns a pointer to the next character in a utf-8 stream */
		char *F_utf8_next_char( char *str );

		/* returns the unicode character codepoint of the utf-8 character starting at str */
		int F_utf8_char_code(const char *str);

		/* returns the number of bytes of the utf-8 character starting at str */
		uint F_utf8_nbytes(const char *str);

		/* returns the number of utf-8 character in a zero-terminated utf-8 string */
		uint F_utf8_char_count(const char *str);

		/* Converts a unicode code-point to a utf-8 character by filling in the buffer
		 * passed at the second argument, and returns the number of bytes taken by that
		 * utf-8 character.
		 * It's valid to pass a null buffer pointer, in which case only the byte count is
		 * returned (useful to figure out how much memory to allocate for a buffer).
		 */
		size_t F_utf8_from_char_code(int code, char *buf);

		/* Converts a unicode utf-16 wchar_t string to utf-8, filling in the buffer passed
		 * at the second argument. Returns the size of the resulting string in bytes.
		 *
		 * It's valid to pass a null buffer pointer, in which case only the size gets
		 * calculated and returned, which is useful for figuring out how much memory to
		 * allocate for the utf-8 buffer.
		 */
		size_t F_utf8_from_string(const wchar_t *str, char *buf);


		/* ---- metrics ---- */
		float F_line_height(void);

		/* rendered dimensions of a single glyph */
		void F_glyph_box(int code, Box2D *box);
		float F_glyph_width(int code);
		float F_glyph_height(int code);

		/* rendered dimensions of a string */
		void F_string_box(const char *str, Box2D *box);
		float F_string_width(const char *str);
		float F_string_height(const char *str);

		/* returns the horizontal position of the n-th character of the rendered string
		 * (useful for placing cursors)
		 */
		float F_char_pos(const char *str, int n);

		int F_char_at_pt(const char *str, float pt);


		void F_set_glyphmap_texture( F_Glyphmap *gmap);
		/* returns zero if it should NOT be printed and modifies xpos/ypos */				
		/* implemented in font.c */															 
		F_Glyphmap *F_proc_char(int code, float *xpos, float *ypos);	   
		int F_gl_init(void);




		void calc_best_size(int total_width, int max_glyph_height, int padding, int pow2, int *imgw, int *imgh);
#endif
	}
}







namespace DTX
{		
	struct dtx_glyphmap;		  

	struct dtx_font {
		/* freetype FT_Face */						 
		void *face;

		/* list of glyphmaps */										
		dtx_glyphmap *gmaps;

		/* last returned glyphmap (cache) */						  
		dtx_glyphmap *last_gmap;
	};		

	struct glyph {
		int code;
		float x, y, width, height;
		/* normalized coords [0, 1] */		   
		float nx, ny, nwidth, nheight;
		float orig_x, orig_y;
		float advance;
		glyph *next;
	};

	struct dtx_glyphmap {
		int ptsize;

		int xsz, ysz;
		unsigned char *pixels;
		unsigned int tex;

		int cstart, cend;	/* character range */			  
		int crange;

		float line_advance;

		glyph *glyphs;
		dtx_glyphmap *next;
	};

	struct dtx_box {
		float x, y;
		float width, height;
	};

	/*#ifdef __cplusplus
	extern "C" {
	#endif */

	/* Open a truetype/opentype/whatever font.
	 *
	 * If sz is non-zero, the default ASCII glyphmap at the requested point size is
	 * automatically created as well, and ready to use.
	 *
	 * To use other unicode ranges and different font sizes you must first call
	 * dtx_prepare or dtx_prepare_range before issuing any drawing calls, otherwise
	 * nothing will be rendered.
	 */
	dtx_font *dtx_open_font(const char *fname, int sz);
	void dtx_close_font(dtx_font *fnt);

	/* prepare an ASCII glyphmap for the specified font size */
	void dtx_prepare(dtx_font *fnt, int sz);
	/* prepare an arbitrary unicode range glyphmap for the specified font size */
	void dtx_prepare_range(dtx_font *fnt, int sz, int cstart, int cend);

	/* Finds the glyphmap that contains the specified character code and matches the requested size
	 * Returns null if it hasn't been created (you should call dtx_prepare/dtx_prepare_range).
	 */
	dtx_glyphmap *dtx_get_font_glyphmap(dtx_font *fnt, int sz, int code);

	/* Finds the glyphmap that contains the specified unicode range and matches the requested font size
	 * Will automatically generate one if it can't find it.
	 */
	dtx_glyphmap *dtx_get_font_glyphmap_range(dtx_font *fnt, int sz, int cstart, int cend);

	/* Creates and returns a glyphmap for a particular unicode range and font size.
	 * The generated glyphmap is added to the font's list of glyphmaps.
	 */
	dtx_glyphmap *dtx_create_glyphmap_range(dtx_font *fnt, int sz, int cstart, int cend);
	/* free a glyphmap */
	void dtx_free_glyphmap(dtx_glyphmap *gmap);

	/* returns a pointer to the raster image of a glyphmap (1 byte per pixel grayscale) */
	unsigned char *dtx_get_glyphmap_image(dtx_glyphmap *gmap);
	/* returns the width of the glyphmap image in pixels */
	int dtx_get_glyphmap_width(dtx_glyphmap *gmap);
	/* returns the height of the glyphmap image in pixels */
	int dtx_get_glyphmap_height(dtx_glyphmap *gmap);

	/* The following functions can be used even when the library is compiled without
	 * freetype support. (TODO)
	 */
	dtx_glyphmap *dtx_load_glyphmap(const char *fname);
	dtx_glyphmap *dtx_load_glyphmap_stream(FILE *fp);
	int dtx_save_glyphmap(const char *fname, const dtx_glyphmap *gmap);
	int dtx_save_glyphmap_stream(FILE *fp, const dtx_glyphmap *gmap);


	/* ---- rendering ---- */

	/* before drawing anything this function must set the font to use */
	SE_API void dtx_use_font(dtx_font *fnt, int sz);

	/* sets the buffering mode
	 * - DTX_NBUF: every call to dtx_string gets rendered immediately.
	 * - DTX_LBUF: renders when the buffer is full or the string contains a newline.
	 * - DTX_FBUF: renders only when the buffer is full (you must call dtx_flush explicitly).
	 */
	void dtx_draw_buffering(int mode);

	/* Sets the vertex attribute indices to use for passing vertex and texture coordinate
	 * data. By default both are -1 which means you don't have to use a shader, and if you
	 * do they are accessible through gl_Vertex and gl_MultiTexCoord0, as usual.
	 *
	 * NOTE: If you are using OpenGL ES 2.x or OpenGL >= 3.1 pure (non-compatibility) context
	 * you must specify valid attribute indices.
	 */
	void dtx_vertex_attribs(int vert_attr, int tex_attr);

	/* draws a single glyph at the origin */
	void dtx_glyph(int code);
	/* draws a utf-8 string starting at the origin. \n \r and \t are handled appropriately. */
	SE_API void dtx_string(const char *str, uchar Pos = 0 );

	/* render any pending glyphs (see dtx_draw_buffering) */
	void dtx_flush(void);


	/* ---- encodings ---- */

	/* returns a pointer to the next character in a utf-8 stream */
	char *dtx_utf8_next_char(char *str);

	/* returns the unicode character codepoint of the utf-8 character starting at str */
	int dtx_utf8_char_code(const char *str);

	/* returns the number of bytes of the utf-8 character starting at str */
	int dtx_utf8_nbytes(const char *str);

	/* returns the number of utf-8 character in a zero-terminated utf-8 string */
	int dtx_utf8_char_count(const char *str);

	/* Converts a unicode code-point to a utf-8 character by filling in the buffer
	 * passed at the second argument, and returns the number of bytes taken by that
	 * utf-8 character.
	 * It's valid to pass a null buffer pointer, in which case only the byte count is
	 * returned (useful to figure out how much memory to allocate for a buffer).
	 */
	size_t dtx_utf8_from_char_code(int code, char *buf);

	/* Converts a unicode utf-16 wchar_t string to utf-8, filling in the buffer passed
	 * at the second argument. Returns the size of the resulting string in bytes.
	 *
	 * It's valid to pass a null buffer pointer, in which case only the size gets
	 * calculated and returned, which is useful for figuring out how much memory to
	 * allocate for the utf-8 buffer.
	 */
	size_t dtx_utf8_from_string(const wchar_t *str, char *buf);


	/* ---- metrics ---- */
	float dtx_line_height(void);

	/* rendered dimensions of a single glyph */
	void dtx_glyph_box(int code, dtx_box *box);
	float dtx_glyph_width(int code);
	float dtx_glyph_height(int code);

	/* rendered dimensions of a string */
	void dtx_string_box(const char *str, dtx_box *box);
	float dtx_string_width(const char *str);
	float dtx_string_height(const char *str);

	/* returns the horizontal position of the n-th character of the rendered string
	 * (useful for placing cursors)
	 */
	float dtx_char_pos(const char *str, int n);

	int dtx_char_at_pt(const char *str, float pt);


	void set_glyphmap_texture(dtx_glyphmap *gmap);
	/* returns zero if it should NOT be printed and modifies xpos/ypos */				
	/* implemented in font.c */															 
	dtx_glyphmap *dtx_proc_char(int code, float *xpos, float *ypos);	   
	int dtx_gl_init(void);
}







#endif

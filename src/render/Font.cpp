#include "Font.hpp"
using namespace SE_Font;
using namespace Font_Helper;

#include "../module/Module.hpp"
#include "../gl/se_gl.hpp"
								
#define F_FTSZ_TO_PIXELS(x)	((x) / 64)
#define F_MAX_IMG_WIDTH		4096
#define QBUF_SZ		512

#define DEFAULT_FONT_SIZE 32

namespace SE_Font
{
/*************************    GLOBALS    *************************/
	FT_Library FT_Lib = NULL;

	F_Font* Cur_Font = NULL;
	uint Cur_Font_Size = DEFAULT_FONT_SIZE;
	uint Cur_Font_Tex = 0;
							   
//	uint vattr, tattr
	std::vector< F_Font* > m_Fonts;;
	F_Font* Font_Default = NULL;

	uchar Buf_Mode = BUF_LINE; 
	int vattr = -1, tattr = -1;
	quad qbuf[ QBUF_SZ ];
	int num_quads = 0;

	SE_Module_ID m_Mod = NULL;

/* **************************   FUNCTIONS   ******************** */
	uint Initialize( const std::vector<std::string>& mArgs );
	uint Cleanup();
}	


							
#ifdef USE_SE_FONT
uint SE_Font::Initialize( const std::vector<std::string>& mArgs )
{  
	if( FT_Init_FreeType( &FT_Lib ) != 0 )
	{
		sePrintf( SEPRINT_ERROR, "Failed to initialize FreeType\n" );
		return SE_FAILURE;
	}
	else
		sePrintf( SEPRINT_INFO, "Initialized FreeType\n" );

	Font_Default = (SE_Font::F_Font*)DTX::dtx_open_font( "fonts/default.ttf", DEFAULT_FONT_SIZE );
	Cur_Font = Font_Default;

	assert( Cur_Font != NULL );

	return SE_SUCCESS;
}

uint SE_Font::Cleanup()
{
	if( FT_Lib != NULL )
	{
		FT_Done_FreeType( FT_Lib );
		FT_Lib = NULL;
	}

	return SE_SUCCESS;
}	 


void SE_Font::Register_Module()
{
	m_Mod = S_Engine::Generate_Module( "font-manager" );
	S_Engine::Register_Module_Render( m_Mod, S_Engine::Reg_Mod_Time::MED_LEVEL, Initialize, Cleanup );
}


Vector2f SE_Font::Get_Str_Size( std::string Str )
{
	char* C_Str = (char*)Str.c_str();
	F_Glyphmap *gmap;
	float pos_x = 0.0f;
	float pos_y = 0.0f;

	Vector2f Ret;

	if(!Cur_Font) {
		return Ret;
	}

	while( *C_Str )
	{
		int code = F_utf8_char_code( C_Str );
		C_Str = F_utf8_next_char( (char*)C_Str );

		if((gmap = F_proc_char(code, &pos_x, &pos_y))) {
			int idx = code - gmap->cstart;

			if( ((F_Glyph*)(gmap->glyphs + idx))->height > Ret.y() )
				Ret.y() = ((F_Glyph*)(gmap->glyphs + idx))->height;
		}
	}

	Ret.x() = pos_x / 2.f;
	//Ret.y = pos_y;
	if( Ret.y() == 0 )
		Ret.y() = 32.f;

	return Ret;
}
		 
uint SE_Font::Open_Font( std::string Name, uint Size )// 0 == default
{				
	return Get_Font( Open_Font_Ptr( Name, Size ) );
}

F_Font* SE_Font::Open_Font_Ptr( std::string Name, uint Size )// 0 == default
{				
	F_Font* fnt;

	fnt = (F_Font*)calloc(1, sizeof *fnt);
	if( fnt == NULL )
	{
		sePrintf( SEPRINT_ERROR, "failed to allocate font structure\n" );
		return 0;
	}

	if(FT_New_Face( FT_Lib, Name.c_str(), 0, (FT_Face*)&fnt->face) != 0) {
		sePrintf( SEPRINT_ERROR, "failed to open font file: %s\n", Name.c_str() );
		return 0;
	}

	// pre-create the extended ASCII range glyphmap 
	if( Size )
		Prepare_Range(fnt, Size, 0, 256);

	fnt->Size = Size;
	Add_Font( fnt );

	return fnt;
}

uint SE_Font::Delete_Font( F_Font& Which )				 
{			 
	return 0;
}

uint SE_Font::Delete_Font( uint Which )
{			 
	return 0;
}

void SE_Font::Use_Font( F_Font* Which, uint Size )// 0 == default	   
{
	if( Which == NULL )
		Cur_Font = Font_Default;
	else
		Cur_Font = Which;

	Cur_Font_Size = Size;
}

void SE_Font::Use_Font( uint Which, uint Size )// 0 ==; default	   
{
	if( Which < m_Fonts.size() )
	{
		Cur_Font = m_Fonts[ Which ];
		Cur_Font_Size = m_Fonts[ Which ]->Size;//Size;
	}
}

void SE_Font::Draw_String( std::string Str, uchar Pos )
{
	char* C_Str = (char*)Str.c_str();//new char[ Str.length() ];
	//strcpy( C_Str, Str.c_str() );

	F_Glyphmap *gmap;

	bool should_flush = Buf_Mode == BUF_NONE;
	float pos_x = 0.0f;
	float pos_y = 0.0f;

	if(!Cur_Font) {
		return;
	}

	while( *C_Str )
	{
		float px, py;
		int code = F_utf8_char_code( C_Str );
		C_Str = F_utf8_next_char( (char*)C_Str );

		if( Buf_Mode == BUF_LINE && (code == '\n' || code == '\r' || code == '\0') )
		{
			should_flush = true;
			break;
		}

		px = pos_x;
		py = pos_y;

		if((gmap = F_proc_char(code, &pos_x, &pos_y))) {
			int idx = code - gmap->cstart;

			F_set_glyphmap_texture(gmap);
			Add_Glyph(gmap->glyphs + idx, px, py);
		}	 
	}

	// now fix glyphs
	if( should_flush )
	{
		glPushMatrix();

		switch( Pos )
		{
		case SE_Gravity::Mid_Bottom:
//			glTranslatef( -pos_x/2.f, 0, 0 );
			break;
		case SE_Gravity::Right_Bottom:
//			glTranslatef( -pos_x, 0, 0 );
			break;
		}

		Flush();	

		glPopMatrix();
	}
}

void SE_Font::Flush()
{	 
	int vbo;

	if(!num_quads) {
		return;
	}


	glPushMatrix();
	glScalef( 1, -1, 1 );


	if(glBindBuffer) {
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
   
#ifndef GL_ES
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
#endif
	segl_Use_Texture( Cur_Font_Tex );

	if(vattr != -1 && glEnableVertexAttribArray) {
		glEnableVertexAttribArray(vattr);
		glVertexAttribPointer(vattr, 2, GL_FLOAT, 0, sizeof(vertex), qbuf);
#ifndef GL_ES
	} else {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, sizeof(vertex), qbuf);
#endif
	}
	if(tattr != -1 && glEnableVertexAttribArray) {
		glEnableVertexAttribArray(tattr);
		glVertexAttribPointer(tattr, 2, GL_FLOAT, 0, sizeof(vertex), &qbuf->v[0].s);
#ifndef GL_ES
	} else {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(vertex), &qbuf->v[0].s);
#endif
	}

	glEnable(GL_BLEND);
	segl_Set1i( GL_TEXTURE_2D, SEGL_TEXTURE_ALPHA );

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   
	//glColor3f( -1, -1, -1 );
	//glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_DST_ALPHA); 

	glDepthMask(0);

	glDrawArrays(GL_TRIANGLES, 0, num_quads * 6);

	glDepthMask(1);

	if(vattr != -1 && glDisableVertexAttribArray) {
		glDisableVertexAttribArray(vattr);
#ifndef GL_ES
	} else {
		glDisableClientState(GL_VERTEX_ARRAY);
#endif
	}
	if(tattr != -1 && glDisableVertexAttribArray) {
		glDisableVertexAttribArray(tattr);
#ifndef GL_ES
	} else {
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#endif
	}

#ifndef GL_ES
	glPopAttrib();
#else
	glDisable(GL_BLEND);
#endif

	if(glBindBuffer && vbo) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);	  
	}							 			  

	num_quads = 0;

	glPopMatrix();
}
			 
uint SE_Font::Add_Font( F_Font* nFont )
{
	m_Fonts.push_back( nFont );
	return m_Fonts.size() - 1;
}

uint SE_Font::Get_Font( F_Font* nFont )
{
	for( uint i = 0;i < m_Fonts.size(); i++ )
	{
		if( m_Fonts[i] == nFont )
			return i;
	}

	return -1;
}








// HELPER FUNCTIONS
void Font_Helper::Prepare( F_Font* fnt, uint sz )
{			  
	Get_Font_Glyphmap_Range( fnt, sz, 0, 256 );
}

void Font_Helper::Prepare_Range( F_Font* fnt, uint sz, int cstart, int cend )
{		 
	Get_Font_Glyphmap_Range(fnt, sz, cstart, cend);
}

F_Glyphmap* Font_Helper::Get_Font_Glyphmap( F_Font* fnt, uint sz, int code )
{					 
	F_Glyphmap *gm;

	// check to see if the last we've given out fits the bill 
	if(fnt->last_gmap && code >= fnt->last_gmap->cstart && code < fnt->last_gmap->cend && fnt->last_gmap->ptsize == sz) 
		return fnt->last_gmap;

	// otherwise search for the appropriate glyphmap 
	gm = fnt->gmaps;
	while(gm) 
	{
		if(code >= gm->cstart && code < gm->cend && sz == gm->ptsize)
		{
			fnt->last_gmap = gm;
			return gm;
		}
		gm = gm->next;
	}

	return NULL;
}

F_Glyphmap* Font_Helper::Get_Font_Glyphmap_Range( F_Font* fnt, uint sz, int cstart, int cend )
{		  		 
	F_Glyphmap *gm;

	/* search the available glyphmaps to see if we've got one that includes
	 * the requested range
	 */
	gm = fnt->gmaps;
	while(gm)
	{
		if(gm->cstart <= cstart && gm->cend >= cend && gm->ptsize == sz)
			return gm;

		gm = gm->next;
	}

	/* not found, create one and add it to the list */
	if(!(gm = Create_Glyphmap_Range(fnt, sz, cstart, cend)))
		return NULL;

	return gm;
}
					   
F_Glyphmap* Font_Helper::Create_Glyphmap_Range( F_Font* fnt, uint sz, int cstart, int cend )
{		
	FT_Face face = (FT_Face)fnt->face;
	F_Glyphmap *gmap;
	int i, j;
	int gx, gy;
	int padding = 4;
	int total_width = padding;
	int max_height = 0;

	FT_Set_Char_Size((FT_Face)fnt->face, 0, sz * 64, 72, 72);

	if(!(gmap = (F_Glyphmap*)calloc(1, sizeof *gmap))) {
		return 0;
	}

	gmap->ptsize = sz;
	gmap->cstart = cstart;
	gmap->cend = cend;
	gmap->crange = cend - cstart;
	gmap->line_advance = F_FTSZ_TO_PIXELS((float)face->size->metrics.height);

	
	gmap->glyphs = (F_Glyph*)malloc(gmap->crange * sizeof *gmap->glyphs);

	if( gmap->glyphs == NULL )
	{
		free(gmap);
		return 0;
	}

	for(i=0; i<gmap->crange; i++) {
		int h;

		FT_Load_Char(face, i + cstart, 0);
		h = F_FTSZ_TO_PIXELS(face->glyph->metrics.height);

		if(h > max_height) {
			max_height = h;
		}
		total_width += F_FTSZ_TO_PIXELS(face->glyph->metrics.width) + padding;
	}

	calc_best_size(total_width, max_height, padding, 1, (int*)&gmap->xsz, (int*)&gmap->ysz);

	if(!(gmap->pixels = (unsigned char*)malloc(gmap->xsz * gmap->ysz))) {
		free(gmap->glyphs);
		free(gmap);
		return 0;
	}
	memset(gmap->pixels, 0, gmap->xsz * gmap->ysz);

	gx = padding;
	gy = padding;

	for(i=0; i<gmap->crange; i++) {
		float gwidth, gheight;
		unsigned char *src, *dst;
		FT_GlyphSlot glyph;

		FT_Load_Char(face, i + cstart, FT_LOAD_RENDER);
		glyph = face->glyph;
		gwidth = F_FTSZ_TO_PIXELS((float)glyph->metrics.width);
		gheight = F_FTSZ_TO_PIXELS((float)glyph->metrics.height);

		if(gx > gmap->xsz - gwidth - padding) {
			gx = padding;
			gy += max_height + padding;
		}

		src = glyph->bitmap.buffer;
		dst = gmap->pixels + gy * gmap->xsz + gx;

		for(j=0; j<glyph->bitmap.rows; j++) {
			memcpy(dst, src, glyph->bitmap.width);
			dst += gmap->xsz;
			src += glyph->bitmap.pitch;
		}

		gmap->glyphs[i].code = i;
		gmap->glyphs[i].x = gx - 1.f;
		gmap->glyphs[i].y = gy - 1.f;
		gmap->glyphs[i].width = gwidth + 2;
		gmap->glyphs[i].height = gheight + 2;
		gmap->glyphs[i].orig_x = -F_FTSZ_TO_PIXELS((float)glyph->metrics.horiBearingX) + 1;
		gmap->glyphs[i].orig_y = F_FTSZ_TO_PIXELS((float)glyph->metrics.height - glyph->metrics.horiBearingY) + 1;
		gmap->glyphs[i].advance = F_FTSZ_TO_PIXELS((float)glyph->metrics.horiAdvance);
		/* also precalc normalized */
		gmap->glyphs[i].nx = (float)gmap->glyphs[i].x / (float)gmap->xsz;
		gmap->glyphs[i].ny = (float)gmap->glyphs[i].y / (float)gmap->ysz;
		gmap->glyphs[i].nwidth = (float)gmap->glyphs[i].width / (float)gmap->xsz;
		gmap->glyphs[i].nheight = (float)gmap->glyphs[i].height / (float)gmap->ysz;

		gx += (int)gwidth + padding;
	}

	/* add it to the glyphmaps list of the font */
	gmap->next = fnt->gmaps;
	fnt->gmaps = gmap;

	return gmap;
}

void Font_Helper::Free_Glyphmap( F_Glyphmap* gmap )
{				  
	if(gmap) {
		free(gmap->pixels);
		free(gmap->glyphs);
		free(gmap);
	}
}

uchar* Font_Helper::Get_Glyphmap_Image( F_Glyphmap* gmap )		  // @@@@@@@@@@@@@@@@@@@@@@@@@@@@ ERROR
{					 
	return gmap->pixels;
}

uint Font_Helper::Get_Glyphmap_Width( F_Glyphmap* gmap )
{			   
	return gmap->xsz;
}

uint Font_Helper::Get_Glyphmap_Height( F_Glyphmap* gmap )
{				
	return gmap->ysz;
}

F_Glyphmap* Font_Helper::Load_Glyphmap( std::string File )
{					
	char buf[512];
	int hdr_lines = 0;
	F_Glyphmap *gmap;
	F_Glyph *glyphs = 0;
	int min_code = INT_MAX;
	int max_code = INT_MIN;
	int i, max_pixval, num_pixels;

	if(!(gmap = (F_Glyphmap*)calloc(1, sizeof *gmap))) {
		sePrintf( SEPRINT_ERROR, "failed to allocate glyphmap\n" );
		return 0;
	}
				
	FILE *fp = fopen(File.c_str(), "r");
	if( fp == NULL )
		return 0;

	while(hdr_lines < 3) {
		char *line = buf;
		if(!fgets(buf, sizeof buf, fp )) {
			sePrintf( SEPRINT_ERROR, "unexpected end of file\n" );
			goto err;
		}

		while(isspace(*line)) {
			line++;
		}

		if(line[0] == '#') {
			F_Glyph *g;
			int c;
			float x, y, xsz, ysz, res;

			res = (float)sscanf(line + 1, "%d: %fx%f+%f+%f\n", &c, &xsz, &ysz, &x, &y);
			if(res != 5) {
//				fprintf(stderr, "%s: invalid glyph info line\n", __func__);
				goto err;
			}

			if(!(g = (F_Glyph*)malloc(sizeof *g))) {
				sePrintf( SEPRINT_ERROR, "failed to allocate glyph\n" );
				goto err;
			}
			g->code = c;
			g->x = x;
			g->y = y;
			g->width = xsz;
			g->height = ysz;
			g->next = glyphs;
			glyphs = g;

			if(c < min_code) {
				min_code = c;
			}
			if(c > max_code) {
				max_code = c;
			}
		} else {
			switch(hdr_lines) {
			case 0:
				if(0[line] != 'P' || 1[line] != '6') {
					sePrintf( SEPRINT_ERROR, "invalid file format (magic)\n" );
					goto err;
				}
				break;

			case 1:
				if(sscanf(line, "%d %d", &gmap->xsz, &gmap->ysz) != 2) {
					sePrintf( SEPRINT_ERROR, "invalid file format (dim)\n" );
					goto err;
				}
				break;

			case 2:
				{
					char *endp;
					max_pixval = strtol(line, &endp, 10);
					if(endp == line) {
						sePrintf( SEPRINT_ERROR, "invalid file format (maxval)\n" );
						goto err;
					}
				}
				break;

			default:
				break;	/* can't happen */
			}
			hdr_lines++;
		}
	}

	num_pixels = gmap->xsz * gmap->ysz;
	if(!(gmap->pixels = (unsigned char*)malloc(num_pixels))) {
		sePrintf( SEPRINT_ERROR, "failed to allocate pixels" );
		goto err;
	}

	for(i=0; i<num_pixels; i++) {
		long c = fgetc(fp);
		if(c == -1) {
			sePrintf( SEPRINT_ERROR, "unexpected end of file while reading pixels\n" );
			goto err;
		}
		gmap->pixels[i] = 255 * (uchar)c / max_pixval;
		fseek(fp, 2, SEEK_CUR);
	}

	gmap->cstart = min_code;
	gmap->cend = max_code + 1;
	gmap->crange = gmap->cend - gmap->cstart;

	if(!(gmap->glyphs = (F_Glyph*)calloc(gmap->crange, sizeof *gmap->glyphs))) {
		sePrintf( SEPRINT_ERROR, "failed to allocate glyph info" );
		goto err;
	}

	while(glyphs) {
		F_Glyph *g = glyphs;
		glyphs = glyphs->next;

		gmap->glyphs[g->code - gmap->cstart] = *g;
		free(g);
	}
	return gmap;

err:
	Free_Glyphmap(gmap);
	while(glyphs) {
		void *tmp = glyphs;
		glyphs = glyphs->next;
		free(tmp);
	}
	return 0;
}

int Font_Helper::Save_Glyphmap( std::string File, F_Glyphmap* gmap )
{
	//
	return 0;
}


void F_qvertex(vertex *v, float x, float y, float s, float t)
{
	v->x = x;
	v->y = y;
	v->s = s;
	v->t = t;
}

void Font_Helper::Add_Glyph(F_Glyph *g, float x, float y)
{
	quad *qptr = qbuf + num_quads;

	x -= g->orig_x;
	y -= g->orig_y;

	F_qvertex(qptr->v + 0, x, y, g->nx, g->ny + g->nheight);
	F_qvertex(qptr->v + 1, x + g->width, y, g->nx + g->nwidth, g->ny + g->nheight);
	F_qvertex(qptr->v + 2, x + g->width, y + g->height, g->nx + g->nwidth, g->ny);  

	F_qvertex(qptr->v + 3, x, y, g->nx, g->ny + g->nheight);
	F_qvertex(qptr->v + 4, x + g->width, y + g->height, g->nx + g->nwidth, g->ny);
	F_qvertex(qptr->v + 5, x, y + g->height, g->nx, g->ny); 

	if( ++num_quads >= QBUF_SZ )
		Flush();
}

void Font_Helper::F_set_glyphmap_texture( F_Glyphmap *gmap)
{			  
	if(!gmap->tex)
	{
		gmap->tex = segl_Gen_Tex();

		segl_Use_Texture( gmap->tex );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

#ifdef GL_ES
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, gmap->xsz, gmap->ysz, 0, GL_ALPHA, GL_UNSIGNED_BYTE, gmap->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
#else
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_ALPHA, gmap->xsz, gmap->ysz, GL_ALPHA, GL_UNSIGNED_BYTE, gmap->pixels);
#endif
#endif
	}

	//if( Cur_Font_Tex != gmap->tex) 
//		Flush();

	Cur_Font_Tex = gmap->tex;
}

F_Glyphmap *Font_Helper::F_proc_char(int code, float *xpos, float *ypos)
{				   
	F_Glyphmap *gmap;
	gmap = Get_Font_Glyphmap((F_Font*)Cur_Font, Cur_Font_Size, code);

	switch(code) {
	case '\n':
		*xpos = 0.0;
		if(gmap) {
			*ypos -= gmap->line_advance;
		}
		return 0;

	case '\t':
		if(gmap) {
			*xpos = (fmod(*xpos, 4) + 4.f) * gmap->glyphs[0].advance;
		}
		return 0;

	case '\r':
		*xpos = 0.0;
		return 0;

	default:
		break;
	}

	if(gmap) {
		*xpos += gmap->glyphs[code - gmap->cstart].advance;
	}
	return gmap;
}							
		
void vertex_attribs(int vert_attr, int tex_attr)
{
	vattr = vert_attr;
	tattr = tex_attr;
}





#define	F_U8_IS_FIRST(x)		(((((x) >> 7) & 1) == 0) || ((((x) >> 6) & 3) == 3))

 const char F_first_mask[] = {
	0,
	0x7f,	/* single byte, 7 bits valid */
	0x1f,	/* two-bytes, 5 bits valid */
	0xf,	/* three-bytes, 4 bits valid */
	0x7		/* four-bytes, 3 bits valid */
};
 const char F_first_shift[] = { 0, 7, 5, 4, 3 };	/* see above */

#define F_CONT_PREFIX	0x80
#define F_CONT_MASK	0x3f
#define F_CONT_SHIFT	6

/* last charcodes for 1, 2, 3 or 4-byte utf8 chars */
 const int F_utf8_lastcode[] = { 0x7f, 0x7ff, 0xfff, 0x1fffff };

#define F_prefix_mask(x)	(~F_first_mask[x])
#define F_prefix(x)		((F_prefix_mask(x) << 1) & 0xff)


char *Font_Helper::F_utf8_next_char( char *str )
{				   
	return str + F_utf8_nbytes(str);
}

int Font_Helper::F_utf8_char_code(const char *str)
{					
	int i, nbytes, shift, code = 0;
	int mask;

	if(!F_U8_IS_FIRST(*str)) {
		return -1;
	}

	nbytes = F_utf8_nbytes(str);
	mask = F_first_mask[nbytes];
	shift = 0;

	for(i=0; i<nbytes; i++) {
		if(!*str) {
			break;
		}

		code = (code << shift) | (*str++ & mask);
		mask = 0x3f;
		shift = 6;
	}
	return code;
}			   

uint Font_Helper::F_utf8_nbytes(const char *str)
{				   
	int i, numset = 0;
	int c = *str;

	if(!F_U8_IS_FIRST(c)) {
		for(i=0; !F_U8_IS_FIRST(str[i]); i++);
		return i;
	}

	/* count the leading 1s */
	for(i=0; i<4; i++) {
		if(((c >> (7 - i)) & 1) == 0) {
			break;
		}
		numset++;
	}

	if(!numset) {
		return 1;
	}
	return numset;
}

int next_pow2(int x)
{
	x--;
	x = (x >> 1) | x;
	x = (x >> 2) | x;
	x = (x >> 4) | x;
	x = (x >> 8) | x;
	x = (x >> 16) | x;
	return x + 1;
}

void Font_Helper::calc_best_size(int total_width, int max_glyph_height, int padding, int pow2, int *imgw, int *imgh)
{
	int xsz, ysz, num_rows;
	float aspect;

	for(xsz=2; xsz<=F_MAX_IMG_WIDTH; xsz *= 2) {
		num_rows = total_width / xsz + 1;

		/* take into account the one extra padding for each row after the first */
		num_rows = (total_width + padding * (num_rows - 1)) / xsz + 1;

		ysz = num_rows * (max_glyph_height + padding) + padding;
		if(pow2) {
			ysz = next_pow2(ysz);
		}
		aspect = (float)xsz / (float)ysz;

		if(aspect >= 1.0) {
			break;
		}
	}

	if(xsz > F_MAX_IMG_WIDTH) {
		xsz = F_MAX_IMG_WIDTH;
	}

	*imgw = xsz;
	*imgh = ysz;
}				  








/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    FONT.CPP    @@@@@@@@@@@@@@@@@@@@@
/*
libdrawtext - a simple library for fast text rendering in OpenGL
Copyright (C) 2011  John Tsiombikas <nuclear@member.fsf.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/							
#include <cstdlib>

#ifndef NO_FREETYPE
#define USE_FREETYPE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <ctype.h>
#include <float.h>
#include <errno.h>
#ifdef USE_FREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H
#endif


#define FTSZ_TO_PIXELS(x)	((x) / 64)
#define MAX_IMG_WIDTH		4096

using namespace DTX;


#ifdef USE_FREETYPE
 int init_freetype(void);
 void cleanup_FT();

// void calc_best_size(int total_width, int max_glyph_height, int padding, int pow2, int *imgw, int *imgh);
// int next_pow2(int x);

// FT_Library ft;


 int init_done;

/* int init_freetype(void)
{
	if(!init_done) {
		if(FT_Init_FreeType(&ft) != 0) {
			return -1;
		}
		atexit(cleanup_FT);
		init_done = 1;
	}
	return 0;
} */

 void cleanup_FT()
{
	if(init_done) {
//		FT_Done_FreeType(ft);
	}
}

dtx_font *DTX::dtx_open_font(const char *fname, int sz)
{
	return (dtx_font*)Open_Font_Ptr( fname, sz );
}

void DTX::dtx_close_font(dtx_font *fnt)
{
	if(!fnt) return;

	FT_Done_Face((FT_Face)fnt->face);

	/* destroy the glyphmaps */
	while(fnt->gmaps) {
		DTX::dtx_glyphmap* tmp = fnt->gmaps;
		fnt->gmaps = fnt->gmaps->next;
		dtx_free_glyphmap(tmp);
	}

	free(fnt);
}

void DTX::dtx_prepare(dtx_font *fnt, int sz)
{
	Font_Helper::Prepare_Range((F_Font*)fnt, sz, 0, 256);
}

void DTX::dtx_prepare_range(dtx_font *fnt, int sz, int cstart, int cend)
{
	Font_Helper::Get_Font_Glyphmap_Range( (F_Font*)fnt, sz, cstart, cend);
}

dtx_glyphmap* DTX::dtx_get_font_glyphmap(dtx_font *fnt, int sz, int code)
{
	return (dtx_glyphmap*)Font_Helper::Get_Font_Glyphmap( (F_Font*)fnt, sz, code );
}

dtx_glyphmap *DTX::dtx_get_font_glyphmap_range(dtx_font *fnt, int sz, int cstart, int cend)
{
	return (dtx_glyphmap *)Font_Helper::Get_Font_Glyphmap_Range( (F_Font*)fnt, sz, cstart, cend );
}

dtx_glyphmap *DTX::dtx_create_glyphmap_range(dtx_font *fnt, int sz, int cstart, int cend)
{
	return (dtx_glyphmap *)Font_Helper::Create_Glyphmap_Range( (F_Font*)fnt, sz, cstart, cend );
}
#endif	/* USE_FREETYPE */

void DTX::dtx_free_glyphmap(dtx_glyphmap *gmap)
{
	if(gmap) {
		free(gmap->pixels);
		free(gmap->glyphs);
		free(gmap);
	}
}

unsigned char *dtx_get_glyphmap_pixels(dtx_glyphmap *gmap)
{
	return gmap->pixels;
}

int DTX::dtx_get_glyphmap_width(dtx_glyphmap *gmap)
{
	return gmap->xsz;
}

int DTX::dtx_get_glyphmap_height(dtx_glyphmap *gmap)
{
	return gmap->ysz;
}

dtx_glyphmap *DTX::dtx_load_glyphmap(const char *fname)
{
	return (dtx_glyphmap *)Font_Helper::Load_Glyphmap( fname );
}

dtx_glyphmap *DTX::dtx_load_glyphmap_stream(FILE *fp)
{
	return 0;
}

int DTX::dtx_save_glyphmap(const char *fname, const dtx_glyphmap *gmap)
{
	FILE *fp;
	int res;

	if(!(fp = fopen(fname, "wb"))) {
//		fprintf(stderr, "%s: failed to open file: %s: %s\n", __func__, fname, strerror(errno));
		return -1;
	}
	res = dtx_save_glyphmap_stream(fp, gmap);
	fclose(fp);
	return res;
}

int DTX::dtx_save_glyphmap_stream(FILE *fp, const dtx_glyphmap *gmap)
{
	int i, num_pixels;
	glyph *g = gmap->glyphs;

	fprintf(fp, "P6\n%d %d\n", gmap->xsz, gmap->ysz);
	for(i=0; i<gmap->crange; i++) {
		fprintf(fp, "# %d: %fx%f+%f+%f\n", g->code, g->width, g->height, g->x, g->y);
		g++;
	}
	fprintf(fp, "255\n");

	num_pixels = gmap->xsz * gmap->ysz;
	for(i=0; i<num_pixels; i++) {
		int c = gmap->pixels[i];
		fputc(c, fp);
		fputc(c, fp);
		fputc(c, fp);
	}
	return 0;
}


void DTX::dtx_use_font(dtx_font *fnt, int sz)
{
	//Use_Font( *((F_Font*)fnt), sz );
	//return;
	dtx_gl_init();

	if( fnt )
		Cur_Font = (SE_Font::F_Font*)fnt;
	else
		Cur_Font = Font_Default;

	Cur_Font_Size = sz;
}

float DTX::dtx_line_height(void)
{
	dtx_glyphmap *gmap = dtx_get_font_glyphmap((dtx_font*)Cur_Font, Cur_Font_Size, '\n');

	return gmap->line_advance;
}

void DTX::dtx_glyph_box(int code, dtx_box *box)
{
	int cidx;
	dtx_glyphmap *gmap;
	gmap = dtx_get_font_glyphmap((dtx_font*)Cur_Font, Cur_Font_Size, code);
	cidx = code - gmap->cstart;

	box->x = gmap->glyphs[cidx].orig_x;
	box->y = gmap->glyphs[cidx].orig_y;
	box->width = gmap->glyphs[cidx].width;
	box->height = gmap->glyphs[cidx].height;
}

float DTX::dtx_glyph_width(int code)
{
	dtx_box box;
	dtx_glyph_box(code, &box);
	return box.width;
}

float DTX::dtx_glyph_height(int code)
{
	dtx_box box;
	dtx_glyph_box(code, &box);
	return box.height;
}

void DTX::dtx_string_box(const char *str, dtx_box *box)
{
	int code;
	float pos_x = 0.0f, pos_y = 0.0f;
	glyph *g = 0;
	float x0, y0, x1, y1;

	x0 = y0 = FLT_MAX;
	x1 = y1 = -FLT_MAX;

	while(*str) {
		float px, py;
		dtx_glyphmap *gmap;

		code = dtx_utf8_char_code(str);
		str = dtx_utf8_next_char((char*)str);

		px = pos_x;
		py = pos_y;

		if((gmap = dtx_proc_char(code, &pos_x, &pos_y))) {
			g = gmap->glyphs + code - gmap->cstart;

			if(px + g->orig_x < x0) {
				x0 = px + g->orig_x;
			}
			if(py - g->orig_y < y0) {
				y0 = py - g->orig_y;
			}
			if(px + g->orig_x + g->width > x1) {
				x1 = px + g->orig_x + g->width;
			}
			if(py - g->orig_y + g->height > y1) {
				y1 = py - g->orig_y + g->height;
			}
		}
	}

	box->x = x0;
	box->y = y0;
	box->width = x1 - x0;
	box->height = y1 - y0;
}

float DTX::dtx_string_width(const char *str)
{
	dtx_box box;

	dtx_string_box(str, &box);
	return box.width;
}

float DTX::dtx_string_height(const char *str)
{
	dtx_box box;

	dtx_string_box(str, &box);
	return box.height;
}

float DTX::dtx_char_pos(const char *str, int n)
{
	int i;
	float pos = 0.0;
	dtx_glyphmap *gmap;

	for(i=0; i<n; i++) {
		int code = dtx_utf8_char_code(str);
		str = dtx_utf8_next_char((char*)str);

		gmap = dtx_get_font_glyphmap((dtx_font*)Cur_Font, Cur_Font_Size, code);
		pos += gmap->glyphs[i].advance;
	}
	return pos;
}

int DTX::dtx_char_at_pt(const char *str, float pt)
{
	int i;
	float prev_pos = 0.0f, pos = 0.0f;
	dtx_glyphmap *gmap;

	for(i=0; *str; i++) {
		int code = dtx_utf8_char_code(str);
		str = dtx_utf8_next_char((char*)str);

		gmap = dtx_get_font_glyphmap((dtx_font*)Cur_Font, Cur_Font_Size, code);
		pos += gmap->glyphs[i].advance;

		if(fabs(pt - prev_pos) < fabs(pt - pos)) {
			break;
		}
		prev_pos = pos;
	}
	return i;
}

dtx_glyphmap *DTX::dtx_proc_char(int code, float *xpos, float *ypos)
{
	dtx_glyphmap *gmap;
	gmap = dtx_get_font_glyphmap((dtx_font*)Cur_Font, Cur_Font_Size, code);

	switch(code) {
	case '\n':
		*xpos = 0.0;
		if(gmap) {
			*ypos -= gmap->line_advance;
		}
		return 0;

	case '\t':
		if(gmap) {
			*xpos = (fmod(*xpos, 4) + 4.f) * gmap->glyphs[0].advance;
		}
		return 0;

	case '\r':
		*xpos = 0.0;
		return 0;

	default:
		break;
	}

	if(gmap) {
		*xpos += gmap->glyphs[code - gmap->cstart].advance;
	}
	return gmap;
}

 void calc_best_size(int total_width, int max_glyph_height, int padding, int pow2, int *imgw, int *imgh)
{
	int xsz, ysz, num_rows;
	float aspect;

	for(xsz=2; xsz<=MAX_IMG_WIDTH; xsz *= 2) {
		num_rows = total_width / xsz + 1;

		/* take into account the one extra padding for each row after the first */
		num_rows = (total_width + padding * (num_rows - 1)) / xsz + 1;

		ysz = num_rows * (max_glyph_height + padding) + padding;
		if(pow2) {
			ysz = next_pow2(ysz);
		}
		aspect = (float)xsz / (float)ysz;

		if(aspect >= 1.f) {
			break;
		}
	}

	if(xsz > MAX_IMG_WIDTH) {
		xsz = MAX_IMG_WIDTH;
	}

	*imgw = xsz;
	*imgh = ysz;
}






/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    DRAWGL.CPP    @@@@@@@@@@@@@@@@@@@@@*/
/*
libdrawtext - a simple library for fast text rendering in OpenGL
Copyright (C) 2011-2012  John Tsiombikas <nuclear@member.fsf.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/						
#ifndef NO_OPENGL
#ifdef TARGET_IPHONE
#include <OpenGLES/ES2/gl.h>
#else
#include <GL/glew.h>
#endif





 void cleanup_GL(void);
 void add_glyph(glyph *g, float x, float y);

#define QBUF_SZ		512
// quad *qbuf;
// int num_quads;
// int vattr = -1;
// int tattr = -1;
 unsigned int font_tex;
// int buf_mode = DTX_NBF;


int DTX::dtx_gl_init(void)
{
	if(qbuf) {
		return 0;	/* already initialized */
	}

	glewInit();
/*
	if(!(qbuf = (quad*)malloc(QBUF_SZ * sizeof *qbuf))) {
		return -1;
	}*/
	num_quads = 0;

	atexit(cleanup_GL);
	return 0;
}

 void cleanup_GL(void)
{
//	free(qbuf);
}


void dtx_vertex_attribs(int vert_attr, int tex_attr)
{
	vattr = vert_attr;
	tattr = tex_attr;
}

void DTX::set_glyphmap_texture(dtx_glyphmap *gmap)
{
	if(!gmap->tex) {
		gmap->tex = segl_Gen_Tex();

		segl_Use_Texture( gmap->tex );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

#ifdef GL_ES
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, gmap->xsz, gmap->ysz, 0, GL_ALPHA, GL_UNSIGNED_BYTE, gmap->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
#else
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_ALPHA, gmap->xsz, gmap->ysz, GL_ALPHA, GL_UNSIGNED_BYTE, gmap->pixels);
#endif
	}

	if(font_tex != gmap->tex) {
		dtx_flush();
	}
	font_tex = gmap->tex;
}


void DTX::dtx_glyph(int code)
{
	dtx_glyphmap *gmap;

	if(!Cur_Font || !(gmap = dtx_get_font_glyphmap((dtx_font*)Cur_Font, Cur_Font_Size, code))) {
		return;
	}

	set_glyphmap_texture(gmap);

	add_glyph(gmap->glyphs + code - gmap->cstart, 0, 0);
	dtx_flush();
}

#include "../utility/Draw_Utility.hpp"
void DTX::dtx_string(const char *str, uchar Pos )
{
	dtx_glyphmap *gmap;
	int should_flush = Buf_Mode == DTX_NBF;
	float pos_x = 0.0f;
	float pos_y = 0.0f;

	float width = 64.f;//DEFUALT_FONT_WIDTH;

	if(!Cur_Font) {
		return;
	}

	while(*str)
	{
		float px, py;
		int code = dtx_utf8_char_code(str);
		str = dtx_utf8_next_char((char*)str);

		if(Buf_Mode == DTX_LBF && (code == '\n' || code == '\r') )
			should_flush = 1;

		px = pos_x;
		py = pos_y;

		if( px > width )
			width = px;

		if((gmap = dtx_proc_char(code, &pos_x, &pos_y))) {
			int idx = code - gmap->cstart;

			set_glyphmap_texture(gmap);
			add_glyph(gmap->glyphs + idx, px, py);
		}
	}

	if( should_flush )
	{
		glPushMatrix();

		switch( Pos )
		{
		case SE_Gravity::Mid_Bottom:
//			sePrintf( SEPRINT_DEBUG, "%f-", width );
			glTranslatef( -width / 2.f, 0, 0 );
			break;
		case SE_Gravity::Right_Bottom:
			glTranslatef( -width, 0, 0 );
			break;
		}

//		glTranslatef( 1000, 0, 0 );

		dtx_flush();

		glPopMatrix();
	}
}

 void qvertex(vertex *v, float x, float y, float s, float t)
{
	v->x = x;
	v->y = y;
	v->s = s;
	v->t = t;
}

/* ORIGINAL

 void add_glyph(glyph *g, float x, float y)
{
	quad *qptr = qbuf + num_quads;

	x -= g->orig_x;
	y -= g->orig_y;

	qvertex(qptr->v + 0, x, y, g->nx, g->ny + g->nheight);
	qvertex(qptr->v + 1, x + g->width, y, g->nx + g->nwidth, g->ny + g->nheight);
	qvertex(qptr->v + 2, x + g->width, y + g->height, g->nx + g->nwidth, g->ny);

	qvertex(qptr->v + 3, x, y, g->nx, g->ny + g->nheight);
	qvertex(qptr->v + 4, x + g->width, y + g->height, g->nx + g->nwidth, g->ny);
	qvertex(qptr->v + 5, x, y + g->height, g->nx, g->ny);

	if(++num_quads >= QBUF_SZ) {
		dtx_flush();
	}
}

*/

 void add_glyph(glyph *g, float x, float y)
{
	quad *qptr = qbuf + num_quads;

	x -= g->orig_x;
	y -= g->orig_y;

	qvertex(qptr->v + 0, x, y, g->nx, g->ny + g->nheight);
	qvertex(qptr->v + 1, x + g->width, y, g->nx + g->nwidth, g->ny + g->nheight);
	qvertex(qptr->v + 2, x + g->width, y + g->height, g->nx + g->nwidth, g->ny);

	qvertex(qptr->v + 3, x, y, g->nx, g->ny + g->nheight);
	qvertex(qptr->v + 4, x + g->width, y + g->height, g->nx + g->nwidth, g->ny);
	qvertex(qptr->v + 5, x, y + g->height, g->nx, g->ny);

	if(++num_quads >= QBUF_SZ) {
		dtx_flush();
	}
}

void DTX::dtx_flush(void)
{
	int vbo;	  
	if(!num_quads) {
		return;
	}

	glPushMatrix();
	glScalef( 1, -1, 1 );

	if(glBindBuffer) {
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

#ifndef GL_ES
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
#endif
	//glBindTexture(GL_TEXTURE_2D, font_tex);

	segl_Use_Texture( font_tex );

	if(vattr != -1 && glEnableVertexAttribArray) {
		glEnableVertexAttribArray(vattr);
		glVertexAttribPointer(vattr, 2, GL_FLOAT, 0, sizeof(vertex), qbuf);
#ifndef GL_ES
	} else {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, sizeof(vertex), qbuf);
#endif
	}
	if(tattr != -1 && glEnableVertexAttribArray) {
		glEnableVertexAttribArray(tattr);
		glVertexAttribPointer(tattr, 2, GL_FLOAT, 0, sizeof(vertex), &qbuf->v[0].s);
#ifndef GL_ES
	} else {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(vertex), &qbuf->v[0].s);
#endif
	}

	glEnable(GL_BLEND);
	segl_Set1i( GL_TEXTURE_2D, SEGL_TEXTURE_ALPHA );

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   
	//glColor3f( -1, -1, -1 );
	//glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_DST_ALPHA); 

	glDepthMask(0);

	glDrawArrays(GL_TRIANGLES, 0, num_quads * 6);
	glDepthMask(1);

	if(vattr != -1 && glDisableVertexAttribArray) {
		glDisableVertexAttribArray(vattr);
#ifndef GL_ES
	} else {
		glDisableClientState(GL_VERTEX_ARRAY);
#endif
	}
	if(tattr != -1 && glDisableVertexAttribArray) {
		glDisableVertexAttribArray(tattr);
#ifndef GL_ES
	} else {
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#endif
	}

#ifndef GL_ES
	glPopAttrib();
#else
	glDisable(GL_BLEND);
#endif

	if(glBindBuffer && vbo) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
	}

	num_quads = 0;

	glPopMatrix();
}

#endif	/* !def NO_OPENGL */








/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    UTF8.CPP    @@@@@@@@@@@@@@@@@@@@@*/
/*
libdrawtext - a simple library for fast text rendering in OpenGL
Copyright (C) 2011  John Tsiombikas <nuclear@member.fsf.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#define	U8_IS_FIRST(x)		(((((x) >> 7) & 1) == 0) || ((((x) >> 6) & 3) == 3))

 const char first_mask[] = {
	0,
	0x7f,	/* single byte, 7 bits valid */
	0x1f,	/* two-bytes, 5 bits valid */
	0xf,	/* three-bytes, 4 bits valid */
	0x7		/* four-bytes, 3 bits valid */
};
 const char first_shift[] = { 0, 7, 5, 4, 3 };	/* see above */

#define CONT_PREFIX	0x80
#define CONT_MASK	0x3f
#define CONT_SHIFT	6

/* last charcodes for 1, 2, 3 or 4-byte utf8 chars */
 const int utf8_lastcode[] = { 0x7f, 0x7ff, 0xfff, 0x1fffff };

#define prefix_mask(x)	(~first_mask[x])
#define prefix(x)		((prefix_mask(x) << 1) & 0xff)


char *DTX::dtx_utf8_next_char(char *str)
{
	return str + dtx_utf8_nbytes(str);
}

int DTX::dtx_utf8_char_code(const char *str)
{
	int i, nbytes, shift, code = 0;
	int mask;

	if(!U8_IS_FIRST(*str)) {
		return -1;
	}

	nbytes = dtx_utf8_nbytes(str);
	mask = first_mask[nbytes];
	shift = 0;

	for(i=0; i<nbytes; i++) {
		if(!*str) {
			break;
		}

		code = (code << shift) | (*str++ & mask);
		mask = 0x3f;
		shift = 6;
	}
	return code;
}

int DTX::dtx_utf8_nbytes(const char *str)
{
	int i, numset = 0;
	int c = *str;

	if(!U8_IS_FIRST(c)) {
		for(i=0; !U8_IS_FIRST(str[i]); i++);
		return i;
	}

	/* count the leading 1s */
	for(i=0; i<4; i++) {
		if(((c >> (7 - i)) & 1) == 0) {
			break;
		}
		numset++;
	}

	if(!numset) {
		return 1;
	}
	return numset;
}

int DTX::dtx_utf8_char_count(const char *str)
{
	int n = 0;

	while(*str) {
		n++;
		str = dtx_utf8_next_char((char*)str);
	}
	return n;
}

size_t DTX::dtx_utf8_from_char_code(int code, char *buf)
{
	size_t nbytes = 0;
	uint i;

	for(i=0; i<4; i++) {
		if(code <= utf8_lastcode[i]) {
			nbytes = i + 1;
			break;
		}
	}

	if(!nbytes && buf) {
		for(i=0; i<nbytes; i++) {
			int idx = nbytes - i - 1;
			int mask, shift, prefix;

			if(idx > 0) {
				mask = CONT_MASK;
				shift = CONT_SHIFT;
				prefix = CONT_PREFIX;
			} else {
				mask = first_mask[nbytes];
				shift = first_shift[nbytes];
				prefix = prefix(nbytes);
			}

			buf[idx] = (code & mask) | (prefix & ~mask);
			code >>= shift;
		}
	}
	return nbytes;
}

size_t DTX::dtx_utf8_from_string(const wchar_t *str, char *buf)
{
	size_t nbytes = 0;
	char *ptr = buf;

	while(*str) {
		int cbytes = dtx_utf8_from_char_code(*str++, ptr);
		if(ptr) {
			ptr += cbytes;
		}
		nbytes += cbytes;
	}
	return nbytes;
}			


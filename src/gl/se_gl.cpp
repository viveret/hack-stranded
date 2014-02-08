#include "se_gl.hpp"
#include "OpenGL.hpp"
#include "Shader.hpp"

#include "../math/Math.hpp"
#include "../filesys/Filesystem.hpp"
#include "../module/Module.hpp"
#include "../module/Events.hpp"
#include "../module/Event_Defs.hpp"
#include "../app/Window.hpp"

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

namespace segl
{
	struct Tex_Load_Stats;
	SE_Filesys::File_ID Tex_Load_New();
	uchar Tex_Load_Init( SE_Filesys::File_ID This );
	uchar Tex_Load_Clean( SE_Filesys::File_ID This );

/***************    GLOBALS    *************/
	Vector3f segl_Clear_Screen_Col( 0.1f, 0.1f, 0.1f );

	segl_ID Texture_Current = -1;
	std::vector< segl_ID > Active_Textures;
	std::vector< Tex_Load_Stats* > Active_Texture_Loads;
//	std::vector< Tex_Load_Stats* > Active_Load_Finished; // Done; need to be final proccessed

	SE_Module_ID m_Mod = NULL;
/****************    FUNCTIONS   *************/
	bool IL_Assert( std::string Comment, uchar Level );

	uint File_Name_To_Type( std::string F_Name );// get type from extension name
	GLuint IL_Format_GL( uint IL_Format );

//	void Tex_Load_Part( SE_Raw_Buffer Data, void* This );
//	void Tex_Load_End( Tex_Load_Stats* This );

	uint segl_Initialize( const std::vector< std::string >& mArgs );
	uint segl_Cleanup();
	bool segl_Events( SE_Events::Event_Base* nEv );

}using namespace segl;


struct segl::Tex_Load_Stats: public SE_Filesys::File_Stats
{
	segl_ID m_ID;
//	SE_Filesys::File_ID m_File_ID;
	ILuint IL_ID;

	uchar Type()
	{
		return 0;
	}
};

SE_Filesys::File_ID segl::Tex_Load_New()
{
	return new Tex_Load_Stats;
}

uchar segl::Tex_Load_Init( SE_Filesys::File_ID This_Ptr )
{
	Tex_Load_Stats* This = (Tex_Load_Stats*)This_Ptr;

	glBindTexture( GL_TEXTURE_2D, This->m_ID );
	OpenGL_Assert( FILE_LINE, 1 );

	ilBindImage( This->IL_ID );
 	if( IL_Assert( "bind " + SE_Filesys::Get_FS_Name(This), 1 ) )
	{
		ilBindImage(0);
		ilDeleteImages( 1, &This->IL_ID );
		return 0;
	}

	ilLoadL( File_Name_To_Type(SE_Filesys::Get_FS_Name(This)), SE_Filesys::Get_Data(This).Data(), SE_Filesys::Get_Data(This).Size() );// load from memory
 	if( IL_Assert( "load " + SE_Filesys::Get_FS_Name(This), 1 ) )
	{
		ilBindImage(0);
		ilDeleteImages( 1, &This->IL_ID );
		return 0;
	}


	GLuint Int_Format = IL_Format_GL( ilGetInteger(IL_IMAGE_FORMAT) );
	uint Width = ilGetInteger(IL_IMAGE_WIDTH), Height = ilGetInteger(IL_IMAGE_HEIGHT);

	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);
	if( ImageInfo.Origin == IL_ORIGIN_LOWER_LEFT )
	   iluFlipImage();


	uint Create_Image_Err = 0;

	if( Int_Format == GL_RGB )
		Create_Image_Err = SE_OpenGL::Create_Texture( GL_RGB, GL_RGB8, Width, Height, ilGetData() );
	else
	{
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		Int_Format = GL_RGBA;
		Create_Image_Err = SE_OpenGL::Create_Texture( GL_RGBA, GL_RGBA8, Width, Height, ilGetData() );
	}

	glGenerateMipmap( GL_TEXTURE_2D );  //Generate num_mipmaps number of mipmaps here.
	OpenGL_Assert( FILE_LINE, 1 );
 	if( Create_Image_Err != 0 )
	{
		ilBindImage(0);
		ilDeleteImages(1, &This->IL_ID);
		sePrintf( SEPRINT_ERROR, "Failed\n" );// Log_Type::TYPE_EROR);
		return 0;
	}

	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);*/

 	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );


 	if( OpenGL_Assert( "Error with texture: " + SE_Filesys::Get_FS_Name(This),2 ) )
	{
		ilBindImage(0);
		ilDeleteImages(1, &This->IL_ID);
//		sePrintf( SEPRINT_ERROR, "Failed\n" );
		return 0;
	}

	//loaded successfully

	//std::stringstream Report;Report<<"ID: "<<this->Images.size() << ", GL_ID: " << nID << "\n";
	//Engine.Log( Report.str() );

	ilBindImage(0);
	ilDeleteImages(1, &This->IL_ID);

	sePrintf( SEPRINT_DEBUG, "GL: New tex: %d\n", This->m_ID );

//	Active_Textures.push_back( nID );
//	return this->Add_Tex(nID);//return index of image
//	return Active_Textures.size() - 1;*/

	return 0;
}

uchar segl::Tex_Load_Clean( SE_Filesys::File_ID This )
{
	return 0;
}


segl_ID segl::Texture_Load( std::string File )
{
	OpenGL_Assert( "Before Load", 1 );

	sePrintf( SEPRINT_INFO, "Loading image \'%s\'...\n", File.c_str() );

	Tex_Load_Stats* nTex = (Tex_Load_Stats*)SE_Filesys::Open_For_Read_Whole( File ); //new Tex_Load_Stats;

	if( nTex == NULL ) return 0;

	Active_Texture_Loads.push_back( nTex );

	glGenTextures( 1, &nTex->m_ID );
	if( OpenGL_Assert( "Failed to gen texture", 3 ) )
	{
//		sePrintf( SEPRINT_ERROR, "Failed\n" );
		return 0;
	}

	ilGenImages( 1, &nTex->IL_ID );
	if( IL_Assert( "gen " + File, 1 ) )
	{
		return 0;
	}

//	nTex->m_File_ID = SE_Filesys::Open_For_Read_Whole( File );
	return nTex->m_ID;
}

segl_ID segl::segl_Gen_Tex()
{
	segl_ID nID = 0;
	glGenTextures( 1, &nID );
	if( OpenGL_Assert( "Failed to gen texture", 3 ) )
	{
//		sePrintf( SEPRINT_ERROR, "Failed\n" );
		return 0;
	}

	return nID;
}
/*
void segl::Tex_Load_Part( SE_Raw_Buffer Data, void* This_Ptr )
{
	Tex_Load_Stats* This = (Tex_Load_Stats*)This_Ptr;

	ilBindImage( This->IL_ID );
 	if( IL_Assert( "bind " + SE_Filesys::Get_FS_Name(This), 1 ) )
	{
		ilBindImage(0);
		ilDeleteImages( 1, &This->IL_ID );
		return;
	}

	ilLoadL( File_Name_To_Type(SE_Filesys::Get_FS_Name(This)), Data.Data(), Data.Size() );// load from memory
 	if( IL_Assert( "load " + SE_Filesys::Get_FS_Name(This), 1 ) )
	{
		ilBindImage(0);
		ilDeleteImages( 1, &This->IL_ID );
//		return;
	}

	ilBindImage(0);
}

void segl::Tex_Load_End( Tex_Load_Stats* This )
{
}
*/
void segl::segl_Use_Texture( segl_ID What )
{
	OpenGL_Assert( FILE_LINE, 1 );

//	if( What != Texture_Current && What < Active_Textures.size() )
	{
		Texture_Current = What;
		glBindTexture( GL_TEXTURE_2D, What );
		OpenGL_Assert( FILE_LINE, 1 );

		segl_Enable( SEGL_TEXTURE );
		OpenGL_Assert( FILE_LINE, 1 );
	}
//	else
//		Texture_Current = 0;
}


void segl::segl_Clear_Screen()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
	glClearColor( segl_Clear_Screen_Col.x(), segl_Clear_Screen_Col.y(), segl_Clear_Screen_Col.z(), 1 );
}

void segl::segl_Reshape( uint w, uint h )
{
//		sePrintf( SEPRINT_DEBUG, "Resizing...\n" );
	glViewport( 0, 0, w, h );
}

// Push all opengl settings as a profile (dpeth test, clearcolor, etc )
void segl::segl_Push_Profile()
{

}

// Handle extra OpenGL stuff
void segl::segl_Frame_Start()
{
	// Begin by checking loads
	for( uint i = 0; i < Active_Texture_Loads.size(); i++ )
	{
		if( SE_Filesys::Get_State( Active_Texture_Loads[i] ) == SE_Filesys::File_State::FILE_EOF )
		{
			Tex_Load_Init( Active_Texture_Loads[i] );
			
//			sePrintf( SEPRINT_DEBUG, "Deleting loaded data: %d\n", Active_Texture_Loads[i]->m_ID );

			Active_Textures.push_back( Active_Texture_Loads[i]->m_ID );
			delete Active_Texture_Loads[i];
			Active_Texture_Loads.erase( Active_Texture_Loads.begin() + i );
			i--;
		}
	}

	// Now continue with rendering
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	glOrtho( 0.f, SE_Window::Get_W(), SE_Window::Get_H(), 0, 0, 1.0 );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	segl_Clear_Screen();
}

void segl::segl_Frame_End()
{
	segl_Use_Texture( 1 );

//	glColor3f( 1, 0, 0 );
//	segl_Disable( GL_TEXTURE_2D );
//	
/*	glDisable(GL_DEPTH_TEST);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
*/
/*
	glColor4f( 0, 1, 0, 0.2f );

	glBegin( GL_QUADS );
		glTexCoord2f( 0, 0 );glVertex2f( 0, 0 );
		glTexCoord2f( 1, 0 );glVertex2f( 512, 0 );
		glTexCoord2f( 1, 1 );glVertex2f( 512, 512 );
		glTexCoord2f( 0, 1 );glVertex2f( 0, 512 );
	glEnd();
*/
	glColor4f( 1, 1, 1, 1 );

//    glFlush();
}
/***********   PARAM SET    **********************/
// Each function's parameter corresponds with a data type
#define Cur_Shader segl_Shader::Get_Current()


/***************    ETC OPENGL / DEVIL FUNCS    *******/

uint segl::File_Name_To_Type( std::string F_Name )// get type from extension name
{
	F_Name = F_Name.substr( F_Name.find_last_of('.') + 1, F_Name.length() - F_Name.find_last_of('.') - 1 );// get only extension to speed up proccess

	const char* Ext = F_Name.c_str();

	if( strstr(Ext, "png") )
		return IL_PNG;
	else if( strstr(Ext, "bmp") )
		return IL_BMP;
	else if( strstr(Ext, "jpg") || strstr(Ext, "jpeg") || strstr(Ext, "jpe") )
		return IL_JPG;
	else if( strstr(Ext, "sgi") || strstr(Ext, "bw") || strstr(Ext, "rgb") || strstr(Ext, "rgba") )
		return IL_SGI;
	else if( strstr(Ext, "tga") || strstr(Ext, "vda") || strstr(Ext, "icb") || strstr(Ext, "vst") )
		return IL_TGA;
	else if( strstr(Ext, "raw") )
		return IL_RAW;
	else if( strstr(Ext, "mdl") )
		return IL_MDL;
	else if( strstr(Ext, "dds") )
		return IL_DDS;
	else if( strstr(Ext, "vtf") )
		return IL_VTF;
/*	else if( strstr(Ext, "") )
		return IL_;*/
	else
		return IL_TYPE_UNKNOWN;

/*#define IL_CUT          0x0421  //!< Dr. Halo - .cut extension
#define IL_DOOM         0x0422  //!< DooM walls - no specific extension
#define IL_DOOM_FLAT    0x0423  //!< DooM flats - no specific extension
#define IL_ICO          0x0424  //!< Microsoft Windows Icons and Cursors - .ico and .cur extensions
#define IL_JFIF         0x0425  //!<
#define IL_ILBM         0x0426  //!< Amiga IFF (FORM ILBM) - .iff, .ilbm, .lbm extensions
#define IL_PCD          0x0427  //!< Kodak PhotoCD - .pcd extension
#define IL_PCX          0x0428  //!< ZSoft PCX - .pcx extension
#define IL_PIC          0x0429  //!< PIC - .pic extension
#define IL_PNM          0x042B  //!< Portable Any Map - .pbm, .pgm, .ppm and .pnm extensions
#define IL_TIF          0x042E  //!< Tagged Image File Format - .tif and .tiff extensions
#define IL_CHEAD        0x042F  //!< C-Style Header - .h extension
#define IL_WAL          0x0432  //!< Quake 2 Texture - .wal extension
#define IL_LIF          0x0434  //!< Homeworld Texture - .lif extension
#define IL_MNG          0x0435  //!< Multiple-image Network Graphics - .mng extension
#define IL_JNG          0x0435  //!<
#define IL_GIF          0x0436  //!< Graphics Interchange Format - .gif extension
#define IL_DCX          0x0438  //!< ZSoft Multi-PCX - .dcx extension
#define IL_PSD          0x0439  //!< Adobe PhotoShop - .psd extension
#define IL_EXIF         0x043A  //!<
#define IL_PSP          0x043B  //!< PaintShop Pro - .psp extension
#define IL_PIX          0x043C  //!< PIX - .pix extension
#define IL_PXR          0x043D  //!< Pixar - .pxr extension
#define IL_XPM          0x043E  //!< X Pixel Map - .xpm extension
#define IL_HDR          0x043F  //!< Radiance High Dynamic Range - .hdr extension
#define IL_ICNS			0x0440  //!< Macintosh Icon - .icns extension
#define IL_JP2			0x0441  //!< Jpeg 2000 - .jp2 extension
#define IL_EXR			0x0442  //!< OpenEXR - .exr extension
#define IL_WDP			0x0443  //!< Microsoft HD Photo - .wdp and .hdp extension
#define IL_WBMP			0x0445  //!< Wireless Bitmap - .wbmp extension
#define IL_SUN			0x0446  //!< Sun Raster - .sun, .ras, .rs, .im1, .im8, .im24 and .im32 extensions
#define IL_IFF			0x0447  //!< Interchange File Format - .iff extension
#define IL_TPL			0x0448  //!< Gamecube Texture - .tpl extension
#define IL_FITS			0x0449  //!< Flexible Image Transport System - .fit and .fits extensions
#define IL_DICOM		0x044A  //!< Digital Imaging and Communications in Medicine (DICOM) - .dcm and .dicom extensions
#define IL_IWI			0x044B  //!< Call of Duty Infinity Ward Image - .iwi extension
#define IL_BLP			0x044C  //!< Blizzard Texture Format - .blp extension
#define IL_FTX			0x044D  //!< Heavy Metal: FAKK2 Texture - .ftx extension
#define IL_ROT			0x044E  //!< Homeworld 2 - Relic Texture - .rot extension
#define IL_TEXTURE		0x044F  //!< Medieval II: Total War Texture - .texture extension
#define IL_DPX			0x0450  //!< Digital Picture Exchange - .dpx extension
#define IL_UTX			0x0451  //!< Unreal (and Unreal Tournament) Texture - .utx extension
#define IL_MP3			0x0452  //!< MPEG-1 Audio Layer 3 - .mp3 extension*/
}

GLuint segl::IL_Format_GL( uint IL_Format )
{
	switch( IL_Format )
	{
	case IL_RGB:return GL_RGB;
	case IL_RGBA:return GL_RGBA;
	case IL_BGR:return GL_BGR;
	case IL_BGRA:return GL_BGRA;
	case IL_COLOR_INDEX:return IL_COLOR_INDEX;
	//case IL_LUMINANCE:return GL_LUMINANCE;
	default:return 0;
	}
}

/*************************************   MODULE OPERATIONS   *************************************/

bool segl::segl_Events( SE_Events::Event_Base* nEv )
{
	if( nEv->m_Type == SE_Modules::Mod_Graphics )
	{
		switch( nEv->Code )
		{
		case Event_Graphic::EVENT_RESIZE:
			{
				segl_Reshape( ((SE_Events::Event_Var2<uint,uint>*)nEv)->Argument1, ((SE_Events::Event_Var2<uint,uint>*)nEv)->Argument2 );
			}break;
		}
	}

	return true;
}

uint segl::segl_Initialize( const std::vector< std::string >& mArgs )
{
	ilInit();
	iluInit();
	ilutRenderer( ILUT_OPENGL );

	if( ilGetInteger(IL_VERSION_NUM) < IL_VERSION ||
		iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION ||
		ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION )
	{
		sePrintf( SEPRINT_ERROR, "DevIL version is different\n" );

		segl::segl_Cleanup();
		return SE_FAILURE;
	}


	segl_Shader::Load( "default", true );
	segl_Shader::Load( "mesh" );

	segl_Set1p( SEGL_SHADER, NULL );
	
	Texture_Load( "engine/error.bmp" );

	segl_Enable(GL_TEXTURE_2D);
	segl_Disable(GL_LIGHTING);
#ifdef _TEST
	Engine.Application.OpenGL.GL_Shader->Change(GL_LIGHTING,Vector3(0,128,0));
#endif
	segl_Disable(GL_FOG);

	segl_Set1f(SEGL_GREYSCALE, 100.f);


	return SE_SUCCESS;
}

#include <queue>
bool segl::IL_Assert( std::string Comment, uchar Level )
{
	bool Ret = false;
	ILenum Error_Now = ilGetError();// first error, latest error

	std::queue< S_Error > Reverse_Stack;

	ILenum Error;
	while((Error = ilGetError()) != IL_NO_ERROR)
	{// + To_Str(Error)
		std::string L_Info = "DevIL " + Comment + ": " + To_Str(Error, 16) + ", " + std::string( iluErrorString(Error) );
		Reverse_Stack.push( S_Error(L_Info, 2) );
		Ret = true;
	}

	while( Reverse_Stack.size() > 0 )
	{
		sePrintf( SEPRINT_ERROR, Reverse_Stack.front().Message.c_str() );
		Reverse_Stack.pop();
	}

	if( Error_Now != IL_NO_ERROR )
	{
		Comment = "DevIL " + Comment + ": " + To_Str(Error_Now, 16) + ", " + std::string( iluErrorString(Error_Now) ) + "\n";
		sePrintf( SEPRINT_ERROR, Comment.c_str() );

		Ret = true;
	}

//#ifdef SE_USE_ASSERT
	return Ret;
//#else
//	return false;
//#endif
}

uint segl::segl_Cleanup()
{
	return true;
}

void segl::Register_Module()
{
	m_Mod = S_Engine::Generate_Module( "segl" );
	S_Engine::Register_Module_Render( m_Mod, S_Engine::Reg_Mod_Time::LOW_LEVEL_NEXT, segl_Initialize, segl_Cleanup );

	S_Engine::Register_Module_Event_Render( m_Mod, segl_Events );
	S_Engine::Register_Module_Thread_Render( segl_Frame_Start, S_Engine::Module_Positions::POS_START, 254 );	
	S_Engine::Register_Module_Thread_Render( segl_Frame_End, S_Engine::Module_Positions::POS_END, 254 );

	// Register file types
	SE_Filesys::File_Type_Desc Tex_File_Type;
	Tex_File_Type.Name = "texture";
	Tex_File_Type.Extension.push_back( "bmp" );
	Tex_File_Type.Extension.push_back( "png" );

	Tex_File_Type.Func_New = Tex_Load_New;
//	Tex_File_Type.Func_Init = Tex_Load_Init; // We initialize our own
	Tex_File_Type.Func_Clean = Tex_Load_Clean;

	SE_Filesys::Register_File_Type( Tex_File_Type );
}

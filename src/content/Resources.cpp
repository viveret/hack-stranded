#include "Resources.hpp"
using namespace E_Resources;

#include "../engine/Engine.hpp"

#include "File_Types.hpp"
using namespace SE_Content_File_Types;

#include "../utility/Draw_Utility.hpp"

#define LOAD_TEX_BMP(X) Engine.Resources.Load_Texture_BMP(X)
#define LOAD_TEX_PNG(X) Engine.Resources.Load_Texture_PNG(X)

#define VF_RES(X) Engine.Content.Get_Res(X)

#include <fstream>


GL_ID_Pair::GL_ID_Pair()
{
	this->m_ID = 0;
	this->GL_ID = 0;
}

GL_Prerender_Hint::GL_Prerender_Hint()
{
	this->m_ID = NULL;
	this->w = 0;this->h = 0;
	this->Data = NULL;
	this->Prerender_Func = NULL;
}

R_Res_Req::R_Res_Req()
{
	this->Error = 0;
	this->Text = 0;

	// GUI
	this->GUI_Background = 0;
	//screen tops
	this->GUI_Splash = 0;
	//sounds
	this->GUI_Click = 0;
	//the rest
	this->GUI_Buttons = 0;
	this->GUI_Scroll_Buttons = 0;
}

void R_Res_Req::Get_Resources()// get from virtual folder
{
	// GUI
	//this->GUI_Background = VF_RES( "screen/gui/background.bmp" );
	//sounds
	this->GUI_Click = *Type_Sound::Get_From_VFS( "screen/gui/mouse_click.wav" );
	//the rest
	this->GUI_Buttons = *Type_Sound::Get_From_VFS( "screen/gui/buttons.png" );
	this->GUI_Scroll_Buttons = 0;
}



R_Resources::R_Resources()
{
	this->Loaded_Necesities=false;
	this->Load_Fast = false;

	this->Current_Texture=0;
}

R_Resources::~R_Resources()
{
	this->Cleanup();
}

bool R_Resources::Initialize()
{
	ilInit();
	iluInit();
	ilutRenderer( ILUT_OPENGL );

	if( ilGetInteger(IL_VERSION_NUM) < IL_VERSION ||
		iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION ||
		ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION )
	{
#ifdef _TEST
		Engine.Register_Error(S_Error( "DevIL version is different", 3 ));
#endif

		this->Cleanup();
		return false;
	}

	return true;
}

void R_Resources::Cleanup()
{
#ifdef _TEST
	if( Engine.Is_Running == true ) return;// only at end of program
#endif

//	this->Font_Manager.Delete();
}

void R_Resources::Update()
{
	while( this->Prerender_Hints.size() > 0 )
	{
		if( this->Prerender_Hints[0].w != 0 && this->Prerender_Hints[0].h != 0 && this->Prerender_Hints[0].Prerender_Func != NULL && this->Prerender_Hints[0].m_ID != NULL )
		{
			if( this->Get_GL( *this->Prerender_Hints[0].m_ID ) == 0 || *this->Prerender_Hints[0].m_ID == 0 )// not there or new tex
			{
				*this->Prerender_Hints[0].m_ID = this->New_Tex();
			}
#ifdef _TEST
			Engine.Application.OpenGL.GL_RTT->Start_Render( this->Prerender_Hints[0].w, this->Prerender_Hints[0].h, *this->Prerender_Hints[0].m_ID );
			this->Prerender_Hints[0].Prerender_Func( this->Prerender_Hints[0].Data );
			*this->Prerender_Hints[0].m_ID = Engine.Application.OpenGL.GL_RTT->End_Render();
#endif
		}

		this->Prerender_Hints.erase( this->Prerender_Hints.begin() );
	}
}

id R_Resources::Load_Texture(std::string Filename)
{
/*	if( Filename.find(".bmp") != Filename.npos )
		return this->Load_Texture_BMP( Filename );
	else*/
		return this->Load_Texture_PNG( Filename );
}

id R_Resources::Load_Texture_BMP(std::string Filename)//draw splash screen too
{
/*	if(this->Loaded_Necesities)
	{
		Draw_Utility::Splash("Loading Image: "+Filename);
	}

	Bitmap* nImage = new Bitmap();
#ifdef _TEST
	Engine.Mem_Add( nImage, "new bitmap" );
#endif

	if(nImage->loadBMP(Filename.c_str()))
	{
		GLuint nID;

		glGenTextures(1,&nID);
#ifdef _TEST
		if( Engine.OpenGL_Assert( "Failed to gen texture", 3 ) )
		{
			Engine.Log("Failed\n", Log_Type::TYPE_EROR);
			return 0;
		}
#endif
		glBindTexture(GL_TEXTURE_2D,nID);

		// select modulate to mix texture with color for shading
		//glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );


		gluBuild2DMipmaps(GL_TEXTURE_2D,3,nImage->width,nImage->height,GL_RGB,GL_UNSIGNED_BYTE,nImage->data);
#ifdef _TEST
		if( Engine.OpenGL_Assert( "Failed to build mipmaps", 2 ) )
		{
			Engine.Log("Failed\n", Log_Type::TYPE_EROR);
			return 0;
		}
		//loaded successfully

		//this->Images.push_back(nID);//add to list of images
		std::stringstream Report;Report<<"ID: "<<this->Images.size()-1 << ", GL_ID: " << nID << "\n";
		//Engine.Log( Report.str() );

		Engine.Mem_Remove( nImage );
#endif
		delete nImage;

		return this->Add_Tex(nID);//return index of image
	}
	else
	{
#ifdef _TEST
		Engine.Log("File "+Filename+" Had Error "+nImage->error, Log_Type::TYPE_EROR);
		Engine.Mem_Remove( nImage );
#endif
		delete nImage;
		return 0;
	}*/
	return 0;
}

id R_Resources::Load_Texture_PNG(std::string Filename)//draw splash screen too
{
	//Engine.Log("Loading Image: " + Filename + "... ");

	if(this->Loaded_Necesities)
		Draw_Utility::Splash("Loading Image: "+Filename);


	SE_Raw_Buffer Data;
#ifdef _TEST
	= Engine.Application.Input.Load_File( Filename, true );
#endif

	if( Data.Data() == NULL )
	{
		//Engine.Log("Failed\n");
		return 0;
	}

	ILuint IL_ID;
	ilGenImages( 1, &IL_ID );
#ifdef _TEST
	if( Engine.Assert_IL( "gen \"" + Filename, 1 ) )
	{
		Data.Delete();
		return 0;
	}

	ilBindImage( IL_ID );
	if( Engine.Assert_IL( "bind \"" + Filename, 1 ) )
	{
		ilBindImage(0);
		ilDeleteImages( 1, &IL_ID );
		Data.Delete();
		return 0;
	}

	ilLoadL( this->Texture_Type(Filename), Data.Get_Data(), Data.Get_Size() );// load from memory
	if( Engine.Assert_IL( "load \"" + Filename, 1 ) )
	{
		ilBindImage(0);
		ilDeleteImages( 1, &IL_ID );
		Data.Delete();
		return 0;
	}

	GLuint nID;

	glGenTextures(1,&nID);
	if( Engine.OpenGL_Assert( "Failed to gen texture", 3 ) )
	{
		ilBindImage(0);
		ilDeleteImages(1, &IL_ID);
		Data.Delete();
		Engine.Log("Failed\n", Log_Type::TYPE_EROR);
		return 0;
	}

	glBindTexture(GL_TEXTURE_2D,nID);

	GLuint Int_Format = this->IL_Format_GL(ilGetInteger(IL_IMAGE_FORMAT));
	uint Width = ilGetInteger(IL_IMAGE_WIDTH), Height = ilGetInteger(IL_IMAGE_HEIGHT);

	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);
	if( ImageInfo.Origin == IL_ORIGIN_LOWER_LEFT )
	   iluFlipImage();


	uint Create_Image_Err = 0;

	if( Int_Format == GL_RGB )
		Create_Image_Err = sgl.glCreate_Image( GL_RGB, GL_RGB8, Width, Height, ilGetData() );
	else
	{
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		Int_Format = GL_RGBA;
		Create_Image_Err = sgl.glCreate_Image( GL_RGBA, GL_RGBA8, Width, Height, ilGetData() );
	}

	if( Engine.OpenGL_Assert( "Failed to tex storage", 2 ) )
	{
		ilBindImage(0);
		ilDeleteImages(1, &IL_ID);
		Data.Delete();
		Engine.Log("Failed\n", Log_Type::TYPE_EROR);
		return 0;
	}

	glGenerateMipmap(GL_TEXTURE_2D);  //Generate num_mipmaps number of mipmaps here.
	if( Create_Image_Err != 0 )
	{
		ilBindImage(0);
		ilDeleteImages(1, &IL_ID);
		Data.Delete();
		Engine.Log("Failed\n", Log_Type::TYPE_EROR);
		return 0;
	}

	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);*/

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );


	if( Engine.OpenGL_Assert( "Error with texture: "+Filename,2 ) )
	{
		ilBindImage(0);
		ilDeleteImages(1, &IL_ID);
		Data.Delete();
		Engine.Log("Failed\n");
		return 0;
	}

	//loaded successfully

	//std::stringstream Report;Report<<"ID: "<<this->Images.size() << ", GL_ID: " << nID << "\n";
	//Engine.Log( Report.str() );

	ilBindImage(0);
	ilDeleteImages(1, &IL_ID);
	Data.Delete();
	return this->Add_Tex(nID);//return index of image
#endif

	return 0;
}

uint R_Resources::Texture_Type( std::string F_Name )// get type from extension name
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

GLuint R_Resources::IL_Format_GL( uint IL_Format )
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

void R_Resources::Use_Texture(id ID)
{
	segl_Enable(GL_TEXTURE_2D);
	//glActiveTexture( GL_TEXTURE0 );

	if( this->Current_Texture != ID )//if it is different
	{
		if( ID < this->Images.size() )//within range
		{
			this->Current_Texture = ID;
			glBindTexture( GL_TEXTURE_2D, this->Get_GL(ID) );
		}
		else
			//failed to load
			this->Current_Texture=0;
	}
}

GLuint R_Resources::Get_GL(id ID)
{
	if( ID < this->Images.size() )
	{
		for( uint i = 0;i < this->Images.size(); i++ )
			if( this->Images[i].m_ID == ID )
				return this->Images[ID].GL_ID;
	}

	return 0;
}

id R_Resources::New_Tex()
{
	GLuint n_Tex = 0;
	glGenTextures( 1, &n_Tex );

	return this->Add_Tex(n_Tex);
}

id R_Resources::Add_Tex(GLuint ID)
{
	for( uint i = 0;i < this->Images.size(); i++ )
		if( this->Images[i].GL_ID == ID )
			return this->Images[i].m_ID;

	// not found, is new
	GL_ID_Pair nPair;nPair.m_ID = this->Get_New_ID();nPair.GL_ID =  ID;
	this->Images.push_back( nPair );
	this->Use_Texture( nPair.m_ID );
	return this->Current_Texture;
}

void R_Resources::Hint_Prerender( GL_Prerender_Hint& nHint )
{
	for( uint i = 0;i < this->Prerender_Hints.size(); i++ )
	{
		if( nHint.m_ID == this->Prerender_Hints[i].m_ID )// same prerender hint
			return;
	}

	this->Prerender_Hints.push_back( nHint );
}

id R_Resources::Get_New_ID()
{
	id nID = 0;

	for( uint i = 0;i < this->Images.size(); i++ )
		if( this->Images[i].m_ID == nID )
			nID++;// next id

	return nID;
}


#ifdef SE_OLD_PNG_LOADER
id R_Resources::Load_Texture_PNG(std::string Filename)//draw splash screen too
{
	//Engine.Log("Loading Image: " + Filename + "... ");

	if(this->Loaded_Necesities)
		Draw_Utility::Splash("Loading Image: "+Filename);

	//Engine.Opengl_Error(S_Error("Before loading PNG texture: "+Filename,4));


	/*uint Flags = SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB;

	if(this->Load_Fast == false)
		Flags = Flags | SOIL_FLAG_COMPRESS_TO_DXT;*/

	E_Application::Raw_Buffer Data = Engine.Application.Input.Load_File( Filename, true );


	if( Data.Get_Data() == NULL )
	{
		Engine.Log("Failed\n");
		return 0;
	}

	GLuint nID = 0;
//#ifdef _WIN32
			/*SOIL_load_OGL_texture_from_memory
	(
		(const u_char*)Data.Get_Data(),
		Data.Get_Size(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		Flags
    );	*/

	Data.Delete();

	/*GLuint nID=SOIL_load_OGL_texture
    (
      Filename.c_str(),
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
	  Flags
    );*/

	if( Engine.OpenGL_Assert( "Error with PNG texture: "+Filename,2 ) )
	{
		Engine.Log("Failed\n");
		return 0;
	}

	//loaded successfully

	std::stringstream Report;Report<<"ID: "<<this->Images.size() << ", GL_ID: " << nID << "\n";
	//Engine.Log( Report.str() );

	//this->Current_Texture = this->Images.size();

	return this->Add_Tex(nID);//return index of image
}
#endif

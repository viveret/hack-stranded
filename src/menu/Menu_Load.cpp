#include "Menu.hpp"
using namespace SE_Menu;

#include "Menu_Buttons.hpp"
//using namespace Menu_Buttons;
//using namespace Button_Var;
							  
#include "../S_Engine.hpp"


		//Menu_Base* Load_Menuinternal( std::string Which ); // load from internal








/*void Menu_Manager::Load_Ext_Menu_Cont()
{		
	if( this->Changing_Menu == false )// not actually changing, abort
		return;


	this->Wait_For_Threads( true );// are paused

	Engine.Application.Error_Log.Write_Log("Menu Ext Before");
	if(this->Cur_Menu)
	{
		this->Cur_Menu->Clean_Up();
		Engine.Mem_Remove( this->Cur_Menu );
		delete this->Cur_Menu;
		this->Cur_Menu=NULL;
	}	   
	Engine.Application.Error_Log.Write_Log("Menu Ext: Cur_Menu");
	
	//try to load file	  

	//this->Cur_Menu = this->Load_Ext_Non_Compiled(this->Menu_Ext_Trace[this->Menu_Ext_Trace.size()-1]);




	Menu_Base* nMenu = new Menu_Base;
	Engine.Mem_Add( nMenu, "new menu" );						 

	Engine.Application.Error_Log.Write_Log("Menu Ext: new menu");
	assert( nMenu != NULL );

	// load from virtual filesystem

	E_Application::Raw_Buffer* nBuf = NULL;//Engine.Content.Get_Raw( "screen/menus/" + this->Menu_Ext_Trace[this->Menu_Ext_Trace.size()-1] );
	Engine.Application.Error_Log.Write_Log("Menu Ext: Get Raw");	
	//assert( nBuf != NULL );

	if( nBuf && nBuf->Get_Data() )//char* Data = ((E_Content::C_Filesystem::F_Binary*)Engine.Content.Virtual_Folder.Get_Child( "screen/menus/" + this->Menu_Ext_Trace[this->Menu_Ext_Trace.size()-1] ))->Buffer.my_Buf;//Engine.Application.Input.Load_File("stuff/screen/menus/"+this->Menu_Ext_Trace[this->Menu_Ext_Trace.size()-1]);
	{
		char* Data = nBuf->Get_Data();
		assert( Data != NULL );

		char Buffer[64];memset(Buffer,0,64);
		strcpy(Buffer,Data);//get title						 
		Engine.Application.Error_Log.Write_Log("Menu Ext: title");	

		nMenu->Title = Buffer;
		memset(Buffer,0,64);

		uint Buf_At = nMenu->Title.length()+1;
		nMenu->Make_Buttons( Data[Buf_At] );//number is after title
		Engine.Application.Error_Log.Write_Log("Menu Ext: make buttons");

		Buf_At++;

		for(u_char l=0;l<nMenu->Buttons.Get_Size();l++)
		{
			u_char Cur_But_Type = Data[Buf_At];	Buf_At++;

			switch( Cur_But_Type )//type
			{
			case BUTTON_INT_MENU:
				nMenu->Buttons[l] = new Button_Menu;
				break;
			default:
				nMenu->Buttons[l] = new Button_Base;
				Cur_But_Type = BUTTON_INT_MENU;
				assert( false );
				break;
			}
			Engine.Mem_Add( nMenu->Buttons[l], "button" );
			Engine.Application.Error_Log.Write_Log("Menu Ext: new button");
															  
			memcpy(&nMenu->Buttons[l]->x,&Data[Buf_At],4);Buf_At+=4;
			memcpy(&nMenu->Buttons[l]->y,&Data[Buf_At],4);Buf_At+=4;

			Engine.Application.Error_Log.Write_Log("Menu Ext: memcpy pos");

			switch(Cur_But_Type)
			{		   		 /*
			case BUTTON_EXT_MENU://what to show
				{
					strcpy(Buffer,&Data[Buf_At]);
					((Button_Ext_Menu*)nMenu->Buttons[l])->Show = Buffer;
					Buf_At+=strlen(Buffer)+1;memset(Buffer,0,64);
																 
					strcpy(Buffer,&Data[Buf_At]);
					((Button_Ext_Menu*)nMenu->Buttons[l])->Go_To = Buffer;  
					Buf_At+=strlen(Buffer)+1;memset(Buffer,0,64);
				}break;
							   */
/*			case BUTTON_INT_MENU://what to show
				{								   
					strcpy(Buffer,&Data[Buf_At]);
					((Button_Menu*)nMenu->Buttons[l])->Show=Buffer;	   
					Buf_At+=strlen(Buffer)+1;memset(Buffer,0,64);
													   
					memcpy(&((Button_Menu*)nMenu->Buttons[l])->Go_To , &Data[Buf_At],4);Buf_At+=4;
				}break;
			}
			Engine.Application.Error_Log.Write_Log("Menu Ext: memcpy butt data");
		}									   

		//Engine.Mem_Remove( nMenu );
		End:

		this->Cur_Menu = nMenu;		 
	}

	Engine.Application.Error_Log.Write_Log("Menu Ext After");

			
	this->Changing_Menu = false;
	Wait_For_Threads( false );// they are unpaused

	
	if( this->Cur_Menu == NULL )// failed to load menu
	{
		this->Cleanup();
		this->Load_Int_Menu( MENU_TITLE );
	}
}		*/

/*Menu_Base* Menu_Manager::Load_Ext_Non_Compiled(std::string Which)
{																				   
	//this->Compile_Menu_Ext(Which,"1_"+Which);
	std::string Data=Engine.Application.Input.Load_File_Str("stuff/screen/menus/"+Which); 
	
	if(Data.length()==0)
	{
		Engine.Register_Error(S_Error("Cannot load menu, no data",1));
		return NULL;
	}
		   
	Menu_Base* Ret = new Menu_Base;
	Engine.Mem_Add( Ret, "new menu" );


	uint Line_At=0;//to tell what info it is reading

	char Buffer[64];memset(Buffer,0,64);
	char Buf_At=0;
	bool Is_Data=false;//if we want to use it

	uint But_At=0;
	uint Cur_But_Type=0;

	for(uint l=0;l<Data.size();l++)
	{		
		if(Data[l]!=':' && Data[l]!='\n' && Data[l]!=13 && Is_Data && Buf_At<63)
		{
			Buffer[Buf_At]=Data[l];
			Buf_At++;
		}
		else if(Data[l]==':')
			Is_Data=true;//we want to use it

		else if(Data[l]=='\n' || l==Data.length()-1)//done with that data
		{
			if(Buf_At>0)
			{									   
				if(Line_At==0)//title of menu
					Ret->Title=std::string(Buffer);


				if(Line_At==1)//number of buttons
					Ret->Make_Buttons(atoi(Buffer));	   


				else if(Line_At==2)//button type
				{
					std::string Type(Buffer);		   
					if(Type=="menu_ext")
						Cur_But_Type=BUTTON_EXT_MENU;
					else if(Type=="menuint")
						Cur_But_Type=BUTTON_INT_MENU;

					switch(Cur_But_Type)
					{					  
						case BUTTON_INT_MENU:
							Ret->Buttons[But_At] = new Button_Menu; 
							break;
						default:
							Ret->Buttons[But_At] = new Button_Base;
							break;
					}
					Engine.Mem_Add( Ret->Buttons[But_At], "button" );
				}
															   
				else if(Line_At==3)//position x
					Ret->Buttons[But_At]->x=atoi(Buffer);

				else if(Line_At==4)//position y
					Ret->Buttons[But_At]->y=atoi(Buffer);


				else if(Line_At==5)
				{
					switch(Cur_But_Type)
					{		   
					case BUTTON_INT_MENU://what to show
						((Button_Menu*)Ret->Buttons[But_At])->Show=Buffer;
						break;
					}
				}
					

				else if(Line_At==6)	  
				{
					switch(Cur_But_Type)
					{			  
					case BUTTON_EXT_MENU://what to goto
						((Button_Menu*)Ret->Buttons[But_At])->Go_To=Buffer;
						break;
					/*case BUTTON_INT_MENU://what to goto
						{
							uint Goto=0;//menu back
							std::string nGoto(Buffer);

							Goto = this->Menu_String_TO_Int( nGoto );
												
						((Button_Int_Menu*)Ret->Buttons[But_At])->Go_To=Goto;
						}break;*/
/*					}
				}

				//done
				memset(Buffer,0,64);
				Buf_At=0;	   
				Is_Data=false;
				Line_At++;

				if(Line_At==7)//reset tex counter
				{
					Line_At=2;
					But_At++;
					Cur_But_Type=0;

					if( But_At == Ret->Buttons.Get_Size() )
						break;
				}
			}
		}
	}

	return Ret;
}	  */

/*void Menu_Manager::Compile_Menu_Ext(std::string Source,std::string Dest)
{
	std::ofstream Writer(("stuff/screen/menus/"+Dest).c_str(),std::ios::binary);

	std::string Data = Engine.Application.Input.Load_File_Str("stuff/screen/menus/"+Source);

	uint Line_At=0;//to tell what info it is reading

	char Buffer[64];memset(Buffer,0,64);
	char Buf_At=0;
	bool Is_Data=false;//if we want to use it

	uint But_At=0;
	uint Cur_But_Type=0;
	u_char Num_Buts=0;

	for(uint l=0;l<Data.size();l++)
	{		
		if(Data[l]!=':' && Data[l]!='\n' && Data[l]!=13 && Is_Data && Buf_At<63)
		{
			Buffer[Buf_At]=Data[l];
			Buf_At++;
		}
		else if(Data[l]==':')
			Is_Data=true;//we want to use it

		else if(Data[l]=='\n' || l==Data.length()-1)//done with that data
		{
			if(Buf_At>0)
			{									   
				if(Line_At==0)//title of menu
					Writer.write(Buffer,strlen(Buffer)+1);


				if(Line_At==1)//number of buttons
				{
					Num_Buts = atoi(Buffer);
					Writer.write((const char*)&Num_Buts,1);	   
				}


				else if(Line_At==2)//button type
				{
					std::string Type(Buffer);		   
					if(Type=="menu_ext")
						Cur_But_Type=BUTTON_EXT_MENU;
					else if(Type=="menuint")
						Cur_But_Type=BUTTON_INT_MENU;

					Writer.write((const char*)&Cur_But_Type,1);
				}
															   
				else if(Line_At==3)//position x
				{				  
					int Amount = atoi(Buffer);
					Writer.write((const char*)&Amount,4);
				}

				else if(Line_At==4)//position y				  
				{				  
					int Amount = atoi(Buffer);
					Writer.write((const char*)&Amount,4);
				}


				else if(Line_At==5)
				{
					switch(Cur_But_Type)
					{		   
					case BUTTON_EXT_MENU://what to show
						Writer.write(Buffer,strlen(Buffer)+1);
						break;

					case BUTTON_INT_MENU://what to show
						Writer.write(Buffer,strlen(Buffer)+1);
						break;
					}
				}
					

				else if(Line_At==6)	  
				{
					switch(Cur_But_Type)
					{			  
					case BUTTON_EXT_MENU://what to goto
						Writer.write(Buffer,strlen(Buffer)+1);
						break;
					case BUTTON_INT_MENU://what to goto
						{
							uint Goto=0;//menu back
							std::string nGoto(Buffer);

							Goto = this->Menu_String_TO_Int( nGoto );

							Writer.write((const char*)&Goto,4);
						}break;
					}
				}

				//done
				memset(Buffer,0,64);
				Buf_At=0;	   
				Is_Data=false;
				Line_At++;

				if(Line_At==7)//reset tex counter
				{
					Line_At=2;
					But_At++;
					Cur_But_Type=0;

					if(But_At==Num_Buts)
						break;
				}
			}
		}
	}
}	   */

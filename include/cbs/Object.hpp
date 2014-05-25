#ifndef SURVIVOR_ENGINE_CBS_OBJECT
#define SURVIVOR_ENGINE_CBS_OBJECT



namespace SECBS_Object
{
	struct Object;
	typedef Object* Obj_ID;


	void* Get_Attr( Obj_ID This, std::string Name ); // Get attribute

	std::string& Get_Str();
	uint& Get_uint();
	float& Get_Float();
}



#endif

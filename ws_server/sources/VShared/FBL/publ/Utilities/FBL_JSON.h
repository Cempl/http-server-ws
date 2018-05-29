/**********************************************************************************************/
/* FBL_JSON.h																				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#ifndef _FBL_JSON_h
	#define _FBL_JSON_h
#pragma once

// FBL:
#include <VShared/FBL/publ/Headers/FBL.h>
#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>

// STD:
#include <stdint.h>

// FINAL:
#include <VShared/FBL/publ/Headers/FBL_pre_header.h>

/**********************************************************************************************/
struct cJSON;


/**********************************************************************************************/
// See
// https://github.com/vivkin/gason
//

/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
#define JSON_ZONE_SIZE 4096
#define JSON_STACK_SIZE 32


/**********************************************************************************************/
struct FBL_SHARED_EXP_CLASS JsonAllocator
{
	public://///////////////////////////////////////////////////////////////////////////////////

		struct Zone
		{
			Zone*	next;
			char*	end;
		};


	public://///////////////////////////////////////////////////////////////////////////////////

#if FBL_WIN
								JsonAllocator( void )
								:
									head(nullptr)
								{
								}
#endif // FBL_WIN
								~JsonAllocator();
	
		void*					allocate( size_t n, size_t align = 8 );


	public://///////////////////////////////////////////////////////////////////////////////////

#if FBL_WIN
		Zone*					head;
#else
		Zone*					head = nullptr;
#endif // FBL_WIN
};


/**********************************************************************************************/
#define JSON_VALUE_PAYLOAD_MASK		0x00007FFFFFFFFFFFULL
#define JSON_VALUE_NAN_MASK			0x7FF8000000000000ULL
#define JSON_VALUE_NULL				0x7FFF800000000000ULL
#define JSON_VALUE_TAG_MASK			0xF
#define JSON_VALUE_TAG_SHIFT		47


/**********************************************************************************************/
enum JsonTag
{
	JSON_TAG_NUMBER,
	JSON_TAG_STRING,
	JSON_TAG_BOOL,
	JSON_TAG_ARRAY,
	JSON_TAG_OBJECT,
	JSON_TAG_NULL = 0xF
};


/**********************************************************************************************/
struct JsonNode;
struct FBL_SHARED_EXP_CLASS JsonValue
{
	public://///////////////////////////////////////////////////////////////////////////////////

								JsonValue()
								{
									data.i = JSON_VALUE_NULL;
								}
				
								JsonValue( JsonTag tag, void* p )
								{
									uint64_t x = (uint64_t)p;
									FBL_CHECK( (int)tag <= JSON_VALUE_TAG_MASK );
									FBL_CHECK( x <= JSON_VALUE_PAYLOAD_MASK );
									data.i = JSON_VALUE_NAN_MASK
												| ((uint64_t)tag << JSON_VALUE_TAG_SHIFT) | x;
								}
				
explicit						JsonValue( double x )
								{
									data.f = x;
								}


	public://///////////////////////////////////////////////////////////////////////////////////

		bool					isDouble() const
								{
									return (int64_t)data.i <= (int64_t)JSON_VALUE_NAN_MASK;
								}
	
		JsonTag					getTag() const
								{
									return isDouble() ?
												JSON_TAG_NUMBER :
												JsonTag(
															(data.i >> JSON_VALUE_TAG_SHIFT)
														&	JSON_VALUE_TAG_MASK );
								}
	
		uint64_t				getPayload() const
								{
									FBL_CHECK( !isDouble() );
									return data.i & JSON_VALUE_PAYLOAD_MASK;
								}
	
		double					toNumber() const
								{
									FBL_CHECK( getTag() == JSON_TAG_NUMBER );
									return data.f;
								}
	
		bool					toBool() const
								{
									FBL_CHECK( getTag() == JSON_TAG_BOOL );
									return (bool)getPayload();
								}
	
		char*					toString() const
								{
									FBL_CHECK( getTag() == JSON_TAG_STRING );
									return (char*)getPayload();
								}
	
		JsonNode*				toNode() const
								{
									FBL_CHECK(		getTag() == JSON_TAG_ARRAY
												||	getTag() == JSON_TAG_OBJECT );
												
									return (JsonNode*)getPayload();
								}
				
	public://///////////////////////////////////////////////////////////////////////////////////

		union
		{
			uint64_t	i;
			double		f;
		} data;
				
};


/**********************************************************************************************/
struct JsonNode
{
	JsonValue	value;
	JsonNode*	next;
	char*		key;
};


/**********************************************************************************************/
struct JsonIterator
{
	public://///////////////////////////////////////////////////////////////////////////////////

#if FBL_WIN
								JsonIterator( JsonNode* inpNode )
								: 
									p(inpNode)
								{
								}
#endif // FBL_WIN

		void					operator++()
									{ p = p->next; }
	
		bool					operator!=( const JsonIterator &x ) const
									{ return p != x.p; }
	
		JsonNode*				operator*() const
									{ return p; }
		
		JsonNode*				operator->() const
									{ return p; }
				
	public://///////////////////////////////////////////////////////////////////////////////////

		JsonNode*				p;
				
};


/**********************************************************************************************/
inline JsonIterator begin( JsonValue o )
{
#if FBL_WIN
	return JsonIterator( o.toNode() );
#else
	return JsonIterator{ o.toNode() };
#endif // FBL_WIN
}


/**********************************************************************************************/
inline JsonIterator end( JsonValue )
{
#if FBL_WIN
	return JsonIterator( nullptr );
#else
	return JsonIterator{ nullptr };
#endif // FBL_WIN
}


/**********************************************************************************************/
enum JsonParseStatus
{
	JSON_PARSE_OK,
	JSON_PARSE_BAD_NUMBER,
	JSON_PARSE_BAD_STRING,
	JSON_PARSE_BAD_IDENTIFIER,
	JSON_PARSE_STACK_OVERFLOW,
	JSON_PARSE_STACK_UNDERFLOW,
	JSON_PARSE_MISMATCH_BRACKET,
	JSON_PARSE_UNEXPECTED_CHARACTER,
	JSON_PARSE_UNQUOTED_KEY,
	JSON_PARSE_BREAKING_BAD
};


/**********************************************************************************************/
FBL_SHARED_EXP JsonParseStatus json_parse(
	char*			str,
	char**			endptr,
	JsonValue*		value,
	JsonAllocator&	allocator);


/**********************************************************************************************/
FBL_SHARED_EXP bool GetSimpleValue( JsonValue inSource, const String& inKey, String& outValue );


#pragma mark -

/**********************************************************************************************/
// Another JSON parser (simple + UTF8 support)
//

/**********************************************************************************************/
struct FBL_SHARED_EXP_CLASS Json
{
	public://///////////////////////////////////////////////////////////////////////////////////

								Json( const char* inJsonStr );
	
virtual							~Json();

	public://///////////////////////////////////////////////////////////////////////////////////
	
		bool						isValid( void );
		
		String						get_ErrorStr( void );

		String						get_Value( const char* inpKey );

		ArrayOfValueArrays_Ptr		get_Matrix( const char* inpKey );
	

	protected://////////////////////////////////////////////////////////////////////////////////

		ArrayOfValues_Ptr			get_Array( cJSON* inpArray );
		

	protected://////////////////////////////////////////////////////////////////////////////////
	
		cJSON*					mpRoot;
};


/**********************************************************************************************/
FBL_End_Namespace

#endif // _FBL_JSON_h

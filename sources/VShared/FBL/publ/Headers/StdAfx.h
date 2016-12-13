/**********************************************************************************************/
/* FBL_StdAfx.h																				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015                                        					  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#if FBL_UNIX
#include <string>
#endif //FBL_UNIX


#ifdef _MSC_VER

	// ---------------------------------------------------------------------------
	// STL headers:
	//
	#include <algorithm>
	#include <cstring>
	#include <iostream>
	#include <memory>
	#include <map>	
	#include <new>

	// ---------------------------------------------------------------------------
	// ANTLR headers:

	#if KERNEL // only for KERNEL project

		#pragma warning(push)
		#pragma warning( disable : 4512 ) // assignment operator could not be generated
		#pragma warning( disable : 4100 ) // assignment operator could not be generated
		#pragma warning( disable : 4267 ) // conversion from 'size_t' to 'int', possible loss of data

		#include <antlr/config.hpp>
			
		#include <antlr/AST.hpp>
		#include <antlr/ASTFactory.hpp>
		#include <antlr/BitSet.hpp>
		#include <antlr/CharBuffer.hpp>
		#include <antlr/CharStreamException.hpp>
		#include <antlr/CharStreamIOException.hpp>
		#include <antlr/CommonToken.hpp>
		#include <antlr/InputBuffer.hpp>
			
		#include <antlr/LLkParser.hpp>
		#include <antlr/MismatchedTokenException.hpp>
			
		#include <antlr/NoViableAltForCharException.hpp>
		#include <antlr/NoViableAltException.hpp>
			
		#include <antlr/SemanticException.hpp>
			
		#include <antlr/Token.hpp>
		#include <antlr/TokenStream.hpp>
		#include <antlr/TokenBuffer.hpp>
		#include <antlr/TokenStreamException.hpp>
		#include <antlr/TokenStreamIOException.hpp>
		#include <antlr/TokenStreamRecognitionException.hpp>

		#pragma warning(pop)

	#endif // KERNEL


	// ---------------------------------------------------------------------------
	// VSHARED headers:

		#include <VShared/FBL/publ/Headers/FBL.h>

	#if V4MD // only for V4MD project
		#include <VShared/FBL/publ/Util_classes/FBL_String.h>

		#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>
		#include <VShared/FBL/publ/Interfaces/FBL_I_IStream.h>
		#include <VShared/FBL/publ/Interfaces/FBL_I_OStream.h>

		#include <VShared/FBL/publ/Properties/FBL_Property.h>
		#include <VShared/FBL/publ/Properties/FBL_PropertyContainer.h>
		#include <VShared/FBL/publ/Properties/FBL_FieldProperties.h>
	#endif // V4MD

	#if 0

		#include <VShared/FBL/publ/Headers/FBL.h>
		#include <VShared/FBL/publ/Sets/FBL_Set.h>
		#include <VShared/FBL/publ/Sets/FBL_BitSet.h>
		#include <VShared/FBL/publ/Sets/FBL_ArraySet.h>
		#include <VShared/FBL/publ/Sets/FBL_SetIterator.h>
		#include <VShared/FBL/publ/Sets/FBL_BitSetIterator.h>
		#include <VShared/FBL/publ/Sets/FBL_ArraySetIterator.h>

		#include <VShared/FBL/publ/Util_classes/FBL_String.h>

		#include <VShared/FBL/publ/Util_Strings/FBL_ustdio.h>

		#include <VShared/FBL/publ/Interfaces/FBL_I_Value.h>
		#include <VShared/FBL/publ/Interfaces/FBL_I_IStream.h>
		#include <VShared/FBL/publ/Interfaces/FBL_I_OStream.h>

		#include <VShared/FBL/publ/Properties/FBL_Property.h>
		#include <VShared/FBL/publ/Properties/FBL_PropertyContainer.h>
		#include <VShared/FBL/publ/Properties/FBL_FieldProperties.h>

	#endif // 0

#endif // _MSC_VER


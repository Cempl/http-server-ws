/**********************************************************************************************/
/* VRP_Types.h     		                                                      				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2015															  */
/* All Rights Reserved                                                   					  */
/**********************************************************************************************/

#pragma once

// VSHARED
#include <VShared/FBL/publ/Interfaces/FBL_I_Field.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Values/FBL_Value_String.h>

// VREPORT
#include <VReport/Printing/VRP_AnchorPosition.h>

// STD
#include <exception>
#include <queue>
#include <string>
#include <vector>

// FORM PLUGIN
#include <VStudio/Plugins/FormEditor/Headers/vsFormEnums.h>

// LT
#include <LT/Core/LVector.h> // Used in VReport
#include <LT/Drawing/LColor.h>
#include <LT/Drawing/LPoint.h>
#include <Extras/Scene/Headers/LSceneProperty.h>


/**********************************************************************************************/
class VArea;
class VAreaInstance;
class VControlInstance;
class VControl;
class VPage;


/**********************************************************************************************/
struct VAreaPart;
struct VConditionalStyle;
struct VPropertyField;
struct VReportTransaction;


/**********************************************************************************************/
using std::queue;
using std::string;
using std::vector;


/**********************************************************************************************/
typedef shared_ptr<VControlInstance>	VControlInstancePtr;
typedef map<wstring,LSceneProperty>		VControlProperties;
typedef shared_ptr<VPage>				VPagePtr;
typedef shared_ptr<VReportTransaction>	VReportTransactionPtr;
typedef LVector<VControlInstancePtr>	VectorOfControls;
typedef vector<VPropertyField>			VPropertyFields;


/**********************************************************************************************/
// Enumerations


/**********************************************************************************************/
enum EVControlType
{
	kControlUnknown = 8000,
	kControlBarCode,
	kControlBreak,
	kControlButton,
	kControlChart,
	kControlCircle,
	kControlColumnFooter,
	kControlColumnHeader,
	kControlExpression,
	kControlField,
	kControlFieldBool,
	kControlFieldPicture,
	kControlForm,
	kControlGroupFooter,
	kControlGroupHeader,
	kControlHTML,
    kControlHyperLink,
	kControlLabel,
	kControlLine,
	kControlPageFooter,
	kControlPageHeader,
	kControlPanel,
	kControlPicture,
	kControlRect,
	kControlReport,
	kControlReportBody,
	kControlReportHeader,
	kControlReportLabel,
	kControlReportFooter,
	kControlShape,
	kControlSubReport,
	kControlSummary,
	kControlTable
};


/**********************************************************************************************/
enum EVPartKind
{
	kPartControl,
	kPartPageBreak,
	kPartSubReport,
	kPartTable
};


/**********************************************************************************************/
enum EVAnchorKind
{
	kAnchorAbsolute,
	kAnchorShift
};


/**********************************************************************************************/
struct VColumnCache
{
	vector<vector<char> >		data;
	vector<FBL::I_Value_Ptr>	values;
};


/**********************************************************************************************/
struct VConditionalStyle
{
	VConditionalStyle( void )
	:
		fieldIndex	( -1 ),
		kind		( kExpression )
	{
	}

	LColor					back;
	LColor					fore;

	bool					bold		: 1;
	bool					italic		: 1;
	bool					strike		: 1;
	bool					transparent	: 1;
	bool					underline	: 1;

	int						fieldIndex;
	wstring					fieldName;

	EVConditionStyleKind	kind;
};


/**********************************************************************************************/
struct VReportGroup
{
	VReportGroup( void )
	:
		fieldIndex( -1 )
	{
	}

	int				fieldIndex;
	std::wstring	fieldName;
	VArea*			footer;
	VArea*			header;
	std::wstring	value_curr;		// TODO
	std::wstring	value_prev;		// TODO
};


/**********************************************************************************************/
enum EVPreviewParts
{
	kPreviewAnchors			= 1 << 0,
	kPreviewArea			= 1 << 1,
	kPreviewBorders			= 1 << 2,
	kPreviewCorners			= 1 << 3,
	kPreviewMargins			= 1 << 4,
	kPreviewMeasure			= 1 << 5,
	kPreviewNonPrintable	= 1 << 6,
	kPreviewObjectName		= 1 << 7,
	kPreviewWarnings		= 1 << 8,
	kPreviewSplitPositions	= 1 << 9
};


/**********************************************************************************************/
enum EVPropertyFieldKind
{
	kPropFieldInvalid,
	
	kPropFieldColumn,
	kPropFieldColumnCount,
	kPropFieldDatabaseName,
	kPropFieldDate,
	kPropFieldDateTime,
	kPropFieldDay,
	kPropFieldField,
	kPropFieldHour,
	kPropFieldMinute,
	kPropFieldMonth,
	kPropFieldMonthName,
	kPropFieldPage,
	kPropFieldPageCount,
	kPropFieldPaperFormat,
	kPropFieldParameter,
	kPropFieldRecord,
	kPropFieldRecordCount,
	kPropFieldReportName,
	kPropFieldText,
	kPropFieldTime,
	kPropFieldWeekDay,
	kPropFieldYear
};


/**********************************************************************************************/
struct VPropertyField
{
	VPropertyField( 
		EVPropertyFieldKind pkind,
		const std::wstring&	pvalue,
		std::wstring&&		pdef	= wstring() )
	:
		kind	( pkind ),
		value	( pvalue ),
		def		( std::move( pdef ) )
	{
	}
	
	VPropertyField( 
		EVPropertyFieldKind pkind	= kPropFieldInvalid,
		std::wstring&&		pvalue	= wstring(),
		std::wstring&&		pdef	= wstring() )
	:
		kind	( pkind ),
		value	( std::move( pvalue ) ),
		def		( std::move( pdef ) )
	{
	}
		
	EVPropertyFieldKind	kind;
	std::wstring		value;
	std::wstring		def;
};


/**********************************************************************************************/
struct AreaPrintingScope
{
	AreaPrintingScope( vector<VArea*>& q, VArea& area ) : q_( q ) { q.push_back( &area ); }
	~AreaPrintingScope( void ) { if( q_.size() ) q_.pop_back(); }
	
	vector<VArea*>& q_;
};
	
	
/**********************************************************************************************/
enum VSummaryType
{
	kAvg,
	kCount,
	kMax,
	kMin,
	kSum
};


/**********************************************************************************************/
struct VReportSummary
{
	const VControl*	control;
	size_t					count;
	std::wstring			expr;
	std::wstring			field;
	VSummaryType			type;
	LSceneProperty			value;
};


/**********************************************************************************************/
enum EVAllocation
{
	kAllocMalloc,
	kAllocNewArrayUChar,
	kAllocPaintlib
};


/**********************************************************************************************/
struct VSpace
{
	LColor	color;
	int		height;
	int		position;
	int		width;
	int		x;
};


/**********************************************************************************************/
struct VColumnFullException : public std::exception
{
	VColumnFullException( const LT::LPoint& p, const wchar_t* s )	: pos( p ), content( s ) {}
	VColumnFullException( const LT::LPoint& p, const wstring& s )	: pos( p ), content( s ) {}
	VColumnFullException( const LT::LPoint& p, wstring&& s )			: pos( p ), content( std::move( s ) ) {}
	VColumnFullException( const LT::LPoint& p, LString&& s )			: pos( p ), content( std::move( s.str() ) ) {}
	
	const char* what( void ) const throw() override { return "Column is full"; }	
	
	LT::LPoint	pos;
	wstring		content;
};


/**********************************************************************************************/
class VPagesLimitEception : public std::exception
{
	const char* what( void ) const throw() override { return "Pages limit reached"; }	
};


/**********************************************************************************************/
enum EVExpressionEngine
{
	kExpressionEngine_Datasource,
	kExpressionEngine_ValentinaReports
};


/**********************************************************************************************/
enum EVAddAreaFlag
{
	kCanDontFit,
	kDontCheckFreeSpace,
	kMustFitInSingleColumn
};

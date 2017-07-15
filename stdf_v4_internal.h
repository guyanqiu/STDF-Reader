#ifndef _STDF_V4_INTERNAL_H_
#define _STDF_V4_INTERNAL_H_

#include <fstream>
#include <bitset>
#include <vector>
#include <iostream>
#include <string>
#include <array>

/********************************************************************************
The STDF test data file must contain one FAR, one MIR, at least one PCR, and one MRR.
All other records are optional. The file may therefore contain any combination of datalog,
summary, and site summary for that lot of parts.
Initial Sequence:
FAR - MIR
FAR - ATRs - MIR
FAR - MIR  - RDR
FAR - ATRs - MIR - RDR
FAR - MIR  - SDRs
FAR - ATRs - MIR - SDRs
FAR - MIR  - RDR - SDRs
FAR - ATRs - MIR - RDR - SDRs

Optional Fields and Missing/Invalid Data
---------------------------------------------------------------------------
Data Type                      |          Missing/Invalid Data Flag
---------------------------------------------------------------------------
Variable-length string         |     Set the length byte to 0.
---------------------------------------------------------------------------
Fixed-length character string  |     Fill the field with spaces.
---------------------------------------------------------------------------
Fixed-length binary string     |     Set a flag bit in an Optional Data byte.
---------------------------------------------------------------------------
Time and date fields           |     Use a binary 0.
---------------------------------------------------------------------------
Signed and unsigned integers   |     Use the indicated reserved value or set
and floating point values      |     a flag bit in an OptionalDatabyte.
---------------------------------------------------------------------------

Alphabetical Listing:
---------------------------------------------------
Abbreviations |   Record Type
--------------|------------------------------------
ATR           |  Audit Trail Record
BPS           |  Begin Program Section Record
DTR           |  Datalog Text Record
EPS           |  End Program Section Record
FAR           |  File Attributes Record
FTR           |  Functional Test Record
GDR           |  Generic Data Record
HBR           |  Hardware Bin Record
MIR           |  Master Information Record
MPR           |  Multiple-Result Parametric Record
MRR           |  Master Results Record
PCR           |  Part Count Record
PGR           |  Pin Group Record
PIR           |  Part Information Record
PLR           |  Pin List Record
PMR           |  Pin Map Record
PRR           |  Part Results Record
PTR           |  Parametric Test Record
RDR           |  Retest Data Record
SBR           |  Software Bin Record
SDR           |  Site Description Record
TSR           |  Test Synopsis Record
WCR           |  Wafer Configuration Record
WIR           |  Wafer Information Record
WRR           |  Wafer Results Record
---------------------------------------------------
********************************************************************************/
#define MAX_REC_LENGTH   65536U
#define BYTE_LENGTH      8U
#define NIBBLE_LENGTH    4U
#define MAX_BITS_LENGTH  65535U
#define MAX_BYTE_LENGTH  255U

//REC_TYPE Code
#define REC_TYP_INFO        0   // Information about the STDF file
#define REC_TYP_PER_LOT     1   // Data collected on a per lot basis
#define REC_TYP_PER_WAFER   2   // Data collected per wafer
#define REC_TYP_PER_PART    5   // Data collected on a per part basis
#define REC_TYP_PER_TEST    10  // Data collected per test in the test program
#define REC_TYP_PER_EXEC    15  // Data collected per test execution
#define REC_TYP_PER_PROG    20  // Data collected per program segment
#define REC_TYP_GENERIC     50  // Generic Data
#define REC_TYP_RESV_IMAGE  180 // Reserved for use by Image Software
#define REC_TYP_RESV_IG900  181 // Reserved for use by IG900 Software

// REC_SUB Code
#define  REC_SUB_FAR   10 // File Attributes Record
#define  REC_SUB_ATR   20 // Audit Trail Record
#define  REC_SUB_MIR   10 // Master Information Record
#define  REC_SUB_MRR   20 // Master Result Record
#define  REC_SUB_PCR   30 // Part Count Record
#define  REC_SUB_HBR   40 // Hardware Bin Record
#define  REC_SUB_SBR   50 // Software Bin Record
#define  REC_SUB_PMR   60 // Pin Map Record
#define  REC_SUB_PGR   62 // Pin Group Record
#define  REC_SUB_PLR   63 // Pin List Record
#define  REC_SUB_RDR   70 // Reset Data Record
#define  REC_SUB_SDR   80 // Site Description Record
#define  REC_SUB_WIR   10 // Wafer Information Record
#define  REC_SUB_WRR   20 // Wafer Result Record
#define  REC_SUB_WCR   30 // Wafer Configuration Record
#define  REC_SUB_PIR   10 // Part Information Record
#define  REC_SUB_PRR   20 // Part Result Record
#define  REC_SUB_TSR   30 // Test Synopsis Record
#define  REC_SUB_PTR   10 // Parametric Test Record
#define  REC_SUB_MPR   15 // Multiple-Result Parametric Record
#define  REC_SUB_FTR   20 // Functional Test Record
#define  REC_SUB_BPS   10 // Begin Program Section Record
#define  REC_SUB_EPS   20 // End Program Section Record
#define  REC_SUB_GDR   10 // Generic Data Record
#define  REC_SUB_DTR   30 // Datalog Text Record

#define REC_FAR (REC_TYP_INFO      <<8 | REC_SUB_FAR)    // FAR
#define REC_ATR (REC_TYP_INFO      <<8 | REC_SUB_ATR)    // ATR
#define REC_MIR (REC_TYP_PER_LOT   <<8 | REC_SUB_MIR)    // MIR
#define REC_MRR (REC_TYP_PER_LOT   <<8 | REC_SUB_MRR)    // MRR
#define REC_PCR (REC_TYP_PER_LOT   <<8 | REC_SUB_PCR)    // PCR
#define REC_HBR (REC_TYP_PER_LOT   <<8 | REC_SUB_HBR)    // HBR
#define REC_SBR (REC_TYP_PER_LOT   <<8 | REC_SUB_SBR)    // SBR
#define REC_PMR (REC_TYP_PER_LOT   <<8 | REC_SUB_PMR)    // PMR
#define REC_PGR (REC_TYP_PER_LOT   <<8 | REC_SUB_PGR)    // PGR
#define REC_PLR (REC_TYP_PER_LOT   <<8 | REC_SUB_PLR)    // PLR
#define REC_RDR (REC_TYP_PER_LOT   <<8 | REC_SUB_RDR)    // RDR
#define REC_SDR (REC_TYP_PER_LOT   <<8 | REC_SUB_SDR)    // SDR
#define REC_WIR (REC_TYP_PER_WAFER <<8 | REC_SUB_WIR)    // WIR
#define REC_WRR (REC_TYP_PER_WAFER <<8 | REC_SUB_WRR)    // WRR
#define REC_WCR (REC_TYP_PER_WAFER <<8 | REC_SUB_WCR)    // WCR
#define REC_PIR (REC_TYP_PER_PART  <<8 | REC_SUB_PIR)    // PIR
#define REC_PRR (REC_TYP_PER_PART  <<8 | REC_SUB_PRR)    // PRR
#define REC_TSR (REC_TYP_PER_TEST  <<8 | REC_SUB_TSR)    // TSR
#define REC_PTR (REC_TYP_PER_EXEC  <<8 | REC_SUB_PTR)    // PTR
#define REC_MPR (REC_TYP_PER_EXEC  <<8 | REC_SUB_MPR)    // MPR
#define REC_FTR (REC_TYP_PER_EXEC  <<8 | REC_SUB_FTR)    // FTR
#define REC_BPS (REC_TYP_PER_PROG  <<8 | REC_SUB_BPS)    // BPS
#define REC_EPS (REC_TYP_PER_PROG  <<8 | REC_SUB_EPS)    // EPS
#define REC_GDR (REC_TYP_GENERIC   <<8 | REC_SUB_GDR)    // GDR
#define REC_DTR (REC_TYP_GENERIC   <<8 | REC_SUB_DTR)    // DTR


// STDF TYPE
typedef char                             C1; //One character
typedef unsigned char                    U1; //One byte unsigned integer
typedef unsigned short                   U2; //Two byte unsigned integer
typedef unsigned int                     U4; //Four byte unsigned integer
typedef signed char                      I1; //One byte signed integer
typedef signed short                     I2; //Two byte signed integer
typedef signed int                       I4; //Four byte signed integer
typedef float                            R4; //Four byte floating point number
typedef double                           R8; //Eight byte floating point number
typedef std::bitset<BYTE_LENGTH>         B1; //One byte bit-encoded data
typedef std::bitset<NIBBLE_LENGTH>       N1; //(Nibble = 4 bits of a byte).First item in low 4 bits, second item in high 4 bits.
typedef std::string                      Cn; //first byte = unsigned count of bytes to follow (maximum of 255 bytes)

typedef struct{ U1 count; std::array<B1, MAX_BYTE_LENGTH> data;} Bn; //First byte = unsigned count of bytes to follow (maximum of 255 bytes).
// Replace std::bitset with std::array
// Because of the VS2010's bug. BUG ID:612692
#if _MSC_VER == 1600   
// https://connect.microsoft.com/VisualStudio/feedback/details/612692
typedef struct{ U2 count; std::array<C1, MAX_BITS_LENGTH> data;} Dn; //First two bytes = unsigned count of bits to follow (maximum of 65,535 bits).
#else
typedef struct{ U2 count; std::bitset<MAX_BITS_LENGTH>    data;} Dn; //First two bytes = unsigned count of bits to follow (maximum of 65,535 bits).
#endif

typedef std::vector<U1>    kxU1; // Array of data of the type U1.
typedef std::vector<U2>    kxU2; // Array of data of the type U2.
typedef std::vector<U4>    kxU4; // Array of data of the type U4.
typedef std::vector<I1>    kxI1; // Array of data of the type I1.
typedef std::vector<I2>    kxI2; // Array of data of the type I2.
typedef std::vector<I4>    kxI4; // Array of data of the type I4.
typedef std::vector<N1>    kxN1; // Array of data of the type N1.
typedef std::vector<R4>    kxR4; // Array of data of the type R4.
typedef std::vector<R8>    kxR8; // Array of data of the type R8.
typedef std::vector<Cn>    kxCn; // Array of data of the type Cn.

typedef struct {U1 type; void *data;} V1; //The data type is specified by a code in the first byte, and the data follows (maximum of 255 bytes)
typedef std::vector<V1>    Vn;

class   RecordHeader
{
public:
    U2 REC_LEN; //The number of bytes of data following the record header.
                //REC_LEN does not include the four bytes of the record header.
    U1 REC_TYP; //An integer identifying a group of related STDF record types.
    U1 REC_SUB; //An integer identifying a specific STDF record type within each REC_TYP group.
public:
    int ReadRecord(std::ifstream& file_stream);
    int WriteRecord(std::ofstream& file_stream);

    RecordHeader();
    ~RecordHeader(){};
    const char*GetReadOnlyData() const;
    char* GetWriteOnlyData();
    int GetRecordType() const;

private:
    char rawdata[MAX_REC_LENGTH];
    RecordHeader(RecordHeader& );
    RecordHeader& operator=(const RecordHeader& src);
};


class   FileAttributes  
{
public:
    U2  REC_LEN ;  // Bytes of data following header
    U1  REC_TYP ;  // Record type (0)
    U1  REC_SUB ;  // Record sub-type (10)
    U1  CPU_TYPE;  // CPU type that wrote this file
    U1  STDF_VER;  // STDF version number

public:
    FileAttributes();
    ~FileAttributes(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    FileAttributes(const FileAttributes& );
    FileAttributes& operator=(const FileAttributes& );
};

/*
CPU_TYPE
    Indicates which type of CPU wrote this STDF file. This information is useful for determining
    the CPU-dependent data representation of the integer and floating point fields in the file's
    records. The valid values are:
        0 = DEC PDP-11 and VAX processors.
            F and D floating point formats will be used.
            G and H floating point formats will not be used.
        1 = Sun1,2,3, and 4computers.
        2 = Sun 386i computers, and IBM PC, IBM PC-AT, and IBM PC-XT computers.
        3-127 = Reserved for future use by Teradyne.
        128-255 = Reserved for use by customers.
    A code defined here may also be valid for other CPU types whose data formats are fully compatible
    with that of the type listed here. Before using one of these codes for a CPU type not listed
    here, please check with the Teradyne hot line, which can provide additional information on CPU
    compatibility.
STDF_VER
    Identifies the version number of the STDF specification used in generating the data.
    This allows data analysis programs to handle STDF specification enhancements.
*/



class   AuditTrail //: public Record
{
public:
    U2 REC_LEN ; //Bytes of data following header
    U1 REC_TYP ; //Record type (0)
    U1 REC_SUB ; //Record sub-type (20)
    U4 MOD_TIM ; //Date and time of STDF file modification
    Cn  CMD_LINE; //Command line of program

public:
    AuditTrail();
    ~AuditTrail(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    AuditTrail(const AuditTrail& );
    AuditTrail& operator=(const AuditTrail& );
};


class   MasterInformation  //: public Record
{
public:
    U2 REC_LEN  ; // Bytes of data following header
    U1 REC_TYP  ; // Record type (1)
    U1 REC_SUB  ; // Record sub-type (10)
    U4 SETUP_T  ; // Date and time of job setup
    U4 START_T  ; // Date and time first part tested
    U1 STAT_NUM ; // Tester station number
    C1 MODE_COD ; // Test mode code (e.g. prod, dev) ,default: space
    C1 RTST_COD ; // Lot retest code ,default: space
    C1 PROT_COD ; // Data protection code ,default: space
    U2 BURN_TIM ; // Burn-in time (in minutes) ,default: 65,535
    C1 CMOD_COD ; // Command mode code ,default: space
    Cn LOT_ID   ; // Lot ID (customer specified)
    Cn PART_TYP ; // Part Type (or product ID)
    Cn NODE_NAM ; // Name of node that generated data
    Cn TSTR_TYP ; // Tester type
    Cn JOB_NAM  ; // Job name (test program name)
    Cn JOB_REV  ; // Job (test program) revision number ,default: length byte = 0
    Cn SBLOT_ID ; // Sublot ID ,default: length byte = 0
    Cn OPER_NAM ; // Operator name or ID (at setup time) ,default: length byte = 0
    Cn EXEC_TYP ; // Tester executive software type ,default: length byte = 0
    Cn EXEC_VER ; // Tester exec software version number ,default: length byte = 0
    Cn TEST_COD ; // Test phase or step code ,default: length byte = 0
    Cn TST_TEMP ; // Test temperature ,default: length byte = 0
    Cn USER_TXT ; // Generic user text ,default: length byte = 0
    Cn AUX_FILE ; // Name of auxiliary data file ,default: length byte = 0
    Cn PKG_TYP  ; // Package type ,default: length byte = 0
    Cn FAMLY_ID ; // Product family ID ,default: length byte = 0
    Cn DATE_COD ; // Date code ,default: length byte = 0
    Cn FACIL_ID ; // Test facility ID ,default: length byte = 0
    Cn FLOOR_ID ; // Test floor ID ,default: length byte = 0
    Cn PROC_ID  ; // Fabrication process ID ,default: length byte = 0
    Cn OPER_FRQ ; // Operation frequency or step ,default: length byte = 0
    Cn SPEC_NAM ; // Test specification name ,default: length byte = 0
    Cn SPEC_VER ; // Test specification version number ,default: length byte = 0
    Cn FLOW_ID  ; // Test flow ID ,default: length byte = 0
    Cn SETUP_ID ; // Test setup ID ,default: length byte = 0
    Cn DSGN_REV ; // Device design revision ,default: length byte = 0
    Cn ENG_ID   ; // Engineering lot ID ,default: length byte = 0
    Cn ROM_COD  ; // ROM code ID ,default: length byte = 0
    Cn SERL_NUM ; // Tester serial number ,default: length byte = 0
    Cn SUPR_NAM ; // Supervisor name or ID ,default: length byte = 0

public:
    MasterInformation();
    ~MasterInformation(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    MasterInformation(const MasterInformation& );
    MasterInformation& operator=(const MasterInformation& );
};


/*
MODE_COD
    Indicates the station mode under which the parts were tested.
    Currently defined values for the MODE_COD field are:
        A = AEL (Automatic Edge Lock) mode
        C = Checker mode
        D = Development / Debug test mode
        E = Engineering mode (same as Development mode)
        M = Maintenance mode
        P = Production test mode
        Q = Quality Control
    All other alphabetic codes are reserved for future use by Teradyne.
    The characters 0 -9 are available for customer use.
RTST_COD
    Indicates whether the lot of parts has been previously tested under the same test conditions.
    Suggested values are:
        Y = Lot was previously tested.
        N = Lot has not been previously tested.
        space = Not known if lot has been previously tested.
        0-9 = Number of times lot has previously been tested.
PROT_COD
    User-defined field indicating the protection desired for the test data being stored.
    Valid values are the ASCII characters 0 -9 and A -Z.
    A space in this field indicates a missing value (default protection).
CMOD_COD
    Indicates the command mode of the tester during testing of the parts.
    The user or the tester executive software defines command mode values.
    Valid values are the ASCII characters 0 -9 and A -Z. A space indicates a missing value.
TEST_COD
    A user-defined field specifying the phase or step in the device testing process.
TST_TEMP
    The test temperature is an ASCII string. Therefore, it can be stored as degrees Celsius,
    Fahrenheit, Kelvin or whatever. It can also be expressed in terms like HOT, ROOM,
    and COLD if that is preferred.
*/

class   MasterResults //: public Record
{
public:
    U2 REC_LEN  ; // Bytes of data following header
    U1 REC_TYP  ; // Record type (1)
    U1 REC_SUB  ; // Record sub-type (20)
    U4 FINISH_T ; // Date and time last part tested
    C1 DISP_COD ; // Lot disposition code,default: space
    Cn USR_DESC ; // Lot description supplied by user ,default: length byte = 0
    Cn EXC_DESC ; // Lot description supplied by exec ,default: length byte = 0


public:
    MasterResults();
    ~MasterResults(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    MasterResults(const MasterResults& );
    MasterResults& operator=(const MasterResults& );
};
/*
DISP_COD
    Supplied by the user to indicate the disposition of the lot of parts (or of the tester itself,
    in the case of checker or AEL data). The meaning of DISP_COD values are user-defined.
    A valid value is an ASCII alphanumeric character (0 -9 or A -Z).
    A space indicates a missing value.

*/

class   PartCount //: public Record
{
public:
    U2 REC_LEN  ; // Bytes of data following header
    U1 REC_TYP  ; // Record type (1)
    U1 REC_SUB  ; // Record sub-type (30)
    U1 HEAD_NUM ; // Test head number
    U1 SITE_NUM ; // Test site number
    U4 PART_CNT ; // Number of parts tested
    U4 RTST_CNT ; // Number of parts retested ,default: 4,294,967,295
    U4 ABRT_CNT ; // Number of aborts during testing ,default: 4,294,967,295
    U4 GOOD_CNT ; // Number of good (passed) parts tested ,default: 4,294,967,295
    U4 FUNC_CNT ; // Number of functional parts tested ,default: 4,294,967,295
public:
    PartCount();
    ~PartCount(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    PartCount(const PartCount& );
    PartCount& operator=(const PartCount& );
};

/*
HEAD_NUM
    If this PCR contains a summary of the part counts for all test sites,
    this field must be set to 255.
GOOD_CNT,FUNC_CNT
    A part is considered good when it is binned into one of the "passing" hardware bins.
    A part is considered functional when it is good enough to test, whether it passes or not.
    Parts that are incomplete or have shorts or opens are considered non-functional.
*/

class   HardwareBin //: public Record
{
public:
    U2 REC_LEN  ; // Bytes of data following header
    U1 REC_TYP  ; // Record type (1)
    U1 REC_SUB  ; // Record sub-type (40)
    U1 HEAD_NUM ; // Test head number
    U1 SITE_NUM ; // Test site number
    U2 HBIN_NUM ; // Hardware bin number
    U4 HBIN_CNT ; // Number of parts in bin
    C1 HBIN_PF  ; // Pass/fail indication ,default: space
    Cn HBIN_NAM ; // Name of hardware bin ,default: length byte = 0

public:
    HardwareBin();
    ~HardwareBin(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    HardwareBin(const HardwareBin& );
    HardwareBin& operator=(const HardwareBin& );
};

/*
HEAD_NUM
    If this HBR contains a summary of the hardware bin counts for all test sites,
    this field must be set to 255.
HBIN_NUM
    Has legal values in the range 0 to 32767.
HBIN_PF
    This field indicates whether the hardware bin was a passing or failing bin.
    Valid values for this field are:
        P = Passing bin
        F = Failing bin
        space = Unknown
*/

class   SoftwareBin //: public Record
{
public:
    U2 REC_LEN  ; // Bytes of data following header
    U1 REC_TYP  ; // Record type (1)
    U1 REC_SUB  ; // Record sub-type (50)
    U1 HEAD_NUM ; // Test head number
    U1 SITE_NUM ; // Test site number
    U2 SBIN_NUM ; // Software bin number
    U4 SBIN_CNT ; // Number of parts in bin
    C1 SBIN_PF  ; // Pass/fail indication ,default: space
    Cn SBIN_NAM ; // Name of software bin ,default: length byte = 0

public:
    SoftwareBin();
    ~SoftwareBin(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    SoftwareBin(const SoftwareBin& );
    SoftwareBin& operator=(const SoftwareBin& );
};
/*
HEAD_NUM
    If this SBR contains a summary of the software bin counts for all test sites,
    this field must be set to 255.
SBIN_NUM
    Has legal values in the range 0 to 32767.
SBIN_PF
    This field indicates whether the software bin was a passing or failing bin.
    Valid values for this field are:
        P = Passing bin
        F = Failing bin
        space = Unknown
*/



class   PinMap //: public Record
{
public:
    U2 REC_LEN  ; // Bytes of data following header
    U1 REC_TYP  ; // Record type (1)
    U1 REC_SUB  ; // Record sub-type (60)
    U2 PMR_INDX ; // Unique index associated with pin
    U2 CHAN_TYP ; // Channel type ,default: 0
    Cn CHAN_NAM ; // Channel name ,default: length byte = 0
    Cn PHY_NAM  ; // Physical name of pin ,default: length byte = 0
    Cn LOG_NAM  ; // Logical name of pin ,default: length byte = 0
    U1 HEAD_NUM ; // Head number associated with channel ,default: 1
    U1 SITE_NUM ; // Site number associated with channel ,default: 1

public:
    PinMap();
    ~PinMap(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    PinMap(const PinMap& );
    PinMap& operator=(const PinMap& );
};
/*
PMR_INDX
    This number is used to associate the channel and pin name information with data in
    the FTR or MPR. Reporting programs can then look up the PMR index and choose which
    of the three associated names they will use. The range of legal PMR indexesis1 -32,767.
    The size of the FAIL_PIN and SPIN_MAP arrays in the FTR are directly proportional to
    the highest PMR index number. Therefore, it is important to start PMR indexes with a
    low number and use consecutive numbers if possible.
CHAN_TYP
    The channel type values are tester-specific.
    Please refer to the tester documentation for a list of the valid tester channel types and codes.
HEAD_NUM, SITE_NUM
    If a test system does not support parallel testing
    and does not have a standard way of identifying its single test site or head,
    these fields should be set to 1. If missing, the value of these fields will default to 1.
*/


class   PinGroup //: public Record
{
public:
    U2     REC_LEN  ; // Bytes of data following header
    U1     REC_TYP  ; // Record type (1)
    U1     REC_SUB  ; // Record sub-type (62)
    U2     GRP_INDX ; // Unique index associated with pin group
    Cn     GRP_NAM  ; // Name of pin group ,default: length byte = 0
    U2     INDX_CNT ; // Count of PMR indexes
    kxU2   PMR_INDX ; // Array of indexes for pins in the group ,default: INDX_CNT = 0
public:
    PinGroup();
    ~PinGroup(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    PinGroup(const PinGroup& );
    PinGroup& operator=(const PinGroup& );
};

/*
GRP_INDX
    The range of legal group index numbers is 32,768 -65,535.
INDX_CNT,PMR_INDX
    PMR_INDX is an array of PMR indexes whose length is defined by INDX_CNT.
    The order of the PMR indexes should be from most significant to least significant bit
    in the pin group (regardless of the order of PMR index numbers).
*/


class   PinList //: public Record
{
public:
    U2   REC_LEN  ; // Bytes of data following header
    U1   REC_TYP  ; // Record type (1)
    U1   REC_SUB  ; // Record sub-type (63)
    U2   GRP_CNT  ; // Count (k) of pins or pin groups
    kxU2 GRP_INDX ; // Array of pin or pin group indexes
    kxU2 GRP_MODE ; // Operating mode of pin group ,default: 0
    kxU1 GRP_RADX ; // Display radix of pin group ,default: 0
    kxCn PGM_CHAR ; // Program state encoding characters ,default: length byte = 0
    kxCn RTN_CHAR ; // Return state encoding characters ,default: length byte = 0
    kxCn PGM_CHAL ; // Program state encoding characters ,default: length byte = 0
    kxCn RTN_CHAL ; // Return state encoding characters ,default: length byte = 0
public:
    PinList();
    ~PinList(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    PinList(const PinList& );
    PinList& operator=(const PinList& );
};

/*
GRP_CNT
    GRP_CNT defines the number of pins or pin groups whose radix and mode are being defined.
    Therefore, it defines the size of each of the arrays that follow in the record.
    GRP_CNT must be greater than zero.
GRP_MODE
    The following are valid values for the pin group mode:
        00 = Unknown
        10 = Normal
        20 = SCIO (Same Cycle I/O)
        21 = SCIO Midband
        22 = SCIO Valid
        23 = SCIO Window Sustain
        30 = Dual drive (two drive bits per cycle)
        31 = Dual drive Midband
        32 = Dual drive Valid
        33 = Dual drive Window Sustain
    Unused pin group modes in the range of 1 through 32,767 are reserved for future use.
    Pin group modes 32,768 through 65,535 are available for customer use.
GRP_RADX
    The following are valid values for the pin group display radix:
        0 = Use display program default
        2 = Display in Binary
        8 = Display in Octal
        10 = Display in Decimal
        16 = Display in Hexadecimal
        20 = Display as symbolic
PGM_CHAR, PGM_CHAL
    These ASCII characters are used to display the programmed state in the FTR or MPR.
    Use of these character arrays makes it possible to store tester-dependent display
    representations in a tester-independent format. If a single character is used to
    represent each programmed state, then only the PGM_CHAR array need be used.
    If two characters represent each state, then the first (left) character is stored
    in PGM_CHAL and the second (right) character is stored in PGM_CHAR.
RTN_CHAR, RTN_CHAL
    These ASCII characters are used to display the returned state in the FTR or MPR.
    Use of these character arrays makes it possible to store tester-dependent display
    representations in a tester-independent format. If a single character is used to
    represent each returned state, then only the RTN_CHAR array need be used.
    If two characters represent each state, then the first (left) character is stored
    in RTN_CHAL and the second (right) character is stored in RTN_CHAR.
*/

class   RetestData //: public Record
{
public:
    U2   REC_LEN  ; // Bytes of data following header
    U1   REC_TYP  ; // Record type (1)
    U1   REC_SUB  ; // Record sub-type (70)
    U2   NUM_BINS ; // Number (k) of bins being retested
    kxU2 RTST_BIN ; // Array of retest bin numbers ,default: NUM_BINS = 0
public:
    RetestData();
    ~RetestData(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    RetestData(const RetestData& );
    RetestData& operator=(const RetestData& );
};

/*
NUM_BINS,RTST_BIN
    NUM_BINS indicates the number of hardware bins being retested and therefore the size
    of the RTST_BIN array that follows. If NUM_BINS is zero, then all bins in the lot are
    being retested and RTST_BIN is omitted. The LOT_ID, SUBLOT_ID, and TEST_COD of the
    current STDF file should match those of the STDF file that is being retested,
    so the data can be properly merged at a later time.
*/

class   SiteDescription //: public Record
{
public:
    U2   REC_LEN  ; // Bytes of data following header
    U1   REC_TYP  ; // Record type (1)
    U1   REC_SUB  ; // Record sub-type (80)
    U1   HEAD_NUM ; // Test head number
    U1   SITE_GRP ; // Site group number
    U1   SITE_CNT ; // Number (k) of test sites in site group
    kxU1 SITE_NUM ; // Array of test site numbers
    Cn   HAND_TYP ; // Handler or prober type ,default: length byte = 0
    Cn   HAND_ID  ; // Handler or prober ID ,default: length byte = 0
    Cn   CARD_TYP ; // Probe card type ,default: length byte = 0
    Cn   CARD_ID  ; // Probe card ID ,default: length byte = 0
    Cn   LOAD_TYP ; // Load board type ,default: length byte = 0
    Cn   LOAD_ID  ; // Load board ID ,default: length byte = 0
    Cn   DIB_TYP  ; // DIB board type ,default: length byte = 0
    Cn   DIB_ID   ; // DIB board ID ,default: length byte = 0
    Cn   CABL_TYP ; // Interface cable type ,default: length byte = 0
    Cn   CABL_ID  ; // Interface cable ID ,default: length byte = 0
    Cn   CONT_TYP ; // Handler contactor type ,default: length byte = 0
    Cn   CONT_ID  ; // Handler contactor ID ,default: length byte = 0
    Cn   LASR_TYP ; // Laser type ,default: length byte = 0
    Cn   LASR_ID  ; // Laser ID ,default: length byte = 0
    Cn   EXTR_TYP ; // Extra equipment type field ,default: length byte = 0
    Cn   EXTR_ID  ; // Extra equipment ID ,default: length byte = 0
public:
    SiteDescription();
    ~SiteDescription(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    SiteDescription(const SiteDescription& );
    SiteDescription& operator=(const SiteDescription& );
};

/*
SITE_GRP
    Specifies a site group number (called a station number on some testers) for the group
    of sites whose configuration is defined by this record. Note that this is different
    from the station number specified in the MIR, which refers to a software station only.
    The value in this field must be unique within the STDF file.
SITE_CNT, SITE_NUM
    SITE_CNT tells how many sites are in the site group that the current SDR configuration
    applies to. SITE_NUM is an array of those site numbers.
_TYP fields
    These are the type or model number of the interface or peripheral equipment being used
    for testing:
        HAND_TYP,CARD_TYP,LOAD_TYP,DIB_TYP,
        CABL_TYP,CONT_TYP,LASR_TYP,EXTR_TYP
_ID fields
    These are the IDs or serial numbers of the interface or peripheral equipment being used
    for testing:
        HAND_ID,CARD_ID,LOAD_ID,DIB_ID,
        CABL_ID,CONT_ID,LASR_ID,EXTR_ID
*/

class   WaferInformation //: public Record
{
public:
    U2 REC_LEN  ; // Bytes of data following header
    U1 REC_TYP  ; // Record type (2)
    U1 REC_SUB  ; // Record sub-type (10)
    U1 HEAD_NUM ; // Test head number
    U1 SITE_GRP ; // Site group number 255
    U4 START_T  ; // Date and time first part tested
    Cn WAFER_ID ; // Wafer ID length byte = 0
public:
    WaferInformation();
    ~WaferInformation(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    WaferInformation(const WaferInformation& );
    WaferInformation& operator=(const WaferInformation& );
};

/*
SITE_GRP
    Refers to the site group in the SDR.Thisisa means of relating the wafer information
    to the configuration of the equipment used to test it. If this information is not known,
    or the tester does not support the concept of site groups, this field should be set to 255.
WAFER_ID
    Is optional, but is strongly recommended in order to make the resultant data files as
    useful as possible.

*/

class   WaferResults //: public Record
{
public:
    U2 REC_LEN  ; // Bytes of data following header
    U1 REC_TYP  ; // Record type (2)
    U1 REC_SUB  ; // Record sub-type (20)
    U1 HEAD_NUM ; // Test head number
    U1 SITE_GRP ; // Site group number ,default: 255
    U4 FINISH_T ; // Date and time last part tested
    U4 PART_CNT ; // Number of parts tested
    U4 RTST_CNT ; // Number of parts retested ,default: 4,294,967,295
    U4 ABRT_CNT ; // Number of aborts during testing ,default: 4,294,967,295
    U4 GOOD_CNT ; // Number of good (passed) parts tested ,default: 4,294,967,295
    U4 FUNC_CNT ; // Number of functional parts tested ,default: 4,294,967,295
    Cn WAFER_ID ; // Wafer ID ,default: length byte = 0
    Cn FABWF_ID ; // Fab wafer ID ,default: length byte = 0
    Cn FRAME_ID ; // Wafer frame ID ,default: length byte = 0
    Cn MASK_ID  ; // Wafer mask ID ,default: length byte = 0
    Cn USR_DESC ; // Wafer description supplied by user ,default: length byte = 0
    Cn EXC_DESC ; // Wafer description supplied by exec ,default: length byte = 0
public:
    WaferResults();
    ~WaferResults(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    WaferResults(const WaferResults& );
    WaferResults& operator=(const WaferResults& );
};

/*
SITE_GRP
    Refers to the site group in the SDR.Thisisa meansof relating the wafer information to the
    configuration of the equipment used to test it. If this information is not known, or the
    tester does not support the concept of site groups, this field should be set to 255.
WAFER_ID
    Is optional, but is strongly recommended in order to make the resultant data files as useful
    as possible. A Wafer ID in the WRR supersedes any Wafer ID found in the WIR.
FABWF_ID
    Is the ID of the wafer when it was in the fabrication process. This facilitates tracking of
    wafers and correlation of yield with fabrication variations.
FRAME_ID
    Facilitates tracking of wafers once the wafer has been through the saw step and the wafer ID
    is no longer readable on the wafer itself. This is an important piece of information for
    implementing an inkless binning scheme.

*/

class   WaferConfiguration //: public Record
{
public:
    U2 REC_LEN  ; // Bytes of data following header
    U1 REC_TYP  ; // Record type (2)
    U1 REC_SUB  ; // Record sub-type (30)
    R4 WAFR_SIZ ; // Diameter of wafer in WF_UNITS ,default: 0
    R4 DIE_HT   ; // Height of die in WF_UNITS ,default: 0
    R4 DIE_WID  ; // Width of die in WF_UNITS ,default: 0
    U1 WF_UNITS ; // Units for wafer and die dimensions ,default: 0
    C1 WF_FLAT  ; // Orientation of wafer flat ,default: space
    I2 CENTER_X ; // X coordinate of center die on wafer ,default: -32768
    I2 CENTER_Y ; // Y coordinate of center die on wafer ,default: -32768
    C1 POS_X    ; // Positive X direction of wafer ,default: space
    C1 POS_Y    ; // Positive Y direction of wafer ,default: space
public:
    WaferConfiguration();
    ~WaferConfiguration(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    WaferConfiguration(const WaferConfiguration& );
    WaferConfiguration& operator=(const WaferConfiguration& );
};


/*
WF_UNITS
    Has these valid values:
        0   =   Unknown units
        1   =   Units are in inches
        2   =   Units are in centimeters
        3   =   Units are in millimeters
        4   =   Units are in mils
WF_FLAT
    Has these valid values:
        U   =   Up
        D   =   Down
        L   =   Left
        R   =   Right
        space   =   Unknown
CENTER_X,CENTER_Y
    Use the value -32768 to indicate that the field is invalid.
POS_X
    Has these valid values:
        L   =   Left
        R   =   Right
        space   =   Unknown
POS_Y
    Has these valid values:
        U   =   Up
        D   =   Down
        space   =   Unknown
*/

class   PartInformation //: public Record
{
public:
    U2 REC_LEN  ; // Bytes of data following header
    U1 REC_TYP  ; // Record type (5)
    U1 REC_SUB  ; // Record sub-type (10)
    U1 HEAD_NUM ; // Test head number
    U1 SITE_NUM ; // Test site number
public:
    PartInformation();
    ~PartInformation(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    PartInformation(const PartInformation& );
    PartInformation& operator=(const PartInformation& );
};

/*
HEAD_NUM,SITE_NUM
    If a test system does not support parallel testing, and does not have a standard way to
    identify its single test site or head, then these fields should be set to 1.
    When parallel testing, these fields are used to associate individual datalogged results
    (FTRsand PTRs) with a PIR/PRR pair. An FTR or PTR belongs to the PIR/PRR pair having
    the same values for HEAD_NUM and SITE_NUM.

*/

class   PartResults //: public Record
{
public:
    U2 REC_LEN  ; //Bytes of data following header
    U1 REC_TYP  ; //Record type (5)
    U1 REC_SUB  ; //Record sub-type (20)
    U1 HEAD_NUM ; //Test head number
    U1 SITE_NUM ; //Test site number
    B1 PART_FLG ; //Part information flag
    U2 NUM_TEST ; //Number of tests executed
    U2 HARD_BIN ; //Hardware bin number
    U2 SOFT_BIN ; //Software bin number ,default: 65535
    I2 X_COORD  ; //(Wafer) X coordinate ,default: -32768
    I2 Y_COORD  ; //(Wafer) Y coordinate ,default: -32768
    U4 TEST_T   ; //Elapsed test time in milliseconds ,default: 0
    Cn PART_ID  ; //Part identification ,default: length byte = 0
    Cn PART_TXT ; //Part description text ,default: length byte = 0
    Bn PART_FIX ; //Part repair information ,default: length byte = 0
public:
    PartResults();
    ~PartResults(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    PartResults(const PartResults& );
    PartResults& operator=(const PartResults& );
};

/*
HEAD_NUM, SITE_NUM
    If a test system does not support parallel testing, and does not have a standard way
    to identify its single test site or head, then these fields should be set to 1.
    When parallel testing, these fields are used to associate individual datalogged results
    (FTRsand PTRs) with a PIR/PRR pair. An FTR or PTR belongs to the PIR/PRR pair having
    the same values for HEAD_NUM and SITE_NUM.
X_COORD,Y_COORD
    Have legal values in the range -32767 to 32767. A missing value is indicated by the
    value -32768.
X_COORD, Y_COORD, PART_ID
    Are all optional, but you should provide either the PART_ID or the X_COORD and Y_COORD
    in order to make the resultant data useful for analysis.
PART_FLG
    Contains the following fields:
    bit 0:  0 = This is a new part.
                Its data device does not supersede that of any previous device.
            1 = The PIR, PTR, MPR, FTR,and PRR records that make up the current
                sequence (identified as having the same HEAD_NUM and SITE_NUM)
                supersede any previous sequence of records with the same PART_ID.
                (A repeated part sequence usually indicates a mistested part.)
    bit 1:  0 = This is a new part.
                Its data device does not supersede that of any previous device.
            1 = The PIR, PTR, MPR, FTR,and PRR records that make up the current sequence
            (identified as having the same HEAD_NUM and SITE_NUM)
            supersede any previous sequence of records with the same X_COORD and Y_COORD.
            (A repeated part sequence usually indicates a mistested part.)
            Note: Either Bit 0 or Bit 1 can be set, but not both.
            (It is also valid to have neither set.)
    bit 2:  0 = Part testing completed normally
            1 = Abnormal end of testing
    bit 3:  0 = Part passed
            1 = Part failed
    bit 4:  0 = Pass/fail flag (bit 3) is valid
            1 = Device completed testing with no pass/fail indication
            (i.e., bit 3 is invalid)
    bits 5 -7: Reserved for future use - must be 0
HARD_BIN
    Has legal values in the range 0 to 32767.
SOFT_BIN
    Has legal values in the range 0 to 32767.
    A missing value is indicated by the value 65535.
PART_FIX
    This is an application-specific field for storing device repair information.
    It may be used for bit-encoded, integer, floating point, or character information.
    Regardless of the information stored, the first byte must contain the number of bytes to follow.
    This field can be decoded only by an application-specific analysis program.
*/

class   TestSynopsis //: public Record
{
public:
    U2 REC_LEN  ; // Bytes of data following header
    U1 REC_TYP  ; // Record type (10)
    U1 REC_SUB  ; // Record sub-type (30)
    U1 HEAD_NUM ; // Test head number
    U1 SITE_NUM ; // Test site number
    C1 TEST_TYP ; // Test type ,default: space
    U4 TEST_NUM ; // Test number
    U4 EXEC_CNT ; // Number of test executions ,default: 4,294,967,295
    U4 FAIL_CNT ; // Number of test failures ,default: 4,294,967,295
    U4 ALRM_CNT ; // Number of alarmed tests ,default: 4,294,967,295
    Cn TEST_NAM ; // Test name ,default: length byte = 0
    Cn SEQ_NAME ; // Sequencer (program segment/flow) name ,default: length byte = 0
    Cn TEST_LBL ; // Test label or text ,default: length byte = 0
    B1 OPT_FLAG ; // Optional data flag
    R4 TEST_TIM ; // Average test execution time in seconds ,default: OPT_FLAG bit 2 = 1
    R4 TEST_MIN ; // Lowest test result value ,default: OPT_FLAG bit 0 = 1
    R4 TEST_MAX ; // Highest test result value ,default: OPT_FLAG bit 1 = 1
    R4 TST_SUMS ; // Sum of test result values ,default: OPT_FLAG bit 4 = 1
    R4 TST_SQRS ; // Sum of squares of test result values ,default: OPT_FLAG bit 5 = 1
public:
    TestSynopsis();
    ~TestSynopsis(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    TestSynopsis(const TestSynopsis& );
    TestSynopsis& operator=(const TestSynopsis& );
};


/*
HEAD_NUM
    If this TSR contains a summary of the test counts for all test sites, this field must be
    set to 255.
TEST_TYP
    Indicates what type of test this summary data is for. Valid values are:
        P = Parametric test
        F = Functional test
        M = Multiple-result parametric test
        space = Unknown
EXEC_CNT,FAIL_CNT,ALRM_CNT
    Are optional, but are strongly recommended because they are needed to compute
    values for complete final summary sheets.

OPT_FLAG
    Contains the following fields:
        bit 0 set = TEST_MIN value is invalid
        bit 1 set = TEST_MAX value is invalid
        bit 2 set = TEST_TIM value is invalid
        bit 3 is reserved for future use and must be 1
        bit 4 set = TST_SUMS value is invalid
        bit 5 set = TST_SQRS value is invalid
        bits 6 -7 are reserved for future use and must be 1
    OPT_FLAG is optional if it is the last field in the record.
TST_SUMS,TST_SQRS
    Are useful in calculating the mean and standard deviation for a single lot or when
    combining test data from multiple STDF files.
*/

class   ParametricTest  //: public Record
{
public:
    U2 REC_LEN  ; // Bytes of data following header
    U1 REC_TYP  ; // Record type (15)
    U1 REC_SUB  ; // Record sub-type (10)
    U4 TEST_NUM ; // Test number
    U1 HEAD_NUM ; // Test head number
    U1 SITE_NUM ; // Test site number
    B1 TEST_FLG ; // Test flags (fail, alarm, etc.)
    B1 PARM_FLG ; // Parametric test flags (drift, etc.)
    R4 RESULT   ; // Test result ,default: TEST_FLG bit 1 = 1
    Cn TEST_TXT ; // Test description text or label ,default: length byte = 0
    Cn ALARM_ID ; // Name of alarm ,default: length byte = 0
    B1 OPT_FLAG ; // Optional data flag
    I1 RES_SCAL ; // Test results scaling exponent ,default: OPT_FLAG bit 0 = 1
    I1 LLM_SCAL ; // Low limit scaling exponent ,default: OPT_FLAG bit 4 or 6 = 1
    I1 HLM_SCAL ; // High limit scaling exponent ,default: OPT_FLAG bit 5 or 7 = 1
    R4 LO_LIMIT ; // Low test limit value ,default: OPT_FLAG bit 4 or 6 = 1
    R4 HI_LIMIT ; // High test limit value ,default: OPT_FLAG bit 5 or 7 = 1
    Cn UNITS    ; // Test units ,default: length byte = 0
    Cn C_RESFMT ; // ANSI C result format string ,default: length byte = 0
    Cn C_LLMFMT ; // ANSI C low limit format string ,default: length byte = 0
    Cn C_HLMFMT ; // ANSI C high limit format string ,default: length byte = 0
    R4 LO_SPEC  ; // Low specification limit value ,default: OPT_FLAG bit 2 = 1
    R4 HI_SPEC  ; // High specification limit value ,default: OPT_FLAG bit 3 = 1
public:
    ParametricTest();
    ~ParametricTest(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    ParametricTest(const ParametricTest& );
    ParametricTest& operator=(const ParametricTest& );
};

/*
Default Data
    All data following the OPT_FLAG field has a special function in the STDF file.
    The first PTR for each test will have these fields filled in.
    These values will be the default for each subsequent PTR with the same test number:
    if a subsequent PTR has a value for one of these fields, it will be used instead of the default,
    for that one record only; if the field is blank, the default will be used.
    This method replaces use of the PDR in STDF V3.
    If the PTR is not associated with a test execution (that is, it contains only default information),
    bit 4 of the TEST_FLG field must be set, and the PARM_FLG field must be zero.
    Unless the default is being overridden, the default data fields should be omitted in
    order to save space in the file. Note that RES_SCAL, LLM_SCAL, HLM_SCAL, UNITS, C_RESFMT,
    C_LLMFMT,and C_HLMFMT are interdependent. If you are overriding the default value of one,
    make sure that you also make appropriate changes to the others in order to keep them consistent.
    For character strings, you can override the default with a null value by setting the string
    length to 1 and the string itself to a single binary 0.
HEAD_NUM, SITE_NUM
    If a test system does not support parallel testing, and does not have a standard way of
    identifying its single test site or head, these fields should be set to 1.
    When parallel testing, these fields are used to associate individual datalogged results
    with a PIR/PRR pair. A PTR belongs to the PIR/PRR pair having the same values for HEAD_NUM
    and SITE_NUM.
TEST_FLG
    Contains the following fields:
        bit 0:  0 = No alarm
                1 = Alarm detected during testing
        bit 1:  0 = The value in the RESULT field is valid (see note on RESULT)
                1 = The value in the RESULT field is not valid.
                This setting indicates that the test was executed, but no datalogged value was taken.
                You should read bits 6 and 7 of TEST_FLG to determine if the test passed or failed.
        bit 2:  0 = Test result is reliable
                1 = Test result is unreliable
        bit 3:  0 = No timeout
                1 = Timeout occurred
        bit 4:  0 = Test was executed
                1 = Test not executed
        bit 5:  0 = No abort
                1 = Test aborted
        bit 6:  0 = Pass/fail flag (bit 7) is valid
                1 = Test completed with no pass/fail indication
        bit7:   0 = Test passed
                1 = Test failed
PARM_FLG
    Is the parametric flag field, and contains the following bits:
        bit 0:  0 = No scale error
                1 = Scale error
        bit 1:  0 = No drift error
                1 = Drift error (unstable measurement)
        bit 2:  0 = No oscillation
                1 = Oscillation detected
        bit 3:  0 = Measured value not high
                1 = Measured value higher than high test limit
        bit 4:  0 = Measured value not low
                1 = Measured value lower than low test limit
        bit 5:  0 = Test failed or test passed standard limits
                1 = Test passed alternate limits
        bit 6:  0 = If result = low limit, then result is "fail."
                1 = If result = low limit, then result is "pass."
        bit 7:  0 = If result = high limit, then result is "fail."
                1 = If result = high limit, then result is "pass."
RESULT
    The RESULT value is considered useful only if all the following bits from TEST_FLG and
    PARM_FLG are 0:
        TEST_FLG
            bit 0 = 0 no alarm
            bit 1 = 0 value in result field is valid
            bit 2 = 0 test result is reliable
            bit 3 = 0 no timeout
            bit 4 = 0 test was executed
            bit 5 = 0 no abort
        PARM_FLG
            bit 0 = 0 no scale error
            bit 1 = 0 no drift error
            bit 2 = 0 no oscillation
    If any one of these bits is 1, then the PTR result should not be used.
ALARM_ID
    If the alarm flag (bit 0 of TEST_FLG) is set, this field can contain the name or ID
    of the alarms that were triggered. Alarm names are tester-dependent.
OPT_FLAG
    Is the Optional data flag and contains the following bits:
        bit 0 set = RES_SCAL value is invalid.
              The default set by the first PTR with this test number will be used.
        bit 1 reserved for future used and must be 1.
        bit 2 set = No low specification limit.
        bit 3 set = No high specification limit.
        bit 4 set = LO_LIMIT and LLM_SCAL are invalid.
            The default values set for these fields in the first PTR with this test number will be used.
        bit 5 set = HI_LIMIT and HLM_SCAL are invalid.
            The default values set for these fields in the first PTR with this test number will be used.
        bit 6 set = No Low Limit for this test(LO_LIMIT and LLM_SCAL are invalid).
        bit 7 set = No High Limit for this test (HI_LIMIT and HLM_SCAL are invalid).
            The OPT_FLAG field may be omitted if it is the last field in the record.
C_RESFMT, C_LLMFMT, C_HLMFMT
    ANSI C format strings for use in formatting the test result and low and high limits
    (both test and spec). For example, "%7.2f".The format string is also known as an output
    specification string, as used with the printf statement. See any ANSI C reference man,
    or the man page on printf.
LO_SPEC, HI_SPEC
    The specification limits are set in the first PTR and should never change.
    They use the same scaling and format strings as the corresponding test limits.

*/

class   MultipleResultParametric //: public Record
{
public:
    U2   REC_LEN  ; // Bytes of data following header
    U1   REC_TYP  ; // Record type (15)
    U1   REC_SUB  ; // Record sub-type (15)
    U4   TEST_NUM ; // Test number
    U1   HEAD_NUM ; // Test head number
    U1   SITE_NUM ; // Test site number
    B1   TEST_FLG ; // Test flags (fail, alarm, etc.)
    B1   PARM_FLG ; // Parametric test flags (drift, etc.)
    U2   RTN_ICNT ; // Count of PMR indexes
    U2   RSLT_CNT ; // Count of returned results
    kxN1 RTN_STAT ; // Array of returned states ,default: RTN_ICNT = 0
    kxR4 RTN_RSLT ; // Array of returned results ,default: RSLT_CNT = 0
    Cn   TEST_TXT ; // Descriptive text or label ,default: length byte = 0
    Cn   ALARM_ID ; // Name of alarm ,default: length byte = 0
    B1   OPT_FLAG ; // Optional data flag
    I1   RES_SCAL ; // Test result scaling exponent ,default: OPT_FLAG bit 0 = 1
    I1   LLM_SCAL ; // Test low limit scaling exponent ,default: OPT_FLAG bit 4 or 6 = 1
    I1   HLM_SCAL ; // Test high limit scaling exponent ,default: OPT_FLAG bit 5 or 7 = 1
    R4   LO_LIMIT ; // Test low limit value ,default: OPT_FLAG bit 4 or 6 = 1
    R4   HI_LIMIT ; // Test high limit value ,default: OPT_FLAG bit 5 or 7 = 1
    R4   START_IN ; // Starting input value (condition) ,default: OPT_FLAG bit 1 = 1
    R4   INCR_IN  ; // Increment of input condition ,default: OPT_FLAG bit 1 = 1
    kxU2 RTN_INDX ; // Array of PMR indexes ,default: RTN_ICNT = 0
    Cn   UNITS    ; // Units of returned results ,default: length byte = 0
    Cn   UNITS_IN ; // Input condition units ,default: length byte = 0
    Cn   C_RESFMT ; // ANSI C result format string ,default: length byte = 0
    Cn   C_LLMFMT ; // ANSI C low limit format string ,default: length byte = 0
    Cn   C_HLMFMT ; // ANSI C high limit format string ,default: length byte = 0
    R4   LO_SPEC  ; // Low specification limit value ,default: OPT_FLAG bit 2 = 1
    R4   HI_SPEC  ; // High specification limit value ,default: OPT_FLAG bit 3 = 1
public:
    MultipleResultParametric();
    ~MultipleResultParametric(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    MultipleResultParametric(const MultipleResultParametric& );
    MultipleResultParametric& operator=(const MultipleResultParametric& );
};


/*
Default Data
    All data beginning with the OPT_FLAG field has a special function in the STDF file.
    The first MPR for each test will have these fields filled in. These values will be
    the default for each subsequent MPR with the same test number: if a subsequent MPR
    has a value for one of these fields, it will be used instead of the default,
    for that one record only; if the field is blank, the default will be used.
    If the MPR is not associated with a test execution (that is, it contains only default
    information), bit 4 of the TEST_FLG field must be set, and the PARM_FLG field must be zero.
    Unless the default is being overridden, the default data fields should be omitted in
    order to save space in the file. Note that RES_SCAL, LLM_SCAL, HLM_SCAL, UNITS, C_RESFMT,
    C_LLMFMT,and C_HLMFMT are interdependent.
    If you are overriding the default value of one, make sure that you also make appropriate changes
    to the others in order to keep them consistent. For character strings, you can override the default
    with a null value by setting the string length to 1 and the string itself to a single binary 0.
TEST_NUM
    The test number does not implicitly increment for successive values in the result array.
HEAD_NUM, SITE_NUM
    If a test system does not support parallel testing, and does not have a standard way of
    identifying its single test site or head, these fields should be set to 1.
    When parallel testing, these fields are used to associate individual datalogged results with a
    PIR/PRR pair. An MPR belongs to the PIR/PRR pair having the same values for HEAD_NUM and SITE_NUM.
TEST_FLG
    Contains the following fields:
    bit 0:  0 = No alarm
            1 = Alarm detected during testing
    bit 1: Reserved for future use. Must be zero.
    bit 2:  0 = Test results are reliable
            1 = Test results are unreliable
    bit 3:  0 = No timeout
            1 = Timeout occurred
    bit 4:  0 = Test was executed
            1= Test not executed
    bit 5:  0 = No abort
            1= Test aborted
    bit 6:  0 = Pass/fail flag (bit 7) is valid
            1 = Test completed with no pass/fail indication
    bit7:   0 = Test passed
            1 = Test failed
PARM_FLG
    Is the parametric flag field, and contains the following bits:
    bit 0:  0 = No scale error
            1 = Scale error
    bit 1:  0 = No drift error
            1 = Drift error (unstable measurement)
    bit 2:  0 = No oscillation
            1 = Oscillation detected
    bit 3:  0 = Measured value not high
            1 = Measured value higher than high test limit
    bit 4:  0 = Measured value not low
            1 = Measured value lower than low test limit
    bit 5:  0 = Test failed or test passed standard limits
            1 = Test passed alternate limits
    bit 6:  0 = If result = low limit, then result is "fail."
            1 = If result = low limit, then result is "pass."
    bit 7:  0 = If result = high limit, then result is "fail."
            1 = If result = high limit, then result is "pass."
RTN_ICNT, RTN_INDX, RTN_STAT
    The number of element in the RTN_INDX and RTN_STAT arrays is determined by the value of RTN_ICNT.
    The RTN_STAT field is stored 4 bits per value. The first value is stored in the low order 4 bits
    of the byte. If the number of indexes is odd, the high order 4 bits of the last byte in RTN_STAT
    will be padded with zero. The indexes referred to in the RTN_INDX are the PMR indexes defined in
    the Pin Map Record (PMR). The return state codes are the same as those defined for the RTN_STAT
    field in the FTR. RTN_ICNT may be omitted if it is zero and it is the last field in the record.
RSLT_CNT, RTN_RSLT
    RSLT_CNT defines the number of parametric test results in the RTN_RSLT.If this is a multiple pin
    measurement, and if PMR indexes will be specified, then the value of RSLT_CNT should be the same
    as RTN_ICNT. RTN_RSLT is an array of the parametric test result values.
    RSLT_CNT may be omitted if it is zero and it is the last field in the record.
ALARM_ID
    If the alarm flag (bit 0 of TEST_FLG) is set, this field can contain the name or ID of the alarms
    that were triggered. Alarm names are tester-dependent.
OPT_FLAG
   Is the Optional Data Flag and contains the following bits:
    bit 0 set = RES_SCAL value is invalid.
        The default set by the first MPR with this test number will be used.
    bit 1 set = START_IN and INCR_IN are invalid.
    bit 2 set = No low specification limit.
    bit 3 set = No high specification limit.
    bit 4 set = LO_LIMIT and LLM_SCAL are invalid.
        The default values set for these fields in the first MPR with this test number will be used.
    bit 5 set = HI_LIMIT and HLM_SCAL are invalid.
        The default values set for these fields in the first MPR with this test number will be used.
    bit 6 set= No Low Limit for this test(LO_LIMIT and LLM_SCAL are invalid).
    bit7 set = No High Limit for this test (HI_LIMIT and HLM_SCAL are invalid).
        The OPT_FLAG field may be omitted if it is the last field in the record.
START_IN, INCR_IN, UNITS_IN
    For logging shmoo data, these fields specify the input conditions. START_IN is the beginning
    input value and INCR_IN is the increment, in UNITS_IN units. The input is applied and the
    output measured RSLT_CNT number of times. Values for INCR_IN can be positive or negative.
LO_LIMIT, HI_LIMIT, UNITS
    Regardless of how many test measurements are made, all must use the same limits, units, scaling,
    and significant digits.
C_RESFMT, C_LLMFMT, C_HLMFMT
    ANSI C format strings for use in formatting the test result and low and high limits
    (both test and spec). For example, "%7.2f".The format string is also known as an output
    specification string, as used with the printf statement. See any ANSI C reference man,
    or the man page on printf.
LO_SPEC, HI_SPEC
    The specification limits are set in the first MPR and should never change. They use the same
    scaling and format strings as the corresponding test limits.
*/

class   FunctionalTest //: public Record
{
public:
    U2   REC_LEN  ; // Bytes of data following header
    U1   REC_TYP  ; // Record type (15)
    U1   REC_SUB  ; // Record sub-type (20)
    U4   TEST_NUM ; // Test number
    U1   HEAD_NUM ; // Test head number
    U1   SITE_NUM ; // Test site number
    B1   TEST_FLG ; // Test flags (fail, alarm, etc.)
    B1   OPT_FLAG ; // Optional data flag
    U4   CYCL_CNT ; // Cycle count of vector ,default: OPT_FLAG bit 0 = 1
    U4   REL_VADR ; // Relative vector address ,default: OPT_FLAG bit 1 = 1
    U4   REPT_CNT ; // Repeat count of vector ,default: OPT_FLAG bit 2 = 1
    U4   NUM_FAIL ; // Number of pins with 1 or more failures ,default: OPT_FLAG bit 3 = 1
    I4   XFAIL_AD ; // X logical device failure address ,default: OPT_FLAG bit 4 = 1
    I4   YFAIL_AD ; // Y logical device failure address ,default: OPT_FLAG bit 4 = 1
    I2   VECT_OFF ; // Offset from vector of interest ,default: OPT_FLAG bit 5 = 1
    U2   RTN_ICNT ; // Count of return data PMR indexes
    U2   PGM_ICNT ; // Count of programmed state indexes
    kxU2 RTN_INDX ; // Array of return data PMR indexes ,default: RTN_ICNT = 0
    kxN1 RTN_STAT ; // Array of returned states ,default: RTN_ICNT = 0
    kxU2 PGM_INDX ; // Array of programmed state indexes ,default: PGM_ICNT = 0
    kxN1 PGM_STAT ; // Array of programmed states ,default: PGM_ICNT = 0
    Dn   FAIL_PIN ; // Failing pin bitfield ,default: length bytes = 0
    Cn   VECT_NAM ; // Vector module pattern name ,default: length byte = 0
    Cn   TIME_SET ; // Time set name ,default: length byte = 0
    Cn   OP_CODE  ; // Vector Op Code ,default: length byte = 0
    Cn   TEST_TXT ; // Descriptive text or label ,default: length byte = 0
    Cn   ALARM_ID ; // Name of alarm ,default: length byte = 0
    Cn   PROG_TXT ; // Additional programmed information ,default: length byte = 0
    Cn   RSLT_TXT ; // Additional result information ,default: length byte = 0
    U1   PATG_NUM ; // Pattern generator number ,default: 255
    Dn   SPIN_MAP ; // Bit map of enabled comparators ,default: length byte = 0
public:
    FunctionalTest();
    ~FunctionalTest(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    FunctionalTest(const FunctionalTest& );
    FunctionalTest& operator=(const FunctionalTest& );
};



/*
Default Data
    All data starting with the PATG_NUM field has a special function in the STDF file.
    The first FTR for each test will have these fields filled in. These values will be
    the default for each subsequent FTR with the same test number. If a subsequent FTR
    has a value for one of these fields, it will be used instead of the default, for that
    one record only. If the field is blank, the default will be used. This method replaces
    use of the FDR in STDF V3. Unless the default is being overridden, the default data
    fields should be omitted in order to save space in the file.
HEAD_NUM, SITE_NUM
    If a test system does not support parallel testing, and does not have a standard way
    of identifying its single test site or head, these fields should be set to 1.
    When parallel testing, these fields are used to associate individual datalogged results
    with a PIR/PRR pair. An FTR belongs to the PIR/PRR pair having the same values for
    HEAD_NUM and SITE_NUM.
TEST_FLG
    Contains the following fields:
    bit 0:  0 = No alarm
            1 = Alarm detected during testing
    bit 1: Reserved for future use - must be 0
    bit 2:  0 = Test result is reliable
            1 = Test result is unreliable
    bit 3:  0 = No timeout
            1 = Timeout occurred
    bit 4:  0 = Test was executed
            1 = Test not executed
    bit 5:  0 = No abort
            1 = Test aborted
    bit 6:  0 = Pass/fail flag (bit 7) is valid
            1 = Test completed with no pass/fail indication
    bit7:   0 = Test passed
            1 = Test failed
OPT_FLAG
    Contains the following fields:
    bit 0 set = CYCL_CNT data is invalid
    bit 1 set = REL_VADR data is invalid
    bit 2 set = REPT_CNT data is invalid
    bit 3 set = NUM_FAIL data is invalid
    bit 4 set = XFAIL_AD and YFAIL_AD data are invalid
    bit 5 set = VECT_OFF data is invalid (offset defaults to 0)
    bits 6 -7 are reserved for future use and must be 1
    This field is only optional if it is the last field in the record.
XFAIL_AD, YFAIL_AD
    The logical device address produced by the memory pattern generator, before going through
    conversion to a physical memory address. This logical address can be different from the
    physical address presented to the DUT pins.
VECT_OFF
    This is the integer offset of this vector (in sequence of execution) from the vector of
    interest (usually the failing vector). For example, if this FTR contains data for the
    vector before the vector of interest, this field is set to -1. If this FTR contains data
    for the third vector after the vector of interest, this field is set to 3.
    If this FTR is the vector of interest, VECT_OFF is set to 0. It is therefore possible to
    record an entire sequence of vectors around a failing vector for use with an offline
    debugger or analysis program.
RTN_ICNT, PGM_ICNT
    These fields may be omitted if all data following them is missing or invalid.
RTN_ICNT, RTN_INDX, RTN_STAT
    The size of the RTN_INDX and RTN_STAT arrays is determined by the value of RTN_ICNT.
    The RTN_STAT field is stored 4 bits per value. The first value is stored in the low order
    4 bits of the byte. If the number of indexes is odd, the high order 4 bits of the last byte
    in RTN_STAT will be padded with zero. The indexes referred to in the RTN_INDX are those
    defined in the PMR.
RTN_STAT
    The table of valid returned state values (expressed as hexadecimal digits) is:
        0 = 0orlow
        1 = 1orhigh
        2 = midband
        3 = glitch
        4 = undetermined
        5 = failed low
        6 = failed high
        7 = failed midband
        8 = failed with a glitch
        9 = open
        A = short
    The characters generated to represent these values are tester-dependent, and are specified in the PLR.
PGM_ICNT, PGM_INDX, PGM_STAT
    The size of the PGM_INDX and PGM_STAT arrays is determined by the value of PGM_ICNT.
    The indexes referred to in the PGM_INDX are those defined in the PMR.
PGM_STAT
    The table of valid program state values (expressed in hexadecimal) is listed below.
    Note that there are three defined program modes:
    Normal, Dual Drive (two drive bits per cycle), and SCIO (same cycle I/O).
    The characters generated to represent these values are tester-dependent,
    and are specified in the PLR.
    -----------------------------------------------------------------------------------
    Normal Mode Program States                         |Typical State Representation
    ---------------------------------------------------|-------------------------------
    0 = DriveLow                                       |  0
    1 = Drive High                                     |  1
    2 = Expect Low                                     |  L
    3 = Expect High                                    |  H
    4 = Expect Midband                                 |  M
    5 = Expect Valid (not midband)                     |  V
    6 = Don't drive,or compare.                        |  X
    7 = Keep window open from prior cycle.             |  W
      (used to "stretch" a comparison across cycles)   |
    ------------------------------------------------------------------------------------
    Dual Drive Mode Program States                     |Typical State Representations
    ------------------------------------------------------------------------------------
    0 = Low at D2, Low at D1 times                     |  00       0
    1 = Low at D2, High at D1 times                    |  10       1
    2 = Hi at D2, Low at D1 times                      |  01       2
    3 = Hi at D2,High atD1times                        |  11       3
    4 = Compare Low                                    |  L
    5 = Compare High                                   |  H
    6 = Compare Midband                                |  M
    7 = Don't Compare                                  |  X
    ------------------------------------------------------------------------------------
    SCIO Mode Program States                           | Typical State Representations
    ------------------------------------------------------------------------------------
    0 = Drive Low, Compare Low.                        |  0L       l
    1 = Drive Low, Compare High                        |  0H       h
    2 = Drive Low, Compare Midband                     |  0M       m
    3 = Drive Low, Don't Compare                       |  0X       x
    4 = Drive High, Compare Low.                       |  1L       L
    5 = Drive High, Compare High                       |  1H       H
    6 = Drive High, Compare Midband                    |  1M       M
    7 = Drive High, Don't Compare                      |  1X       X
    -----------------------------------------------------------------------------------
FAIL_PIN
    Encoded with PMR index 0 in bit 0 of the field, PMR index 1 in the 1st position, and so on.
    Bits representing PMR indexes of failing pins are set to 1.
ALARM_ID
    If the alarm flag (bit 0 of TEST_FLG) is set, this field can optionally contain the name or
    ID of the alarm or alarms that were triggered. The names of these alarms are
    tester-dependent.
SPIN_MAP
    This field contains an array of bits corresponding to the PMR index numbers of the
    enabled comparators. The 0th bit corresponds to PMR index 0, the 1st bit corresponds
    to PMR index 1, and so on. Each comparator that is enabled will have its corresponding
    PMR index bit set to 1.
*/

class   BeginProgramSection //: public Record
{
public:
    U2 REC_LEN  ; // Bytes of data following header
    U1 REC_TYP  ; // Record type (20)
    U1 REC_SUB  ; // Record sub-type (10)
    Cn SEQ_NAME ; // Program section (or sequencer) name length byte = 0
public:
    BeginProgramSection();
    ~BeginProgramSection(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    BeginProgramSection(const BeginProgramSection& );
    BeginProgramSection& operator=(const BeginProgramSection& );
};

class   EndProgramSection //: public Record
{
public:
    U2 REC_LEN ; // Bytes of data following header
    U1 REC_TYP ; // Record type (20)
    U1 REC_SUB ; // Record sub-type (20)
public:
    EndProgramSection();
    ~EndProgramSection(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    EndProgramSection(const EndProgramSection& );
    EndProgramSection& operator=(const EndProgramSection& );
};

class   GenericData //: public Record
{
public:
    U2 REC_LEN  ; // Bytes of data following header
    U1 REC_TYP  ; // Record type (50)
    U1 REC_SUB  ; // Record sub-type (10)
    U2 FLD_CNT  ; // Count of data fields in record
    Vn GEN_DATA ; // Data type code and data for one field(Repeat GEN_DATA for each data field)
public:
    GenericData();
    ~GenericData();
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    GenericData(const GenericData& );
    GenericData& operator=(const GenericData& );
};


/*
GEN_DATA
    Is repeated FLD_CNT number of times. Each GEN_DATA field consists of a data type code
    followed by the actual data. The data type code is the first unsigned byte of the field.
    Valid data types are:
        0   =   B*0     Special pad field, of length 0 ( below)
        1   =   U*1     One byte unsigned integer
        2   =   U*2     Two byte unsigned integer
        3   =   U*4     Four byte unsigned integer
        4   =   I*1     One byte signed integer
        5   =   I*2     Two byte signed integer
        6   =   I*4     Four byte signed integer
        7   =   R*4     Four byte floating point number
        8   =   R*8     Eight byte floating point number
        10  =   C*n     Variable length ASCII character string
                        (first byte is string length in bytes)
        11  =   B*n     Variable length binary data string
                        (first byte is string length in bytes)
        12  =   D*n     Bit encoded data
                        (first two bytes of string are length in bits)
        13  =   N*1     Unsigned nibble
*/

class   DatalogText //: public Record
{
public:
    U2 REC_LEN  ; // Bytes of data following header
    U1 REC_TYP  ; // Record type (50)
    U1 REC_SUB  ; // Record sub-type (30)
    Cn TEXT_DAT ; // ASCII text string
public:
    DatalogText();
    ~DatalogText(){};
    unsigned int Parse(const RecordHeader& header);
    unsigned int Unparse(RecordHeader& header);
    void Print(std::ostream& os);

private:
    DatalogText(const DatalogText& );
    DatalogText& operator=(const DatalogText& );
};

#endif // _STDF_V4_INTERNAL_H_

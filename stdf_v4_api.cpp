#include "stdf_v4_api.h"
#include "stdf_v4_internal.h"
#include "debug_api.h"
#include <cstring>
//=====================================================================

static STDF_TYPE make_record_type(int type)
{
	STDF_TYPE record_type;
	switch(type)
	{
	case REC_FAR: record_type = FAR_TYPE;  break;
	case REC_ATR: record_type = ATR_TYPE;  break;
	case REC_MIR: record_type = MIR_TYPE;  break;
	case REC_MRR: record_type = MRR_TYPE;  break;
	case REC_PCR: record_type = PCR_TYPE;  break;
	case REC_HBR: record_type = HBR_TYPE;  break;
	case REC_SBR: record_type = SBR_TYPE;  break;
	case REC_PMR: record_type = PMR_TYPE;  break;
	case REC_PGR: record_type = PGR_TYPE;  break;
	case REC_PLR: record_type = PLR_TYPE;  break;
	case REC_RDR: record_type = RDR_TYPE;  break;
	case REC_SDR: record_type = SDR_TYPE;  break;
	case REC_WIR: record_type = WIR_TYPE;  break;
	case REC_WRR: record_type = WRR_TYPE;  break;
	case REC_WCR: record_type = WCR_TYPE;  break;
	case REC_PIR: record_type = PIR_TYPE;  break;
	case REC_PRR: record_type = PRR_TYPE;  break;
	case REC_TSR: record_type = TSR_TYPE;  break;
	case REC_PTR: record_type = PTR_TYPE;  break;
	case REC_MPR: record_type = MPR_TYPE;  break;
	case REC_FTR: record_type = FTR_TYPE;  break;
	case REC_BPS: record_type = BPS_TYPE;  break;
	case REC_EPS: record_type = EPS_TYPE;  break;
	case REC_GDR: record_type = GDR_TYPE;  break;
	case REC_DTR: record_type = DTR_TYPE;  break;
	default:  record_type = UNKNOWN_TYPE;  break;
	}
	return record_type;
}

StdfHeader::StdfHeader()
{
    header = new RecordHeader();
#ifdef _DEBUG_API_H_
    if(header == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfHeader::~StdfHeader()
{
    delete header;
    header = nullptr;
}

STDF_TYPE StdfHeader::read(std::ifstream& file_stream)
{
    int type =  header->ReadRecord(file_stream);
    return make_record_type(type);
}

int StdfHeader::write(std::ofstream& file_stream)
{
    return header->WriteRecord(file_stream);
}

unsigned short StdfHeader::get_length() const
{
    return header->REC_LEN;
}

STDF_TYPE StdfHeader::get_type() const
{
    int type =  header->GetRecordType();
    return make_record_type(type);;
}

unsigned char StdfHeader::get_main_type() const
{
    return header->REC_TYP;
}

unsigned char StdfHeader::get_sub_type() const
{
    return header->REC_SUB;
}

StdfRecord* StdfHeader::create_record(STDF_TYPE type)
{
    StdfRecord* record = nullptr;
    switch(type)
    {
	case FAR_TYPE:  record = new StdfFAR(); break;
    case ATR_TYPE:  record = new StdfATR(); break;
	case MIR_TYPE:  record = new StdfMIR(); break;
	case MRR_TYPE:  record = new StdfMRR(); break;
	case PCR_TYPE:  record = new StdfPCR(); break;
	case HBR_TYPE:  record = new StdfHBR(); break;
	case SBR_TYPE:  record = new StdfSBR(); break;
	case PMR_TYPE:  record = new StdfPMR(); break;
	case PGR_TYPE:  record = new StdfPGR(); break;
	case PLR_TYPE:  record = new StdfPLR(); break;
	case RDR_TYPE:  record = new StdfRDR(); break;
	case SDR_TYPE:  record = new StdfSDR(); break;
	case WIR_TYPE:  record = new StdfWIR(); break;
	case WRR_TYPE:  record = new StdfWRR(); break;
	case WCR_TYPE:  record = new StdfWCR(); break;
	case PIR_TYPE:  record = new StdfPIR(); break;
	case PRR_TYPE:  record = new StdfPRR(); break;
	case TSR_TYPE:  record = new StdfTSR(); break;
	case PTR_TYPE:  record = new StdfPTR(); break;
	case MPR_TYPE:  record = new StdfMPR(); break;
	case FTR_TYPE:  record = new StdfFTR(); break;
	case BPS_TYPE:  record = new StdfBPS(); break;
	case EPS_TYPE:  record = new StdfEPS(); break;
	case GDR_TYPE:  record = new StdfGDR(); break;
	case DTR_TYPE:  record = new StdfDTR(); break;
	default : record = nullptr; break;
    }
	return record;
}

//=================================================================

//====================================================================
StdfRecord::StdfRecord(const char* name, STDF_TYPE type)
{
	std::strncpy(m_name, name, 3);
	m_name[3] = '\0';
	m_type = type;
}

const char* StdfRecord::name()
{
	return m_name;
}

STDF_TYPE StdfRecord::type()
{
	return m_type;
}

StdfRecord::~StdfRecord()
{

}

std::ostream& operator << (std::ostream& os, const StdfRecord& record)
{
	record.print(os);
	return os;
}
//=====================================================================
StdfFAR::StdfFAR() : StdfRecord("FAR", FAR_TYPE)
{
    impl = new FileAttributes();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfFAR::~StdfFAR()
{
    delete impl;
    impl = nullptr;
}

unsigned char StdfFAR::get_cpu_type() const
{
    return impl->CPU_TYPE;
}

unsigned char StdfFAR::get_stdf_version() const
{
    return impl->STDF_VER;
}

//int StdfFAR::set_cpu_type(unsigned char type)
//{
//  if(type != 2)
//  {
//      std::cout<<"Only Support CPU Type 2."<<std::endl;
//      return -1;
//  }
//  impl->CPU_TYPE = U1(type);
//  return 0;
//}
//
//int StdfFAR::set_stdf_version(unsigned char version)
//{
//  if(version != 4)
//  {
//      std::cout<<"Only Support STDF V4."<<std::endl;
//      return -1;
//  }
//  impl->STDF_VER = U1(4);
//  return 0;
//}

unsigned int StdfFAR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}

unsigned int StdfFAR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfFAR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfATR::StdfATR()  : StdfRecord("ATR", ATR_TYPE)
{
    impl = new AuditTrail();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

//////////////////////////////////////////////////////////////////////////

StdfATR::~StdfATR()
{
    delete impl;
    impl = nullptr;
}

const char* StdfATR::get_command_line() const
{
    if(impl->CMD_LINE.empty()) return nullptr;
    return impl->CMD_LINE.c_str();
}

time_t StdfATR::get_modify_time() const
{
    return (time_t)(impl->MOD_TIM);
}

void StdfATR::set_command_line(const char* command)
{
    if(command) impl->CMD_LINE.assign(command);
}

void StdfATR::set_modify_time(time_t t)
{
    impl->MOD_TIM = U4(t);
}

unsigned int StdfATR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}

unsigned int StdfATR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfATR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfMIR::StdfMIR() : StdfRecord("MIR", MIR_TYPE)
{
    impl = new MasterInformation();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfMIR::~StdfMIR()
{
    delete impl;
    impl = nullptr;
}

time_t StdfMIR::get_setup_time()  const
{
    return (time_t)(impl->SETUP_T);
}

time_t StdfMIR::get_start_time() const
{
    return (time_t)(impl->START_T);
}

unsigned char StdfMIR::get_station_number() const
{
    return impl->STAT_NUM;
}

char StdfMIR::get_mode_code() const
{
    return impl->MODE_COD;
}

char StdfMIR::get_retest_code() const
{
    return impl->RTST_COD;
}

char StdfMIR::get_protection_code() const
{
    return impl->PROT_COD;
}
unsigned short StdfMIR::get_burn_time() const
{
    return impl->BURN_TIM;
}

char StdfMIR::get_command_code() const
{
    return impl->CMOD_COD;
}
const char* StdfMIR::get_lot_id() const
{
    if(impl->LOT_ID.empty()) return nullptr;
    return impl->LOT_ID.c_str();
}
const char* StdfMIR::get_part_type() const
{
    if(impl->PART_TYP.empty()) return nullptr;
    return impl->PART_TYP.c_str();
}

const char* StdfMIR::get_node_name() const
{
    if(impl->NODE_NAM.empty()) return nullptr;
    return impl->NODE_NAM.c_str();
}

const char* StdfMIR::get_tester_type() const
{
    if(impl->TSTR_TYP.empty()) return nullptr;
    return impl->TSTR_TYP.c_str();
}

const char* StdfMIR::get_program_name() const
{
    if(impl->JOB_NAM.empty()) return nullptr;
    return impl->JOB_NAM.c_str();
}

const char* StdfMIR::get_program_revision() const
{
    if(impl->JOB_REV.empty()) return nullptr;
    return impl->JOB_REV.c_str();
}

const char* StdfMIR::get_sublot_id() const
{
    if(impl->SBLOT_ID.empty()) return nullptr;
    return impl->SBLOT_ID.c_str();
}
const char* StdfMIR::get_operator_id() const
{
    if(impl->OPER_NAM.empty()) return nullptr;
    return impl->OPER_NAM.c_str();
}

const char* StdfMIR::get_exec_file_type() const
{
    if(impl->EXEC_TYP.empty()) return nullptr;
    return impl->EXEC_TYP.c_str();
}

const char* StdfMIR::get_exec_file_version() const
{
    if(impl->EXEC_VER.empty()) return nullptr;
    return impl->EXEC_VER.c_str();
}

const char* StdfMIR::get_test_code() const
{
    if(impl->TEST_COD.empty()) return nullptr;
    return impl->TEST_COD.c_str();
}

const char* StdfMIR::get_test_temperature() const
{
    if(impl->TST_TEMP.empty()) return nullptr;
    return impl->TST_TEMP.c_str();
}

const char* StdfMIR::get_user_text() const
{
    if(impl->USER_TXT.empty()) return nullptr;
    return impl->USER_TXT.c_str();
}

const char* StdfMIR::get_auxiliary_filename() const
{
    if(impl->AUX_FILE.empty()) return nullptr;
    return impl->AUX_FILE.c_str();
}

const char* StdfMIR::get_package_type() const
{
    if(impl->PKG_TYP.empty()) return nullptr;
    return impl->PKG_TYP.c_str();
}
const char* StdfMIR::get_family_id() const
{
    if(impl->FAMLY_ID.empty()) return nullptr;
    return impl->FACIL_ID.c_str();
}

const char* StdfMIR::get_date_code() const
{
    if(impl->DATE_COD.empty()) return nullptr;
    return impl->DATE_COD.c_str();
}
const char* StdfMIR::get_facility_id() const
{
    if(impl->FACIL_ID.empty()) return nullptr;
    return impl->FACIL_ID.c_str();
}

const char* StdfMIR::get_floor_id() const
{
    if(impl->FLOOR_ID.empty()) return nullptr;
    return impl->FLOOR_ID.c_str();
}

const char* StdfMIR::get_process_id() const
{
    if(impl->PROC_ID.empty()) return nullptr;
    return impl->PROC_ID.c_str();
}

const char* StdfMIR::get_operation_freq() const
{
    if(impl->OPER_FRQ.empty()) return nullptr;
    return impl->OPER_FRQ.c_str();
}

const char* StdfMIR::get_spec_name() const
{
    if(impl->SPEC_NAM.empty()) return nullptr;
    return impl->SPEC_NAM.c_str();
}

const char* StdfMIR::get_spec_version() const
{
    if(impl->SPEC_VER.empty()) return nullptr;
    return impl->SPEC_VER.c_str();
}

const char* StdfMIR::get_testflow_id() const
{
    if(impl->FLOW_ID.empty()) return nullptr;
    return impl->FLOW_ID.c_str();
}

const char* StdfMIR::get_setup_id() const
{
    if(impl->SETUP_ID.empty()) return nullptr;
    return impl->SETUP_ID.c_str();
}

const char* StdfMIR::get_design_version() const
{
    if(impl->DSGN_REV.empty()) return nullptr;
    return impl->DSGN_REV.c_str();
}

const char* StdfMIR::get_engineering_id() const
{
    if(impl->ENG_ID.empty()) return nullptr;
    return impl->ENG_ID.c_str();
}

const char* StdfMIR::get_rom_id() const
{
    if(impl->ROM_COD.empty()) return nullptr;
    return impl->ROM_COD.c_str();
}

const char* StdfMIR::get_tester_number() const
{
    if(impl->SERL_NUM.empty()) return nullptr;
    return impl->SERL_NUM.c_str();
}

const char* StdfMIR::get_supervisor_name() const
{
    if(impl->SUPR_NAM.empty()) return nullptr;
    return impl->SUPR_NAM.c_str();
}

void StdfMIR::set_setup_time(time_t t)
{
    impl->SETUP_T = U4(t);
}

void StdfMIR::set_start_time(time_t t)
{
    impl->START_T = U4(t);
}

void StdfMIR::set_station_number(unsigned char number)
{
    impl->STAT_NUM = number;
}

void StdfMIR::set_mode_code(ModeCode code)
{
    impl->MODE_COD = code;
}

void StdfMIR::set_retest_code(char code)
{
    impl->RTST_COD = code;
}
void StdfMIR::set_protection_code(char code)
{
    impl->PROT_COD = code;
}

void StdfMIR::set_burn_time(unsigned short t)
{
    impl->BURN_TIM = t;
}

void StdfMIR::set_command_code(char code)
{
    impl->CMOD_COD = code;
}

void StdfMIR::set_lot_id(const char * id)
{
    if(id) impl->LOT_ID.assign(id);
}

void StdfMIR::set_part_type(const char *type)
{
    if(type) impl->PART_TYP.assign(type);
}

void StdfMIR::set_node_name(const char *name)
{
    if(name) impl->NODE_NAM.assign(name);
}

void StdfMIR::set_tester_type(const char *type)
{
    if(type) impl->TSTR_TYP.assign(type);
}

void StdfMIR::set_program_name(const char *name)
{
    if(name) impl->JOB_NAM.assign(name);
}

void StdfMIR::set_program_revision(const char *revision)
{
    if(revision) impl->JOB_REV.assign(revision);
}

void StdfMIR::set_sublot_id(const char *id)
{
    if(id) impl->SBLOT_ID.assign(id);
}

void StdfMIR::set_operator_id(const char *id)
{
    if(id) impl->OPER_NAM.assign(id);
}

void StdfMIR::set_exec_file_type(const char *type)
{
    if(type) impl->EXEC_TYP.assign(type);
}
void StdfMIR::set_exec_file_version(const char *version)
{
    if(version) impl->EXEC_VER.assign(version);
}

void StdfMIR::set_test_code(const char *code)
{
    if(code) impl->TEST_COD.assign(code);
}

void StdfMIR::set_test_temperature(const char *temp)
{
    if(temp) impl->TST_TEMP.assign(temp);
}

void StdfMIR::set_user_text(const char *text)
{
    if(text) impl->USER_TXT.assign(text);
}

void StdfMIR::set_auxiliary_filename(const char *filename)
{
    if(filename) impl->AUX_FILE.assign(filename);
}

void StdfMIR::set_package_type(const char *package)
{
    if(package) impl->PKG_TYP.assign(package);
}

void StdfMIR::set_family_id(const char *id)
{
    if(id) impl->FAMLY_ID.assign(id);
}

void StdfMIR::set_date_code(const char *date)
{
    if(date) impl->DATE_COD.assign(date);
}

void StdfMIR::set_facility_id(const char *id)
{
    if(id) impl->FACIL_ID.assign(id);
}

void StdfMIR::set_floor_id(const char *id)
{
    if(id) impl->FLOOR_ID.assign(id);
}

void StdfMIR::set_process_id(const char *id)
{
    if(id) impl->PROC_ID.assign(id);
}

void StdfMIR::set_operation_freq(const char *freq)
{
    if(freq) impl->OPER_FRQ.assign(freq);
}

void StdfMIR::set_spec_name(const char *name)
{
    if(name) impl->SPEC_NAM.assign(name);
}

void StdfMIR::set_spec_version(const char *version)
{
    if(version) impl->SPEC_VER.assign(version);
}

void StdfMIR::set_testflow_id(const char *id)
{
    if(id) impl->FLOW_ID.assign(id);
}

void StdfMIR::set_setup_id(const char *id)
{
    if(id) impl->SETUP_ID.assign(id);
}
void StdfMIR::set_design_version(const char *version)
{
    if(version) impl->DSGN_REV.assign(version);
}

void StdfMIR::set_engineering_id(const char *lot_id)
{
    if(lot_id) impl->ENG_ID.assign(lot_id);
}

void StdfMIR::set_rom_id(const char *rom_id)
{
    if(rom_id) impl->ROM_COD.assign(rom_id);
}

void StdfMIR::set_tester_number(const char *serial_number)
{
    if(serial_number) impl->SERL_NUM.assign(serial_number);
}

void StdfMIR::set_supervisor_name(const char *name)
{
    if(name) impl->SUPR_NAM.assign(name);
}

unsigned int StdfMIR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}
unsigned int StdfMIR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfMIR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfMRR::StdfMRR() : StdfRecord("MRR", MRR_TYPE)
{
    impl = new MasterResults();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfMRR::~StdfMRR()
{
    delete impl;
    impl = nullptr;
}

time_t StdfMRR::get_finish_time() const
{
    return (time_t)(impl->FINISH_T);
}

char StdfMRR::get_disposition_code() const
{
    return impl->DISP_COD;
}

const char* StdfMRR::get_user_discription() const
{
    if(impl->USR_DESC.empty()) return nullptr;
    return impl->USR_DESC.c_str();
}

const char* StdfMRR::get_exec_discription() const
{
    if(impl->EXC_DESC.empty()) return nullptr;
    return impl->EXC_DESC.c_str();
}

void StdfMRR::set_finish_time(time_t t)
{
    impl->FINISH_T = U4(t);
}

void StdfMRR::set_disposition_code(char code)
{
    impl->DISP_COD = code;
}

void StdfMRR::set_user_discription(const char* discription)
{
    if(discription) impl->USR_DESC.assign(discription);
}

void StdfMRR::set_exec_discription(const char* discription)
{
    if(discription) impl->EXC_DESC.assign(discription);
}

unsigned int StdfMRR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}

unsigned int StdfMRR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfMRR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////

StdfPCR::StdfPCR() : StdfRecord("PCR", PCR_TYPE)
{
    impl = new PartCount();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfPCR::~StdfPCR()
{
    delete impl;
    impl = nullptr;
}

unsigned char StdfPCR::get_head_number() const
{
    return impl->HEAD_NUM;
}

unsigned char StdfPCR::get_site_number() const
{
    return impl->SITE_NUM;
}

unsigned int  StdfPCR::get_part_count() const
{
    return impl->PART_CNT;
}

unsigned int  StdfPCR::get_retest_count() const
{
    return impl->RTST_CNT;
}

unsigned int  StdfPCR::get_abort_count() const
{
    return impl->ABRT_CNT;
}

unsigned int  StdfPCR::get_passed_count() const
{
    return impl->GOOD_CNT;
}

unsigned int  StdfPCR::get_func_test_count() const
{
    return impl->FUNC_CNT;
}

void StdfPCR::set_head_number(unsigned char number)
{
    impl->HEAD_NUM = number;
}

void StdfPCR::set_site_number(unsigned char number)
{
    impl->SITE_NUM = number;
}
void StdfPCR::set_part_count(unsigned int  count)
{
    impl->PART_CNT = count;
}

void StdfPCR::set_retest_count(unsigned int count)
{
    impl->RTST_CNT = count;
}

void StdfPCR::set_abort_count(unsigned int count)
{
    impl->ABRT_CNT = count;
}

void StdfPCR::set_passed_count(unsigned int count)
{
    impl->GOOD_CNT = count;
}

void StdfPCR::set_func_test_count(unsigned int count)
{
    impl->FUNC_CNT = count;
}

unsigned int StdfPCR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}

unsigned int StdfPCR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfPCR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfHBR::StdfHBR() : StdfRecord("HBR", HBR_TYPE)
{
    impl = new HardwareBin();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfHBR::~StdfHBR()
{
    delete impl;
    impl = nullptr;
}

unsigned char StdfHBR::get_head_number() const
{
    return impl->HEAD_NUM;
}

unsigned char StdfHBR::get_site_number() const
{
    return impl->SITE_NUM;
}

unsigned short StdfHBR::get_hardbin_number() const
{
    return impl->HBIN_NUM;
}

unsigned int StdfHBR::get_hardbin_count() const
{
    return impl->HBIN_CNT;
}

char StdfHBR::get_hardbin_indication() const
{
    return impl->HBIN_PF;
}

const char* StdfHBR::get_hardbin_name() const
{
    if(impl->HBIN_NAM.empty()) return nullptr;
    return impl->HBIN_NAM.c_str();
}

void StdfHBR::set_head_number(unsigned char number)
{
    impl->HEAD_NUM = number;
}

void StdfHBR::set_site_number(unsigned char number)
{
    impl->SITE_NUM = number;
}
void StdfHBR::set_hardbin_number(unsigned short number)
{
    impl->HBIN_NUM = number;
}

void StdfHBR::set_hardbin_count(unsigned int count)
{
    impl->HBIN_CNT = count;
}
void StdfHBR::set_hardbin_indication(char pass_fail)
{
    impl->HBIN_PF = pass_fail;
}

void StdfHBR::set_hardbin_name(const char* name)
{
    if(name) impl->HBIN_NAM.assign(name);
}

unsigned int StdfHBR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}

unsigned int StdfHBR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfHBR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfSBR::StdfSBR() : StdfRecord("SBR", SBR_TYPE)
{
    impl = new SoftwareBin();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfSBR::~StdfSBR()
{
    delete impl;
    impl = nullptr;
}

unsigned char StdfSBR::get_head_number() const
{
    return impl->HEAD_NUM;
}

unsigned char StdfSBR::get_site_number() const
{
    return impl->SITE_NUM;
}

unsigned short StdfSBR::get_softbin_number() const
{
    return impl->SBIN_NUM;
}

unsigned int StdfSBR::get_softbin_count() const
{
    return impl->SBIN_CNT;
}

char StdfSBR::get_softbin_indication() const
{
    return impl->SBIN_PF;
}

const char* StdfSBR::get_softbin_name() const
{
    if(impl->SBIN_NAM.empty()) return nullptr;
    return impl->SBIN_NAM.c_str();
}

void StdfSBR::set_head_number(unsigned char number)
{
    impl->HEAD_NUM = number;
}

void StdfSBR::set_site_number(unsigned char number)
{
    impl->SITE_NUM = number;
}

void StdfSBR::set_softbin_number(unsigned short number)
{
    impl->SBIN_NUM = number;
}

void StdfSBR::set_softbin_count(unsigned int count)
{
    impl->SBIN_CNT = count;
}

void StdfSBR::set_softbin_indication(char pass_fail)
{
    impl->SBIN_PF = pass_fail;
}

void StdfSBR::set_softbin_name(const char* name)
{
    if(name) impl->SBIN_NAM.assign(name);
}

unsigned int StdfSBR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}
unsigned int StdfSBR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfSBR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfPMR::StdfPMR() : StdfRecord("PMR", PMR_TYPE)
{
    impl = new PinMap();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfPMR::~StdfPMR()
{
    delete impl;
    impl = nullptr;
}

unsigned short StdfPMR::get_pin_index() const
{
    return impl->PMR_INDX;
}

unsigned short StdfPMR::get_channel_type() const
{
    return impl->CHAN_TYP;
}

const char* StdfPMR::get_channel_name() const
{
    if(impl->CHAN_NAM.empty()) return nullptr;
    return impl->CHAN_NAM.c_str();
}

const char* StdfPMR::get_physical_name() const
{
    if(impl->PHY_NAM.empty()) return nullptr;
    return impl->PHY_NAM.c_str();
}

const char* StdfPMR::get_logical_name() const
{
    if(impl->LOG_NAM.empty()) return nullptr;
    return impl->LOG_NAM.c_str();
}

unsigned char StdfPMR::get_head_number() const
{
    return impl->HEAD_NUM;
}

unsigned char StdfPMR::get_site_number() const
{
    return impl->SITE_NUM;
}

void StdfPMR::set_pin_index(unsigned short index)
{
    impl->PMR_INDX = index;
}

void StdfPMR::set_channel_type(unsigned short type)
{
    impl->CHAN_TYP = type;
}

void StdfPMR::set_channel_name(const char* name)
{
    if(name) impl->CHAN_NAM.assign(name);
}

void StdfPMR::set_physical_name(const char* name)
{
    if(name) impl->PHY_NAM.assign(name);
}

void StdfPMR::set_logical_name(const char* name)
{
    if(name) impl->LOG_NAM.assign(name);
}

void StdfPMR::set_head_number(unsigned char number)
{
    impl->HEAD_NUM = number;
}

void StdfPMR::set_site_number(unsigned char number)
{
    impl->SITE_NUM = number;
}

unsigned int StdfPMR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}

unsigned int StdfPMR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfPMR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfPGR::StdfPGR() : StdfRecord("PGR", PGR_TYPE)
{
    impl = new PinGroup();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfPGR::~StdfPGR()
{
    delete impl;
    impl = nullptr;
}

unsigned short StdfPGR::get_group_index() const
{
    return impl->GRP_INDX;
}

const char* StdfPGR::get_group_name() const
{
    if(impl->GRP_NAM.empty()) return nullptr;
    return impl->GRP_NAM.c_str();
}

unsigned short StdfPGR::get_pin_count() const
{
    return impl->INDX_CNT;
}
unsigned short StdfPGR::get_pin_number(unsigned short index) const
{
    if(index >= impl->PMR_INDX.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->PMR_INDX.size(), index);
        throw INDEX_OVER_RANGE( impl->PMR_INDX.size(), index);
#endif
        return (unsigned short)(-1);
    }

    return impl->PMR_INDX[index];
}

void StdfPGR::set_group_index(unsigned short index)
{
    impl->GRP_INDX = index;
}

void StdfPGR::set_group_name(const char* name)
{
    if(name) impl->GRP_NAM.assign(name);
}

void StdfPGR::set_pin_count(unsigned short count)
{
    impl->INDX_CNT = count;
    //impl->PMR_INDX.clear();
    impl->PMR_INDX.resize(count);
}

void StdfPGR::set_pin_number(unsigned short index, unsigned short pin_number)
{
    if(index >= impl->PMR_INDX.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->PMR_INDX.size(), index);
        throw INDEX_OVER_RANGE(impl->PMR_INDX.size(), index);
#endif
        return;
    }

    impl->PMR_INDX[index] = pin_number;
}

unsigned int StdfPGR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}
unsigned int StdfPGR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfPGR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfPLR::StdfPLR() : StdfRecord("PLR", PLR_TYPE)
{
    impl = new PinList();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfPLR::~StdfPLR()
{
    delete impl;
    impl = nullptr;
}

unsigned short StdfPLR::get_group_count() const
{
    return impl->GRP_CNT;
}

unsigned short StdfPLR::get_group_number(unsigned short index) const
{
    if(index >= impl->GRP_INDX.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->GRP_INDX.size(), index);
        throw INDEX_OVER_RANGE(impl->GRP_INDX.size(), index);
#endif
        return (unsigned short)(-1);
    }
    return impl->GRP_INDX[index];
}

unsigned short StdfPLR::get_group_mode(unsigned short index) const
{
    if(index >= impl->GRP_MODE.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->GRP_MODE.size(), index);
        throw INDEX_OVER_RANGE(impl->GRP_MODE.size(), index);
#endif
        return 0;
    }
    return impl->GRP_MODE[index];
}

unsigned char StdfPLR::get_group_radix(unsigned short index) const
{
    if(index >= impl->GRP_RADX.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->GRP_RADX.size(), index);
        throw INDEX_OVER_RANGE( impl->GRP_RADX.size(), index);
#endif
        return 0;
    }
    return impl->GRP_RADX[index];
}

const char* StdfPLR::get_program_state_right(unsigned short index) const
{
    if(index >= impl->PGM_CHAR.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->PGM_CHAR.size(), index);
        throw INDEX_OVER_RANGE( impl->PGM_CHAR.size(), index);
#endif
        return nullptr;
    }
    if(impl->PGM_CHAR[index].empty()) return nullptr;
    return impl->PGM_CHAR[index].c_str();
}

const char* StdfPLR::get_return_state_right(unsigned short index) const
{
    if(index >= impl->RTN_CHAR.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->RTN_CHAR.size(), index);
        throw INDEX_OVER_RANGE( impl->RTN_CHAR.size(), index);
#endif
        return nullptr;
    }
    if(impl->RTN_CHAR[index].empty()) return nullptr;
    return impl->RTN_CHAR[index].c_str();
}

const char* StdfPLR::get_program_state_left(unsigned short index) const
{
    if(index >= impl->PGM_CHAL.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->PGM_CHAL.size(), index);
        throw INDEX_OVER_RANGE(impl->PGM_CHAL.size(), index);
#endif
        return nullptr;
    }
    if(impl->PGM_CHAL[index].empty()) return nullptr;
    return impl->PGM_CHAL[index].c_str();
}

const char* StdfPLR::get_return_state_left(unsigned short index) const
{
    if(index >= impl->RTN_CHAL.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->RTN_CHAL.size(), index);
        throw INDEX_OVER_RANGE(impl->RTN_CHAL.size(), index);
#endif
        return nullptr;
    }
    if(impl->RTN_CHAL[index].empty()) return nullptr;
    return impl->RTN_CHAL[index].c_str();
}

void StdfPLR::set_group_count(unsigned short count)
{
    impl->GRP_CNT = count;

    impl->GRP_INDX.resize(count);
    impl->GRP_MODE.resize(count);
    impl->GRP_RADX.resize(count);
    impl->PGM_CHAL.resize(count);
    impl->PGM_CHAR.resize(count);
    impl->RTN_CHAL.resize(count);
    impl->RTN_CHAR.resize(count);
}

void StdfPLR::set_group_number(unsigned short index, unsigned short number)
{
    if(index >= impl->GRP_INDX.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->GRP_INDX.size(), index);
        throw INDEX_OVER_RANGE( impl->GRP_INDX.size(), index);
#endif
        return;
    }
    impl->GRP_INDX[index] = number;
}

void StdfPLR::set_group_mode(unsigned short index, GroupMode mode)
{
    if(index >= impl->GRP_MODE.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->GRP_MODE.size(), index);
        throw INDEX_OVER_RANGE(impl->GRP_MODE.size(), index);
#endif
        return;
    }
    impl->GRP_MODE[index] = mode;
}

void StdfPLR::set_group_radix(unsigned short index, GroupRadix radix)
{
    if(index >= impl->GRP_RADX.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->GRP_RADX.size(), index);
        throw INDEX_OVER_RANGE(impl->GRP_RADX.size(), index);
#endif
        return;
    }
    impl->GRP_RADX[index] = radix;
}

void StdfPLR::set_program_state_right(unsigned short index, const char* state)
{
    if(index >= impl->PGM_CHAR.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->PGM_CHAR.size(), index);
        throw INDEX_OVER_RANGE(impl->PGM_CHAR.size(), index);
#endif
        return;
    }
    if(state) impl->PGM_CHAR[index].assign(state);
}

void StdfPLR::set_return_state_right(unsigned short index, const char* state)
{
    if(index >= impl->RTN_CHAR.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->RTN_CHAR.size(), index);
        throw INDEX_OVER_RANGE(impl->RTN_CHAR.size(), index);
#endif
        return;
    }
    if(state) impl->RTN_CHAR[index].assign(state);
}

void StdfPLR::set_program_state_left(unsigned short index, const char* state)
{
    if(index >= impl->PGM_CHAL.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->PGM_CHAL.size(), index);
        throw INDEX_OVER_RANGE(impl->PGM_CHAL.size(), index);
#endif
        return;
    }
    if(state) impl->PGM_CHAL[index].assign(state);
}

void StdfPLR::set_return_state_left(unsigned short index, const char* state)
{
    if(index >= impl->RTN_CHAL.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->RTN_CHAL.size(), index);
        throw INDEX_OVER_RANGE( impl->RTN_CHAL.size(), index);
#endif
        return;
    }
    if(state) impl->RTN_CHAL[index].assign(state);
}

unsigned int StdfPLR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}

unsigned int StdfPLR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfPLR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfRDR::StdfRDR() : StdfRecord("RDR", RDR_TYPE)
{
    impl = new RetestData();
#ifdef _DEBUG_API_H_
    if(impl == nullptr) {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfRDR::~StdfRDR()
{
    delete impl;
    impl = nullptr;
}

unsigned short StdfRDR::get_bin_count() const
{
    return impl->NUM_BINS;
}

unsigned short StdfRDR::get_bin_number(unsigned short index) const
{
    if(index >= impl->RTST_BIN.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->RTST_BIN.size(), index);
        throw INDEX_OVER_RANGE(impl->RTST_BIN.size(), index);
#endif
        return (unsigned short)(-1);
    }
    return impl->RTST_BIN[index];
}

void StdfRDR::set_bin_count(unsigned short count)
{
    impl->NUM_BINS = count;
    impl->RTST_BIN.resize(count);
}

void StdfRDR::set_bin_number(unsigned short index, unsigned short bin_number)
{
    if(index >= impl->RTST_BIN.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->RTST_BIN.size(), index);
        throw INDEX_OVER_RANGE(impl->RTST_BIN.size(), index);
#endif
        return;
    }
    impl->RTST_BIN[index] = bin_number;
}

unsigned int StdfRDR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}

unsigned int StdfRDR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfRDR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfSDR::StdfSDR() : StdfRecord("SDR", SDR_TYPE)
{
    impl = new SiteDescription();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfSDR::~StdfSDR()
{
    delete impl;
    impl = nullptr;
}

unsigned char StdfSDR::get_head_number() const
{
    return impl->HEAD_NUM;
}

unsigned char StdfSDR::get_site_group_number() const
{
    return impl->SITE_GRP;
}

unsigned char StdfSDR::get_site_count() const
{
    return impl->SITE_CNT;
}

unsigned char StdfSDR::get_site_number(unsigned char index) const
{
    if(index >= impl->SITE_NUM.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->SITE_NUM.size(), index);
        throw INDEX_OVER_RANGE( impl->SITE_NUM.size(), index);
#endif
        return 0;
    }
    return impl->SITE_NUM[index];
}

const char* StdfSDR::get_handler_type() const
{
    if(impl->HAND_TYP.empty()) return nullptr;
    return impl->HAND_TYP.c_str();
}

const char* StdfSDR::get_handler_id() const
{
    if(impl->HAND_ID.empty()) return nullptr;
    return impl->HAND_ID.c_str();
}

const char* StdfSDR::get_probecard_type() const
{
    if(impl->CABL_TYP.empty()) return nullptr;
    return impl->CARD_TYP.c_str();
}

const char* StdfSDR::get_probecard_id() const
{
    if(impl->CABL_ID.empty()) return nullptr;
    return impl->CARD_ID.c_str();
}

const char* StdfSDR::get_loadboard_type() const
{
    if(impl->LOAD_TYP.empty()) return nullptr;
    return impl->LOAD_TYP.c_str();
}

const char* StdfSDR::get_loadboard_id() const
{
    if(impl->LOAD_ID.empty()) return nullptr;
    return impl->LOAD_ID.c_str();
}

const char* StdfSDR::get_dibboard_type() const
{
    if(impl->DIB_TYP.empty()) return nullptr;
    return impl->DIB_TYP.c_str();
}

const char* StdfSDR::get_dibboard_id() const
{
    if(impl->DIB_ID.empty()) return nullptr;
    return impl->DIB_ID.c_str();
}

const char* StdfSDR::get_cable_type() const
{
    if(impl->CABL_TYP.empty()) return nullptr;
    return impl->CABL_TYP.c_str();
}

const char* StdfSDR::get_cable_id() const
{
    if(impl->CARD_ID.empty()) return nullptr;
    return impl->CABL_ID.c_str();
}

const char* StdfSDR::get_contactor_type() const
{
    if(impl->CONT_TYP.empty()) return nullptr;
    return impl->CONT_TYP.c_str();
}

const char* StdfSDR::get_contactor_id() const
{
    if(impl->CONT_ID.empty()) return nullptr;
    return impl->CONT_ID.c_str();
}

const char* StdfSDR::get_laser_type() const
{
    if(impl->LASR_TYP.empty()) return nullptr;
    return impl->LASR_TYP.c_str();
}

const char* StdfSDR::get_laser_id() const
{
    if(impl->LASR_ID.empty()) return nullptr;
    return impl->LASR_ID.c_str();
}

const char* StdfSDR::get_equipment_type() const
{
    if(impl->EXTR_TYP.empty()) return nullptr;
    return impl->EXTR_TYP.c_str();
}

const char* StdfSDR::get_equipment_id() const
{
    if(impl->EXTR_ID.empty()) return nullptr;
    return impl->EXTR_ID.c_str();
}

void StdfSDR::set_head_number(unsigned char number)
{
    impl->HEAD_NUM = number;
}

void StdfSDR::set_site_group_number(unsigned char number)
{
    impl->SITE_GRP = number;
}

void StdfSDR::set_site_count(unsigned char count)
{
    impl->SITE_CNT = count;
    impl->SITE_NUM.resize(count);
}

void StdfSDR::set_site_number(unsigned char index, unsigned char site_number)
{
    if(index >= impl->SITE_NUM.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->SITE_NUM.size(), index);
        throw INDEX_OVER_RANGE( impl->SITE_NUM.size(), index);
#endif
        return;
    }
    impl->SITE_NUM[index] = site_number;
}

void StdfSDR::set_handler_type(const char* type)
{
    if(type) impl->HAND_TYP.assign(type);
}

void StdfSDR::set_handler_id(const char* id)
{
    if(id) impl->HAND_ID.assign(id);
}

void StdfSDR::set_probecard_type(const char* type)
{
    if(type) impl->CARD_TYP.assign(type);
}

void StdfSDR::set_probecard_id(const char* id)
{
    if(id) impl->CARD_ID.assign(id);
}

void StdfSDR::set_loadboard_type(const char* type)
{
    if(type) impl->LOAD_TYP.assign(type);
}

void StdfSDR::set_loadboard_id(const char* id)
{
    if(id) impl->LOAD_ID.assign(id);
}

void StdfSDR::set_dibboard_type(const char* type)
{
    if(type) impl->DIB_TYP.assign(type);
}

void StdfSDR::set_dibboard_id(const char* id)
{
    if(id) impl->DIB_ID.assign(id);
}

void StdfSDR::set_cable_type(const char* type)
{
    if(type) impl->CABL_TYP.assign(type);
}

void StdfSDR::set_cable_id(const char* id)
{
    if(id) impl->CABL_ID.assign(id);
}

void StdfSDR::set_contactor_type(const char* type)
{
    if(type) impl->CONT_TYP.assign(type);
}

void StdfSDR::set_contactor_id(const char* id)
{
    if(id) impl->CONT_ID.assign(id);
}

void StdfSDR::set_laser_type(const char* type)
{
    if(type) impl->LASR_TYP.assign(type);
}

void StdfSDR::set_laser_id(const char* id)
{
    if(id) impl->LASR_ID.assign(id);
}

void StdfSDR::set_equipment_type(const char* type)
{
    if(type) impl->EXTR_TYP.assign(type);
}

void StdfSDR::set_equipment_id(const char* id)
{
    if(id) impl->EXTR_ID.assign(id);
}

unsigned int StdfSDR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}

unsigned int StdfSDR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfSDR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfWIR::StdfWIR() : StdfRecord("WIR", WIR_TYPE)
{
    impl = new WaferInformation();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfWIR::~StdfWIR()
{
    delete impl;
    impl = nullptr;
}

unsigned char StdfWIR::get_head_number() const
{
    return impl->HEAD_NUM;
}

unsigned char StdfWIR::get_group_number() const
{
    return impl->SITE_GRP;
}

time_t StdfWIR::get_start_time() const
{
    return (time_t)(impl->START_T);
}

const char* StdfWIR::get_wafer_id() const
{
    if(impl->WAFER_ID.empty()) return nullptr;
    return impl->WAFER_ID.c_str();
}

void StdfWIR::set_head_number(unsigned char number)
{
    impl->HEAD_NUM = number;
}

void StdfWIR::set_group_number(unsigned char number)
{
    impl->SITE_GRP = number;
}

void StdfWIR::set_start_time(time_t t)
{
    impl->START_T = U4(t);
}

void StdfWIR::set_wafer_id(const char* id)
{
    if(id) impl->WAFER_ID.assign(id);
}

unsigned int StdfWIR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}

unsigned int StdfWIR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfWIR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfWRR::StdfWRR() : StdfRecord("WRR", WRR_TYPE)
{
    impl = new WaferResults();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfWRR::~StdfWRR()
{
    delete impl;
    impl = nullptr;
}

unsigned char StdfWRR::get_head_number() const
{
    return impl->HEAD_NUM;
}

unsigned char StdfWRR::get_group_number() const
{
    return impl->SITE_GRP;
}

time_t StdfWRR::get_finish_time() const
{
    return (time_t)(impl->FINISH_T);
}

unsigned int StdfWRR::get_part_count() const
{
    return impl->PART_CNT;
}

unsigned int StdfWRR::get_retest_count() const
{
    return impl->RTST_CNT;
}

unsigned int StdfWRR::get_abort_count() const
{
    return impl->ABRT_CNT;
}

unsigned int StdfWRR::get_pass_count() const
{
    return impl->GOOD_CNT;
}

unsigned int StdfWRR::get_func_count() const
{
    return impl->FUNC_CNT;
}

const char* StdfWRR::get_wafer_id() const
{
    if(impl->WAFER_ID.empty()) return nullptr;
    return impl->WAFER_ID.c_str();
}

const char* StdfWRR::get_fabwafer_id() const
{
    if(impl->FABWF_ID.empty()) return nullptr;
    return impl->FABWF_ID.c_str();
}

const char* StdfWRR::get_frame_id() const
{
    if(impl->FRAME_ID.empty()) return nullptr;
    return impl->FRAME_ID.c_str();
}

const char* StdfWRR::get_mask_id() const
{
    if(impl->MASK_ID.empty()) return nullptr;
    return impl->MASK_ID.c_str();
}

const char* StdfWRR::get_user_discription() const
{
    if(impl->USR_DESC.empty()) return nullptr;
    return impl->USR_DESC.c_str();
}

const char* StdfWRR::get_exec_discription() const
{
    if(impl->EXC_DESC.empty()) return nullptr;
    return impl->EXC_DESC.c_str();
}

void StdfWRR::set_head_number(unsigned char number)
{
    impl->HEAD_NUM = number;
}

void StdfWRR::set_group_number(unsigned char number)
{
    impl->SITE_GRP = number;
}

void StdfWRR::set_finish_time(time_t  t)
{
    impl->FINISH_T = U4(t);
}

void StdfWRR::set_part_count(unsigned int count)
{
    impl->PART_CNT = count;
}

void StdfWRR::set_retest_count(unsigned int count)
{
    impl->RTST_CNT = count;
}

void StdfWRR::set_abort_count(unsigned int count)
{
    impl->ABRT_CNT = count;
}

void StdfWRR::set_pass_count(unsigned int count)
{
    impl->GOOD_CNT = count;
}

void StdfWRR::set_func_count(unsigned int count)
{
    impl->FUNC_CNT = count;
}

void StdfWRR::set_wafer_id(const char* id)
{
    if(id) impl->WAFER_ID.assign(id);
}

void StdfWRR::set_fabwafer_id(const char* id)
{
    if(id) impl->FABWF_ID.assign(id);
}

void StdfWRR::set_frame_id(const char* id)
{
    if(id) impl->FRAME_ID.assign(id);
}

void StdfWRR::set_mask_id(const char* id)
{
    if(id) impl->MASK_ID.assign(id);
}

void StdfWRR::set_user_discription(const char* text)
{
    if(text) impl->USR_DESC.assign(text);
}

void StdfWRR::set_exec_discription(const char* text)
{
    if(text) impl->EXC_DESC.assign(text);
}

unsigned int StdfWRR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}

unsigned int StdfWRR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfWRR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfWCR::StdfWCR() : StdfRecord("WCR", WCR_TYPE)
{
    impl = new WaferConfiguration();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfWCR::~StdfWCR()
{
    delete impl;
    impl = nullptr;
}

float StdfWCR::get_wafer_size() const
{
    return impl->WAFR_SIZ;
}

float StdfWCR::get_die_height() const
{
    return impl->DIE_HT;
}

float StdfWCR::get_die_width() const
{
    return impl->DIE_WID;
}

unsigned char StdfWCR::get_wafer_unit() const
{
    return impl->WF_UNITS;
}

char StdfWCR::get_wafer_flat() const
{
    return impl->WF_FLAT;
}

short StdfWCR::get_center_x() const
{
    return impl->CENTER_X;
}

short StdfWCR::get_center_y() const
{
    return impl->CENTER_Y;
}

char StdfWCR::get_positive_x() const
{
    return impl->POS_X;
}

char StdfWCR::get_positive_y() const
{
    return impl->POS_Y;
}

void StdfWCR::set_wafer_size(float size)
{
    impl ->WAFR_SIZ = size;
}

void StdfWCR::set_die_height(float height)
{
    impl->DIE_HT = height;
}

void StdfWCR::set_die_width(float width)
{
    impl->DIE_WID = width;
}

void StdfWCR::set_wafer_unit(WaferUnits unit)
{
    impl->WF_UNITS = unit;
}

void StdfWCR::set_wafer_flat(WaferFlat flat_orientation)
{
    impl->WF_FLAT = flat_orientation;
}

void StdfWCR::set_center_x(short x)
{
    impl->CENTER_X = x;
}

void StdfWCR::set_center_y(short y)
{
    impl->CENTER_Y = y;
}

void StdfWCR::set_positive_x(PosDirection pos)
{
    impl->POS_X = pos;
}

void StdfWCR::set_positive_y(PosDirection pos)
{
    impl->POS_Y = pos;
}

unsigned int StdfWCR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}

unsigned int StdfWCR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfWCR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfPIR::StdfPIR() : StdfRecord("PIR", PIR_TYPE)
{
	impl = new PartInformation();
#ifdef _DEBUG_API_H_
	if(impl == nullptr)
	{
		SHOW_MEMORY_ERROR();
		throw MEMORY_ALLOCATED_ERROR();
	}
#endif
}

StdfPIR::~StdfPIR()
{
	delete impl;
	impl = nullptr;
}

unsigned char StdfPIR::get_head_number() const
{
	return impl->HEAD_NUM;
}

unsigned char StdfPIR::get_site_number() const
{
	return impl->SITE_NUM;
}

void StdfPIR::set_head_number(unsigned char number)
{
	impl->HEAD_NUM = number;
}

void StdfPIR::set_site_number(unsigned char number)
{
	impl->SITE_NUM = number;
}

unsigned int StdfPIR::parse(const StdfHeader& record)
{
	return impl->Parse(*(record.header));
}

unsigned int StdfPIR::unparse(StdfHeader& record)
{
	return impl->Unparse(*(record.header));
}

void StdfPIR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfPRR::StdfPRR() : StdfRecord("PRR", PRR_TYPE)
{
    impl = new PartResults();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfPRR::~StdfPRR()
{
    delete impl;
    impl = nullptr;
}

unsigned char StdfPRR::get_head_number() const
{
    return impl->HEAD_NUM;
}

unsigned char StdfPRR::get_site_number() const
{
    return impl->SITE_NUM;
}

unsigned char StdfPRR::get_part_information_flag() const
{
	return (unsigned char)(impl->PART_FLG.to_ulong());
}

unsigned short StdfPRR::get_number_test() const
{
    return impl->NUM_TEST;
}

unsigned short StdfPRR::get_hardbin_number() const
{
    return impl->HARD_BIN;
}

unsigned short StdfPRR::get_softbin_number() const
{
    return impl->SOFT_BIN;
}

short StdfPRR::get_x_coordinate() const
{
    return impl->X_COORD;
}

short StdfPRR::get_y_coordinate() const
{
    return impl->Y_COORD;
}

unsigned int StdfPRR::get_elapsed_ms() const
{
    return impl->TEST_T;
}

const char* StdfPRR::get_part_id() const
{
    if(impl->PART_ID.empty()) return nullptr;
    return impl->PART_ID.c_str();
}

const char* StdfPRR::get_part_discription() const
{
    if(impl->PART_TXT.empty()) return nullptr;
    return impl->PART_TXT.c_str();
}

bool StdfPRR::part_supersede_flag() const
{
    return (impl->PART_FLG[0] || impl->PART_FLG[1]);
}

bool StdfPRR::part_abnormal_flag() const
{
    return impl->PART_FLG[2];
}

bool StdfPRR::part_failed_flag() const
{
    return impl->PART_FLG[3];
}

bool StdfPRR::pass_fail_flag_invalid() const
{
    return impl->PART_FLG[4];
}

void StdfPRR::set_head_number(unsigned char number)
{
    impl->HEAD_NUM = number;
}

void StdfPRR::set_site_number(unsigned char number)
{
    impl->SITE_NUM = number;
}

void StdfPRR::set_number_test(unsigned short count)
{
    impl->NUM_TEST = count;
}

void StdfPRR::set_hardbin_number(unsigned short number)
{
    impl->HARD_BIN = number;
}

void StdfPRR::set_softbin_number(unsigned short number)
{
    impl->SOFT_BIN = number;
}

void StdfPRR::set_x_coordinate(short x)
{
    impl->X_COORD = x;
}

void StdfPRR::set_y_coordinate(short y)
{
    impl->Y_COORD = y;
}

void StdfPRR::set_elapsed_ms(unsigned int time_ms)
{
    impl->TEST_T = time_ms;
}

void StdfPRR::set_part_discription(const char* text)
{
    if(text) impl->PART_TXT.assign(text);
}

void StdfPRR::set_part_id(const char* id)
{
    if(id) impl->PART_ID.assign(id);
}

void StdfPRR::part_supersede_flag(bool flag)
{
    impl->PART_FLG[0] = flag;
    impl->PART_FLG[1] = false;
}

void StdfPRR::part_abnormal_flag(bool flag)
{
    impl->PART_FLG[2] = flag;
}
void StdfPRR::part_failed_flag(bool flag)
{
    impl->PART_FLG[3] = flag;
}

void StdfPRR::pass_fail_flag_invalid(bool flag)
{
    impl->PART_FLG[4] = flag;
}

unsigned int StdfPRR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}

unsigned int StdfPRR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfPRR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfTSR::StdfTSR() : StdfRecord("TSR", TSR_TYPE)
{
    impl = new TestSynopsis();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfTSR::~StdfTSR()
{
    delete impl;
    impl = nullptr;
}

unsigned char StdfTSR::get_head_number() const
{
    return impl->HEAD_NUM;
}

unsigned char StdfTSR::get_site_number() const
{
    return impl->SITE_NUM;
}

char StdfTSR::get_test_type() const
{
    return impl->TEST_TYP;
}

unsigned int StdfTSR::get_test_number() const
{
    return impl->TEST_NUM;
}

unsigned int StdfTSR::get_exec_count() const
{
    return impl->EXEC_CNT;
}

unsigned int StdfTSR::get_fail_count() const
{
    return impl->FAIL_CNT;
}

unsigned int StdfTSR::get_alarm_count() const
{
    return impl->ALRM_CNT;
}

const char* StdfTSR::get_test_name() const
{
    if(impl->TEST_NAM.empty()) return nullptr;
    return impl->TEST_NAM.c_str();
}

const char* StdfTSR::get_sequencer_name() const
{
    if(impl->SEQ_NAME.empty()) return nullptr;
    return impl->SEQ_NAME.c_str();
}

const char* StdfTSR::get_test_label() const
{
    if(impl->TEST_LBL.empty()) return nullptr;
    return impl->TEST_LBL.c_str();
}

unsigned char StdfTSR::get_optional_data_flag() const
{
	return (unsigned char)(impl->OPT_FLAG.to_ulong());
}

float StdfTSR::get_average_time_s() const
{
    return impl->TEST_TIM;
}

float StdfTSR::get_result_min() const
{
    return impl->TEST_MIN;
}

float StdfTSR::get_result_max() const
{
    return impl->TEST_MAX;
}

float StdfTSR::get_result_sum() const
{
    return impl->TST_SUMS;
}

float StdfTSR::get_result_squares_sum() const
{
    return impl->TST_SQRS;
}

bool StdfTSR::result_min_invalid() const
{
    return impl->OPT_FLAG[0];
}

bool StdfTSR::result_max_invalid() const
{
    return impl->OPT_FLAG[1];
}

bool StdfTSR::test_time_invalid() const
{
    return impl->OPT_FLAG[2];
}

bool StdfTSR::result_sum_invalid() const
{
    return impl->OPT_FLAG[4];
}

bool StdfTSR::result_squares_sum_invalid() const
{
    return impl->OPT_FLAG[5];
}

void StdfTSR::set_head_number(unsigned char number)
{
    impl->HEAD_NUM = number;
}

void StdfTSR::set_site_number(unsigned char number)
{
    impl->SITE_NUM = number;
}

void StdfTSR::set_test_type(TestType type)
{
    impl->TEST_TYP = type;
}

void StdfTSR::set_test_number(unsigned int number)
{
    impl->TEST_NUM = number;
}

void StdfTSR::set_exec_count(unsigned int count)
{
    impl->EXEC_CNT = count;
}

void StdfTSR::set_fail_count(unsigned int count)
{
    impl->FAIL_CNT = count;
}

void StdfTSR::set_alarm_count(unsigned int count)
{
    impl->ALRM_CNT = count;
}

void StdfTSR::set_test_name(const char* name)
{
    if(name) impl->TEST_NAM.assign(name);
}

void StdfTSR::set_sequencer_name(const char* name)
{
    if(name) impl->SEQ_NAME.assign(name);
}

void StdfTSR::set_test_label(const char* label)
{
    if(label) impl->TEST_LBL.assign(label);
}

void StdfTSR::set_average_time_s(float seconds)
{
    impl->TEST_TIM = seconds;
    impl->OPT_FLAG[2] = false;
}

void StdfTSR::set_result_min(float value)
{
    impl->TEST_MIN = value;
    impl->OPT_FLAG[0] = false;
}

void StdfTSR::set_result_max(float value)
{
    impl->TEST_MAX = value;
    impl->OPT_FLAG[1] = false;
}

void StdfTSR::set_result_sum(float value)
{
    impl->TST_SUMS = value;
    impl->OPT_FLAG[4] = false;
}

void StdfTSR::set_result_squares_sum(float value)
{
    impl->TST_SQRS = value;
    impl->OPT_FLAG[5] = false;
}

void StdfTSR::result_min_invalid(bool flag)
{
    impl->OPT_FLAG[0] = flag;
}

void StdfTSR::result_max_invalid(bool flag)
{
    impl->OPT_FLAG[1] = flag;
}

void StdfTSR::test_time_invalid(bool flag)
{
    impl->OPT_FLAG[2] = flag;
}

void StdfTSR::result_sum_invalid(bool flag)
{
    impl->OPT_FLAG[4] = flag;
}

void StdfTSR::result_squares_sum_invalid(bool flag)
{
    impl->OPT_FLAG[5] = flag;
}

unsigned int StdfTSR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}

unsigned int StdfTSR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfTSR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfPTR::StdfPTR() : StdfRecord("PTR", PTR_TYPE)
{
    impl = new ParametricTest();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfPTR::~StdfPTR()
{
    delete impl;
    impl = nullptr;
}

unsigned int StdfPTR::get_test_number() const
{
    return impl->TEST_NUM;
}

unsigned char StdfPTR::get_head_number() const
{
    return impl->HEAD_NUM;
}

unsigned char StdfPTR::get_site_number() const
{
    return impl->SITE_NUM;
}

unsigned char StdfPTR::get_test_flag() const
{
	return (unsigned char)(impl->TEST_FLG.to_ulong());
}

unsigned char StdfPTR::get_parametric_test_flag() const
{
	return (unsigned char)(impl->PARM_FLG.to_ulong());
}

float StdfPTR::get_result() const
{
    return impl->RESULT;
}

const char* StdfPTR::get_test_text() const
{
    if(impl->TEST_TXT.empty()) return nullptr;
    return impl->TEST_TXT.c_str();
}

const char* StdfPTR::get_alarm_id() const
{
    if(impl->ALARM_ID.empty()) return nullptr;
    return impl->ALARM_ID.c_str();
}

unsigned char StdfPTR::get_optional_data_flag() const
{
	return (unsigned char)(impl->OPT_FLAG.to_ulong());
}

signed char StdfPTR::get_result_exponent() const
{
    return impl->RES_SCAL;
}

signed char StdfPTR::get_lowlimit_exponent() const
{
    return impl->LLM_SCAL;
}

signed char StdfPTR::get_highlimit_exponent() const
{
    return impl->HLM_SCAL;
}

float StdfPTR::get_low_limit() const
{
    return impl->LO_LIMIT;
}

float StdfPTR::get_high_limit() const
{
    return impl->HI_LIMIT;
}

const char* StdfPTR::get_unit() const
{
    if(impl->UNITS.empty()) return nullptr;
    return impl->UNITS.c_str();
}

const char* StdfPTR::get_result_format() const
{
    if(impl->C_RESFMT.empty()) return nullptr;
    return impl->C_RESFMT.c_str();
}

const char* StdfPTR::get_lowlimit_format() const
{
    if(impl->C_LLMFMT.empty()) return nullptr;
    return impl->C_LLMFMT.c_str();
}

const char* StdfPTR::get_highlimit_format() const
{
    if(impl->C_HLMFMT.empty()) return nullptr;
    return impl->C_HLMFMT.c_str();
}

float StdfPTR::get_low_spec() const
{
    return impl->LO_SPEC;
}

float StdfPTR::get_high_spec() const
{
    return impl->HI_SPEC;
}

// TEST_FLG
bool StdfPTR::alarm_detected() const
{
    return impl->TEST_FLG[0];
}

bool StdfPTR::result_invalid() const
{
    return impl->TEST_FLG[1];
}

bool StdfPTR::result_unreliable() const
{
    return impl->TEST_FLG[2];
}

bool StdfPTR::timeout_occured() const
{
    return impl->TEST_FLG[3];
}

bool StdfPTR::test_unexecuted() const
{
    return impl->TEST_FLG[4];
}

bool StdfPTR::test_aborted() const
{
    return impl->TEST_FLG[5];
}

bool StdfPTR::test_pfflag_invalid() const
{
    return impl->TEST_FLG[6];
}

bool StdfPTR::test_failed() const
{
    return impl->TEST_FLG[7];
}

//PARM_FLG
bool StdfPTR::param_scale_error() const
{
    return impl->PARM_FLG[0];
}

bool StdfPTR::param_drift_error() const
{
    return impl->PARM_FLG[1];
}

bool StdfPTR::param_oscillation() const
{
    return impl->PARM_FLG[2];
}

bool StdfPTR::result_higher_limit() const
{
    return impl->PARM_FLG[3];
}

bool StdfPTR::result_lower_limit() const
{
    return impl->PARM_FLG[4];
}

bool StdfPTR::passed_alternate_limit() const
{
    return impl->PARM_FLG[5];
}

bool StdfPTR::equal_lowlimit_pass() const
{
    return impl->PARM_FLG[6];
}

bool StdfPTR::equal_highlimit_pass() const
{
    return impl->PARM_FLG[7];
}

//OPT_FLAG
bool StdfPTR::result_exponent_invalid() const
{
    return impl->OPT_FLAG[0];
}

bool StdfPTR::no_low_spec() const
{
    return impl->OPT_FLAG[2];
}

bool StdfPTR::no_high_spec() const
{
    return impl->OPT_FLAG[3];
}

bool StdfPTR::low_limit_invalid() const
{
    return impl->OPT_FLAG[4];
}

bool StdfPTR::high_limit_invalid() const
{
    return impl->OPT_FLAG[5];
}

bool StdfPTR::no_low_limit() const
{
    return impl->OPT_FLAG[6];
}

bool StdfPTR::no_high_limit() const
{
    return impl->OPT_FLAG[7];
}

void StdfPTR::set_test_number(unsigned int number)
{
    impl->TEST_NUM = number;
}

void StdfPTR::set_head_number(unsigned char number)
{
    impl->HEAD_NUM = number;
}

void StdfPTR::set_site_number(unsigned char number)
{
    impl->SITE_NUM = number;
}

void StdfPTR::set_result(float value)
{
    impl->RESULT = value;
    // result is useful
    impl->TEST_FLG[0] = false;
    impl->TEST_FLG[1] = false;
    impl->TEST_FLG[2] = false;
    impl->TEST_FLG[3] = false;
    impl->TEST_FLG[4] = false;
    impl->TEST_FLG[5] = false;

    impl->PARM_FLG[0] = false;
    impl->PARM_FLG[1] = false;
    impl->PARM_FLG[2] = false;

    impl->OPT_FLAG[0] = false;
}

void StdfPTR::set_test_text(const char* text)
{
    if(text) impl->TEST_TXT.assign(text);
}

void StdfPTR::set_alarm_id(const char* id)
{
    if(id) impl->ALARM_ID.assign(id);
}

void StdfPTR::set_result_exponent(signed char exponent)
{
    impl->RES_SCAL = exponent;
    impl->OPT_FLAG[0] = false;
}

void StdfPTR::set_lowlimit_exponent(signed char exponent)
{
    impl->LLM_SCAL = exponent;
    impl->OPT_FLAG[4] = false;
    impl->OPT_FLAG[6] = false;
}

void StdfPTR::set_highlimit_exponent(signed char exponent)
{
    impl->HLM_SCAL = exponent;
    impl->OPT_FLAG[5] =false;
    impl->OPT_FLAG[7] =false;
}

void StdfPTR::set_low_limit(float limit)
{
    impl->LO_LIMIT = limit;
    impl->OPT_FLAG[6] = false;
    impl->OPT_FLAG[4] = false;
}

void StdfPTR::set_high_limit(float limit)
{
    impl->HI_LIMIT = limit;
    impl->OPT_FLAG[5] =false;
    impl->OPT_FLAG[7] = false;
}

void StdfPTR::set_unit(const char* unit)
{
    impl->UNITS.assign(unit);
}

void StdfPTR::set_result_format(const char* format)
{
    if(format) impl->C_RESFMT.assign(format);
}

void StdfPTR::set_lowlimit_format(const char* format)
{
    if(format) impl->C_LLMFMT.assign(format);
}

void StdfPTR::set_highlimit_format(const char* format)
{
    if(format) impl->C_HLMFMT.assign(format);
}

void StdfPTR::set_low_spec(float spec)
{
    impl->LO_SPEC = spec;
    impl->OPT_FLAG[2] = false;
}

void StdfPTR::set_high_spec(float spec)
{
    impl->HI_SPEC = spec;
    impl->OPT_FLAG[3] = false;
}

// TEST_FLG
void StdfPTR::alarm_detected(bool flag)
{
    impl->TEST_FLG[0] = flag;
}

void StdfPTR::result_invalid(bool flag)
{
    impl->TEST_FLG[1] = flag;
}

void StdfPTR::result_unreliable(bool flag)
{
    impl->TEST_FLG[2] = flag;
}

void StdfPTR::timeout_occured(bool flag)
{
    impl->TEST_FLG[3] = flag;
}

void StdfPTR::test_unexecuted(bool flag)
{
    impl->TEST_FLG[4] = flag;
}

void StdfPTR::test_aborted(bool flag)
{
    impl->TEST_FLG[5] = flag;
}

void StdfPTR::test_pfflag_invalid(bool flag)
{
    impl->TEST_FLG[6] = flag;
}

void StdfPTR::test_failed(bool flag)
{
    impl->TEST_FLG[7] = flag;
}

//PARM_FLG
void StdfPTR::param_scale_error(bool flag)
{
    impl->PARM_FLG[0] = flag;
}

void StdfPTR::param_drift_error(bool flag)
{
    impl->PARM_FLG[1] = flag;
}

void StdfPTR::param_oscillation(bool flag)
{
    impl->PARM_FLG[2] = flag;
}

void StdfPTR::result_higher_limit(bool flag)
{
    impl->PARM_FLG[3] = flag;
}

void StdfPTR::result_lower_limit(bool flag)
{
    impl->PARM_FLG[4] = flag;
}

void StdfPTR::passed_alternate_limit(bool flag)
{
    impl->PARM_FLG[5] = flag;
}

void StdfPTR::equal_lowlimit_pass(bool flag)
{
    impl->PARM_FLG[6] = flag;
}

void StdfPTR::equal_highlimit_pass(bool flag)
{
    impl->PARM_FLG[7] = flag;
}

//OPT_FLAG
void StdfPTR::result_exponent_invalid(bool flag)
{
    impl->OPT_FLAG[0] = flag;
}

void StdfPTR::no_low_spec(bool flag)
{
    impl->OPT_FLAG[2] = flag;
}

void StdfPTR::no_high_spec(bool flag)
{
    impl->OPT_FLAG[3] = flag;
}

void StdfPTR::low_limit_invalid(bool flag)
{
    impl->OPT_FLAG[4] = flag;
}

void StdfPTR::high_limit_invalid(bool flag)
{
    impl->OPT_FLAG[5] = flag;
}

void StdfPTR::no_low_limit(bool flag)
{
    impl->OPT_FLAG[6] = flag;
}

void StdfPTR::no_high_limit(bool flag)
{
    impl->OPT_FLAG[7] = flag;
}

unsigned int StdfPTR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}

unsigned int StdfPTR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfPTR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfMPR::StdfMPR() : StdfRecord("MPR", MPR_TYPE)
{
    impl = new MultipleResultParametric();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfMPR::~StdfMPR()
{
    delete impl;
    impl = nullptr;
}

unsigned int StdfMPR::get_test_number() const
{
    return impl->TEST_NUM;
}

unsigned char StdfMPR::get_head_number() const
{
    return impl->HEAD_NUM;
}

unsigned char StdfMPR::get_site_number() const
{
    return impl->SITE_NUM;
}

unsigned char StdfMPR::get_test_flag() const
{
	return (unsigned char)(impl->TEST_FLG.to_ulong());
}

unsigned char StdfMPR::get_parametric_test_flag() const
{
	return (unsigned char)(impl->PARM_FLG.to_ulong());
}

unsigned short StdfMPR::get_pin_count() const
{
    return impl->RTN_ICNT;
}

unsigned short StdfMPR::get_result_count() const
{
    return impl->RSLT_CNT;
}

unsigned char StdfMPR::get_return_state(unsigned short index) const
{
    if(index >= impl->RTN_STAT.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->RTN_STAT.size(), index);
        throw INDEX_OVER_RANGE(impl->RTN_STAT.size(), index);
        return 0;
#endif

    }
    unsigned long temp = impl->RTN_STAT[index].to_ulong();
    return U1(temp);
}

float StdfMPR::get_return_result(unsigned short index) const
{
    if(index >= impl->RTN_RSLT.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->RTN_RSLT.size(), index);
        throw INDEX_OVER_RANGE( impl->RTN_RSLT.size(), index);
#endif
        return 0.0;
    }
    return impl->RTN_RSLT[index];
}

const char* StdfMPR::get_test_text() const
{
    if(impl->TEST_TXT.empty()) return nullptr;
    return impl->TEST_TXT.c_str();
}

const char* StdfMPR::get_alarm_id() const
{
    if(impl->ALARM_ID.empty()) return nullptr;
    return impl->ALARM_ID.c_str();
}

unsigned char StdfMPR::get_optional_data_flag() const
{
	return (unsigned char)(impl->OPT_FLAG.to_ulong());
}

signed char StdfMPR::get_result_exponent() const
{
    return impl->RES_SCAL;
}

signed char StdfMPR::get_lowlimit_exponent() const
{
    return impl->LLM_SCAL;
}

signed char StdfMPR::get_highlimit_exponent() const
{
    return impl->HLM_SCAL;
}

float StdfMPR::get_low_limit() const
{
    return impl->LO_LIMIT;
}

float StdfMPR::get_high_limit() const
{
    return impl->HI_LIMIT;
}

float StdfMPR::get_starting_input() const
{
    return impl->START_IN;
}

float StdfMPR::get_increment_input() const
{
    return impl->INCR_IN;
}

unsigned short StdfMPR::get_pin_index(unsigned short index) const
{
    if(index >= impl->RTN_INDX.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->RTN_INDX.size(), index);
        throw INDEX_OVER_RANGE( impl->RTN_INDX.size(), index);
#endif
        return 0;
    }
    return impl->RTN_INDX[index];
}

const char* StdfMPR::get_unit() const
{
    if(impl->UNITS.empty()) return nullptr;
    return impl->UNITS.c_str();
}

const char* StdfMPR::get_unit_input() const
{
    if(impl->UNITS_IN.empty()) return nullptr;
    return impl->UNITS_IN.c_str();
}

const char* StdfMPR::get_result_format() const
{
    if(impl->C_RESFMT.empty()) return nullptr;
    return impl->C_RESFMT.c_str();
}

const char* StdfMPR::get_lowlimit_format() const
{
    if(impl->C_LLMFMT.empty()) return nullptr;
    return impl->C_LLMFMT.c_str();
}

const char* StdfMPR::get_highlimit_format() const
{
    if(impl->C_HLMFMT.empty()) return nullptr;
    return impl->C_HLMFMT.c_str();
}

float StdfMPR::get_low_spec() const
{
    return impl->LO_SPEC;
}

float StdfMPR::get_high_spec() const
{
    return impl->HI_SPEC;
}

// TEST_FLG
bool StdfMPR::alarm_detected() const
{
    return impl->TEST_FLG[0];
}
bool StdfMPR::result_invalid() const
{
    return impl->TEST_FLG[1];
}

bool StdfMPR::result_unreliable() const
{
    return impl->TEST_FLG[2];
}

bool StdfMPR::timeout_occured() const
{
    return impl->TEST_FLG[3];
}

bool StdfMPR::test_unexecuted() const
{
    return impl->TEST_FLG[4];
}

bool StdfMPR::test_aborted() const
{
    return impl->TEST_FLG[5];
}

bool StdfMPR::test_pfflag_invalid() const
{
    return impl->TEST_FLG[6];
}

bool StdfMPR::test_failed() const
{
    return impl->TEST_FLG[7];
}

//PARM_FLG
bool StdfMPR::param_scale_error() const
{
    return impl->PARM_FLG[0];
}

bool StdfMPR::param_drift_error() const
{
    return impl->PARM_FLG[1];
}

bool StdfMPR::param_oscillation() const
{
    return impl->PARM_FLG[2];
}

bool StdfMPR::result_higher_limit() const
{
    return impl->PARM_FLG[3];
}

bool StdfMPR::result_lower_limit() const
{
    return impl->PARM_FLG[4];
}

bool StdfMPR::passed_alternate_limit() const
{
    return impl->PARM_FLG[5];
}

bool StdfMPR::equal_lowlimit_pass() const
{
    return impl->PARM_FLG[6];
}

bool StdfMPR::equal_highlimit_pass() const
{
    return impl->PARM_FLG[7];
}

//OPT_FLAG
bool StdfMPR::result_exponent_invalid() const
{
    return impl->OPT_FLAG[0];
}

bool StdfMPR::start_input_invalid() const
{
    return impl->OPT_FLAG[1];
}

bool StdfMPR::no_low_spec() const
{
    return impl->OPT_FLAG[2];
}

bool StdfMPR::no_high_spec() const
{
    return impl->OPT_FLAG[3];
}

bool StdfMPR::low_limit_invalid() const
{
    return impl->OPT_FLAG[4];
}

bool StdfMPR::high_limit_invalid() const
{
    return impl->OPT_FLAG[5];
}

bool StdfMPR::no_low_limit() const
{
    return impl->OPT_FLAG[6];
}
bool StdfMPR::no_high_limit() const
{
    return impl->OPT_FLAG[7];
}

void StdfMPR::set_test_number(unsigned int number)
{
    impl->TEST_NUM = number;
}

void StdfMPR::set_head_number(unsigned char number)
{
    impl->HEAD_NUM = number;
}

void StdfMPR::set_site_number(unsigned char number)
{
    impl->SITE_NUM = number;
}

void StdfMPR::set_pin_count(unsigned short count)
{
    impl->RTN_ICNT = count;
    impl->RTN_STAT.resize(count);
    impl->RTN_INDX.resize(count);
}

void StdfMPR::set_result_count(unsigned short count)
{
    impl->RSLT_CNT = count;
    impl->RTN_RSLT.resize(count);
}

void StdfMPR::set_return_state(unsigned short index, unsigned char nibble_state)
{
    if(index >= impl->RTN_STAT.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->RTN_STAT.size(), index);
        throw INDEX_OVER_RANGE( impl->RTN_STAT.size(), index);
#endif
        return ;
    }

    N1 temp = N1(nibble_state & 0x0F);
    impl->RTN_STAT[index] = temp;
}
void StdfMPR::set_return_result(unsigned short index, float result)
{
    if(index >= impl->RTN_RSLT.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->RTN_RSLT.size(), index);
        throw INDEX_OVER_RANGE(impl->RTN_RSLT.size(), index);
#endif
        return ;
    }
    impl->RTN_RSLT[index] = result;
}

void StdfMPR::set_test_text(const char* text)
{
    if(text) impl->TEST_TXT.assign(text);
}

void StdfMPR::set_alarm_id(const char* id)
{
    if(id) impl->ALARM_ID.assign(id);
}

void StdfMPR::set_result_exponent(signed char exponent)
{
    impl->RES_SCAL = exponent;
    impl->OPT_FLAG[0] = false;
}

void StdfMPR::set_lowlimit_exponent(signed char exponent)
{
    impl->LLM_SCAL = exponent;
    impl->OPT_FLAG[4] = false;
    impl->OPT_FLAG[6] = false;
}

void StdfMPR::set_highlimit_exponent(signed char exponent)
{
    impl->HLM_SCAL = exponent;
    impl->OPT_FLAG[5] =false;
    impl->OPT_FLAG[7] =false;
}

void StdfMPR::set_low_limit(float limit)
{
    impl->LO_LIMIT = limit;
    impl->OPT_FLAG[6] = false;
    impl->OPT_FLAG[4] = false;
}

void StdfMPR::set_high_limit(float limit)
{
    impl->HI_LIMIT = limit;
    impl->OPT_FLAG[5] =false;
    impl->OPT_FLAG[7] = false;
}

void StdfMPR::set_starting_input(float starting)
{
    impl->START_IN = starting;
    impl->OPT_FLAG[1] = false;
}

void StdfMPR::set_increment_input(float increment)
{
    impl->INCR_IN = increment;
    impl->OPT_FLAG[1] = false;
}

void StdfMPR::set_pin_index(unsigned short index, unsigned short pin_number)
{
    if(index >= impl->RTN_INDX.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->RTN_INDX.size(), index);
        throw INDEX_OVER_RANGE( impl->RTN_INDX.size(), index);
#endif
        return;
    }
    impl->RTN_INDX[index] = pin_number;
}

void StdfMPR::set_unit(const char* unit)
{
    if(unit) impl->UNITS.assign(unit);
}

void StdfMPR::set_unit_input(const char* input_unit)
{
    if(input_unit) impl->UNITS_IN.assign(input_unit);
}

void StdfMPR::set_result_format(const char* format)
{
    if(format) impl->C_RESFMT.assign(format);
}

void StdfMPR::set_lowlimit_format(const char* format)
{
    if(format) impl->C_LLMFMT.assign(format);
}

void StdfMPR::set_highlimit_format(const char* format)
{
    if(format) impl->C_HLMFMT.assign(format);
}

void StdfMPR::set_low_spec(float spec)
{
    impl->LO_SPEC = spec;
    impl->OPT_FLAG[2] = false;
}

void StdfMPR::set_high_spec(float spec)
{
    impl->HI_SPEC = spec;
    impl->OPT_FLAG[3] = false;
}

// TEST_FLG
void StdfMPR::alarm_detected(bool flag)
{
    impl->TEST_FLG[0] = flag;
}

void StdfMPR::result_invalid(bool flag)
{
    impl->TEST_FLG[1] = flag;
}

void StdfMPR::result_unreliable(bool flag)
{
    impl->TEST_FLG[2] = flag;
}

void StdfMPR::timeout_occured(bool flag)
{
    impl->TEST_FLG[3] = flag;
}

void StdfMPR::test_unexecuted(bool flag)
{
    impl->TEST_FLG[4] = flag;
}

void StdfMPR::test_aborted(bool flag)
{
    impl->TEST_FLG[5] = flag;
}

void StdfMPR::test_pfflag_invalid(bool flag)
{
    impl->TEST_FLG[6] = flag;
}

void StdfMPR::test_failed(bool flag)
{
    impl->TEST_FLG[7] = flag;
}

//PARM_FLG
void StdfMPR::param_scale_error(bool flag)
{
    impl->PARM_FLG[0] = flag;
}

void StdfMPR::param_drift_error(bool flag)
{
    impl->PARM_FLG[1] = flag;
}

void StdfMPR::param_oscillation(bool flag)
{
    impl->PARM_FLG[2] = flag;
}

void StdfMPR::result_higher_limit(bool flag)
{
    impl->PARM_FLG[3] = flag;
}

void StdfMPR::result_lower_limit(bool flag)
{
    impl->PARM_FLG[4] = flag;
}

void StdfMPR::passed_alternate_limit(bool flag)
{
    impl->PARM_FLG[5] = flag;
}

void StdfMPR::equal_lowlimit_pass(bool flag)
{
    impl->PARM_FLG[6] = flag;
}

void StdfMPR::equal_highlimit_pass(bool flag)
{
    impl->PARM_FLG[7] = flag;
}

//OPT_FLAG
void StdfMPR::result_exponent_invalid(bool flag)
{
    impl->OPT_FLAG[0] = flag;
}

void StdfMPR::start_input_invalid(bool flag)
{
    impl->OPT_FLAG[1] = flag;
}

void StdfMPR::no_low_spec(bool flag)
{
    impl->OPT_FLAG[2] = flag;
}

void StdfMPR::no_high_spec(bool flag)
{
    impl->OPT_FLAG[3] = flag;
}

void StdfMPR::low_limit_invalid(bool flag)
{
    impl->OPT_FLAG[4] = flag;
}

void StdfMPR::high_limit_invalid(bool flag)
{
    impl->OPT_FLAG[5] = flag;
}

void StdfMPR::no_low_limit(bool flag)
{
    impl->OPT_FLAG[6] = flag;
}

void StdfMPR::no_high_limit(bool flag)
{
    impl->OPT_FLAG[7] = flag;
}

unsigned int StdfMPR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}

unsigned int StdfMPR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfMPR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfFTR::StdfFTR() : StdfRecord("FTR", FTR_TYPE)
{
    impl = new FunctionalTest();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfFTR::~StdfFTR()
{
    delete impl;
    impl = nullptr;
}

unsigned int StdfFTR::get_test_number() const
{
    return impl->TEST_NUM;
}

unsigned char StdfFTR::get_head_number() const
{
    return impl->HEAD_NUM;
}

unsigned char StdfFTR::get_site_number() const
{
    return impl->SITE_NUM;
}

unsigned char StdfFTR::get_test_flag() const
{
	return (unsigned char)(impl->TEST_FLG.to_ulong());
}

unsigned char StdfFTR::get_optional_data_flag() const
{
	return (unsigned char)(impl->OPT_FLAG.to_ulong());
}

unsigned int StdfFTR::get_cycle_count() const
{
    return impl->CYCL_CNT;
}

unsigned int StdfFTR::get_relative_address() const
{
    return impl->REL_VADR;
}

unsigned int StdfFTR::get_repeat_count() const
{
    return impl->REPT_CNT;
}

unsigned int StdfFTR::get_failpin_count() const
{
    return impl->NUM_FAIL;
}

int StdfFTR::get_xfail_address() const
{
    return impl->XFAIL_AD;
}

int StdfFTR::get_yfail_address() const
{
    return impl->YFAIL_AD;
}

short StdfFTR::get_vector_offset() const
{
    return impl->VECT_OFF;
}

unsigned short StdfFTR::get_pin_count() const
{
    return impl->RTN_ICNT;
}

unsigned short StdfFTR::get_program_state_count() const
{
    return impl->PGM_ICNT;
}

unsigned short StdfFTR::get_pin_number(unsigned short index) const
{
    if(index >= impl->RTN_INDX.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->RTN_INDX.size(), index);
        throw INDEX_OVER_RANGE(impl->RTN_INDX.size(), index);
#endif
        return (unsigned short)(-1);
    }
    return impl->RTN_INDX[index];
}

unsigned char StdfFTR::get_pin_state(unsigned short index) const
{
    if(index >= impl->RTN_STAT.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->RTN_STAT.size(), index);
        throw INDEX_OVER_RANGE( impl->RTN_STAT.size(), index);
#endif
        return 0;
    }
    unsigned long temp = impl->RTN_STAT[index].to_ulong();
    return U1(temp);
}
unsigned short StdfFTR::get_program_index(unsigned short index) const
{
    if(index >= impl->PGM_INDX.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->PGM_INDX.size(), index);
        throw INDEX_OVER_RANGE(impl->PGM_INDX.size(), index);
#endif
        return (unsigned short)(-1);
    }
    return impl->PGM_INDX[index];
}

unsigned char StdfFTR::get_program_state(unsigned short index) const
{
    if(index >= impl->PGM_STAT.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->PGM_STAT.size(), index);
        throw INDEX_OVER_RANGE(impl->PGM_STAT.size(), index);
#endif
        return 0;
    }
    unsigned long temp = impl->PGM_STAT[index].to_ulong();
    return U1(temp);
}

unsigned short StdfFTR::get_failpin_data_count() const
{
	return impl->FAIL_PIN.count;
}

unsigned char StdfFTR::get_failpin_data(unsigned short index) const
{
#if _MSC_VER == 1600
    char temp = impl->FAIL_PIN.data[index];
    return U1(temp);
#else
    bool temp = impl->FAIL_PIN.data[index];
    if(temp) return U1(1);
    else return U1(0);
#endif
}

const char* StdfFTR::get_vector_pattern_name() const
{
    if(impl->VECT_NAM.empty()) return nullptr;
    return impl->VECT_NAM.c_str();
}

const char* StdfFTR::get_timeset_name() const
{
    if(impl->TIME_SET.empty()) return nullptr;
    return impl->TIME_SET.c_str();
}

const char* StdfFTR::get_vector_op_code() const
{
    if(impl->OP_CODE.empty()) return nullptr;
    return impl->OP_CODE.c_str();
}

const char* StdfFTR::get_test_text() const
{
    if(impl->TEST_TXT.empty()) return nullptr;
    return impl->TEST_TXT.c_str();
}

const char* StdfFTR::get_alarm_id() const
{
    if(impl->ALARM_ID.empty()) return nullptr;
    return impl->ALARM_ID.c_str();
}

const char* StdfFTR::get_result_text() const
{
    if(impl->RSLT_TXT.empty()) return nullptr;
    return impl->RSLT_TXT.c_str();
}

unsigned char StdfFTR::get_pattern_genertor_number() const
{
    return impl->PATG_NUM;
}

unsigned short StdfFTR::get_bitmap_data_count() const
{
	return impl->SPIN_MAP.count;
}

unsigned char StdfFTR::get_bitmap_data(unsigned short index) const
{
#if _MSC_VER == 1600
    char temp = impl->SPIN_MAP.data[index];
    return U1(temp);
#else
    bool temp = impl->SPIN_MAP.data[index];
    if(temp) return U1(1);
    else return U1(0);
#endif
}

void StdfFTR::set_test_number(unsigned int number)
{
    impl->TEST_NUM = number;
}

void StdfFTR::set_head_number(unsigned char number)
{
    impl->HEAD_NUM = number;
}
void StdfFTR::set_site_number(unsigned char number)
{
    impl->SITE_NUM = number;
}

void StdfFTR::set_cycle_count(unsigned int count)
{
    impl->CYCL_CNT = count;
    impl->OPT_FLAG[0] = false;
}

void StdfFTR::set_relative_address(unsigned int address)
{
    impl->REL_VADR = address;
    impl->OPT_FLAG[1] = false;
}

void StdfFTR::set_repeat_count(unsigned int count)
{
    impl->REPT_CNT = count;
    impl->OPT_FLAG[2] = false;
}

void StdfFTR::set_failpin_count(unsigned int count)
{
    impl->NUM_FAIL = count;
    impl->OPT_FLAG[3] = false;
}

void StdfFTR::set_xfail_address(int x)
{
    impl->XFAIL_AD = x;
    impl->OPT_FLAG[4] = false;
}

void StdfFTR::set_yfail_address(int y)
{
    impl->YFAIL_AD = y;
    impl->OPT_FLAG[4] = false;
}

void StdfFTR::set_vector_offset(short offset)
{
    impl->VECT_OFF = offset;
    impl->OPT_FLAG[5] = false;
}

void StdfFTR::set_pin_count(unsigned short count)
{
    impl->RTN_ICNT = count;
    impl->RTN_INDX.resize(count);
    impl->RTN_STAT.resize(count);
}

void StdfFTR::set_program_state_count(unsigned short count)
{
    impl->PGM_ICNT = count;
    impl->PGM_INDX.resize(count);
    impl->PGM_STAT.resize(count);
}

void StdfFTR::set_pin_number(unsigned short index, unsigned short pin_number)
{
    if(index >= impl->RTN_INDX.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->RTN_INDX.size(), index);
        throw INDEX_OVER_RANGE( impl->RTN_INDX.size(), index);
#endif
        return ;
    }

    impl->RTN_INDX[index] = pin_number;
}

void StdfFTR::set_pin_state(unsigned short index, ReturnState nibble_state)
{
    if(index >= impl->RTN_STAT.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->RTN_STAT.size(), index);
        throw INDEX_OVER_RANGE(impl->RTN_STAT.size(), index);
#endif
        return;
    }

    N1 temp = N1(nibble_state & 0x0F);
    impl->RTN_STAT[index] = temp;
}

void StdfFTR::set_program_index(unsigned short index, unsigned short state_index)
{
    if(index >= impl->PGM_INDX.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->PGM_INDX.size(), index);
        throw INDEX_OVER_RANGE(impl->PGM_INDX.size(), index);
#endif
        return;
    }
    impl->PGM_INDX[index] = state_index;
}

void StdfFTR::set_program_state(unsigned short index, unsigned char nibble_state)
{
    if(index >= impl->PGM_STAT.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->PGM_STAT.size(), index);
        throw INDEX_OVER_RANGE( impl->PGM_STAT.size(), index);
#endif
        return ;
    }
    N1 temp = N1(nibble_state & 0x0F);
    impl->PGM_STAT[index] = temp;
}

void StdfFTR::set_failpin_data(unsigned short index, unsigned char bit_data)
{
    unsigned short count = impl->FAIL_PIN.count;
    if(index > count) impl->FAIL_PIN.count = index;
#if _MSC_VER == 1600
    impl->FAIL_PIN.data[index] = C1(bit_data & 0x01);
#else
    if(bit_data & 0x01) impl->FAIL_PIN.data[index] = true;
    else impl->FAIL_PIN.data[index] = false;
#endif
}

void StdfFTR::set_vector_pattern_name(const char* name)
{
    if(name) impl->VECT_NAM.assign(name);
}

void StdfFTR::set_timeset_name(const char* name)
{
    if(name) impl->TIME_SET.assign(name);
}

void StdfFTR::set_vector_op_code(const char* op_code)
{
    if(op_code) impl->OP_CODE.assign(op_code);
}

void StdfFTR::set_test_text(const char* text)
{
    if(text) impl->TEST_TXT.assign(text);
}

void StdfFTR::set_alarm_id(const char* id)
{
    if(id) impl->ALARM_ID.assign(id);
}

void StdfFTR::set_result_text(const char* text)
{
    if(text) impl->RSLT_TXT.assign(text);
}

void StdfFTR::set_pattern_genertor_number(unsigned char number)
{
    impl->PATG_NUM = number;
}

void StdfFTR::set_bitmap_data(unsigned short index, unsigned char bit_data)
{
    unsigned short count = impl->SPIN_MAP.count;
    if(index > count) impl->SPIN_MAP.count = index;
#if _MSC_VER == 1600
    impl->SPIN_MAP.data[index] = C1(bit_data & 0x01);
#else
    if(bit_data & 0x01) impl->SPIN_MAP.data[index] = true;
    else impl->SPIN_MAP.data[index] = false;
#endif
}

// TEST_FLG
bool StdfFTR::alarm_detected() const
{
    return impl->TEST_FLG[0];
}

bool StdfFTR::result_invalid() const
{
    return impl->TEST_FLG[1];
}

bool StdfFTR::result_unreliable() const
{
    return impl->TEST_FLG[2];
}
bool StdfFTR::timeout_occured() const
{
    return impl->TEST_FLG[3];
}

bool StdfFTR::test_unexecuted() const
{
    return impl->TEST_FLG[4];
}

bool StdfFTR::test_aborted() const
{
    return impl->TEST_FLG[5];
}

bool StdfFTR::test_pfflag_invalid() const
{
    return impl->TEST_FLG[6];
}

bool StdfFTR::test_failed() const
{
    return impl->TEST_FLG[7];
}

// TEST_FLG
void StdfFTR::alarm_detected(bool flag)
{
    impl->TEST_FLG[0] = flag;
}

void StdfFTR::result_invalid(bool flag)
{
    impl->TEST_FLG[1] = flag;
}

void StdfFTR::result_unreliable(bool flag)
{
    impl->TEST_FLG[2] = flag;
}

void StdfFTR::timeout_occured(bool flag)
{
    impl->TEST_FLG[3] = flag;
}

void StdfFTR::test_unexecuted(bool flag)
{
    impl->TEST_FLG[4] = flag;
}

void StdfFTR::test_aborted(bool flag)
{
    impl->TEST_FLG[5] = flag;
}

void StdfFTR::test_pfflag_invalid(bool flag)
{
    impl->TEST_FLG[6] = flag;
}

void StdfFTR::test_failed(bool flag)
{
    impl->TEST_FLG[7] = flag;
}

// OPT_FLG
bool StdfFTR::cycl_count_invalid() const
{
    return impl->OPT_FLAG[0];
}

bool StdfFTR::relative_address_invalid() const
{

    return impl->OPT_FLAG[1];
}
bool StdfFTR::repeat_count_invalid() const
{
    return impl->OPT_FLAG[2];
}

bool StdfFTR::failpin_count_invalid() const
{
    return impl->OPT_FLAG[3];
}

bool StdfFTR::xyfail_address_invalid() const
{
    return impl->OPT_FLAG[4];
}

bool StdfFTR::vector_offset_invalid() const
{
    return impl->OPT_FLAG[5];
}
// OPT_FLG
void StdfFTR::cycl_count_invalid(bool flag)
{
    impl->OPT_FLAG[0] = flag;
}

void StdfFTR::relative_address_invalid(bool flag)
{
    impl->OPT_FLAG[1] = flag;
}

void StdfFTR::repeat_count_invalid(bool flag)
{
    impl->OPT_FLAG[2] = flag;
}

void StdfFTR::failpin_count_invalid(bool flag)
{
    impl->OPT_FLAG[3] = flag;
}

void StdfFTR::xyfail_address_invalid(bool flag)
{
    impl->OPT_FLAG[4] = flag;
}

void StdfFTR::vector_offset_invalid(bool flag)
{
    impl->OPT_FLAG[5] = flag;
}

unsigned int StdfFTR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}

unsigned int StdfFTR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfFTR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfBPS::StdfBPS() : StdfRecord("BPS", BPS_TYPE)
{
    impl = new BeginProgramSection();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfBPS::~StdfBPS()
{
    delete impl;
    impl = nullptr;
}

const char* StdfBPS::get_section_name() const
{
    if(impl->SEQ_NAME.empty()) return nullptr;
    return impl->SEQ_NAME.c_str();
}

void StdfBPS::set_section_name(const char* name)
{
    if(name) impl->SEQ_NAME.assign(name);
}

unsigned int StdfBPS::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}

unsigned int StdfBPS::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfBPS::print(std::ostream& os) const
{
	impl->Print(os);
}

StdfEPS::StdfEPS() : StdfRecord("EPS", EPS_TYPE)
{
    impl = new EndProgramSection();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}
StdfEPS::~StdfEPS()
{
    delete impl;
    impl = nullptr;
}

unsigned int StdfEPS::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}

unsigned int StdfEPS::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfEPS::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfGDR::StdfGDR() : StdfRecord("GDR", GDR_TYPE)
{
    impl = new GenericData();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfGDR::~StdfGDR()
{
    delete impl;
    impl = nullptr;
}

unsigned short StdfGDR::get_data_count() const
{
    return impl->FLD_CNT;
}

void StdfGDR::set_data_count(unsigned short count)
{
    impl->FLD_CNT = count;
    impl->GEN_DATA.clear();
    impl->GEN_DATA.resize(count);
}

unsigned char StdfGDR::get_data_type(unsigned int index) const
{
    if(index >= impl->GEN_DATA.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->GEN_DATA.size(), index);
        throw INDEX_OVER_RANGE(impl->GEN_DATA.size(), index);
#endif
        return 0xF;
    }
	return impl->GEN_DATA[index].type;
}

const unsigned char* StdfGDR::get_data_value(unsigned int index, int* byte_length, int* bit_count) const
{
    if(index >= impl->GEN_DATA.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->GEN_DATA.size(), index);
        throw INDEX_OVER_RANGE(impl->GEN_DATA.size(), index);
#endif
        return nullptr;
    }
	unsigned char type = impl->GEN_DATA[index].type;
	unsigned char* data = rawdata[index];
	int length = 0;

    switch(type)
    {
    case 1 :
		{ /*U1*/
			length = 1;
			U1* temp = (U1*)(impl->GEN_DATA[index].data);
			std::memcpy(data, temp, length);
			if(bit_count) *bit_count = 8 * length;
		}
		break;
    case 2 :
		{ /*U2*/
			length = 2;
			U2* temp = (U2*)(impl->GEN_DATA[index].data);
			std::memcpy(data, temp, length);
			if(bit_count) *bit_count = 8 * length;
		}
		break;
    case 3 :
		{ /*U4*/
			length = 4;
			U4* temp = (U4*)(impl->GEN_DATA[index].data);
			std::memcpy(data, temp, length);
			if(bit_count) *bit_count = 8 * length;
		}
		break;
    case 4 :
		{ /*I1*/
			I1* temp = (I1*)(impl->GEN_DATA[index].data);
			length = 1;
			std::memcpy(data, temp, length);
			if(bit_count) *bit_count = 8 * length;
		}
		break;
    case 5 :
		{ /*I2*/
			I2* temp = (I2*)(impl->GEN_DATA[index].data);
			length = 2;
			std::memcpy(data, temp, length);
			if(bit_count) *bit_count = 8 * length;
		}
		break;
    case 6 :
		{ /*I4*/
			I4* temp = (I4*)(impl->GEN_DATA[index].data);
			length = 4;
			std::memcpy(data, temp, length);
			if(bit_count) *bit_count = 8 * length;
		}
		break;
    case 7 :
		{ /*R4*/
			R4* temp = (R4*)(impl->GEN_DATA[index].data);
			length = 4;
			std::memcpy(data, temp, length);
			if(bit_count) *bit_count = 8 * length;
		}
		break;
    case 8 :
		{ /*R8*/
			R8* temp = (R8*)(impl->GEN_DATA[index].data);
			length = 8;
			std::memcpy(data, temp, length);
			if(bit_count) *bit_count = 8 * length;
		}
		break;
    case 10:
        { /*Cn*/
            Cn* temp = (Cn*)(impl->GEN_DATA[index].data);
            if(temp->empty())
			{
				data =nullptr;
				length = 0;
			}
            else
			{
				length = temp->length();
				int i = 0;
				for(i = 0; i < length; i++)
					data[i] = U1((*temp)[i]);
				data[i] = 0;
			}
			if(bit_count) *bit_count = 8 * length;
        }
        break;
    case 11:
		{ /*Bn*/
			Bn* temp = (Bn*)(impl->GEN_DATA[index].data);
			for(U1 i = 0; i < temp->count; i++)
				data[i] = U1(temp->data[i].to_ulong());
			if(bit_count) *bit_count = 8 * length;
		}
		break;
    case 12:
		{ /*Dn*/
			Dn* temp = (Dn*)(impl->GEN_DATA[index].data);
			U2 count = temp->count;
			unsigned int byte_count = (count - 1)/ 8 + 1;

			U1 last_bit = 0x01;
			for(unsigned int i = 0; i < byte_count; i++)
			{
				U1 byte_temp = 0x0;
				for(U1 n = 0; n < 8; n++)
				{
					if(temp->data[i*8+n])
					{
						byte_temp |= (last_bit<<n);
					}
				}
				data[i] = byte_temp;
			}
			length = byte_count;
			if(bit_count) *bit_count = count;
		}
		break;
    case 13:
		{ /*N1*/
			N1* temp = (N1*)(impl->GEN_DATA[index].data);
			data[0] = U1(temp->to_ulong());
			length = 1;
			if(bit_count) *bit_count = 4;
		}
		break;
    default:
		{
			data = nullptr;
			length = 0;
			if(bit_count) *bit_count = 0;
		}
		break;
    }
    if(byte_length) *byte_length = length;
	return data;
}

void StdfGDR::set_data_type(VnType type, unsigned int index)
{
    if(index >= impl->GEN_DATA.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->GEN_DATA.size(), index);
        throw INDEX_OVER_RANGE( impl->GEN_DATA.size(), index);
#endif
        return;
    }

    impl->GEN_DATA[index].type = type;
}

bool StdfGDR::set_data_value(const unsigned char* data, unsigned int index, int byte_length)
{
    if(index >= impl->GEN_DATA.size())
    {
#ifdef _DEBUG_API_H_
        SHOW_INDEX_ERROR(impl->GEN_DATA.size(), index);
        throw INDEX_OVER_RANGE( impl->GEN_DATA.size(), index);
#endif
        return false;
    }

    U1 type = impl->GEN_DATA[index].type;
    switch(type)
    {
    case 1 :
        { /*U1*/
			if(byte_length != 1) return false;
            U1* void_data = new U1;
            *void_data = data[0];
            impl->GEN_DATA[index].data = void_data;
        }
        break;
    case 2 :
        { /*U2*/
            U2* void_data = new U2;
			if(byte_length != 2) return false;
			std::memcpy(void_data, data, byte_length);
            impl->GEN_DATA[index].data = void_data;
        }
        break;
    case 3 :
        { /*U4*/
            U4* void_data = new U4;
            if(byte_length != 4) return false;
			std::memcpy(void_data, data, byte_length);
            impl->GEN_DATA[index].data = void_data;
        }
        break;
    case 4 :
        { /*I1*/
            I1* void_data = new I1;
            if(byte_length != 1) return false;
			*void_data = I1(data[0]);
            impl->GEN_DATA[index].data = void_data;
        }
        break;
    case 5 :
        { /*I2*/
            I2* void_data = new I2;
            if(byte_length != 2) return false;
			std::memcpy(void_data, data, byte_length);
            impl->GEN_DATA[index].data = void_data;
        }
        break;
    case 6 :
        { /*I4*/
            I4* void_data = new I4;
            if(byte_length != 4) return false;
			std::memcpy(void_data, data, byte_length);
            impl->GEN_DATA[index].data = void_data;
        }
        break;
    case 7 :
        { /*R4*/
            R4* void_data = new R4;
            if(byte_length != 4) return false;
			std::memcpy(void_data, data, byte_length);
            impl->GEN_DATA[index].data = void_data;
        } break;
    case 8 :
        { /*R8*/
            R8* void_data = new R8;
            if(byte_length != 8) return false;
			std::memcpy(void_data, data, byte_length);
            impl->GEN_DATA[index].data = void_data;
        }
        break;
    case 10:
        { /*Cn*/
            if(byte_length > 0)
            {
                Cn* void_data = new Cn;
				void_data->resize(byte_length);
				for(int i = 0; i < byte_length; i++)
					(*void_data)[i] = data[i];
                impl->GEN_DATA[index].data = void_data;
            }
        }
        break;
    case 13:
        { /*N1*/
			if(byte_length != 1) return false;
            N1* void_data = new N1(data[0]);
            impl->GEN_DATA[index].data = void_data;
        } break;
    case 11:
        { /*Bn*/
            Bn* void_data = new Bn;
			void_data->count = byte_length;
			for(int i = 0; i < byte_length; i++)
				void_data->data[i] = B1(data[i]);
            impl->GEN_DATA[index].data = void_data;
        }
        break;
    case 12:
        { /*Dn*/
            Dn* void_data = new Dn;
			void_data->count = U2(byte_length * 8);
			U1 last_bit = 0x01;
			for(U2 i = 0; i < byte_length; i++)
			{
				U1 c = data[i];
				for(U1 n = 0; n < 8; n++)
				{
#if _MSC_VER == 1600
					void_data->data[i*8+n] = (c>>n) & last_bit;
#else
					void_data->data[i*8+n] = bool((c>>n) & last_bit);
#endif
				}
			}
            impl->GEN_DATA[index].data = void_data;
        }
        break;
    default:  return false;
    }
	return true;
}

unsigned int StdfGDR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}

unsigned int StdfGDR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfGDR::print(std::ostream& os) const
{
	impl->Print(os);
}

//////////////////////////////////////////////////////////////////////////
StdfDTR::StdfDTR() : StdfRecord("DTR", DTR_TYPE)
{
    impl = new DatalogText();
#ifdef _DEBUG_API_H_
    if(impl == nullptr)
    {
        SHOW_MEMORY_ERROR();
        throw MEMORY_ALLOCATED_ERROR();
    }
#endif
}

StdfDTR::~StdfDTR()
{
    delete impl;
    impl = nullptr;
}

const char* StdfDTR::get_text_data() const
{
    if(impl->TEXT_DAT.empty()) return nullptr;
    return impl->TEXT_DAT.c_str();
}

void StdfDTR::set_text_data(const char*data)
{
    if(data) impl->TEXT_DAT.assign(data);
}

unsigned int StdfDTR::parse(const StdfHeader& record)
{
    return impl->Parse(*(record.header));
}

unsigned int StdfDTR::unparse(StdfHeader& record)
{
    return impl->Unparse(*(record.header));
}

void StdfDTR::print(std::ostream& os) const
{
	impl->Print(os);
}

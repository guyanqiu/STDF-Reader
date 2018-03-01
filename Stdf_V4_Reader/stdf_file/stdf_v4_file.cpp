#include "stdf_v4_file.h"
#include <string>

const char* REC_NAME[STDF_V4_RECORD_COUNT] =
{
    "FAR(File Attributes Record)",
    "ATR(Audit Trail Record)",
    "MIR(Master Information Record)",
    "MRR(Master Results Record)",
    "PCR(Part Count Record)",
    "HBR(Hardware Bin Record)",
    "SBR(Software Bin Record)",
    "PMR(Pin Map Record)",
    "PGR(Pin Group Record)",
    "PLR(Pin List Record)",
    "RDR(Retest Data Record)",
    "SDR(Site Description Record)",
    "WIR(Wafer Information Record)",
    "WRR(Wafer Results Record)",
    "WCR(Wafer Configuration Record)",
    "PIR(Part Information Record)",
    "PRR(Part Results Record)",
    "TSR(Test Synopsis Record)",
    "PTR(Parametric Test Record)",
    "MPR(Multiple-Result Parametric Record)",
    "FTR(Functional Test Record)",
    "BPS(Begin Program Section Record)",
    "EPS(End Program Section Record)",
    "GDR(Generic Data Record)",
    "DTR(Datalog Text Record)"
};

//////////////////////////////////////////////////////////////////////////
static std::string to_string(STDF_TYPE type)
{
	switch(type)
	{
	case FAR_TYPE: return "\nFAR----File Attributes Record           ";  break;
	case ATR_TYPE: return "\nATR----Audit Trail Record               ";  break;
	case MIR_TYPE: return "\nMIR----Master Information Record        ";  break;
	case MRR_TYPE: return "\nMRR----Master Result Record             ";  break;
	case PCR_TYPE: return "\nPCR----Part Count Record                ";  break;
	case HBR_TYPE: return "\nHBR----Hardware Bin Record              ";  break;
	case SBR_TYPE: return "\nSBR----Software Bin Record              ";  break;
	case PMR_TYPE: return "\nPMR----Pin Map Record                   ";  break;
	case PGR_TYPE: return "\nPGR----Pin Group Record                 ";  break;
	case PLR_TYPE: return "\nPLR----Pin List Record                  ";  break;
	case RDR_TYPE: return "\nRDR----Reset Data Record                ";  break;
	case SDR_TYPE: return "\nSDR----Site Description Record          ";  break;
	case WIR_TYPE: return "\nWIR----Wafer Information Record         ";  break;
	case WRR_TYPE: return "\nWRR----Wafer Result Record              ";  break;
	case WCR_TYPE: return "\nWCR----Wafer Configuration Record       ";  break;
	case PIR_TYPE: return "\nPIR----Part Information Record          ";  break;
	case PRR_TYPE: return "\nPRR----Part Result Record               ";  break;
	case TSR_TYPE: return "\nTSR----Test Synopsis Record             ";  break;
	case PTR_TYPE: return "\nPTR----Parametric Test Record           ";  break;
	case MPR_TYPE: return "\nMPR----Multiple-Result Parametric Record";  break;
	case FTR_TYPE: return "\nFTR----Functional Test Record           ";  break;
	case BPS_TYPE: return "\nBPS----Begin Program Section Record     ";  break;
	case EPS_TYPE: return "\nEPS----End Program Section Record       ";  break;
	case GDR_TYPE: return "\nGDR----Generic Data Record              ";  break;
	case DTR_TYPE: return "\nDTR----Datalog Text Record              ";  break;
	default:   return "\nUNKONWN----Unkown Record Type"; break;
	}
}

STDF_FILE::STDF_FILE()
{
}

STDF_FILE::~STDF_FILE()
{
	for(unsigned int i = 0; i < Record_Vector.size(); i++)
	{
		delete Record_Vector[i];
	}
}

STDF_FILE_ERROR STDF_FILE::read(const char* filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if(!in) return READ_ERROR;

	StdfHeader header;
	STDF_TYPE type = header.read(in);
	if(type != FAR_TYPE) return FORMATE_ERROR;

	StdfFAR* far_record = new StdfFAR();
	far_record->parse(header);

	unsigned char cpu_type = far_record->get_cpu_type();
    if(cpu_type != 2) return STDF_CPU_TYPE_NOT_SUPPORT;

	unsigned char stdf_version = far_record->get_stdf_version();
	if(stdf_version != 4) return STDF_VERSION_NOT_SUPPORT;

	Record_Vector.push_back(far_record);
	append_record_by_type(far_record);

	while(!in.eof())
	{
        type = header.read(in);
		StdfRecord* record = header.create_record(type);
		if(record)
		{
			record->parse(header);
			Record_Vector.push_back(record);
			append_record_by_type(record);
		}
        if(type == MRR_TYPE) break;
	}
    in.close();
    return STDF_OPERATE_OK;
}

STDF_FILE_ERROR STDF_FILE::write(const char* filename)
{
	std::ofstream out(filename, std::ios::out );
	if(!out) return WRITE_ERROR;

	for(unsigned int i = 0; i < Record_Vector.size(); i++)
	{
		StdfRecord* record = Record_Vector[i];
		out<<to_string(record->type())<<"\n";
		//record->print(out);
		out<<(*record);
	}
    out.close();
    return STDF_OPERATE_OK;
}

STDF_FILE_ERROR STDF_FILE::save(const char* filename)
{
    std::ofstream out(filename, std::ios::binary );
    if(!out) return WRITE_ERROR;

    StdfHeader header;
    for(unsigned int i = 0; i < Record_Vector.size(); i++)
    {
        if(i == 1)
        {
            StdfATR* atr = new StdfATR();
            atr->set_command_line("Save As By STDF Reader");
            atr->set_modify_time(time(NULL));
            atr->unparse(header);
            header.write(out);
        }
        StdfRecord* record = Record_Vector[i];
        record->unparse(header);
        header.write(out);
    }
    out.close();
    return STDF_OPERATE_OK;
}


STDF_FILE_ERROR STDF_FILE::write(const char* filename, STDF_TYPE type)
{
	std::ofstream out(filename, std::ios::out );
	if(!out) return WRITE_ERROR;

	unsigned int count = get_count(type);
	for(unsigned int i = 0; i < count; i++)
	{
		StdfRecord* record = get_record(type, i);
		if(record)
		{
			out<<to_string(type)<<"\n";
			out<<(*record);
		}
	}
    return STDF_OPERATE_OK;
}

const char* STDF_FILE::get_name(STDF_TYPE type)
{
	if(type >= STDF_V4_RECORD_COUNT) return nullptr;
	else return REC_NAME[type];
}

unsigned int STDF_FILE::get_count(STDF_TYPE type)
{
	unsigned int count = 0;
	switch(type)
	{
	case FAR_TYPE: count = StdfFAR_Vector.size(); break;
	case ATR_TYPE: count = StdfATR_Vector.size(); break;
	case MIR_TYPE: count = StdfMIR_Vector.size(); break;
	case MRR_TYPE: count = StdfMRR_Vector.size(); break;
	case PCR_TYPE: count = StdfPCR_Vector.size(); break;
	case HBR_TYPE: count = StdfHBR_Vector.size(); break;
	case SBR_TYPE: count = StdfSBR_Vector.size(); break;
	case PMR_TYPE: count = StdfPMR_Vector.size(); break;
	case PGR_TYPE: count = StdfPGR_Vector.size(); break;
	case PLR_TYPE: count = StdfPLR_Vector.size(); break;
	case RDR_TYPE: count = StdfRDR_Vector.size(); break;
	case SDR_TYPE: count = StdfSDR_Vector.size(); break;
	case WIR_TYPE: count = StdfWIR_Vector.size(); break;
	case WRR_TYPE: count = StdfWRR_Vector.size(); break;
	case WCR_TYPE: count = StdfWCR_Vector.size(); break;
	case PIR_TYPE: count = StdfPIR_Vector.size(); break;
	case PRR_TYPE: count = StdfPRR_Vector.size(); break;
	case TSR_TYPE: count = StdfTSR_Vector.size(); break;
	case PTR_TYPE: count = StdfPTR_Vector.size(); break;
	case MPR_TYPE: count = StdfMPR_Vector.size(); break;
	case FTR_TYPE: count = StdfFTR_Vector.size(); break;
	case BPS_TYPE: count = StdfBPS_Vector.size(); break;
	case EPS_TYPE: count = StdfEPS_Vector.size(); break;
	case GDR_TYPE: count = StdfGDR_Vector.size(); break;
	case DTR_TYPE: count = StdfDTR_Vector.size(); break;
	default: count = 0; break;
	}
	return count;
}

unsigned int STDF_FILE::get_total_count()
{
    return Record_Vector.size();
}

StdfRecord* STDF_FILE::get_record(unsigned int index)
{
    StdfRecord* record = nullptr;
    if(index < Record_Vector.size()) record = Record_Vector[index];
    return record;
}

StdfRecord* STDF_FILE::get_record(STDF_TYPE type, unsigned int index)
{
	StdfRecord* record = nullptr;
	switch(type)
	{
	case FAR_TYPE: if(index < StdfFAR_Vector.size()) record = StdfFAR_Vector[index]; break;
	case ATR_TYPE: if(index < StdfATR_Vector.size()) record = StdfATR_Vector[index]; break;
	case MIR_TYPE: if(index < StdfMIR_Vector.size()) record = StdfMIR_Vector[index]; break;
	case MRR_TYPE: if(index < StdfMRR_Vector.size()) record = StdfMRR_Vector[index]; break;
	case PCR_TYPE: if(index < StdfPCR_Vector.size()) record = StdfPCR_Vector[index]; break;
	case HBR_TYPE: if(index < StdfHBR_Vector.size()) record = StdfHBR_Vector[index]; break;
	case SBR_TYPE: if(index < StdfSBR_Vector.size()) record = StdfSBR_Vector[index]; break;
	case PMR_TYPE: if(index < StdfPMR_Vector.size()) record = StdfPMR_Vector[index]; break;
	case PGR_TYPE: if(index < StdfPGR_Vector.size()) record = StdfPGR_Vector[index]; break;
	case PLR_TYPE: if(index < StdfPLR_Vector.size()) record = StdfPLR_Vector[index]; break;
	case RDR_TYPE: if(index < StdfRDR_Vector.size()) record = StdfRDR_Vector[index]; break;
	case SDR_TYPE: if(index < StdfSDR_Vector.size()) record = StdfSDR_Vector[index]; break;
	case WIR_TYPE: if(index < StdfWIR_Vector.size()) record = StdfWIR_Vector[index]; break;
	case WRR_TYPE: if(index < StdfWRR_Vector.size()) record = StdfWRR_Vector[index]; break;
	case WCR_TYPE: if(index < StdfWCR_Vector.size()) record = StdfWCR_Vector[index]; break;
	case PIR_TYPE: if(index < StdfPIR_Vector.size()) record = StdfPIR_Vector[index]; break;
	case PRR_TYPE: if(index < StdfPRR_Vector.size()) record = StdfPRR_Vector[index]; break;
	case TSR_TYPE: if(index < StdfTSR_Vector.size()) record = StdfTSR_Vector[index]; break;
	case PTR_TYPE: if(index < StdfPTR_Vector.size()) record = StdfPTR_Vector[index]; break;
	case MPR_TYPE: if(index < StdfMPR_Vector.size()) record = StdfMPR_Vector[index]; break;
	case FTR_TYPE: if(index < StdfFTR_Vector.size()) record = StdfFTR_Vector[index]; break;
	case BPS_TYPE: if(index < StdfBPS_Vector.size()) record = StdfBPS_Vector[index]; break;
	case EPS_TYPE: if(index < StdfEPS_Vector.size()) record = StdfEPS_Vector[index]; break;
	case GDR_TYPE: if(index < StdfGDR_Vector.size()) record = StdfGDR_Vector[index]; break;
	case DTR_TYPE: if(index < StdfDTR_Vector.size()) record = StdfDTR_Vector[index]; break;
	default: record = nullptr; break;
	}
	return record;
}

void STDF_FILE::append_record_by_type(StdfRecord* record)
{
	STDF_TYPE type = record->type();
	switch(type)
	{
	case FAR_TYPE: StdfFAR_Vector.push_back(static_cast<StdfFAR*>(record)) ; break;
	case ATR_TYPE: StdfATR_Vector.push_back(static_cast<StdfATR*>(record)) ; break;
	case MIR_TYPE: StdfMIR_Vector.push_back(static_cast<StdfMIR*>(record)) ; break;
	case MRR_TYPE: StdfMRR_Vector.push_back(static_cast<StdfMRR*>(record)) ; break;
	case PCR_TYPE: StdfPCR_Vector.push_back(static_cast<StdfPCR*>(record)) ; break;
	case HBR_TYPE: StdfHBR_Vector.push_back(static_cast<StdfHBR*>(record)) ; break;
	case SBR_TYPE: StdfSBR_Vector.push_back(static_cast<StdfSBR*>(record)) ; break;
	case PMR_TYPE: StdfPMR_Vector.push_back(static_cast<StdfPMR*>(record)) ; break;
	case PGR_TYPE: StdfPGR_Vector.push_back(static_cast<StdfPGR*>(record)) ; break;
	case PLR_TYPE: StdfPLR_Vector.push_back(static_cast<StdfPLR*>(record)) ; break;
	case RDR_TYPE: StdfRDR_Vector.push_back(static_cast<StdfRDR*>(record)) ; break;
	case SDR_TYPE: StdfSDR_Vector.push_back(static_cast<StdfSDR*>(record)) ; break;
	case WIR_TYPE: StdfWIR_Vector.push_back(static_cast<StdfWIR*>(record)) ; break;
	case WRR_TYPE: StdfWRR_Vector.push_back(static_cast<StdfWRR*>(record)) ; break;
	case WCR_TYPE: StdfWCR_Vector.push_back(static_cast<StdfWCR*>(record)) ; break;
	case PIR_TYPE: StdfPIR_Vector.push_back(static_cast<StdfPIR*>(record)) ; break;
	case PRR_TYPE: StdfPRR_Vector.push_back(static_cast<StdfPRR*>(record)) ; break;
	case TSR_TYPE: StdfTSR_Vector.push_back(static_cast<StdfTSR*>(record)) ; break;
	case PTR_TYPE: StdfPTR_Vector.push_back(static_cast<StdfPTR*>(record)) ; break;
	case MPR_TYPE: StdfMPR_Vector.push_back(static_cast<StdfMPR*>(record)) ; break;
	case FTR_TYPE: StdfFTR_Vector.push_back(static_cast<StdfFTR*>(record)) ; break;
	case BPS_TYPE: StdfBPS_Vector.push_back(static_cast<StdfBPS*>(record)) ; break;
	case EPS_TYPE: StdfEPS_Vector.push_back(static_cast<StdfEPS*>(record)) ; break;
	case GDR_TYPE: StdfGDR_Vector.push_back(static_cast<StdfGDR*>(record)) ; break;
	case DTR_TYPE: StdfDTR_Vector.push_back(static_cast<StdfDTR*>(record)) ; break;
	default : break;
	}
}

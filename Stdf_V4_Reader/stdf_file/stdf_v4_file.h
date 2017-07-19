/*************************************************************************
 * For testing stdf files.
*************************************************************************/
#ifndef _STDF_V4_AUX_FILE_H_
#define _STDF_V4_AUX_FILE_H_

#include "../stdf_api/stdf_v4_api.h"
#include <vector>

#define STDF_V4_RECORD_COUNT 25

enum STDF_FILE_ERROR : int
{
    STDF_OPERATE_OK = 0,
	READ_ERROR = -1,
	FORMATE_ERROR = -2,
    STDF_CPU_TYPE_NOT_SUPPORT = -3,
	STDF_VERSION_NOT_SUPPORT = -4,
	WRITE_ERROR = -5,
};

class STDF_FILE
{
public:
	STDF_FILE();
	~STDF_FILE();

	STDF_FILE_ERROR read(const char* filename);
	STDF_FILE_ERROR write(const char* filename, STDF_TYPE type);
	STDF_FILE_ERROR write(const char* filename);
    STDF_FILE_ERROR save(const char* filename);
	const char* get_name(STDF_TYPE type);
	unsigned int get_count(STDF_TYPE type);
	StdfRecord* get_record(STDF_TYPE type, unsigned int index);

private:
	void append_record_by_type(StdfRecord* record);
	STDF_FILE(const STDF_FILE& src);
	STDF_FILE& operator=(const STDF_FILE& src);

private:
	std::vector<StdfRecord*> Record_Vector;
	std::vector<StdfFAR*> StdfFAR_Vector;
	std::vector<StdfATR*> StdfATR_Vector;
	std::vector<StdfMIR*> StdfMIR_Vector;
	std::vector<StdfMRR*> StdfMRR_Vector;
	std::vector<StdfPCR*> StdfPCR_Vector;
	std::vector<StdfHBR*> StdfHBR_Vector;
	std::vector<StdfSBR*> StdfSBR_Vector;
	std::vector<StdfPMR*> StdfPMR_Vector;
	std::vector<StdfPGR*> StdfPGR_Vector;
	std::vector<StdfPLR*> StdfPLR_Vector;
	std::vector<StdfRDR*> StdfRDR_Vector;
	std::vector<StdfSDR*> StdfSDR_Vector;
	std::vector<StdfWIR*> StdfWIR_Vector;
	std::vector<StdfWRR*> StdfWRR_Vector;
	std::vector<StdfWCR*> StdfWCR_Vector;
	std::vector<StdfPIR*> StdfPIR_Vector;
	std::vector<StdfPRR*> StdfPRR_Vector;
	std::vector<StdfTSR*> StdfTSR_Vector;
	std::vector<StdfPTR*> StdfPTR_Vector;
	std::vector<StdfMPR*> StdfMPR_Vector;
	std::vector<StdfFTR*> StdfFTR_Vector;
	std::vector<StdfBPS*> StdfBPS_Vector;
	std::vector<StdfEPS*> StdfEPS_Vector;
	std::vector<StdfGDR*> StdfGDR_Vector;
	std::vector<StdfDTR*> StdfDTR_Vector;

};

#endif//_STDF_V4_AUX_FILE_H_

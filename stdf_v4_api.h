#ifndef _STDFV4_API_H_
#define _STDFV4_API_H_
#include <iostream>
#include <fstream>
#include <ctime>

enum STDF_TYPE : int
{
    FAR_TYPE = 0,      // File Attributes Record
    ATR_TYPE = 1,      // Audit Trail Record
    MIR_TYPE = 2,      // Master Information Record
    MRR_TYPE = 3,      // Master Result Record
    PCR_TYPE = 4,      // Part Count Record
    HBR_TYPE = 5,      // Hardware Bin Record
    SBR_TYPE = 6,      // Software Bin Record
    PMR_TYPE = 7,      // Pin Map Record
    PGR_TYPE = 8,      // Pin Group Record
    PLR_TYPE = 9,      // Pin List Record
    RDR_TYPE = 10,     // Reset Data Record
    SDR_TYPE = 11,     // Site Description Record
    WIR_TYPE = 12,     // Wafer Information Record
    WRR_TYPE = 13,     // Wafer Result Record
    WCR_TYPE = 14,     // Wafer Configuration Record
    PIR_TYPE = 15,     // Part Information Record
    PRR_TYPE = 16,     // Part Result Record
    TSR_TYPE = 17,     // Test Synopsis Record
    PTR_TYPE = 18,     // Parametric Test Record
    MPR_TYPE = 19,     // Multiple-Result Parametric Record
    FTR_TYPE = 20,     // Functional Test Record
    BPS_TYPE = 21,     // Begin Program Section Record
    EPS_TYPE = 22,     // End Program Section Record
    GDR_TYPE = 23,     // Generic Data Record
    DTR_TYPE = 24,     // Datalog Text Record
    UNKNOWN_TYPE = 25, // Other Types
};

class StdfRecord;
class StdfHeader
{
    friend class StdfFAR;
    friend class StdfATR ;
    friend class StdfMIR ;
    friend class StdfMRR ;
    friend class StdfPCR ;
    friend class StdfHBR ;
    friend class StdfSBR ;
    friend class StdfPMR ;
    friend class StdfPGR ;
    friend class StdfPLR ;
    friend class StdfRDR ;
    friend class StdfSDR ;
    friend class StdfWIR ;
    friend class StdfWRR ;
    friend class StdfWCR ;
    friend class StdfPIR ;
    friend class StdfPRR ;
    friend class StdfTSR ;
    friend class StdfPTR ;
    friend class StdfMPR ;
    friend class StdfFTR ;
    friend class StdfBPS ;
    friend class StdfEPS ;
    friend class StdfGDR ;
    friend class StdfDTR ;
public:
    StdfHeader();
    ~StdfHeader();

    STDF_TYPE read(std::ifstream& file_stream);
    int write(std::ofstream& file_stream);

	StdfRecord* create_record(STDF_TYPE type);
    // getter
    STDF_TYPE get_type() const;
    unsigned short get_length() const;
    unsigned char get_main_type() const;
    unsigned char get_sub_type() const;

private:
    typedef class RecordHeader Impl;
    Impl* header;
    StdfHeader(const StdfHeader& );
    StdfHeader& operator=(const StdfHeader& src);
	void * operator new(size_t size);
};

class StdfRecord
{
protected:
	char m_name[4];
	STDF_TYPE m_type;
public:
	virtual unsigned int parse(const StdfHeader& record) = 0;
	virtual unsigned int unparse(StdfHeader& record) = 0;
	virtual void print(std::ostream& os) const = 0;
	friend std::ostream& operator << (std::ostream& os, const StdfRecord& record);

	const char* name() ;
	STDF_TYPE type();
	StdfRecord(const char* name, STDF_TYPE type);
	virtual ~StdfRecord();
private:
	StdfRecord(const StdfRecord& );
	StdfRecord& operator=(const StdfRecord& src);
};

class StdfFAR : public StdfRecord
{
public:
    StdfFAR();
    ~StdfFAR();

    unsigned char get_cpu_type() const;
    unsigned char get_stdf_version() const;

    // Just Support CPU Type = 2 and STDF Version = 4.
    //int set_cpu_type(unsigned char type);
    //int set_stdf_version(unsigned char version);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class FileAttributes Impl;
    Impl *impl;
};


class StdfATR : public StdfRecord
{
public:
    StdfATR();
    ~StdfATR();

    const char* get_command_line() const;
    time_t get_modify_time()const;

    void set_command_line(const char* command);
    void set_modify_time(time_t t);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const ;

private:
    typedef class AuditTrail Impl;
    Impl *impl;
};

enum ModeCode : char
{
    Automatic_Edge_Lock = 'A',
    Checker             = 'C',
    Development         = 'D',
    Debug               = 'D',
    Engineering         = 'E',
    Maintenance         = 'M',
    Production_Test     = 'P',
    Quality_Control     = 'Q',
};

class StdfMIR : public StdfRecord
{
public:
    StdfMIR();
    ~StdfMIR();

    //getter
    time_t get_setup_time() const;
    time_t get_start_time() const;
    unsigned char get_station_number() const;
    char get_mode_code() const;
    char get_retest_code() const;
    char get_protection_code() const;
    unsigned short get_burn_time() const;
    char get_command_code()  const;
    const char* get_lot_id() const;
    const char* get_part_type() const;
    const char* get_node_name() const;
    const char* get_tester_type() const;
    const char* get_program_name() const;
    const char* get_program_revision() const;
    const char* get_sublot_id() const;
    const char* get_operator_id() const;
    const char* get_exec_file_type() const;
    const char* get_exec_file_version() const;
    const char* get_test_code()  const;
    const char* get_test_temperature()  const;
    const char* get_user_text()  const;
    const char* get_auxiliary_filename()  const;
    const char* get_package_type()  const;
    const char* get_family_id()  const;
    const char* get_date_code()  const;
    const char* get_facility_id()  const;
    const char* get_floor_id()  const;
    const char* get_process_id() const;
    const char* get_operation_freq()  const;
    const char* get_spec_name()  const;
    const char* get_spec_version()  const;
    const char* get_testflow_id()  const;
    const char* get_setup_id()  const;
    const char* get_design_version()  const;
    const char* get_engineering_id()  const;
    const char* get_rom_id()  const;
    const char* get_tester_number()  const;
    const char* get_supervisor_name()  const;
    //setter
    void set_setup_time(time_t t);
    void set_start_time(time_t t);
    void set_station_number(unsigned char number);
    void set_mode_code(ModeCode code);
    void set_retest_code(char code);
    void set_protection_code(char code);
    void set_burn_time(unsigned short t);
    void set_command_code(char code);
    void set_lot_id(const char * id);
    void set_part_type(const char *type);
    void set_node_name(const char *name);
    void set_tester_type(const char *type);
    void set_program_name(const char *name);
    void set_program_revision(const char *revision);
    void set_sublot_id(const char *id);
    void set_operator_id(const char *id);
    void set_exec_file_type(const char *type);
    void set_exec_file_version(const char *version);
    void set_test_code(const char *code);
    void set_test_temperature(const char *temp);
    void set_user_text(const char *text);
    void set_auxiliary_filename(const char *filename);
    void set_package_type(const char *package);
    void set_family_id(const char *id);
    void set_date_code(const char *date);
    void set_facility_id(const char *id);
    void set_floor_id(const char *id);
    void set_process_id(const char *id);
    void set_operation_freq(const char *freq);
    void set_spec_name(const char *name);
    void set_spec_version(const char *version);
    void set_testflow_id(const char *id);
    void set_setup_id(const char *id);
    void set_design_version(const char *version);
    void set_engineering_id(const char *lot_id);
    void set_rom_id(const char *rom_id);
    void set_tester_number(const char *serial_number);
    void set_supervisor_name(const char *name);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class MasterInformation Impl;
    Impl *impl;
};


class StdfMRR : public StdfRecord
{
public:
    StdfMRR();
    ~StdfMRR();

    time_t get_finish_time()  const;
    char get_disposition_code() const;
    const char* get_user_discription() const;
    const char* get_exec_discription() const;

    void set_finish_time(time_t t);
    void set_disposition_code(char code);
    void set_user_discription(const char* discription);
    void set_exec_discription(const char* discription);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class MasterResults Impl;
    Impl *impl;
};


class StdfPCR : public StdfRecord
{
public:
    StdfPCR();
    ~StdfPCR();

    unsigned char get_head_number() const;
    unsigned char get_site_number() const;
    unsigned int  get_part_count() const;
    unsigned int  get_retest_count() const;
    unsigned int  get_abort_count() const;
    unsigned int  get_passed_count() const;
    unsigned int  get_func_test_count() const;

    void set_head_number(unsigned char number);
    void set_site_number(unsigned char number);
    void set_part_count(unsigned int  count);
    void set_retest_count(unsigned int count);
    void set_abort_count(unsigned int count);
    void set_passed_count(unsigned int count);
    void set_func_test_count(unsigned int count);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class PartCount Impl;
    Impl *impl;
};


class StdfHBR : public StdfRecord
{
public:
    StdfHBR();
    ~StdfHBR();

    unsigned char get_head_number() const;
    unsigned char get_site_number() const;
    unsigned short get_hardbin_number() const;
    unsigned int get_hardbin_count() const;
    char get_hardbin_indication() const;
    const char* get_hardbin_name() const;

    void set_head_number(unsigned char number);
    void set_site_number(unsigned char number);
    void set_hardbin_number(unsigned short number);
    void set_hardbin_count(unsigned int count);
    void set_hardbin_indication(char pass_fail);
    void set_hardbin_name(const char* name);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class HardwareBin Impl;
    Impl *impl;
};


class StdfSBR : public StdfRecord
{
public:
    StdfSBR();
    ~StdfSBR();

    unsigned char get_head_number() const;
    unsigned char get_site_number() const;
    unsigned short get_softbin_number() const;
    unsigned int get_softbin_count() const;
    char get_softbin_indication() const;
    const char* get_softbin_name() const;

    void set_head_number(unsigned char number);
    void set_site_number(unsigned char number);
    void set_softbin_number(unsigned short number);
    void set_softbin_count(unsigned int count);
    void set_softbin_indication(char pass_fail);
    void set_softbin_name(const char* name);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class SoftwareBin Impl;
    Impl *impl;
};


class StdfPMR : public StdfRecord
{
public:
    StdfPMR();
    ~StdfPMR();

    unsigned short get_pin_index() const;
    unsigned short get_channel_type() const;
    const char* get_channel_name() const;
    const char* get_physical_name() const;
    const char* get_logical_name() const ;
    unsigned char get_head_number() const;
    unsigned char get_site_number() const;

    void set_pin_index(unsigned short index);
    void set_channel_type(unsigned short type);
    void set_channel_name(const char* name);
    void set_physical_name(const char* name);
    void set_logical_name(const char* name);
    void set_head_number(unsigned char number);
    void set_site_number(unsigned char number);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class PinMap Impl;
    Impl *impl;
};


class StdfPGR : public StdfRecord
{
public:
    StdfPGR();
    ~StdfPGR();

    unsigned short get_group_index() const;
    const char* get_group_name() const;
    unsigned short get_pin_count() const;
    unsigned short get_pin_number(unsigned short index) const;

    void set_group_index(unsigned short index);
    void set_group_name(const char* name);
    void set_pin_count(unsigned short count);
    void set_pin_number(unsigned short index, unsigned short pin_number);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class PinGroup Impl;
    Impl *impl;
};

enum GroupMode : unsigned short
{
    UnknownMode               = 0,
    NormalMode                = 10,
    SCIO                      = 20,
    SCIO_Midband              = 21,
    SCIO_Valid                = 22,
    SCIO_Window_Sustain       = 23,
    Dual_drive                = 30,
    Dual_drive_Midband        = 31,
    Dual_drive_Valid          = 32,
    Dual_drive_Window_Sustain = 33
};
enum GroupRadix : unsigned char
{
    DefaultRadix     = 0,
    BinaryRadix      = 2,
    OctalRadix       = 8,
    DecimalRadix     = 10,
    HexadecimalRadix = 16,
    SymbolicRadix    = 20,
};

class StdfPLR : public StdfRecord
{
public:
    StdfPLR();
    ~StdfPLR();

    unsigned short get_group_count() const;
    unsigned short get_group_number(unsigned short index) const;
    unsigned short get_group_mode(unsigned short index) const;
    unsigned char get_group_radix(unsigned short index) const;
    const char* get_program_state_right(unsigned short index) const;
    const char* get_return_state_right(unsigned short index) const;
    const char* get_program_state_left(unsigned short index) const;
    const char* get_return_state_left(unsigned short index) const;

    void set_group_count(unsigned short count);
    void set_group_number(unsigned short index, unsigned short number);
    void set_group_mode(unsigned short index, GroupMode mode);
    void set_group_radix(unsigned short index, GroupRadix radix);
    void set_program_state_right(unsigned short index, const char* state);
    void set_return_state_right(unsigned short index, const char* state);
    void set_program_state_left(unsigned short index, const char* state);
    void set_return_state_left(unsigned short index, const char* state);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class PinList Impl;
    Impl *impl;
};


class StdfRDR : public StdfRecord
{
public:
    StdfRDR();
    ~StdfRDR();
    unsigned short get_bin_count() const;
    unsigned short get_bin_number(unsigned short index) const;

    void set_bin_count(unsigned short count);
    void set_bin_number(unsigned short index, unsigned short bin_number);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class RetestData Impl;
    Impl *impl;
};


class StdfSDR : public StdfRecord
{
public:
    StdfSDR();
    ~StdfSDR();

    unsigned char get_head_number() const;
    unsigned char get_site_group_number() const;
    unsigned char get_site_count() const;
    unsigned char get_site_number(unsigned char index) const;
    const char* get_handler_type() const;
    const char* get_handler_id() const;
    const char* get_probecard_type() const;
    const char* get_probecard_id() const;
    const char* get_loadboard_type() const;
    const char* get_loadboard_id() const;
    const char* get_dibboard_type() const;
    const char* get_dibboard_id() const;
    const char* get_cable_type() const;
    const char* get_cable_id() const;
    const char* get_contactor_type() const;
    const char* get_contactor_id() const;
    const char* get_laser_type() const;
    const char* get_laser_id() const;
    const char* get_equipment_type() const;
    const char* get_equipment_id() const;

    void set_head_number(unsigned char number);
    void set_site_group_number(unsigned char number);
    void set_site_count(unsigned char count);
    void set_site_number(unsigned char index, unsigned char site_number);
    void set_handler_type(const char* type);
    void set_handler_id(const char* id);
    void set_probecard_type(const char* type);
    void set_probecard_id(const char* id);
    void set_loadboard_type(const char* type);
    void set_loadboard_id(const char* id);
    void set_dibboard_type(const char* type);
    void set_dibboard_id(const char* id);
    void set_cable_type(const char* type);
    void set_cable_id(const char* id);
    void set_contactor_type(const char* type);
    void set_contactor_id(const char* id);
    void set_laser_type(const char* type);
    void set_laser_id(const char* id);
    void set_equipment_type(const char* type);
    void set_equipment_id(const char* id);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class SiteDescription Impl;
    Impl *impl;
};


class StdfWIR : public StdfRecord
{
public:
    StdfWIR();
    ~StdfWIR();

    unsigned char get_head_number() const;
    unsigned char get_group_number() const;
    time_t get_start_time() const;
    const char* get_wafer_id() const;

    void set_head_number(unsigned char number);
    void set_group_number(unsigned char number);
    void set_start_time(time_t t);
    void set_wafer_id(const char* id);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class WaferInformation Impl;
    Impl *impl;
};


class StdfWRR : public StdfRecord
{
public:
    StdfWRR();
    ~StdfWRR();

    unsigned char get_head_number() const;
    unsigned char get_group_number() const;
    time_t get_finish_time() const;
    unsigned int get_part_count() const;
    unsigned int get_retest_count() const;
    unsigned int get_abort_count() const;
    unsigned int get_pass_count() const;
    unsigned int get_func_count() const;
    const char* get_wafer_id() const;
    const char* get_fabwafer_id() const;
    const char* get_frame_id() const;
    const char* get_mask_id() const;
    const char* get_user_discription() const;
    const char* get_exec_discription() const;

    void set_head_number(unsigned char number);
    void set_group_number(unsigned char number);
    void set_finish_time(time_t  t);
    void set_part_count(unsigned int count);
    void set_retest_count(unsigned int count);
    void set_abort_count(unsigned int count);
    void set_pass_count(unsigned int count);
    void set_func_count(unsigned int count);
    void set_wafer_id(const char* id);
    void set_fabwafer_id(const char* id);
    void set_frame_id(const char* id);
    void set_mask_id(const char* id);
    void set_user_discription(const char* text);
    void set_exec_discription(const char* text);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class WaferResults Impl;
    Impl *impl;
};

enum WaferUnits : unsigned char
{
    Units_Unknown     = 0,
    Units_Inches      = 1,
    Units_Centimeters = 2,
    Units_Millimeters = 3,
    Units_Mils        = 4,
};

enum WaferFlat : char
{
    Flat_Up      = 'U',
    Flat_Down    = 'D',
    Flat_Left    = 'L',
    Flat_Right   = 'R',
    Flat_Unknown = ' ',
};

enum PosDirection : char
{
    Pos_Up      = 'U',
    Pos_Down    = 'D',
    Pos_Unknown = ' ',
};

class StdfWCR : public StdfRecord
{
public:
    StdfWCR();
    ~StdfWCR();

    float get_wafer_size() const;
    float get_die_height() const;
    float get_die_width() const;
    unsigned char get_wafer_unit() const;
    char get_wafer_flat() const;
    short get_center_x() const;
    short get_center_y() const;
    char get_positive_x() const;
    char get_positive_y() const;

    void set_wafer_size(float size);
    void set_die_height(float height);
    void set_die_width(float width);
    void set_wafer_unit(WaferUnits unit);
    void set_wafer_flat(WaferFlat flat_orientation);
    void set_center_x(short x);
    void set_center_y(short y);
    void set_positive_x(PosDirection positive_direction);
    void set_positive_y(PosDirection positive_direction);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class WaferConfiguration Impl;
    Impl *impl;
};


class StdfPIR : public StdfRecord
{
public:
    StdfPIR();
    ~StdfPIR();

    unsigned char get_head_number() const;
    unsigned char get_site_number() const;

    void set_head_number(unsigned char number);
    void set_site_number(unsigned char number);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class PartInformation Impl;
    Impl *impl;
};


class StdfPRR : public StdfRecord
{
public:
    StdfPRR();
    ~StdfPRR();

    unsigned char get_head_number() const;
    unsigned char get_site_number() const;
	unsigned char get_part_information_flag() const;
    unsigned short get_number_test() const;
    unsigned short get_hardbin_number() const;
    unsigned short get_softbin_number() const;
    short get_x_coordinate() const;
    short get_y_coordinate() const;
    unsigned int get_elapsed_ms() const;
    const char* get_part_id() const;
    const char* get_part_discription() const;
    bool part_supersede_flag() const;
    bool part_abnormal_flag() const;
    bool part_failed_flag() const;
    bool pass_fail_flag_invalid() const;

    void set_head_number(unsigned char number);
    void set_site_number(unsigned char number);
    void set_number_test(unsigned short count);
    void set_hardbin_number(unsigned short number);
    void set_softbin_number(unsigned short number);
    void set_x_coordinate(short x);
    void set_y_coordinate(short y);
    void set_elapsed_ms(unsigned int time_ms);
    void set_part_discription(const char* text);
    void set_part_id(const char* id);
    void part_supersede_flag(bool flag);
    void part_abnormal_flag(bool flag);
    void part_failed_flag(bool flag);
    void pass_fail_flag_invalid(bool flag);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class PartResults Impl;
    Impl *impl;
};


enum TestType : char
{
    Parametric_Test      = 'P',
    Functional_Test      = 'F',
    Multiple_Result_Test = 'M',
    UnknownTestType      = ' ',
};

class StdfTSR : public StdfRecord
{
public:
    StdfTSR();
    ~StdfTSR();

    unsigned char get_head_number() const;
    unsigned char get_site_number() const;
    char get_test_type() const;
    unsigned int get_test_number() const;
    unsigned int get_exec_count() const;
    unsigned int get_fail_count() const;
    unsigned int get_alarm_count() const;
    const char* get_test_name() const;
    const char* get_sequencer_name() const;
    const char* get_test_label() const;
	unsigned char get_optional_data_flag() const;
    float get_average_time_s() const;
    float get_result_min() const;
    float get_result_max() const;
    float get_result_sum() const;
    float get_result_squares_sum() const;

    bool result_min_invalid() const;
    bool result_max_invalid() const;
    bool test_time_invalid() const;
    bool result_sum_invalid() const;
    bool result_squares_sum_invalid() const;

    void set_head_number(unsigned char number);
    void set_site_number(unsigned char number);
    void set_test_type(TestType type);
    void set_test_number(unsigned int number);
    void set_exec_count(unsigned int count);
    void set_fail_count(unsigned int count);
    void set_alarm_count(unsigned int count);
    void set_test_name(const char* name);
    void set_sequencer_name(const char* name);
    void set_test_label(const char* label);
    void set_average_time_s(float seconds);
    void set_result_min(float value);
    void set_result_max(float value);
    void set_result_sum(float value);
    void set_result_squares_sum(float value);

    void result_min_invalid(bool flag);
    void result_max_invalid(bool flag);
    void test_time_invalid(bool flag);
    void result_sum_invalid(bool flag);
    void result_squares_sum_invalid(bool flag);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class TestSynopsis Impl;
    Impl *impl;
};


class StdfPTR : public StdfRecord
{
public:
    StdfPTR();
    ~StdfPTR();

    unsigned int get_test_number() const;
    unsigned char get_head_number() const;
    unsigned char get_site_number() const;
	unsigned char get_test_flag() const;
	unsigned char get_parametric_test_flag() const;
    float get_result() const;
    const char* get_test_text() const;
    const char* get_alarm_id() const;
	unsigned char get_optional_data_flag() const;
    signed char get_result_exponent() const;
    signed char get_lowlimit_exponent() const;
    signed char get_highlimit_exponent() const;
    float get_low_limit() const;
    float get_high_limit() const;
    const char* get_unit() const;
    const char* get_result_format() const;
    const char* get_lowlimit_format() const;
    const char* get_highlimit_format() const;
    float get_low_spec() const;
    float get_high_spec() const;

    // TEST_FLG
    bool alarm_detected() const;
    bool result_invalid() const;
    bool result_unreliable() const;
    bool timeout_occured() const;
    bool test_unexecuted() const;
    bool test_aborted() const;
    bool test_pfflag_invalid() const;
    bool test_failed() const;
    //PARM_FLG
    bool param_scale_error() const;
    bool param_drift_error() const;
    bool param_oscillation() const;
    bool result_higher_limit() const;
    bool result_lower_limit() const;
    bool passed_alternate_limit() const;
    bool equal_lowlimit_pass() const;
    bool equal_highlimit_pass() const;
    //OPT_FLAG
    bool result_exponent_invalid() const;
    bool no_low_spec() const;
    bool no_high_spec() const;
    bool low_limit_invalid() const;
    bool high_limit_invalid() const;
    bool no_low_limit() const;
    bool no_high_limit() const;

    void set_test_number(unsigned int number);
    void set_head_number(unsigned char number);
    void set_site_number(unsigned char number);
    void set_result(float value);
    void set_test_text(const char* text);
    void set_alarm_id(const char* id);
    void set_result_exponent(signed char exponent);
    void set_lowlimit_exponent(signed char exponent);
    void set_highlimit_exponent(signed char exponent);
    void set_low_limit(float limit);
    void set_high_limit(float limit);
    void set_unit(const char* unit);
    void set_result_format(const char* format);
    void set_lowlimit_format(const char* format);
    void set_highlimit_format(const char* format);
    void set_low_spec(float spec);
    void set_high_spec(float spec);

    // TEST_FLG
    void alarm_detected(bool flag);
    void result_invalid(bool flag);
    void result_unreliable(bool flag);
    void timeout_occured(bool flag);
    void test_unexecuted(bool flag);
    void test_aborted(bool flag);
    void test_pfflag_invalid(bool flag);
    void test_failed(bool flag);
    //PARM_FLG
    void param_scale_error(bool flag);
    void param_drift_error(bool flag);
    void param_oscillation(bool flag);
    void result_higher_limit(bool flag);
    void result_lower_limit(bool flag);
    void passed_alternate_limit(bool flag);
    void equal_lowlimit_pass(bool flag);
    void equal_highlimit_pass(bool flag);
    //OPT_FLAG
    void result_exponent_invalid(bool flag);
    void no_low_spec(bool flag);
    void no_high_spec(bool flag);
    void low_limit_invalid(bool flag);
    void high_limit_invalid(bool flag);
    void no_low_limit(bool flag);
    void no_high_limit(bool flag);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class ParametricTest Impl;
    Impl *impl;
};


class StdfMPR : public StdfRecord
{
public:
    StdfMPR();
    ~StdfMPR();

    unsigned int get_test_number() const;
    unsigned char get_head_number() const;
    unsigned char get_site_number() const;
	unsigned char get_test_flag() const;
	unsigned char get_parametric_test_flag() const;
    unsigned short get_pin_count() const;
    unsigned short get_result_count() const;
    unsigned char get_return_state(unsigned short index) const;
    float get_return_result(unsigned short index) const;
    const char* get_test_text() const;
    const char* get_alarm_id() const;
	unsigned char get_optional_data_flag() const;
    signed char get_result_exponent() const;
    signed char get_lowlimit_exponent() const;
    signed char get_highlimit_exponent() const;
    float get_low_limit() const;
    float get_high_limit() const;
    float get_starting_input() const;
    float get_increment_input() const;
    unsigned short get_pin_index(unsigned short index) const;
    const char* get_unit() const;
    const char* get_unit_input() const;
    const char* get_result_format() const;
    const char* get_lowlimit_format() const;
    const char* get_highlimit_format() const;
    float get_low_spec() const;
    float get_high_spec() const;

    // TEST_FLG
    bool alarm_detected() const;
    bool result_invalid() const;
    bool result_unreliable() const;
    bool timeout_occured() const;
    bool test_unexecuted() const;
    bool test_aborted() const;
    bool test_pfflag_invalid() const;
    bool test_failed() const;
    //PARM_FLG
    bool param_scale_error() const;
    bool param_drift_error() const;
    bool param_oscillation() const;
    bool result_higher_limit() const;
    bool result_lower_limit() const;
    bool passed_alternate_limit() const;
    bool equal_lowlimit_pass() const;
    bool equal_highlimit_pass() const;
    //OPT_FLAG
    bool result_exponent_invalid() const;
    bool start_input_invalid() const;
    bool no_low_spec() const;
    bool no_high_spec() const;
    bool low_limit_invalid() const;
    bool high_limit_invalid() const;
    bool no_low_limit() const;
    bool no_high_limit() const;

    void set_test_number(unsigned int number);
    void set_head_number(unsigned char number);
    void set_site_number(unsigned char number);
    void set_pin_count(unsigned short count);
    void set_result_count(unsigned short count);
    void set_return_state(unsigned short index,  unsigned char nibble_state);
    void set_return_result(unsigned short index, float result);
    void set_test_text(const char* text);
    void set_alarm_id(const char* id);
    void set_result_exponent(signed char exponent);
    void set_lowlimit_exponent(signed char exponent);
    void set_highlimit_exponent(signed char exponent);
    void set_low_limit(float limit);
    void set_high_limit(float limit);
    void set_starting_input(float starting);
    void set_increment_input(float increment);
    void set_pin_index(unsigned short index, unsigned short pin_number);
    void set_unit(const char* unit);
    void set_unit_input(const char* input_unit);
    void set_result_format(const char* format);
    void set_lowlimit_format(const char* format);
    void set_highlimit_format(const char* format);
    void set_low_spec(float spec);
    void set_high_spec(float spec);

    // TEST_FLG
    void alarm_detected(bool flag);
    void result_invalid(bool flag);
    void result_unreliable(bool flag);
    void timeout_occured(bool flag);
    void test_unexecuted(bool flag);
    void test_aborted(bool flag);
    void test_pfflag_invalid(bool flag);
    void test_failed(bool flag);
    //PARM_FLG
    void param_scale_error(bool flag);
    void param_drift_error(bool flag);
    void param_oscillation(bool flag);
    void result_higher_limit(bool flag);
    void result_lower_limit(bool flag);
    void passed_alternate_limit(bool flag);
    void equal_lowlimit_pass(bool flag);
    void equal_highlimit_pass(bool flag);
    //OPT_FLAG
    void result_exponent_invalid(bool flag);
    void start_input_invalid(bool flag);
    void no_low_spec(bool flag);
    void no_high_spec(bool flag);
    void low_limit_invalid(bool flag);
    void high_limit_invalid(bool flag);
    void no_low_limit(bool flag);
    void no_high_limit(bool flag);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class MultipleResultParametric Impl;
    Impl *impl;
};

enum ReturnState : unsigned char
{
    Return_Low            = 0x0,
    Return_High           = 0x1,
    Return_Midband        = 0x2,
    Return_Glitch         = 0x3,
    Return_Undetermined   = 0x4,
    Return_Failed_Low     = 0x5,
    Return_Failed_High    = 0x6,
    Return_Failed_Midband = 0x7,
    Return_Failed_Glitch  = 0x8,
    Return_Ppen           = 0x9,
    Return_Short          = 0xA,
};

enum NormalModeStates : unsigned char
{
    Normal_Drive_Low        = 0, // 0
    Normal_Drive_High       = 1, // 1
    Normal_Expect_Low       = 2, // L
    Normal_Expect_High      = 3, // H
    Normal_Expect_Midband   = 4, // M
    Normal_Expect_Valid     = 5, // V
    Normal_Compare          = 6, // X
    Normal_Keep_Window_Open = 7, // W
};

enum DualDriveStates: unsigned char
{
    Dual_Low_D2_Low_D1    = 0, // 00
    Dual_Low_D2_High_D1   = 1, // 10
    Dual_High_D2_Low_D1   = 2, // 01
    Dual_High_D2_High_D1  = 3, // 11
    Dual_Compare_Low      = 4, // L
    Dual_Compare_High     = 5, // H
    Dual_Compare_Midband  = 6, // M
    Dual_Donnot_Compare   = 7, // X
};

enum SCIOModeStates : unsigned char
{
    SCIO_Drive_Low_Compare_Low      = 0, // 0L
    SCIO_Drive_Low_Compare_High     = 1, // 0H
    SCIO_Drive_Low_Compare_Midband  = 2, // 0M
    SCIO_Drive_Low_Donnot_Compare   = 3, // 0X
    SCIO_Drive_High_Compare_Low     = 4, // 1L
    SCIO_Drive_High_Compare_High    = 5, // 1H
    SCIO_Drive_High_Compare_Midband = 6, // 1M
    SCIO_Drive_High_Donnot_Compare  = 7, // 1X
};

class StdfFTR : public StdfRecord
{
public:
    StdfFTR();
    ~StdfFTR();

    unsigned int get_test_number() const;
    unsigned char get_head_number() const;
    unsigned char get_site_number() const;
	unsigned char get_test_flag() const;
	unsigned char get_optional_data_flag() const;
    unsigned int get_cycle_count() const;
    unsigned int get_relative_address() const;
    unsigned int get_repeat_count() const;
    unsigned int get_failpin_count() const;
    int get_xfail_address() const;
    int get_yfail_address() const;
    short get_vector_offset() const;
    unsigned short get_pin_count() const;
    unsigned short get_program_state_count() const;
    unsigned short get_pin_number(unsigned short index) const;
    unsigned char get_pin_state(unsigned short index) const;
    unsigned short get_program_index(unsigned short index) const;
    unsigned char get_program_state(unsigned short index) const;
	unsigned short get_failpin_data_count() const;
    unsigned char get_failpin_data(unsigned short index) const;
    const char* get_vector_pattern_name() const;
    const char* get_timeset_name() const;
    const char* get_vector_op_code() const;
    const char* get_test_text() const;
    const char* get_alarm_id() const;
    const char* get_result_text() const;
    unsigned char get_pattern_genertor_number() const;
	unsigned short get_bitmap_data_count() const;
    unsigned char get_bitmap_data(unsigned short index) const;

    void set_test_number(unsigned int number);
    void set_head_number(unsigned char number);
    void set_site_number(unsigned char number);
    void set_cycle_count(unsigned int count);
    void set_relative_address(unsigned int address);
    void set_repeat_count(unsigned int count);
    void set_failpin_count(unsigned int count);
    void set_xfail_address(int x);
    void set_yfail_address(int y);
    void set_vector_offset(short offset);
    void set_pin_count(unsigned short count);
    void set_program_state_count(unsigned short count);
    void set_pin_number(unsigned short index, unsigned short pin_number);
    void set_pin_state(unsigned short index, ReturnState nibble_state);
    void set_program_index(unsigned short index, unsigned short state_index);
    void set_program_state(unsigned short index, unsigned char nibble_state);
    void set_failpin_data(unsigned short index, unsigned char bit_data);
    void set_vector_pattern_name(const char* name);
    void set_timeset_name(const char* name);
    void set_vector_op_code(const char* op_code);
    void set_test_text(const char* text);
    void set_alarm_id(const char* id);
    void set_result_text(const char* text);
    void set_pattern_genertor_number(unsigned char number);
    void set_bitmap_data(unsigned short index, unsigned char bit_data);

    // TEST_FLG
    bool alarm_detected() const;
    bool result_invalid() const;
    bool result_unreliable() const;
    bool timeout_occured() const;
    bool test_unexecuted() const;
    bool test_aborted() const;
    bool test_pfflag_invalid() const;
    bool test_failed() const;
    // TEST_FLG
    void alarm_detected(bool flag);
    void result_invalid(bool flag);
    void result_unreliable(bool flag);
    void timeout_occured(bool flag);
    void test_unexecuted(bool flag);
    void test_aborted(bool flag);
    void test_pfflag_invalid(bool flag);
    void test_failed(bool flag);

    // OPT_FLG
    bool cycl_count_invalid() const;
    bool relative_address_invalid() const;
    bool repeat_count_invalid() const;
    bool failpin_count_invalid() const;
    bool xyfail_address_invalid() const;
    bool vector_offset_invalid() const;
    // OPT_FLG
    void cycl_count_invalid(bool flag);
    void relative_address_invalid(bool flag);
    void repeat_count_invalid(bool flag);
    void failpin_count_invalid(bool flag);
    void xyfail_address_invalid(bool flag);
    void vector_offset_invalid(bool flag);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class FunctionalTest Impl;
    Impl *impl;
};


class StdfBPS : public StdfRecord
{
public:
    StdfBPS();
    ~StdfBPS();

    const char* get_section_name() const;
    void set_section_name(const char* name);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class BeginProgramSection Impl;
    Impl *impl;
};


class StdfEPS : public StdfRecord
{
public:
    StdfEPS();
    ~StdfEPS();

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class EndProgramSection Impl;
    Impl *impl; 
};

enum VnType : unsigned char
{
    Vn_B0 = 0 , // B*0  Special pad field, of length 0 (See note below)
    Vn_U1 = 1 , // U*1  One byte unsigned integer
    Vn_U2 = 2 , // U*2  Two byte unsigned integer
    Vn_U4 = 3 , // U*4  Four byte unsigned integer
    Vn_I1 = 4 , // I*1  One byte signed integer
    Vn_I2 = 5 , // I*2  Two byte signed integer
    Vn_I4 = 6 , // I*4  Four byte signed integer
    Vn_R4 = 7 , // R*4  Four byte floating point number
    Vn_R8 = 8 , // R*8  Eight byte floating point number
    Vn_Cn = 10, // C*n  Variable length ASCII character string(first byte is string length in bytes)
    Vn_Bn = 11, // B*n  Variable length binary data string(first byte is string length in bytes)
    Vn_Dn = 12, // D*n  Bit encoded data(first two bytes of string are length in bits)
    Vn_N1 = 13, // N*1  Unsigned nibble
};

class StdfGDR : public StdfRecord
{
public:
    StdfGDR();
    ~StdfGDR();

    unsigned short get_data_count() const;
    void set_data_count(unsigned short count);

	unsigned char get_data_type(unsigned int index) const;
    const unsigned char* get_data_value(unsigned int index, int* byte_length, int* bit_count = nullptr) const;
    void set_data_type(VnType type, unsigned int index);
	bool set_data_value(const unsigned char* data, unsigned int index, int byte_length);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class GenericData Impl;
    Impl *impl;
	mutable unsigned char rawdata[255][256];//MAX V1 length is 255,MAX FLD_CNT is 255
};


class StdfDTR : public StdfRecord
{
public:
    StdfDTR();
    ~StdfDTR();
    const char* get_text_data() const;
    void set_text_data(const char*data);

    unsigned int parse(const StdfHeader& record);
    unsigned int unparse(StdfHeader& record);
	void print(std::ostream& os) const;

private:
    typedef class DatalogText Impl;
    Impl *impl;
};



#endif//_STDFV4_API_H_

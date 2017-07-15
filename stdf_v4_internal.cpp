#include "stdf_v4_internal.h"
#include <string>
#include <type_traits>
#include <ctime>
#include <iomanip>
#include <cstring>

// Note: call the template functions using explicit template arguments for error proof
//=======================For Number Convert====================
template <typename T>
union NumType
{
    T number;
    char bytes[sizeof(T)];
};

//=======================Read Number===========================
// for C1,U1,U2,U4,I1,I2,I4,R4,R8
// Tested
template <typename T>
unsigned int read_type(T& value, const char* rawdata, unsigned int& start_pos)
{
    unsigned int size = sizeof(T);
    NumType<T> temp;
    for(unsigned int i = 0; i < size; i++)
    {
        temp.bytes[i] = rawdata[start_pos+i];
    }
    start_pos += size;
    value = temp.number;
    return size;
}

//=====================Read Flag===============================
// for B1
// Tested
template <>
unsigned int read_type<B1>(B1& value, const char* rawdata, unsigned int& start_pos)
{
    C1 c = rawdata[start_pos];
    start_pos +=1;
    value = B1(c);
    return 1;
}

//=====================Read Flags Array========================
// for Bn
// Testing
template <>
unsigned int read_type<Bn>(Bn& value, const char* rawdata, unsigned int& start_pos)
{
    U1 n = 0;
    read_type<U1>(n, rawdata, start_pos);
    value.count = n;
    value.data.fill(B1(0));
    if(n == 0) return n;

    for(U1 i = 0; i < n; i++)
    {
        char c = rawdata[start_pos+i];
        value.data[i] = B1(c);
    }
    start_pos += n;
    return n;
}

//=====================Read String=============================
// for Cn
// Tested
template <>
unsigned int read_type<Cn>(Cn& value, const char* rawdata, unsigned int& start_pos)
{
    value.clear();
    U1 n;
    read_type<U1>(n, rawdata, start_pos);
    if(n == 0) return n;

    const char* str = &(rawdata[start_pos]);
    value.assign(str, n);
    start_pos += n;
    return n;
}

//=====================Read One Nibble=========================
// for N1
// Untest
template <>
unsigned int read_type<N1>(N1& value, const char* rawdata, unsigned int& start_pos)
{
    C1 c = rawdata[start_pos];
    start_pos +=1;
    value = N1(c & 0x0F);
    return 1;
}

//=====================Read Bits Map===========================
// for Dn
//Untest
template <>
unsigned int read_type<Dn>(Dn& value, const char* rawdata, unsigned int& start_pos)
{
    U2 bit_count = 0;
    read_type<U2>(bit_count, rawdata, start_pos);
#if _MSC_VER == 1600
    value.data.fill(C1(0));
#else
    value.data.reset();
#endif
    value.count = bit_count;
    if(bit_count == 0) return 0;

    unsigned int byte_count = (bit_count - 1)/ 8 + 1;

    char last_bit = 0x01;
    for(U2 i = 0; i < byte_count; i++)
    {
        char c = rawdata[start_pos+i];
        for(U1 n = 0; n < 8; n++)
        {
#if _MSC_VER == 1600
            value.data[i*8+n] = (c>>n) & last_bit;
#else
            value.data[i*8+n] = bool((c>>n) & last_bit);
#endif
        }
    }
    start_pos += byte_count;
    return byte_count;
}

//=====================Read kxType=============================
// for kxU1,kxU2,kxU4,kxI1,kxI2,kxI4,kxR4,kxR8,kxCn
// Testing
template <typename T>
unsigned int read_type(T& value, const char* rawdata, unsigned int& start_pos, const unsigned int count)
{
    typedef typename T::value_type Tn;
    unsigned int byte_count = 0;
    value.clear();
    if(count == 0) return 0;
    Tn temp;
    for(unsigned int i = 0; i < count; i++)
    {
        byte_count += read_type<Tn>(temp, rawdata, start_pos);
        value.push_back(temp);
    }
    return byte_count;
}

//=====================Read kxType=============================
// for kxN1
// Untested
template <>
unsigned int read_type<kxN1>(kxN1& value, const char* rawdata, unsigned int& start_pos,const unsigned int count)
{
    if(count == 0) return 0;
    unsigned int byte_count = (count - 1) / 2 + 1;
    for(unsigned int i = 0; i < byte_count; i++)
    {
        char c = rawdata[start_pos+i];
        N1 n1 = N1(c);
        value.push_back(n1);

        N1 n2 = N1(c>>NIBBLE_LENGTH);
        value.push_back(n2);
    }
    start_pos += byte_count;
    return byte_count;
}

//=====================Read Vn=================================
// for Vn
// Untested
// Note: this function use "new", need "delete"
template <>
unsigned int read_type<V1>(V1& value, const char* rawdata, unsigned int& start_pos)
{
    U1 type = 0;
    unsigned int byte_count = 0;
    byte_count += read_type<U1>(type, rawdata, start_pos);
    value.type = type;
    value.data = nullptr;
    if(type == 0) return byte_count;

    switch(type)
    {
        case 1 :{ value.data = new U1; byte_count += read_type<U1>(*((U1*)(value.data)), rawdata, start_pos); } break;
        case 2 :{ value.data = new U2; byte_count += read_type<U2>(*((U2*)(value.data)), rawdata, start_pos); } break;
        case 3 :{ value.data = new U4; byte_count += read_type<U4>(*((U4*)(value.data)), rawdata, start_pos); } break;
        case 4 :{ value.data = new I1; byte_count += read_type<I1>(*((I1*)(value.data)), rawdata, start_pos); } break;
        case 5 :{ value.data = new I2; byte_count += read_type<I2>(*((I2*)(value.data)), rawdata, start_pos); } break;
        case 6 :{ value.data = new I4; byte_count += read_type<I4>(*((I4*)(value.data)), rawdata, start_pos); } break;
        case 7 :{ value.data = new R4; byte_count += read_type<R4>(*((R4*)(value.data)), rawdata, start_pos); } break;
        case 8 :{ value.data = new R8; byte_count += read_type<R8>(*((R8*)(value.data)), rawdata, start_pos); } break;
        case 10:{ value.data = new Cn; byte_count += read_type<Cn>(*((Cn*)(value.data)), rawdata, start_pos); } break;
        case 11:{ value.data = new Bn; byte_count += read_type<Bn>(*((Bn*)(value.data)), rawdata, start_pos); } break;
        case 12:{ value.data = new Dn; byte_count += read_type<Dn>(*((Dn*)(value.data)), rawdata, start_pos); } break;
        case 13:{ value.data = new N1; byte_count += read_type<N1>(*((N1*)(value.data)), rawdata, start_pos); } break;
        default: break;
    }
    return byte_count;
}

template <>
unsigned int read_type<Vn>(Vn& value, const char* rawdata, unsigned int& start_pos, unsigned int count)
{
    if(count == 0) return 0;
    unsigned int byte_count = 0;
    for(unsigned int i = 0; i < count; i++)
    {
        V1 temp;
        byte_count += read_type<V1>(temp, rawdata, start_pos);
        value.push_back(temp);
    }
    return byte_count;
}
//==========================Write Number=======================
// for C1,U1,U2,U4,I1,I2,I4,R4,R8
// Tested
template <typename T>
U2 write_type(T& value, char *const rawdata, unsigned int& start_pos, bool *alignment = nullptr)
{
    unsigned int size = sizeof(T);
    NumType<T> temp;
    temp.number = value;
    for(unsigned int i = 0; i < size; i++)
    {
        rawdata[start_pos+i] = temp.bytes[i];
    }
    start_pos += size;
    if(alignment) *alignment = false;
    return U2(size);
}

//=====================Write String============================
// for Cn
// Tested
template <>
U2 write_type<Cn>(Cn& value, char *const rawdata, unsigned int& start_pos, bool *alignment )
{
    const char* temp = value.c_str();
    unsigned int value_length = value.length();
    rawdata[start_pos] = C1(value_length);
    start_pos += 1;
    for(unsigned int i = 0; i < value_length; i++)
    {
        rawdata[start_pos+i] = temp[i];
    }
    start_pos += value_length;
    if(alignment) *alignment = false;
    return U2(value_length+1);
}

//=====================Write Flag==============================
// for B1
// Tested
template <>
U2 write_type<B1>(B1& value, char *const rawdata, unsigned int& start_pos, bool *alignment )
{
    unsigned long temp = value.to_ulong();
    char b1 = char(temp);
    rawdata[start_pos] = b1;
    start_pos += 1;
    if(alignment) *alignment = false;
    return U2(1);
}

//=====================Write One Nibble========================
// for N1
// Untest
template <>
U2 write_type<N1>(N1& value, char *const rawdata, unsigned int& start_pos, bool *alignment )
{
    unsigned long temp = value.to_ulong();
    char n1 = char(temp);
    rawdata[start_pos] = n1;
    start_pos += 1;
    if(alignment) *alignment = true;
    return U2(1);
}

//=====================Write Flags Array=======================
// for Bn
// Tested
template <>
U2 write_type<Bn>(Bn& value, char *const rawdata, unsigned int& start_pos, bool *alignment)
{
    U2 write_length = 0;
    U1 data_count = value.count;
    rawdata[start_pos] = C1(data_count);
    start_pos ++;
    write_length ++;

    for(U1 i = 0; i < data_count; i++)
    {
        B1 temp = value.data[i];
        char b1 = char(temp.to_ulong());
        rawdata[start_pos+i] = b1;
    }
    start_pos += data_count;
    write_length += data_count;
    if(alignment) *alignment = false;
    return write_length;
}

//=====================Write Bits Map=======================
// for Dn
// Tested
template <>
U2 write_type<Dn>(Dn& value, char *const rawdata, unsigned int& start_pos, bool *alignment)
{
    U2 write_length = 0;
    U2 bits_count = value.count;
    write_length += write_type<U2>(bits_count, rawdata, start_pos);
    if(bits_count == 0) return write_length;

    unsigned int bytes_count = (bits_count - 1) / 8 + 1;
    if(alignment)
    {
        unsigned int check_bytes = bits_count / 8;
        if(bytes_count != check_bytes) *alignment = true;
        else *alignment = false;
    }

    char last_bit = 0x01;
    for(unsigned int i = 0; i < bytes_count; i++)
    {
        char temp = 0x0;
        for(U1 n = 0; n < 8; n++)
        {
            if(value.data[i*8+n])
            {
                temp |= (last_bit<<n);
            }
        }
        rawdata[start_pos+i] = temp;
    }
    start_pos += bytes_count;
    write_length += bytes_count;
    return write_length;
}


//=====================Write kxType============================
// for kxU1,kxU2,kxU4,kxI1,kxI2,kxI4,kxR4,kxR8,kxCn
// Testing
// Note:need check the size of vector and the input count value
template <typename T>
U2 write_type(T& value, char *const rawdata, unsigned int& start_pos, const unsigned int count, unsigned int *alignment = nullptr)
{
    U2 write_length = 0;
    U1 data_count = U1(count);

    typedef typename T::value_type Type ;
    for(U1 i = 0; i < data_count; i++)
    {
        Type temp = value.at(i);
        write_length += write_type<Type>(temp, rawdata, start_pos);
    }
    if(alignment) *alignment = 0;
    return write_length;
}

//=====================Write kxType============================
// for kxN1
// Untest
// Note:need check the size of vector and the input count value
template <>
U2 write_type<kxN1>(kxN1& value, char *const rawdata, unsigned int& start_pos, const unsigned int count, unsigned int *alignment)
{
    U2 write_length = 0;
    if(count == 0) return write_length;

    unsigned int byte_count = (count - 1) / 2 + 1;
    unsigned int byte_check = count / 2;
    if(byte_count != byte_check)
    {
        N1 attach("0000");
        value.push_back(attach);
        if(alignment) *alignment = 1;
    }

    for(unsigned int i = 0; i < byte_count; i++)
    {
        N1 n1 = value[2*i];
        N1 n2 = value[2*i + 1];
        U1 low4bits = U1(n1.to_ulong());
        U1 high4bits = U1(n2.to_ulong());
        low4bits = low4bits & 0x0F;
        high4bits = high4bits & 0x0F;
        U1 temp = ((high4bits<<4) | low4bits);
        rawdata[start_pos+i] = temp;
    }
    start_pos += byte_count;
    write_length += byte_count;

    return write_length;
}

//=====================Write Vn=======================
// for Vn
// Untest
// Note:need check the size of vector and the input count value
template <>
U2 write_type<V1>(V1& value, char *const rawdata, unsigned int& start_pos, bool *alignment)
{
    U1 type = value.type;
    U2 write_length = 0;
    if(alignment) *alignment = false;
    switch(type)
    {
        case 1 :
        {
            write_length += write_type<U1>(value.type, rawdata, start_pos);
            write_length += write_type<U1>(*((U1*)(value.data)), rawdata, start_pos);
        }
        break;
        case 4 :
        {
            write_length += write_type<U1>(value.type, rawdata, start_pos);
            write_length += write_type<I1>(*((I1*)(value.data)), rawdata, start_pos);
        }
        break;
        case 10:
        {
            write_length += write_type<U1>(value.type, rawdata, start_pos);
            write_length += write_type<Cn>(*((Cn*)(value.data)), rawdata, start_pos);
        }
        break;
        case 11:
        {
            write_length += write_type<U1>(value.type, rawdata, start_pos);
            write_length += write_type<Bn>(*((Bn*)(value.data)), rawdata, start_pos);
        }
        break;
        case 12:
        {
            write_length += write_type<U1>(value.type, rawdata, start_pos);
            write_length += write_type<Dn>(*((Dn*)(value.data)), rawdata, start_pos);
        }
        break;
        case 13:
        {
            write_length += write_type<U1>(value.type, rawdata, start_pos);
            write_length += write_type<N1>(*((N1*)(value.data)), rawdata, start_pos);
        }
        break;

        case 2 :
        {
            if(start_pos % 2 == 0)
            {
                U1 pad = 0;
                if(alignment) *alignment = true;
                write_length += write_type<U1>(pad, rawdata, start_pos);
            }
            write_length += write_type<U1>(value.type, rawdata, start_pos);
            write_length += write_type<U2>(*((U2*)(value.data)), rawdata, start_pos);
        }
        break;
        case 3 :
        {
            if(start_pos % 2 == 0)
            {
                U1 pad = 0;
                if(alignment) *alignment = true;
                write_length += write_type<U1>(pad, rawdata, start_pos);
            }
            write_length += write_type<U1>(value.type, rawdata, start_pos);
            write_length += write_type<U4>(*((U4*)(value.data)), rawdata, start_pos);
        }
        break;
        case 5 :
        {
            if(start_pos % 2 == 0)
            {
                U1 pad = 0;
                if(alignment) *alignment = true;
                write_length += write_type<U1>(pad, rawdata, start_pos);
            }
            write_length += write_type<U1>(value.type, rawdata, start_pos);
            write_length += write_type<I2>(*((I2*)(value.data)), rawdata, start_pos);
        }
        break;
        case 6 :
        {
            if(start_pos % 2 == 0)
            {
                U1 pad = 0;
                if(alignment) *alignment = true;
                write_length += write_type<U1>(pad, rawdata, start_pos);
            }
            write_length += write_type<U1>(value.type, rawdata, start_pos);
            write_length += write_type<I4>(*((I4*)(value.data)), rawdata, start_pos);
        }
        break;
        case 7 :
        {
            if(start_pos % 2 == 0)
            {
                U1 pad = 0;
                if(alignment) *alignment = true;
                write_length += write_type<U1>(pad, rawdata, start_pos);
            }
            write_length += write_type<U1>(value.type, rawdata, start_pos);
            write_length += write_type<R4>(*((R4*)(value.data)), rawdata, start_pos);
        }
        break;
        case 8 :
        {
            if(start_pos % 2 == 0)
            {
                U1 pad = 0;
                if(alignment) *alignment = true;
                write_length += write_type<U1>(pad, rawdata, start_pos);
            }
            write_length += write_type<U1>(value.type, rawdata, start_pos);
            write_length += write_type<R8>(*((R8*)(value.data)), rawdata, start_pos);
        }
        break;
        default: break;
    }
    return write_length;
}
template <>
U2 write_type<Vn>(Vn& value, char *const rawdata, unsigned int& start_pos, const unsigned int count, unsigned int *alignment)
{
    U2 write_length = 0;
    unsigned int pad_count = 0;
    bool pad = false;
    unsigned int vector_size = value.size();
    for(unsigned int i = 0; i < vector_size; i++)
    {
        V1 temp;
        temp.type = value[i].type;
        temp.data = value[i].data;
        if(temp.type == 0) continue;
        write_length += write_type<V1>(temp, rawdata, start_pos, &pad);
        if(pad) pad_count++;
    }
    write_length += count;
    if(alignment) *alignment = pad_count;
    return write_length;
}

//======================RecordHeader===========================
RecordHeader::RecordHeader()
{
    REC_LEN = 0;
    REC_TYP = 0;
    REC_SUB = 0;
    std::memset(rawdata,0, MAX_REC_LENGTH);
}

int RecordHeader::GetRecordType() const
{
    return ((int)REC_TYP)<<8 | REC_SUB;
}

const char* RecordHeader::GetReadOnlyData() const
{
    return rawdata;
}

char * RecordHeader::GetWriteOnlyData()
{
    std::memset(rawdata,0, MAX_REC_LENGTH);
    return rawdata;
}

int RecordHeader::WriteRecord(std::ofstream& out)
{
    out.write((char*)&REC_LEN, 2);
    out.write((char*)&REC_TYP, 1);
    out.write((char*)&REC_SUB, 1);
    if(REC_LEN > 0)
    {
        out.write(rawdata, REC_LEN);
    }
    return 0;
}

int RecordHeader::ReadRecord(std::ifstream& in)
{
    std::memset(rawdata,0, MAX_REC_LENGTH);
    in.read((char*)&REC_LEN, 2);
    in.read((char*)&REC_TYP, 1);
    in.read((char*)&REC_SUB, 1);
    in.read(rawdata, REC_LEN);

    int type = ((int)REC_TYP)<<8 | REC_SUB;
    return type;
}
//=============================================================

//========================FAR==================================
FileAttributes::FileAttributes()
{
    REC_LEN  = U2(0);
    REC_TYP  = U1(0);
    REC_SUB  = U1(10);
    CPU_TYPE = U1(2);
    STDF_VER = U1(4);
}

unsigned int FileAttributes::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<U1>(CPU_TYPE, rawdata, pos);
    read_type<U1>(STDF_VER, rawdata, pos);
    return pos;
}

unsigned int FileAttributes::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<U1>(CPU_TYPE, rawdata, pos);
    length += write_type<U1>(STDF_VER, rawdata, pos);
    header.REC_LEN = length;
    return pos;
}

void FileAttributes::Print(std::ostream& os)
{
    os<<"CPU_TYPE : "<<(unsigned int)CPU_TYPE<<"\n";
    os<<"STDF_VER : "<<(unsigned int)STDF_VER<<"\n";
}

//=============================================================
//========================ATR==================================
AuditTrail::AuditTrail()
{
     REC_LEN = U2(0);
     REC_TYP = U1(0);
     REC_SUB = U1(20);
     MOD_TIM = U4(0);
}

unsigned int AuditTrail::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<U4>(MOD_TIM , rawdata, pos);
    read_type<Cn>(CMD_LINE, rawdata, pos);
    return pos;
}

unsigned int AuditTrail::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<U4>(MOD_TIM , rawdata, pos);
    length += write_type<Cn>(CMD_LINE, rawdata, pos);
    header.REC_LEN = length;
    return pos;
}

void AuditTrail::Print(std::ostream& os)
{
    time_t mod_tim = (time_t)MOD_TIM;
    os<<"MOD_TIM  : "<<std::string(ctime(&mod_tim));
    os<<"CMD_LINE : "<<CMD_LINE <<"\n";
}
//=============================================================

//========================MIR==================================
MasterInformation::MasterInformation()
{
     REC_LEN  = U2(0);
     REC_TYP  = U1(1);
     REC_SUB  = U1(10);
     SETUP_T  = U4(0);
     START_T  = U4(0);
     STAT_NUM = U1(1);
     MODE_COD = C1(' ');
     RTST_COD = C1(' ');
     PROT_COD = C1(' ');
     BURN_TIM = U2(-1);
     CMOD_COD = C1(' ');
};

unsigned int MasterInformation::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<U4>(SETUP_T , rawdata, pos);
    read_type<U4>(START_T , rawdata, pos);
    read_type<U1>(STAT_NUM, rawdata, pos);
    read_type<C1>(MODE_COD, rawdata, pos);
    read_type<C1>(RTST_COD, rawdata, pos);
    read_type<C1>(PROT_COD, rawdata, pos);
    read_type<U2>(BURN_TIM, rawdata, pos);
    read_type<C1>(CMOD_COD, rawdata, pos);
    read_type<Cn>(LOT_ID  , rawdata, pos);
    read_type<Cn>(PART_TYP, rawdata, pos);
    read_type<Cn>(NODE_NAM, rawdata, pos);
    read_type<Cn>(TSTR_TYP, rawdata, pos);
    read_type<Cn>(JOB_NAM , rawdata, pos);
    read_type<Cn>(JOB_REV , rawdata, pos);
    read_type<Cn>(SBLOT_ID, rawdata, pos);
    read_type<Cn>(OPER_NAM, rawdata, pos);
    read_type<Cn>(EXEC_TYP, rawdata, pos);
    read_type<Cn>(EXEC_VER, rawdata, pos);
    read_type<Cn>(TEST_COD, rawdata, pos);
    read_type<Cn>(TST_TEMP, rawdata, pos);
    read_type<Cn>(USER_TXT, rawdata, pos);
    read_type<Cn>(AUX_FILE, rawdata, pos);
    read_type<Cn>(PKG_TYP , rawdata, pos);
    read_type<Cn>(FAMLY_ID, rawdata, pos);
    read_type<Cn>(DATE_COD, rawdata, pos);
    read_type<Cn>(FACIL_ID, rawdata, pos);
    read_type<Cn>(FLOOR_ID, rawdata, pos);
    read_type<Cn>(PROC_ID , rawdata, pos);
    read_type<Cn>(OPER_FRQ, rawdata, pos);
    read_type<Cn>(SPEC_NAM, rawdata, pos);
    read_type<Cn>(SPEC_VER, rawdata, pos);
    read_type<Cn>(FLOW_ID , rawdata, pos);
    read_type<Cn>(SETUP_ID, rawdata, pos);
    read_type<Cn>(DSGN_REV, rawdata, pos);
    read_type<Cn>(ENG_ID  , rawdata, pos);
    read_type<Cn>(ROM_COD , rawdata, pos);
    read_type<Cn>(SERL_NUM, rawdata, pos);
    read_type<Cn>(SUPR_NAM, rawdata, pos);
    return pos;
}

unsigned int MasterInformation::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<U4>(SETUP_T  , rawdata, pos) ;
    length += write_type<U4>(START_T  , rawdata, pos) ;
    length += write_type<U1>(STAT_NUM , rawdata, pos) ;
    length += write_type<C1>(MODE_COD , rawdata, pos) ;
    length += write_type<C1>(RTST_COD , rawdata, pos) ;
    length += write_type<C1>(PROT_COD , rawdata, pos) ;
    length += write_type<U2>(BURN_TIM , rawdata, pos) ;
    length += write_type<C1>(CMOD_COD , rawdata, pos) ;
    length += write_type<Cn>(LOT_ID   , rawdata, pos) ;
    length += write_type<Cn>(PART_TYP , rawdata, pos) ;
    length += write_type<Cn>(NODE_NAM , rawdata, pos) ;
    length += write_type<Cn>(TSTR_TYP , rawdata, pos) ;
    length += write_type<Cn>(JOB_NAM  , rawdata, pos) ;
    length += write_type<Cn>(JOB_REV  , rawdata, pos) ;
    length += write_type<Cn>(SBLOT_ID , rawdata, pos) ;
    length += write_type<Cn>(OPER_NAM , rawdata, pos) ;
    length += write_type<Cn>(EXEC_TYP , rawdata, pos) ;
    length += write_type<Cn>(EXEC_VER , rawdata, pos) ;
    length += write_type<Cn>(TEST_COD , rawdata, pos) ;
    length += write_type<Cn>(TST_TEMP , rawdata, pos) ;
    length += write_type<Cn>(USER_TXT , rawdata, pos) ;
    length += write_type<Cn>(AUX_FILE , rawdata, pos) ;
    length += write_type<Cn>(PKG_TYP  , rawdata, pos) ;
    length += write_type<Cn>(FAMLY_ID , rawdata, pos) ;
    length += write_type<Cn>(DATE_COD , rawdata, pos) ;
    length += write_type<Cn>(FACIL_ID , rawdata, pos) ;
    length += write_type<Cn>(FLOOR_ID , rawdata, pos) ;
    length += write_type<Cn>(PROC_ID  , rawdata, pos) ;
    length += write_type<Cn>(OPER_FRQ , rawdata, pos) ;
    length += write_type<Cn>(SPEC_NAM , rawdata, pos) ;
    length += write_type<Cn>(SPEC_VER , rawdata, pos) ;
    length += write_type<Cn>(FLOW_ID  , rawdata, pos) ;
    length += write_type<Cn>(SETUP_ID , rawdata, pos) ;
    length += write_type<Cn>(DSGN_REV , rawdata, pos) ;
    length += write_type<Cn>(ENG_ID   , rawdata, pos) ;
    length += write_type<Cn>(ROM_COD  , rawdata, pos) ;
    length += write_type<Cn>(SERL_NUM , rawdata, pos) ;
    length += write_type<Cn>(SUPR_NAM , rawdata, pos) ;
    header.REC_LEN = length;
    return pos;
}

void MasterInformation::Print(std::ostream& os)
{
    time_t setup_tim = (time_t)SETUP_T;
    time_t start_tim = (time_t)START_T;
    os<<"SETUP_T  : "<<std::string(ctime(&setup_tim));
    os<<"START_T  : "<<std::string(ctime(&start_tim));
    os<<"STAT_NUM : "<<(unsigned int)STAT_NUM <<"\n";
    os<<"MODE_COD : "<<MODE_COD <<"\n";
    os<<"RTST_COD : "<<RTST_COD <<"\n";
    os<<"PROT_COD : "<<PROT_COD <<"\n";
    os<<"BURN_TIM : "<<(unsigned int)BURN_TIM <<"\n";
    os<<"CMOD_COD : "<<CMOD_COD <<"\n";
    os<<"LOT_ID   : "<<LOT_ID   <<"\n";
    os<<"PART_TYP : "<<PART_TYP <<"\n";
    os<<"NODE_NAM : "<<NODE_NAM <<"\n";
    os<<"TSTR_TYP : "<<TSTR_TYP <<"\n";
    os<<"JOB_NAM  : "<<JOB_NAM  <<"\n";
    os<<"JOB_REV  : "<<JOB_REV  <<"\n";
    os<<"SBLOT_ID : "<<SBLOT_ID <<"\n";
    os<<"OPER_NAM : "<<OPER_NAM <<"\n";
    os<<"EXEC_TYP : "<<EXEC_TYP <<"\n";
    os<<"EXEC_VER : "<<EXEC_VER <<"\n";
    os<<"TEST_COD : "<<TEST_COD <<"\n";
    os<<"TST_TEMP : "<<TST_TEMP <<"\n";
    os<<"USER_TXT : "<<USER_TXT <<"\n";
    os<<"AUX_FILE : "<<AUX_FILE <<"\n";
    os<<"PKG_TYP  : "<<PKG_TYP  <<"\n";
    os<<"FAMLY_ID : "<<FAMLY_ID <<"\n";
    os<<"DATE_COD : "<<DATE_COD <<"\n";
    os<<"FACIL_ID : "<<FACIL_ID <<"\n";
    os<<"FLOOR_ID : "<<FLOOR_ID <<"\n";
    os<<"PROC_ID  : "<<PROC_ID  <<"\n";
    os<<"OPER_FRQ : "<<OPER_FRQ <<"\n";
    os<<"SPEC_NAM : "<<SPEC_NAM <<"\n";
    os<<"SPEC_VER : "<<SPEC_VER <<"\n";
    os<<"FLOW_ID  : "<<FLOW_ID  <<"\n";
    os<<"SETUP_ID : "<<SETUP_ID <<"\n";
    os<<"DSGN_REV : "<<DSGN_REV <<"\n";
    os<<"ENG_ID   : "<<ENG_ID   <<"\n";
    os<<"ROM_COD  : "<<ROM_COD  <<"\n";
    os<<"SERL_NUM : "<<SERL_NUM <<"\n";
    os<<"SUPR_NAM : "<<SUPR_NAM <<"\n";
}
//=============================================================


//========================MRR==================================
MasterResults::MasterResults()
{
    REC_LEN  = U2(0);
    REC_TYP  = U1(1);
    REC_SUB  = U1(20);
    FINISH_T = U4(0);
    DISP_COD = C1(' ');
}

unsigned int MasterResults::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<U4>(FINISH_T, rawdata, pos);
    read_type<C1>(DISP_COD, rawdata, pos);
    read_type<Cn>(USR_DESC, rawdata, pos);
    read_type<Cn>(EXC_DESC, rawdata, pos);
    return pos;
}

unsigned int MasterResults::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<U4>(FINISH_T, rawdata, pos);
    length += write_type<C1>(DISP_COD, rawdata, pos);
    length += write_type<Cn>(USR_DESC, rawdata, pos);
    length += write_type<Cn>(EXC_DESC, rawdata, pos);
    header.REC_LEN = length;
    return pos;
}

void MasterResults::Print(std::ostream& os)
{
    time_t finish_t = (time_t)FINISH_T;
    os<<"FINISH_T : "<<std::string(ctime(&finish_t));
    os<<"DISP_COD : "<<DISP_COD <<"\n";
    os<<"USR_DESC : "<<USR_DESC <<"\n";
    os<<"EXC_DESC : "<<EXC_DESC <<"\n";
}

//=============================================================
//========================PCR==================================
PartCount::PartCount()
{
     REC_LEN  = U2(0) ;
     REC_TYP  = U1(1) ;
     REC_SUB  = U1(30) ;
     HEAD_NUM = U1(1) ;
     SITE_NUM = U1(1) ;
     PART_CNT = U4(0) ;
     RTST_CNT = U4(-1) ;
     ABRT_CNT = U4(-1) ;
     GOOD_CNT = U4(-1) ;
     FUNC_CNT = U4(-1) ;
}

unsigned int PartCount::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<U1>(HEAD_NUM, rawdata, pos) ;
    read_type<U1>(SITE_NUM, rawdata, pos) ;
    read_type<U4>(PART_CNT, rawdata, pos) ;
    read_type<U4>(RTST_CNT, rawdata, pos) ;
    read_type<U4>(ABRT_CNT, rawdata, pos) ;
    read_type<U4>(GOOD_CNT, rawdata, pos) ;
    read_type<U4>(FUNC_CNT, rawdata, pos) ;
    return pos;
}

unsigned int PartCount::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<U1>(HEAD_NUM, rawdata, pos);
    length += write_type<U1>(SITE_NUM, rawdata, pos);
    length += write_type<U4>(PART_CNT, rawdata, pos);
    length += write_type<U4>(RTST_CNT, rawdata, pos);
    length += write_type<U4>(ABRT_CNT, rawdata, pos);
    length += write_type<U4>(GOOD_CNT, rawdata, pos);
    length += write_type<U4>(FUNC_CNT, rawdata, pos);
    header.REC_LEN = length;
    return pos;
}

void PartCount::Print(std::ostream& os)
{
    os<<"HEAD_NUM : "<<(unsigned int)HEAD_NUM<<"\n";
    os<<"SITE_NUM : "<<(unsigned int)SITE_NUM<<"\n";
    os<<"PART_CNT : "<<PART_CNT<<"\n";
    os<<"RTST_CNT : "<<RTST_CNT<<"\n";
    os<<"ABRT_CNT : "<<ABRT_CNT<<"\n";
    os<<"GOOD_CNT : "<<GOOD_CNT<<"\n";
    os<<"FUNC_CNT : "<<FUNC_CNT<<"\n";
}
//=============================================================

//========================HBR==================================
HardwareBin::HardwareBin()
{
     REC_LEN  = U2(0);
     REC_TYP  = U1(1);
     REC_SUB  = U1(40);
     HEAD_NUM = U1(1);
     SITE_NUM = U1(1);
     HBIN_NUM = U2(0);
     HBIN_CNT = U4(0);
     HBIN_PF  = C1(' ');
}

unsigned int HardwareBin::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<U1>(HEAD_NUM, rawdata, pos);
    read_type<U1>(SITE_NUM, rawdata, pos);
    read_type<U2>(HBIN_NUM, rawdata, pos);
    read_type<U4>(HBIN_CNT, rawdata, pos);
    read_type<C1>(HBIN_PF , rawdata, pos);
    read_type<Cn>(HBIN_NAM, rawdata, pos);
    return pos;
}

unsigned int HardwareBin::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<U1>(HEAD_NUM, rawdata, pos);
    length += write_type<U1>(SITE_NUM, rawdata, pos);
    length += write_type<U2>(HBIN_NUM, rawdata, pos);
    length += write_type<U4>(HBIN_CNT, rawdata, pos);
    length += write_type<C1>(HBIN_PF , rawdata, pos);
    length += write_type<Cn>(HBIN_NAM, rawdata, pos);
    header.REC_LEN = length;
    return pos;
}

void HardwareBin::Print(std::ostream& os)
{
    os<<"HEAD_NUM : "<<(unsigned int)HEAD_NUM <<"\n";
    os<<"SITE_NUM : "<<(unsigned int)SITE_NUM <<"\n";
    os<<"HBIN_NUM : "<<(unsigned int)HBIN_NUM <<"\n";
    os<<"HBIN_CNT : "<<HBIN_CNT <<"\n";
    os<<"HBIN_PF  : "<<HBIN_PF  <<"\n";
    os<<"HBIN_NAM : "<<HBIN_NAM <<"\n";
}
//=============================================================

//========================SBR==================================
SoftwareBin::SoftwareBin()
{
    REC_LEN  = U2(0);
    REC_TYP  = U1(1);
    REC_SUB  = U1(40);
    HEAD_NUM = U1(1);
    SITE_NUM = U1(1);
    SBIN_NUM = U2(0);
    SBIN_CNT = U4(0);
    SBIN_PF  = C1(' ');
}

unsigned int SoftwareBin::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<U1>(HEAD_NUM, rawdata, pos);
    read_type<U1>(SITE_NUM, rawdata, pos);
    read_type<U2>(SBIN_NUM, rawdata, pos);
    read_type<U4>(SBIN_CNT, rawdata, pos);
    read_type<C1>(SBIN_PF , rawdata, pos);
    read_type<Cn>(SBIN_NAM, rawdata, pos);
    return pos;
}

unsigned int SoftwareBin::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<U1>(HEAD_NUM, rawdata, pos);
    length += write_type<U1>(SITE_NUM, rawdata, pos);
    length += write_type<U2>(SBIN_NUM, rawdata, pos);
    length += write_type<U4>(SBIN_CNT, rawdata, pos);
    length += write_type<C1>(SBIN_PF , rawdata, pos);
    length += write_type<Cn>(SBIN_NAM, rawdata, pos);
    header.REC_LEN = length;
    return pos;
}

void SoftwareBin::Print(std::ostream& os)
{
    os<<"HEAD_NUM : "<<(unsigned int)HEAD_NUM <<"\n";
    os<<"SITE_NUM : "<<(unsigned int)SITE_NUM <<"\n";
    os<<"SBIN_NUM : "<<(unsigned int)SBIN_NUM <<"\n";
    os<<"SBIN_CNT : "<<SBIN_CNT <<"\n";
    os<<"SBIN_PF  : "<<SBIN_PF  <<"\n";
    os<<"SBIN_NAM : "<<SBIN_NAM <<"\n";
}
//=============================================================

//========================PMR==================================
PinMap::PinMap()
{
     REC_LEN  = U2(0) ;
     REC_TYP  = U1(1) ;
     REC_SUB  = U1(60) ;
     PMR_INDX = U2(0) ;
     CHAN_TYP = U2(0) ;
     HEAD_NUM = U1(1) ;
     SITE_NUM = U1(1) ;
}

unsigned int PinMap::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<U2>(PMR_INDX, rawdata, pos) ;
    read_type<U2>(CHAN_TYP, rawdata, pos) ;
    read_type<Cn>(CHAN_NAM, rawdata, pos) ;
    read_type<Cn>(PHY_NAM , rawdata, pos) ;
    read_type<Cn>(LOG_NAM , rawdata, pos) ;
    read_type<U1>(HEAD_NUM, rawdata, pos) ;
    read_type<U1>(SITE_NUM, rawdata, pos) ;
    return pos;
}

unsigned int PinMap::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<U2>(PMR_INDX , rawdata, pos);
    length += write_type<U2>(CHAN_TYP , rawdata, pos);
    length += write_type<Cn>(CHAN_NAM , rawdata, pos);
    length += write_type<Cn>(PHY_NAM  , rawdata, pos);
    length += write_type<Cn>(LOG_NAM  , rawdata, pos);
    length += write_type<U1>(HEAD_NUM , rawdata, pos);
    length += write_type<U1>(SITE_NUM , rawdata, pos);
    header.REC_LEN = length;
    return pos;
}

void PinMap::Print(std::ostream& os)
{
    os<<"PMR_INDX : "<<(unsigned int)PMR_INDX<<"\n" ;
    os<<"CHAN_TYP : "<<(unsigned int)CHAN_TYP<<"\n" ;
    os<<"CHAN_NAM : "<<CHAN_NAM<<"\n" ;
    os<<"PHY_NAM  : "<<PHY_NAM <<"\n" ;
    os<<"LOG_NAM  : "<<LOG_NAM <<"\n" ;
    os<<"HEAD_NUM : "<<(unsigned int)HEAD_NUM<<"\n" ;
    os<<"SITE_NUM : "<<(unsigned int)SITE_NUM<<"\n" ;
}
//=============================================================

//========================PGR==================================
PinGroup::PinGroup()
{
   REC_LEN  = U2(0) ;
   REC_TYP  = U1(1) ;
   REC_SUB  = U1(62);
   GRP_INDX = U2(0) ;
   INDX_CNT = U2(0) ;
}

unsigned int PinGroup::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<U2>(GRP_INDX, rawdata, pos) ;
    read_type<Cn>(GRP_NAM , rawdata, pos) ;
    read_type<U2>(INDX_CNT, rawdata, pos) ;
    read_type<kxU2>(PMR_INDX, rawdata, pos, INDX_CNT) ;
    return pos;
}

unsigned int PinGroup::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<U2>(GRP_INDX, rawdata, pos) ;
    length += write_type<Cn>(GRP_NAM , rawdata, pos) ;
    length += write_type<U2>(INDX_CNT, rawdata, pos) ;
    length += write_type<kxU2>(PMR_INDX, rawdata, pos, INDX_CNT) ;
    header.REC_LEN = length;
    return pos;
}

void PinGroup::Print(std::ostream& os)
{
    os<<"GRP_INDX : "<<(unsigned int)GRP_INDX<<"\n";
    os<<"GRP_NAM  : "<<GRP_NAM <<"\n";
    os<<"INDX_CNT : "<<(unsigned int)INDX_CNT<<"\n";
    os<<"PMR_INDX : ";
    for(U2 i = 0; i < INDX_CNT; i++)
    {
        os<<(unsigned int)PMR_INDX[i]<<" ";
    }
    os<<"\n";
}
//=============================================================

//========================PLR==================================
PinList::PinList()
{
    REC_LEN = U2(0)  ;
    REC_TYP = U1(1)  ;
    REC_SUB = U1(63) ;
    GRP_CNT = U2(0)  ;
}

unsigned int PinList::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<U2>( GRP_CNT , rawdata, pos) ;
    read_type<kxU2>( GRP_INDX, rawdata, pos, GRP_CNT) ;
    read_type<kxU2>( GRP_MODE, rawdata, pos, GRP_CNT) ;
    read_type<kxU1>( GRP_RADX, rawdata, pos, GRP_CNT) ;
    read_type<kxCn>( PGM_CHAR, rawdata, pos, GRP_CNT) ;
    read_type<kxCn>( RTN_CHAR, rawdata, pos, GRP_CNT) ;
    read_type<kxCn>( PGM_CHAL, rawdata, pos, GRP_CNT) ;
    read_type<kxCn>( RTN_CHAL, rawdata, pos, GRP_CNT) ;
    return pos;
}

unsigned int PinList::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<U2>(GRP_CNT , rawdata, pos) ;
    length += write_type<kxU2>(GRP_INDX, rawdata, pos, GRP_CNT) ;
    length += write_type<kxU2>(GRP_MODE, rawdata, pos, GRP_CNT) ;
    length += write_type<kxU1>(GRP_RADX, rawdata, pos, GRP_CNT) ;
    length += write_type<kxCn>(PGM_CHAR, rawdata, pos, GRP_CNT) ;
    length += write_type<kxCn>(RTN_CHAR, rawdata, pos, GRP_CNT) ;
    length += write_type<kxCn>(PGM_CHAL, rawdata, pos, GRP_CNT) ;
    length += write_type<kxCn>(RTN_CHAL, rawdata, pos, GRP_CNT) ;
    header.REC_LEN = length;
    return pos;
}

void PinList::Print(std::ostream& os)
{
    os<<"GRP_CNT  : "<<(unsigned int)GRP_CNT <<"\n";
    os<<"GRP_INDX|GRP_MODE|GRP_RADX|PGM_CHAR|RTN_CHAR|PGM_CHAL|RTN_CHAL\n";
    for(U2 i = 0; i < GRP_CNT; i++)
    {
        std::ios::fmtflags old_flags= os.flags();
        os<<std::setw(8)<<(unsigned int)GRP_INDX[i]<<"|";
        os<<std::setw(8)<<(unsigned int)GRP_MODE[i]<<"|";
        os<<std::setw(8)<<(unsigned int)GRP_RADX[i]<<"|";
        os<<std::setw(8)<<PGM_CHAR[i]<<"|";
        os<<std::setw(8)<<PGM_CHAR[i]<<"|";
        os<<std::setw(8)<<PGM_CHAR[i]<<"|";
        os<<std::setw(8)<<PGM_CHAR[i]<<"\n";
        os.flags(old_flags);
    }
    os<<"\n";
}
//=============================================================

//========================SDR==================================
RetestData::RetestData()
{
    REC_LEN  = U2(0) ;
    REC_TYP  = U1(1) ;
    REC_SUB  = U1(70) ;
    NUM_BINS = U2(0) ;
}

unsigned int RetestData::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<U2>( NUM_BINS, rawdata, pos) ;
    read_type<kxU2>( RTST_BIN, rawdata, pos, NUM_BINS) ;
    return pos;
}

unsigned int RetestData::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<U2>(NUM_BINS, rawdata, pos);
    length += write_type<kxU2>(RTST_BIN, rawdata, pos, NUM_BINS);
    header.REC_LEN = length;
    return pos;
}

void RetestData::Print(std::ostream& os)
{
    os<<"NUM_BINS : "<<(unsigned int)NUM_BINS<<"\n";
    os<<"RTST_BIN : ";
    for(U2 i = 0; i < NUM_BINS; i++)
    {
        os<<(unsigned int)RTST_BIN[i]<<" ";
    }
    os<<"\n";
}
//=============================================================

//========================SDR==================================
SiteDescription::SiteDescription()
{
    REC_LEN  = U2(0);
    REC_TYP  = U1(1);
    REC_SUB  = U1(80);
    HEAD_NUM = U1(1);
    SITE_GRP = U1(0);
    SITE_CNT = U1(0);
}

unsigned int SiteDescription::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<U1>(HEAD_NUM, rawdata, pos);
    read_type<U1>(SITE_GRP, rawdata, pos);
    read_type<U1>(SITE_CNT, rawdata, pos);
    read_type<kxU1>(SITE_NUM, rawdata, pos, SITE_CNT);
    read_type<Cn>(HAND_TYP, rawdata, pos);
    read_type<Cn>(HAND_ID , rawdata, pos);
    read_type<Cn>(CARD_TYP, rawdata, pos);
    read_type<Cn>(CARD_ID , rawdata, pos);
    read_type<Cn>(LOAD_TYP, rawdata, pos);
    read_type<Cn>(LOAD_ID , rawdata, pos);
    read_type<Cn>(DIB_TYP , rawdata, pos);
    read_type<Cn>(DIB_ID  , rawdata, pos);
    read_type<Cn>(CABL_TYP, rawdata, pos);
    read_type<Cn>(CABL_ID , rawdata, pos);
    read_type<Cn>(CONT_TYP, rawdata, pos);
    read_type<Cn>(CONT_ID , rawdata, pos);
    read_type<Cn>(LASR_TYP, rawdata, pos);
    read_type<Cn>(LASR_ID , rawdata, pos);
    read_type<Cn>(EXTR_TYP, rawdata, pos);
    read_type<Cn>(EXTR_ID , rawdata, pos);
    return pos;
}

unsigned int SiteDescription::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<U1>(HEAD_NUM, rawdata, pos);
    length += write_type<U1>(SITE_GRP, rawdata, pos);
    length += write_type<U1>(SITE_CNT, rawdata, pos);
    length += write_type<kxU1>(SITE_NUM, rawdata, pos, SITE_CNT);
    length += write_type<Cn>(HAND_TYP, rawdata, pos);
    length += write_type<Cn>(HAND_ID , rawdata, pos);
    length += write_type<Cn>(CARD_TYP, rawdata, pos);
    length += write_type<Cn>(CARD_ID , rawdata, pos);
    length += write_type<Cn>(LOAD_TYP, rawdata, pos);
    length += write_type<Cn>(LOAD_ID , rawdata, pos);
    length += write_type<Cn>(DIB_TYP , rawdata, pos);
    length += write_type<Cn>(DIB_ID  , rawdata, pos);
    length += write_type<Cn>(CABL_TYP, rawdata, pos);
    length += write_type<Cn>(CABL_ID , rawdata, pos);
    length += write_type<Cn>(CONT_TYP, rawdata, pos);
    length += write_type<Cn>(CONT_ID , rawdata, pos);
    length += write_type<Cn>(LASR_TYP, rawdata, pos);
    length += write_type<Cn>(LASR_ID , rawdata, pos);
    length += write_type<Cn>(EXTR_TYP, rawdata, pos);
    length += write_type<Cn>(EXTR_ID , rawdata, pos);
    header.REC_LEN = length;
    return pos;
}

void SiteDescription::Print(std::ostream& os)
{
    os<<"HEAD_NUM : "<<  (unsigned int)HEAD_NUM <<"\n";
    os<<"SITE_GRP : "<<  (unsigned int)SITE_GRP <<"\n";
    os<<"SITE_CNT : "<<  (unsigned int)SITE_CNT <<"\n";
    os<<"SITE_NUM : ";
    for(U1 i = 0; i < SITE_CNT; i++)
    {
        os<< (unsigned int)(SITE_NUM[i])<<" ";
    }
    os << "\n";
    os<<"HAND_TYP : "<< HAND_TYP <<"\n";
    os<<"HAND_ID  : "<< HAND_ID  <<"\n";
    os<<"CARD_TYP : "<< CARD_TYP <<"\n";
    os<<"CARD_ID  : "<< CARD_ID  <<"\n";
    os<<"LOAD_TYP : "<< LOAD_TYP <<"\n";
    os<<"LOAD_ID  : "<< LOAD_ID  <<"\n";
    os<<"DIB_TYP  : "<< DIB_TYP  <<"\n";
    os<<"DIB_ID   : "<< DIB_ID   <<"\n";
    os<<"CABL_TYP : "<< CABL_TYP <<"\n";
    os<<"CABL_ID  : "<< CABL_ID  <<"\n";
    os<<"CONT_TYP : "<< CONT_TYP <<"\n";
    os<<"CONT_ID  : "<< CONT_ID  <<"\n";
    os<<"LASR_TYP : "<< LASR_TYP <<"\n";
    os<<"LASR_ID  : "<< LASR_ID  <<"\n";
    os<<"EXTR_TYP : "<< EXTR_TYP <<"\n";
    os<<"EXTR_ID  : "<< EXTR_ID  <<"\n";
}
//=============================================================

//========================WIR==================================
WaferInformation::WaferInformation()
{
     REC_LEN  = U2(0);
     REC_TYP  = U1(2);
     REC_SUB  = U1(10);
     HEAD_NUM = U1(1);
     SITE_GRP = U1(255);
     START_T  = U4(0);
}

unsigned int WaferInformation::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<U1>( HEAD_NUM, rawdata, pos) ;
    read_type<U1>( SITE_GRP, rawdata, pos) ;
    read_type<U4>( START_T , rawdata, pos) ;
    read_type<Cn>( WAFER_ID, rawdata, pos) ;
    return pos;
}

unsigned int WaferInformation::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<U1>(HEAD_NUM , rawdata, pos);
    length += write_type<U1>(SITE_GRP , rawdata, pos);
    length += write_type<U4>(START_T  , rawdata, pos);
    length += write_type<Cn>(WAFER_ID , rawdata, pos);
    header.REC_LEN = length;
    return pos;
}

void WaferInformation::Print(std::ostream& os)
{
    os<<"HEAD_NUM : "<<(unsigned int)HEAD_NUM<<"\n";
    os<<"SITE_GRP : "<<(unsigned int)SITE_GRP<<"\n";
    time_t start_t = (time_t)START_T;
    os<<"START_T  : "<<std::string(ctime(&start_t));
    os<<"WAFER_ID : "<<WAFER_ID<<"\n";
}
//=============================================================

//========================WRR==================================
WaferResults::WaferResults()
{
     REC_LEN  = U2(0);
     REC_TYP  = U1(2);
     REC_SUB  = U1(20);
     HEAD_NUM = U1(1);
     SITE_GRP = U1(255);
     FINISH_T = U4(0);
     PART_CNT = U4(0);
     RTST_CNT = U4(-1);
     ABRT_CNT = U4(-1);
     GOOD_CNT = U4(-1);
     FUNC_CNT = U4(-1);
}

unsigned int WaferResults::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<U1>( HEAD_NUM , rawdata, pos);
    read_type<U1>( SITE_GRP , rawdata, pos);
    read_type<U4>( FINISH_T , rawdata, pos);
    read_type<U4>( PART_CNT , rawdata, pos);
    read_type<U4>( RTST_CNT , rawdata, pos);
    read_type<U4>( ABRT_CNT , rawdata, pos);
    read_type<U4>( GOOD_CNT , rawdata, pos);
    read_type<U4>( FUNC_CNT , rawdata, pos);
    read_type<Cn>( WAFER_ID , rawdata, pos);
    read_type<Cn>( FABWF_ID , rawdata, pos);
    read_type<Cn>( FRAME_ID , rawdata, pos);
    read_type<Cn>( MASK_ID  , rawdata, pos);
    read_type<Cn>( USR_DESC , rawdata, pos);
    read_type<Cn>( EXC_DESC , rawdata, pos);
    return pos;
}

unsigned int WaferResults::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<U1>(HEAD_NUM , rawdata, pos);
    length += write_type<U1>(SITE_GRP , rawdata, pos);
    length += write_type<U4>(FINISH_T , rawdata, pos);
    length += write_type<U4>(PART_CNT , rawdata, pos);
    length += write_type<U4>(RTST_CNT , rawdata, pos);
    length += write_type<U4>(ABRT_CNT , rawdata, pos);
    length += write_type<U4>(GOOD_CNT , rawdata, pos);
    length += write_type<U4>(FUNC_CNT , rawdata, pos);
    length += write_type<Cn>(WAFER_ID , rawdata, pos);
    length += write_type<Cn>(FABWF_ID , rawdata, pos);
    length += write_type<Cn>(FRAME_ID , rawdata, pos);
    length += write_type<Cn>(MASK_ID  , rawdata, pos);
    length += write_type<Cn>(USR_DESC , rawdata, pos);
    length += write_type<Cn>(EXC_DESC , rawdata, pos);
    header.REC_LEN = length;
    return pos;
}

void WaferResults::Print(std::ostream& os)
{
    os<<"HEAD_NUM : "<<(unsigned int)HEAD_NUM<<"\n" ;
    os<<"SITE_GRP : "<<(unsigned int)SITE_GRP<<"\n" ;
    os<<"FINISH_T : "<<FINISH_T<<"\n" ;
    os<<"PART_CNT : "<<PART_CNT<<"\n" ;
    os<<"RTST_CNT : "<<RTST_CNT<<"\n" ;
    os<<"ABRT_CNT : "<<ABRT_CNT<<"\n" ;
    os<<"GOOD_CNT : "<<GOOD_CNT<<"\n" ;
    os<<"FUNC_CNT : "<<FUNC_CNT<<"\n" ;
    os<<"WAFER_ID : "<<WAFER_ID<<"\n" ;
    os<<"FABWF_ID : "<<FABWF_ID<<"\n" ;
    os<<"FRAME_ID : "<<FRAME_ID<<"\n" ;
    os<<"MASK_ID  : "<<MASK_ID <<"\n" ;
    os<<"USR_DESC : "<<USR_DESC<<"\n" ;
    os<<"EXC_DESC : "<<EXC_DESC<<"\n" ;
}
//=============================================================

//========================WCR==================================
WaferConfiguration::WaferConfiguration()
{
     REC_LEN  = U2(0);
     REC_TYP  = U1(2);
     REC_SUB  = U1(30);
     WAFR_SIZ = R4(0);
     DIE_HT   = R4(0);
     DIE_WID  = R4(0);
     WF_UNITS = U1(0);
     WF_FLAT  = C1(' ');
     CENTER_X = I2(-32768);
     CENTER_Y = I2(-32768);
     POS_X    = C1(' ');
     POS_Y    = C1(' ');
}

unsigned int WaferConfiguration::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<R4>(WAFR_SIZ, rawdata, pos) ;
    read_type<R4>(DIE_HT  , rawdata, pos) ;
    read_type<R4>(DIE_WID , rawdata, pos) ;
    read_type<U1>(WF_UNITS, rawdata, pos) ;
    read_type<C1>(WF_FLAT , rawdata, pos) ;
    read_type<I2>(CENTER_X, rawdata, pos) ;
    read_type<I2>(CENTER_Y, rawdata, pos) ;
    read_type<C1>(POS_X   , rawdata, pos) ;
    read_type<C1>(POS_Y   , rawdata, pos) ;
    return pos;
}

unsigned int WaferConfiguration::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<R4>(WAFR_SIZ, rawdata, pos) ;
    length += write_type<R4>(DIE_HT  , rawdata, pos) ;
    length += write_type<R4>(DIE_WID , rawdata, pos) ;
    length += write_type<U1>(WF_UNITS, rawdata, pos) ;
    length += write_type<C1>(WF_FLAT , rawdata, pos) ;
    length += write_type<I2>(CENTER_X, rawdata, pos) ;
    length += write_type<I2>(CENTER_Y, rawdata, pos) ;
    length += write_type<C1>(POS_X   , rawdata, pos) ;
    length += write_type<C1>(POS_Y   , rawdata, pos) ;
    header.REC_LEN = length;
    return pos;
}

void WaferConfiguration::Print(std::ostream& os)
{
    os<<"WAFR_SIZ : "<<WAFR_SIZ<<"\n";
    os<<"DIE_HT   : "<<DIE_HT  <<"\n";
    os<<"DIE_WID  : "<<DIE_WID <<"\n";
    os<<"WF_UNITS : "<<(unsigned int)WF_UNITS<<" ";
    switch(WF_UNITS)
    {
        case 1: os<<"inches"<<"\n"; break;
        case 2: os<<"centimeters"<<"\n"; break;
        case 3: os<<"millimeters"<<"\n"; break;
        case 4: os<<"mils "<<"\n"; break;
        default: os<<"unknown"<<"\n"; break;
    }
    os<<"WF_FLAT  : "<<WF_FLAT <<"\n";
    os<<"CENTER_X : "<<(int)CENTER_X<<"\n";
    os<<"CENTER_Y : "<<(int)CENTER_Y<<"\n";
    os<<"POS_X    : "<<POS_X   <<"\n";
    os<<"POS_Y    : "<<POS_Y   <<"\n";
}
//=============================================================

//========================PIR==================================
PartInformation::PartInformation()
{
    REC_LEN  = U2(0);
    REC_TYP  = U1(5);
    REC_SUB  = U1(10);
    HEAD_NUM = U1(1);
    SITE_NUM = U1(1);
}

unsigned int PartInformation::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<U1>(HEAD_NUM, rawdata, pos);
    read_type<U1>(SITE_NUM, rawdata, pos);
    return pos;
}

unsigned int PartInformation::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<U1>(HEAD_NUM, rawdata, pos);
    length += write_type<U1>(SITE_NUM, rawdata, pos);
    header.REC_LEN = length;
    return pos;
}

void PartInformation::Print(std::ostream& os)
{
    os<<"HEAD_NUM : "<<(unsigned int)HEAD_NUM<<"\n";
    os<<"SITE_NUM : "<<(unsigned int)SITE_NUM<<"\n";
}
//=============================================================

//========================PRR==================================
PartResults::PartResults()
{
    REC_LEN  = U2(0);
    REC_TYP  = U1(5);
    REC_SUB  = U1(20);
    HEAD_NUM = U1(1);
    SITE_NUM = U1(1);
    PART_FLG = B1("00000000");
    NUM_TEST = U2(1);
    HARD_BIN = U2(0);
    SOFT_BIN = U2(-1);
    X_COORD  = I2(-32768);
    Y_COORD  = I2(-32768);
    TEST_T   = U4(0);
    PART_FIX.count = 0;
}

unsigned int PartResults::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<U1>(HEAD_NUM, rawdata, pos);
    read_type<U1>(SITE_NUM, rawdata, pos);
    read_type<B1>(PART_FLG, rawdata, pos);
    read_type<U2>(NUM_TEST, rawdata, pos);
    read_type<U2>(HARD_BIN, rawdata, pos);
    read_type<U2>(SOFT_BIN, rawdata, pos);
    read_type<I2>(X_COORD , rawdata, pos);
    read_type<I2>(Y_COORD , rawdata, pos);
    read_type<U4>(TEST_T  , rawdata, pos);
    read_type<Cn>(PART_ID , rawdata, pos);
    read_type<Cn>(PART_TXT, rawdata, pos);
    read_type<Bn>(PART_FIX, rawdata, pos);
    return pos;
}

unsigned int PartResults::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<U1>(HEAD_NUM, rawdata, pos);
    length += write_type<U1>(SITE_NUM, rawdata, pos);
    length += write_type<B1>(PART_FLG, rawdata, pos);
    length += write_type<U2>(NUM_TEST, rawdata, pos);
    length += write_type<U2>(HARD_BIN, rawdata, pos);
    length += write_type<U2>(SOFT_BIN, rawdata, pos);
    length += write_type<I2>(X_COORD , rawdata, pos);
    length += write_type<I2>(Y_COORD , rawdata, pos);
    length += write_type<U4>(TEST_T  , rawdata, pos);
    length += write_type<Cn>(PART_ID , rawdata, pos);
    length += write_type<Cn>(PART_TXT, rawdata, pos);
    length += write_type<Bn>(PART_FIX, rawdata, pos);
    header.REC_LEN = length;
    return pos;
}

void PartResults::Print(std::ostream& os)
{
    os<<"HEAD_NUM : "<<(unsigned int)HEAD_NUM <<"\n";
    os<<"SITE_NUM : "<<(unsigned int)SITE_NUM <<"\n";
    os<<"PART_FLG : "<<PART_FLG <<"\n";
    os<<"NUM_TEST : "<<(unsigned int)NUM_TEST <<"\n";
    os<<"HARD_BIN : "<<(unsigned int)HARD_BIN <<"\n";
    os<<"SOFT_BIN : "<<(unsigned int)SOFT_BIN <<"\n";
    os<<"X_COORD  : "<<(int)X_COORD  <<"\n";
    os<<"Y_COORD  : "<<(int)Y_COORD  <<"\n";
    os<<"TEST_T   : "<<TEST_T   <<"\n";
    os<<"PART_ID  : "<<PART_ID  <<"\n";
    os<<"PART_TXT : "<<PART_TXT <<"\n";
    os<<"PART_FIX : ";
    for(unsigned int i = 0; i < PART_FIX.count; i++)
    {
        os<<PART_FIX.data[i]<<" ";
    }
	os<<"\n";
}
//=============================================================

//========================TSR==================================
TestSynopsis::TestSynopsis()
{
     REC_LEN  = U2(0);
     REC_TYP  = U1(10);
     REC_SUB  = U1(30);
     HEAD_NUM = U1(1);
     SITE_NUM = U1(1);
     TEST_TYP = C1(' ');
     TEST_NUM = U4(0);
     EXEC_CNT = U4(-1);
     FAIL_CNT = U4(-1);
     ALRM_CNT = U4(-1);
     OPT_FLAG = B1("11111111");
     TEST_TIM = R4(0.0);
     TEST_MIN = R4(0.0);
     TEST_MAX = R4(0.0);
     TST_SUMS = R4(0.0);
     TST_SQRS = R4(0.0);
}

unsigned int TestSynopsis::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<U1>(HEAD_NUM, rawdata, pos);
    read_type<U1>(SITE_NUM, rawdata, pos);
    read_type<C1>(TEST_TYP, rawdata, pos);
    read_type<U4>(TEST_NUM, rawdata, pos);
    read_type<U4>(EXEC_CNT, rawdata, pos);
    read_type<U4>(FAIL_CNT, rawdata, pos);
    read_type<U4>(ALRM_CNT, rawdata, pos);
    read_type<Cn>(TEST_NAM, rawdata, pos);
    read_type<Cn>(SEQ_NAME, rawdata, pos);
    read_type<Cn>(TEST_LBL, rawdata, pos);
    read_type<B1>(OPT_FLAG, rawdata, pos);
    read_type<R4>(TEST_TIM, rawdata, pos);
    read_type<R4>(TEST_MIN, rawdata, pos);
    read_type<R4>(TEST_MAX, rawdata, pos);
    read_type<R4>(TST_SUMS, rawdata, pos);
    read_type<R4>(TST_SQRS, rawdata, pos);
    return pos;
}

unsigned int TestSynopsis::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<U1>(HEAD_NUM, rawdata, pos);
    length += write_type<U1>(SITE_NUM, rawdata, pos);
    length += write_type<C1>(TEST_TYP, rawdata, pos);
    length += write_type<U4>(TEST_NUM, rawdata, pos);
    length += write_type<U4>(EXEC_CNT, rawdata, pos);
    length += write_type<U4>(FAIL_CNT, rawdata, pos);
    length += write_type<U4>(ALRM_CNT, rawdata, pos);
    length += write_type<Cn>(TEST_NAM, rawdata, pos);
    length += write_type<Cn>(SEQ_NAME, rawdata, pos);
    length += write_type<Cn>(TEST_LBL, rawdata, pos);
    length += write_type<B1>(OPT_FLAG, rawdata, pos);
    length += write_type<R4>(TEST_TIM, rawdata, pos);
    length += write_type<R4>(TEST_MIN, rawdata, pos);
    length += write_type<R4>(TEST_MAX, rawdata, pos);
    length += write_type<R4>(TST_SUMS, rawdata, pos);
    length += write_type<R4>(TST_SQRS, rawdata, pos);
    header.REC_LEN = length;
    return pos;
}

void TestSynopsis::Print(std::ostream& os)
{
    os<<"HEAD_NUM : "<<(unsigned int)HEAD_NUM <<"\n";
    os<<"SITE_NUM : "<<(unsigned int)SITE_NUM <<"\n";
    os<<"TEST_TYP : "<<TEST_TYP <<"\n";
    os<<"TEST_NUM : "<<TEST_NUM <<"\n";
    os<<"EXEC_CNT : "<<EXEC_CNT <<"\n";
    os<<"FAIL_CNT : "<<FAIL_CNT <<"\n";
    os<<"ALRM_CNT : "<<ALRM_CNT <<"\n";
    os<<"TEST_NAM : "<<TEST_NAM <<"\n";
    os<<"SEQ_NAME : "<<SEQ_NAME <<"\n";
    os<<"TEST_LBL : "<<TEST_LBL <<"\n";
    os<<"OPT_FLAG : "<<OPT_FLAG <<"\n";
    os<<"TEST_TIM : "<<TEST_TIM <<"\n";
    os<<"TEST_MIN : "<<TEST_MIN <<"\n";
    os<<"TEST_MAX : "<<TEST_MAX <<"\n";
    os<<"TST_SUMS : "<<TST_SUMS <<"\n";
    os<<"TST_SQRS : "<<TST_SQRS <<"\n";
}
//=============================================================

//========================PTR==================================
ParametricTest::ParametricTest()
{
     REC_LEN  = U2(0);
     REC_TYP  = U1(15);
     REC_SUB  = U1(10);
     TEST_NUM = U4(0);
     HEAD_NUM = U1(1);
     SITE_NUM = U1(1);
     TEST_FLG = B1("00000000");
     PARM_FLG = B1("11000000");
     RESULT   = R4(0);
     OPT_FLAG = B1("11111111");
     RES_SCAL = I1(0);
     LLM_SCAL = I1(0);
     HLM_SCAL = I1(0);
     LO_LIMIT = R4(0);
     HI_LIMIT = R4(0);
     LO_SPEC  = R4(0);
     HI_SPEC  = R4(0);
}

unsigned int ParametricTest::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<U4>(TEST_NUM, rawdata, pos);
    read_type<U1>(HEAD_NUM, rawdata, pos);
    read_type<U1>(SITE_NUM, rawdata, pos);
    read_type<B1>(TEST_FLG, rawdata, pos);
    read_type<B1>(PARM_FLG, rawdata, pos);
    read_type<R4>(RESULT  , rawdata, pos);
    read_type<Cn>(TEST_TXT, rawdata, pos);
    read_type<Cn>(ALARM_ID, rawdata, pos);
    read_type<B1>(OPT_FLAG, rawdata, pos);
    read_type<I1>(RES_SCAL, rawdata, pos);
    read_type<I1>(LLM_SCAL, rawdata, pos);
    read_type<I1>(HLM_SCAL, rawdata, pos);
    read_type<R4>(LO_LIMIT, rawdata, pos);
    read_type<R4>(HI_LIMIT, rawdata, pos);
    read_type<Cn>(UNITS   , rawdata, pos);
    read_type<Cn>(C_RESFMT, rawdata, pos);
    read_type<Cn>(C_LLMFMT, rawdata, pos);
    read_type<Cn>(C_HLMFMT, rawdata, pos);
    read_type<R4>(LO_SPEC , rawdata, pos);
    read_type<R4>(HI_SPEC , rawdata, pos);
    return pos;
}

unsigned int ParametricTest::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<U4>(TEST_NUM, rawdata, pos);
    length += write_type<U1>(HEAD_NUM, rawdata, pos);
    length += write_type<U1>(SITE_NUM, rawdata, pos);
    length += write_type<B1>(TEST_FLG, rawdata, pos);
    length += write_type<B1>(PARM_FLG, rawdata, pos);
    length += write_type<R4>(RESULT  , rawdata, pos);
    length += write_type<Cn>(TEST_TXT, rawdata, pos);
    length += write_type<Cn>(ALARM_ID, rawdata, pos);
    length += write_type<B1>(OPT_FLAG, rawdata, pos);
    length += write_type<I1>(RES_SCAL, rawdata, pos);
    length += write_type<I1>(LLM_SCAL, rawdata, pos);
    length += write_type<I1>(HLM_SCAL, rawdata, pos);
    length += write_type<R4>(LO_LIMIT, rawdata, pos);
    length += write_type<R4>(HI_LIMIT, rawdata, pos);
    length += write_type<Cn>(UNITS   , rawdata, pos);
    length += write_type<Cn>(C_RESFMT, rawdata, pos);
    length += write_type<Cn>(C_LLMFMT, rawdata, pos);
    length += write_type<Cn>(C_HLMFMT, rawdata, pos);
    length += write_type<R4>(LO_SPEC , rawdata, pos);
    length += write_type<R4>(HI_SPEC , rawdata, pos);
    header.REC_LEN = length;
    return pos;
}

void ParametricTest::Print(std::ostream& os)
{
    os<<"TEST_NUM : "<< TEST_NUM <<"\n";
    os<<"HEAD_NUM : "<< (unsigned int)HEAD_NUM <<"\n";
    os<<"SITE_NUM : "<< (unsigned int)SITE_NUM <<"\n";
    os<<"TEST_FLG : "<< TEST_FLG <<"\n";
    os<<"PARM_FLG : "<< PARM_FLG <<"\n";
    os<<"RESULT   : "<< RESULT   <<"\n";
    os<<"TEST_TXT : "<< TEST_TXT <<"\n";
    os<<"ALARM_ID : "<< ALARM_ID <<"\n";
    os<<"OPT_FLAG : "<< OPT_FLAG <<"\n";
    os<<"RES_SCAL : "<< (int)RES_SCAL <<"\n";
    os<<"LLM_SCAL : "<< (int)LLM_SCAL <<"\n";
    os<<"HLM_SCAL : "<< (int)HLM_SCAL <<"\n";
    os<<"LO_LIMIT : "<< LO_LIMIT <<"\n";
    os<<"HI_LIMIT : "<< HI_LIMIT <<"\n";
    os<<"UNITS    : "<< UNITS    <<"\n";
    os<<"C_RESFMT : "<< C_RESFMT <<"\n";
    os<<"C_LLMFMT : "<< C_LLMFMT <<"\n";
    os<<"C_HLMFMT : "<< C_HLMFMT <<"\n";
    os<<"LO_SPEC  : "<< LO_SPEC  <<"\n";
    os<<"HI_SPEC  : "<< HI_SPEC  <<"\n";
}
//=============================================================

//========================MPR==================================
MultipleResultParametric::MultipleResultParametric()
{
   REC_LEN  = U2(0);
   REC_TYP  = U1(15);
   REC_SUB  = U1(15);
   TEST_NUM = U4(0);
   HEAD_NUM = U1(1);
   SITE_NUM = U1(1);
   TEST_FLG = B1("00000000");
   PARM_FLG = B1("00000000");
   RTN_ICNT = U2(0);
   RSLT_CNT = U2(0);
   OPT_FLAG = B1("11111110");
   RES_SCAL = I1(0);
   LLM_SCAL = I1(0);
   HLM_SCAL = I1(0);
   LO_LIMIT = R4(0.0);
   HI_LIMIT = R4(0.0);
   START_IN = R4(0.0);
   INCR_IN  = R4(0.0);
   LO_SPEC  = R4(0.0);
   HI_SPEC  = R4(0.0);
}

unsigned int MultipleResultParametric::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<U4>( TEST_NUM, rawdata, pos) ;
    read_type<U1>( HEAD_NUM, rawdata, pos) ;
    read_type<U1>( SITE_NUM, rawdata, pos) ;
    read_type<B1>( TEST_FLG, rawdata, pos) ;
    read_type<B1>( PARM_FLG, rawdata, pos) ;
    read_type<U2>( RTN_ICNT, rawdata, pos) ;
    read_type<U2>( RSLT_CNT, rawdata, pos) ;
    read_type<kxN1>( RTN_STAT, rawdata, pos, RTN_ICNT) ;
    read_type<kxR4>( RTN_RSLT, rawdata, pos, RSLT_CNT) ;
    read_type<Cn>( TEST_TXT, rawdata, pos) ;
    read_type<Cn>( ALARM_ID, rawdata, pos) ;
    read_type<B1>( OPT_FLAG, rawdata, pos) ;
    read_type<I1>( RES_SCAL, rawdata, pos) ;
    read_type<I1>( LLM_SCAL, rawdata, pos) ;
    read_type<I1>( HLM_SCAL, rawdata, pos) ;
    read_type<R4>( LO_LIMIT, rawdata, pos) ;
    read_type<R4>( HI_LIMIT, rawdata, pos) ;
    read_type<R4>( START_IN, rawdata, pos) ;
    read_type<R4>( INCR_IN , rawdata, pos) ;
    read_type<kxU2>( RTN_INDX, rawdata, pos, RTN_ICNT) ;
    read_type<Cn>( UNITS   , rawdata, pos) ;
    read_type<Cn>( UNITS_IN, rawdata, pos) ;
    read_type<Cn>( C_RESFMT, rawdata, pos) ;
    read_type<Cn>( C_LLMFMT, rawdata, pos) ;
    read_type<Cn>( C_HLMFMT, rawdata, pos) ;
    read_type<R4>( LO_SPEC , rawdata, pos) ;
    read_type<R4>( HI_SPEC , rawdata, pos) ;
    return pos;
}

unsigned int MultipleResultParametric::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<U4>( TEST_NUM , rawdata, pos);
    length += write_type<U1>( HEAD_NUM , rawdata, pos);
    length += write_type<U1>( SITE_NUM , rawdata, pos);
    length += write_type<B1>( TEST_FLG , rawdata, pos);
    length += write_type<B1>( PARM_FLG , rawdata, pos);
    length += write_type<U2>( RTN_ICNT , rawdata, pos);
    length += write_type<U2>( RSLT_CNT , rawdata, pos);
    length += write_type<kxN1>( RTN_STAT , rawdata, pos, RTN_ICNT);
    length += write_type<kxR4>( RTN_RSLT , rawdata, pos, RSLT_CNT);
    length += write_type<Cn>( TEST_TXT , rawdata, pos);
    length += write_type<Cn>( ALARM_ID , rawdata, pos);
    length += write_type<B1>( OPT_FLAG , rawdata, pos);
    length += write_type<I1>( RES_SCAL , rawdata, pos);
    length += write_type<I1>( LLM_SCAL , rawdata, pos);
    length += write_type<I1>( HLM_SCAL , rawdata, pos);
    length += write_type<R4>( LO_LIMIT , rawdata, pos);
    length += write_type<R4>( HI_LIMIT , rawdata, pos);
    length += write_type<R4>( START_IN , rawdata, pos);
    length += write_type<R4>( INCR_IN  , rawdata, pos);
    length += write_type<kxU2>( RTN_INDX , rawdata, pos, RTN_ICNT);
    length += write_type<Cn>( UNITS    , rawdata, pos);
    length += write_type<Cn>( UNITS_IN , rawdata, pos);
    length += write_type<Cn>( C_RESFMT , rawdata, pos);
    length += write_type<Cn>( C_LLMFMT , rawdata, pos);
    length += write_type<Cn>( C_HLMFMT , rawdata, pos);
    length += write_type<R4>( LO_SPEC  , rawdata, pos);
    length += write_type<R4>( HI_SPEC  , rawdata, pos);
    header.REC_LEN = length;
    return pos;
}

void MultipleResultParametric::Print(std::ostream& os)
{
    os<<"TEST_NUM : "<<TEST_NUM<<"\n";
    os<<"HEAD_NUM : "<<(unsigned int)HEAD_NUM<<"\n";
    os<<"SITE_NUM : "<<(unsigned int)SITE_NUM<<"\n";
    os<<"TEST_FLG : "<<TEST_FLG<<"\n";
    os<<"PARM_FLG : "<<PARM_FLG<<"\n";
    os<<"RTN_ICNT : "<<(unsigned int)RTN_ICNT<<"\n";
    os<<"RSLT_CNT : "<<(unsigned int)RSLT_CNT<<"\n";
    os<<"RTN_STAT : ";
    for(U2 i = 0; i < RTN_STAT.size(); i++)
    {
        os<<RTN_STAT[i]<<" ";
    }
    os<<"\n";
    os<<"RTN_RSLT : ";
    for(U2 i = 0; i < RTN_RSLT.size(); i++)
    {
        os<<RTN_RSLT[i]<<" ";
    }
    os<<"\n";
    os<<"TEST_TXT : "<<TEST_TXT<<"\n";
    os<<"ALARM_ID : "<<ALARM_ID<<"\n";
    os<<"OPT_FLAG : "<<OPT_FLAG<<"\n";
    os<<"RES_SCAL : "<<(int)RES_SCAL<<"\n";
    os<<"LLM_SCAL : "<<(int)LLM_SCAL<<"\n";
    os<<"HLM_SCAL : "<<(int)HLM_SCAL<<"\n";
    os<<"LO_LIMIT : "<<LO_LIMIT<<"\n";
    os<<"HI_LIMIT : "<<HI_LIMIT<<"\n";
    os<<"START_IN : "<<START_IN<<"\n";
    os<<"INCR_IN  : "<<INCR_IN <<"\n";
    os<<"RTN_INDX : ";
    for(U2 i = 0; i < RTN_INDX.size(); i++)
    {
        os<<(unsigned int)RTN_INDX[i]<<" ";
    }
    os<<"\n";
    os<<"UNITS    : "<<UNITS   <<"\n";
    os<<"UNITS_IN : "<<UNITS_IN<<"\n";
    os<<"C_RESFMT : "<<C_RESFMT<<"\n";
    os<<"C_LLMFMT : "<<C_LLMFMT<<"\n";
    os<<"C_HLMFMT : "<<C_HLMFMT<<"\n";
    os<<"LO_SPEC  : "<<LO_SPEC <<"\n";
    os<<"HI_SPEC  : "<<HI_SPEC <<"\n";
}
//=============================================================

//========================FTR==================================
FunctionalTest::FunctionalTest()
{
    REC_LEN  = U2(0)  ;
    REC_TYP  = U1(15)  ;
    REC_SUB  = U1(20)  ;
    TEST_NUM = U4(0)  ;
    HEAD_NUM = U1(1)  ;
    SITE_NUM = U1(1)  ;
    TEST_FLG = B1("00000000")  ;
    OPT_FLAG = B1("11111111")  ;
    CYCL_CNT = U4(0)  ;
    REL_VADR = U4(0)  ;
    REPT_CNT = U4(0)  ;
    NUM_FAIL = U4(0)  ;
    XFAIL_AD = I4(0)  ;
    YFAIL_AD = I4(0)  ;
    VECT_OFF = I2(0)  ;
    RTN_ICNT = U2(0)  ;
    PGM_ICNT = U2(0)  ;
    FAIL_PIN.count = 0  ;
    PATG_NUM = U1(255)  ;
    SPIN_MAP.count = 0  ;
}

unsigned int FunctionalTest::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<U4>( TEST_NUM , rawdata, pos);
    read_type<U1>( HEAD_NUM , rawdata, pos);
    read_type<U1>( SITE_NUM , rawdata, pos);
    read_type<B1>( TEST_FLG , rawdata, pos);
    read_type<B1>( OPT_FLAG , rawdata, pos);
    read_type<U4>( CYCL_CNT , rawdata, pos);
    read_type<U4>( REL_VADR , rawdata, pos);
    read_type<U4>( REPT_CNT , rawdata, pos);
    read_type<U4>( NUM_FAIL , rawdata, pos);
    read_type<I4>( XFAIL_AD , rawdata, pos);
    read_type<I4>( YFAIL_AD , rawdata, pos);
    read_type<I2>( VECT_OFF , rawdata, pos);
    read_type<U2>( RTN_ICNT , rawdata, pos);
    read_type<U2>( PGM_ICNT , rawdata, pos);
    read_type<kxU2>( RTN_INDX , rawdata, pos, RTN_ICNT);
    read_type<kxN1>( RTN_STAT , rawdata, pos, RTN_ICNT);
    read_type<kxU2>( PGM_INDX , rawdata, pos, PGM_ICNT);
    read_type<kxN1>( PGM_STAT , rawdata, pos, PGM_ICNT);
    read_type<Dn>( FAIL_PIN , rawdata, pos);
    read_type<Cn>( VECT_NAM , rawdata, pos);
    read_type<Cn>( TIME_SET , rawdata, pos);
    read_type<Cn>( OP_CODE  , rawdata, pos);
    read_type<Cn>( TEST_TXT , rawdata, pos);
    read_type<Cn>( ALARM_ID , rawdata, pos);
    read_type<Cn>( PROG_TXT , rawdata, pos);
    read_type<Cn>( RSLT_TXT , rawdata, pos);
    read_type<U1>( PATG_NUM , rawdata, pos);
    read_type<Dn>( SPIN_MAP , rawdata, pos);
    return pos;
}

unsigned int FunctionalTest::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<U4>( TEST_NUM , rawdata, pos);
    length += write_type<U1>( HEAD_NUM , rawdata, pos);
    length += write_type<U1>( SITE_NUM , rawdata, pos);
    length += write_type<B1>( TEST_FLG , rawdata, pos);
    length += write_type<B1>( OPT_FLAG , rawdata, pos);
    length += write_type<U4>( CYCL_CNT , rawdata, pos);
    length += write_type<U4>( REL_VADR , rawdata, pos);
    length += write_type<U4>( REPT_CNT , rawdata, pos);
    length += write_type<U4>( NUM_FAIL , rawdata, pos);
    length += write_type<I4>( XFAIL_AD , rawdata, pos);
    length += write_type<I4>( YFAIL_AD , rawdata, pos);
    length += write_type<I2>( VECT_OFF , rawdata, pos);
    length += write_type<U2>( RTN_ICNT , rawdata, pos);
    length += write_type<U2>( PGM_ICNT , rawdata, pos);
    length += write_type<kxU2>( RTN_INDX , rawdata, pos, RTN_ICNT);
    length += write_type<kxN1>( RTN_STAT , rawdata, pos, RTN_ICNT);
    length += write_type<kxU2>( PGM_INDX , rawdata, pos, PGM_ICNT);
    length += write_type<kxN1>( PGM_STAT , rawdata, pos, PGM_ICNT);
    length += write_type<Dn>( FAIL_PIN , rawdata, pos);
    length += write_type<Cn>( VECT_NAM , rawdata, pos);
    length += write_type<Cn>( TIME_SET , rawdata, pos);
    length += write_type<Cn>( OP_CODE  , rawdata, pos);
    length += write_type<Cn>( TEST_TXT , rawdata, pos);
    length += write_type<Cn>( ALARM_ID , rawdata, pos);
    length += write_type<Cn>( PROG_TXT , rawdata, pos);
    length += write_type<Cn>( RSLT_TXT , rawdata, pos);
    length += write_type<U1>( PATG_NUM , rawdata, pos);
    length += write_type<Dn>( SPIN_MAP , rawdata, pos);
    header.REC_LEN = length;
    return pos;
}

void FunctionalTest::Print(std::ostream& os)
{
    os<<"TEST_NUM : "<<TEST_NUM<<"\n";
    os<<"HEAD_NUM : "<<(unsigned int)HEAD_NUM<<"\n";
    os<<"SITE_NUM : "<<(unsigned int)SITE_NUM<<"\n";
    os<<"TEST_FLG : "<<TEST_FLG<<"\n";
    os<<"OPT_FLAG : "<<OPT_FLAG<<"\n";
    os<<"CYCL_CNT : "<<CYCL_CNT<<"\n";
    os<<"REL_VADR : "<<REL_VADR<<"\n";
    os<<"REPT_CNT : "<<REPT_CNT<<"\n";
    os<<"NUM_FAIL : "<<NUM_FAIL<<"\n";
    os<<"XFAIL_AD : "<<XFAIL_AD<<"\n";
    os<<"YFAIL_AD : "<<YFAIL_AD<<"\n";
    os<<"VECT_OFF : "<<(int)VECT_OFF<<"\n";
    os<<"RTN_ICNT : "<<(unsigned int)RTN_ICNT<<"\n";
    os<<"PGM_ICNT : "<<(unsigned int)PGM_ICNT<<"\n";
    os<<"RTN_INDX : ";
    for(U2 i = 0; i < RTN_INDX.size(); i++)
    {
        os<<(unsigned int)RTN_INDX[i]<<" ";
    }
    os<<"\n";
    os<<"RTN_STAT : ";
    for(U2 i = 0; i < RTN_STAT.size(); i++)
    {
        unsigned int temp = RTN_STAT[i].to_ulong();
        os<<temp<<" ";
    }
    os<<"\n";
    os<<"PGM_INDX : ";
    for(U2 i = 0; i < PGM_INDX.size(); i++)
    {
        os<<(unsigned int)PGM_INDX[i]<<" ";
    }
    os<<"\n";
    os<<"PGM_STAT : ";
    for(U2 i = 0; i < PGM_STAT.size(); i++)
    {
        unsigned int temp = PGM_STAT[i].to_ulong();
        os<<temp<<" ";
    }
    os<<"\n";
    os<<"FAIL_PIN : ";
    U2 bit_count = FAIL_PIN.count;
    for(U2 i = 0; i < bit_count; i++)
    {
        if((i != 0) && (i % 32 == 0)) os<<"\n          ";
        if((i != 0) && (i % 8 == 0)) os<<" ";
        if(FAIL_PIN.data[i]) os<<"1";
        else os<<"0";
    }
    os<<"\n";
    os<<"VECT_NAM : "<<VECT_NAM<<"\n";
    os<<"TIME_SET : "<<TIME_SET<<"\n";
    os<<"OP_CODE  : "<<OP_CODE <<"\n";
    os<<"TEST_TXT : "<<TEST_TXT<<"\n";
    os<<"ALARM_ID : "<<ALARM_ID<<"\n";
    os<<"PROG_TXT : "<<PROG_TXT<<"\n";
    os<<"RSLT_TXT : "<<RSLT_TXT<<"\n";
    os<<"PATG_NUM : "<<(unsigned int)PATG_NUM<<"\n";
    os<<"SPIN_MAP : ";
    bit_count = SPIN_MAP.count;
    for(U2 i = 0; i < bit_count; i++)
    {
        if((i != 0) && (i % 32 == 0)) os<<"\n          ";
        if((i != 0) && (i % 8 == 0)) os<<" ";
        if(SPIN_MAP.data[i]) os<<"1";
        else os<<"0";
    }
    os<<"\n";
}
//=============================================================


//========================BPS==================================
BeginProgramSection::BeginProgramSection()
{
    REC_LEN = U2(0) ;
    REC_TYP = U1(20) ;
    REC_SUB = U1(10) ;
}

unsigned int BeginProgramSection::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<Cn>( SEQ_NAME , rawdata, pos);
    return pos;
}

unsigned int BeginProgramSection::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<Cn>( SEQ_NAME , rawdata, pos);
    header.REC_LEN = length;
    return pos;
}

void BeginProgramSection::Print(std::ostream& os)
{
    os<<"SEQ_NAME : "<<SEQ_NAME<<"\n";
}

//=============================================================


//========================EPS==================================
EndProgramSection::EndProgramSection()
{
    REC_LEN = U2(0);
    REC_TYP = U1(20);
    REC_SUB = U1(20);
}

unsigned int EndProgramSection::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    return 0;
}
unsigned int EndProgramSection::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    header.REC_LEN = 0;
    return 0;
}

void EndProgramSection::Print(std::ostream& os)
{
    os<<"\n";
}

//=============================================================


//========================GDR==================================
GenericData::GenericData()
{
    REC_LEN = U2(0) ;
    REC_TYP = U1(50) ;
    REC_SUB = U1(10) ;
    FLD_CNT = U2(0);
    GEN_DATA.clear();
}

GenericData::~GenericData()
{
    if(FLD_CNT != 0)
    {
        for(unsigned int i = 0; i < GEN_DATA.size(); i++)
        {
            U1 type = GEN_DATA[i].type;
            switch(type)
            {
            case 1 :{ delete (U1*)(GEN_DATA[i].data);  GEN_DATA[i].data = nullptr; } break;
            case 2 :{ delete (U2*)(GEN_DATA[i].data);  GEN_DATA[i].data = nullptr; } break;
            case 3 :{ delete (U4*)(GEN_DATA[i].data);  GEN_DATA[i].data = nullptr; } break;
            case 4 :{ delete (I1*)(GEN_DATA[i].data);  GEN_DATA[i].data = nullptr; } break;
            case 5 :{ delete (I2*)(GEN_DATA[i].data);  GEN_DATA[i].data = nullptr; } break;
            case 6 :{ delete (I4*)(GEN_DATA[i].data);  GEN_DATA[i].data = nullptr; } break;
            case 7 :{ delete (R4*)(GEN_DATA[i].data);  GEN_DATA[i].data = nullptr; } break;
            case 8 :{ delete (R8*)(GEN_DATA[i].data);  GEN_DATA[i].data = nullptr; } break;
            case 10:{ delete (Cn*)(GEN_DATA[i].data);  GEN_DATA[i].data = nullptr; } break;
            case 11:{ delete (Bn*)(GEN_DATA[i].data);  GEN_DATA[i].data = nullptr; } break;
            case 12:{ delete (Dn*)(GEN_DATA[i].data);  GEN_DATA[i].data = nullptr; } break;
            case 13:{ delete (N1*)(GEN_DATA[i].data);  GEN_DATA[i].data = nullptr; } break;
                default: break;
            }
        }
        GEN_DATA.clear();
    }
}

unsigned int GenericData::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<U2>( FLD_CNT  , rawdata, pos);
    read_type<Vn>( GEN_DATA , rawdata, pos, FLD_CNT);
    return pos;
}

unsigned int GenericData::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    unsigned int pad_count = 0;

    pos += 2; // write data before length
    length += write_type<Vn>( GEN_DATA , rawdata, pos, FLD_CNT, &pad_count);
    unsigned int null_data_count = 0;
    unsigned int vector_count = GEN_DATA.size();
    for(unsigned int i = 0; i < vector_count; i++)
    {
        if(GEN_DATA.at(i).type == 0) null_data_count++;
    }
    FLD_CNT = U2(vector_count + pad_count - null_data_count);
    pos = 0;
    length += write_type<U2>( FLD_CNT , rawdata, pos);
    header.REC_LEN = length;
    return pos;
}

void GenericData::Print(std::ostream& os)
{
    os<<"FLD_CNT  : "<<(unsigned int)FLD_CNT<<"\n";
    if(FLD_CNT == 0)
    {
        os<<"\n";
        return ;
    }

    os<<"GEN_DATA : \n";
    for(U2 i = 0; i < GEN_DATA.size(); i++)
    {
        U1 type = GEN_DATA.at(i).type;
        switch(type)
        {
        case 1 :
            {
                U1 *temp = (U1*)GEN_DATA.at(i).data;
                os<<"        "<<"TYPE : U1"<<"\t";
                os<<"VALUE: "<<(unsigned int)(*temp)<<"\n";
            }
            break;
        case 2 :
            {
                U2 *temp = (U2*)GEN_DATA.at(i).data;
                os<<"        "<<"TYPE : U2"<<"\t";
                os<<"VALUE: "<<(unsigned int)(*temp)<<"\n";
            }
            break;
        case 3 :
            {
                U4 *temp = (U4*)GEN_DATA.at(i).data;
                os<<"        "<<"TYPE : U4"<<"\t";
                os<<"VALUE: "<<(*temp)<<"\n";
            }
            break;
        case 4 :
            {
                I1 *temp = (I1*)GEN_DATA.at(i).data;
                os<<"        "<<"TYPE : I1"<<"\t";
                os<<"VALUE: "<<(int)(*temp)<<"\n";
            }
            break;
        case 5 :
            {
                I2 *temp = (I2*)GEN_DATA.at(i).data;
                os<<"        "<<"TYPE : I2"<<"\t";
                os<<"VALUE: "<<(int)(*temp)<<"\n";

            }
            break;
        case 6 :
            {
                I4 *temp = (I4*)GEN_DATA.at(i).data;
                os<<"        "<<"TYPE : I4"<<"\t";
                os<<"VALUE: "<<(*temp)<<"\n";
            }
            break;
        case 7 :
            {
                R4 *temp = (R4*)GEN_DATA.at(i).data;
                os<<"        "<<"TYPE : R4"<<"\t";
                os<<"VALUE: "<<(*temp)<<"\n";
            }
            break;
        case 8 :
            {
                R8 *temp = (R8*)GEN_DATA.at(i).data;
                os<<"        "<<"TYPE : R8"<<"\t";
                os<<"VALUE: "<<(*temp)<<"\n";
            }
            break;
        case 10:
            {
                Cn *temp = (Cn*)GEN_DATA.at(i).data;
                os<<"        "<<"TYPE : Cn"<<"\t";
                os<<"VALUE: "<<(*temp)<<"\n";
            }
            break;
        case 11:
            {
                os<<"        "<<"TYPE : Bn"<<"\t";
                os<<"VALUE: ";
                Bn* temp = (Bn*)GEN_DATA.at(i).data;
                for(U2 n = 0; n < temp->count; n++)
                {
                    os<<temp->data.at(n)<<" ";
                }
                os<<"\n";
            }
            break;
        case 12:
            {
                os<<"        "<<"TYPE : Dn"<<"\t";
                os<<"VALUE: ";
                Dn *temp = (Dn*)GEN_DATA.at(i).data;
                for(U2 n = 0; n < temp->count; n++)
                {
                    if((n != 0) && (n % 32 == 0)) os<<"\n          ";
                    if((n != 0) && (n % 8 == 0)) os<<" ";
                    if(temp->data[n]) os<<"1";
                    else os<<"0";
                }
                os<<"\n";
            }
            break;
        case 13:
            {
                os<<"        "<<"TYPE : N1"<<"\t";
                os<<"VALUE: ";
                N1 *temp = (N1*)GEN_DATA.at(i).data;
                U1 output = U1(temp->to_ulong());
                std::ios::fmtflags old_flags= os.flags();
                os<<std::hex<<std::uppercase<<(unsigned int)output;
                os.flags(old_flags);
                os<<"\n";
            }
            break;
        default: break;
        }
    }
}

//=============================================================


//========================DTR==================================
DatalogText::DatalogText()
{
    REC_LEN = U2(0) ;
    REC_TYP = U1(50) ;
    REC_SUB = U1(30) ;
}

unsigned int DatalogText::Parse(const RecordHeader& header)
{
    REC_LEN = header.REC_LEN;
    REC_TYP = header.REC_TYP;
    REC_SUB = header.REC_SUB;
    const char* rawdata = header.GetReadOnlyData();

    unsigned int pos = 0;
    read_type<Cn>( TEXT_DAT , rawdata, pos);
    return pos;
}

unsigned int DatalogText::Unparse(RecordHeader& header)
{
    header.REC_TYP = REC_TYP;
    header.REC_SUB = REC_SUB;
    char* const rawdata = header.GetWriteOnlyData();

    unsigned int pos = 0;
    U2 length = 0;
    length += write_type<Cn>( TEXT_DAT , rawdata, pos);
    header.REC_LEN = length;
    return pos;
}

void DatalogText::Print(std::ostream& os)
{
    os<<"TEXT_DAT : "<<TEXT_DAT<<"\n";
}
//=============================================================

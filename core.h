#ifndef CORE_H
#define CORE_H
#define INTERFACE
#define DATABLOCK
#define INNER_FUNCTION
#define UNIT
#include <QMap>
#include <QString>

const int hash_base = 31;
const int hash_mod = 1000000007;
const int check_code_mod = 998244353;

//type code
const int type_head_apply = 0x1c;
const int type_head_datablock = 0x2c;
const int type_head_return_code = 0x3c;
const int type_head_command_info = 0x4c;
const int type_head_datagram = 0x5c;
const int type_head_file_info = 0x6c;
const int type_trans_info = 0x7c;
//version code
const int version_beta = 0x1a;
//return code :
const int connected = 0x1b;//connection accepted
const int accepted = 0x10d;
const int password_wrong = 0x7b;
const int login_successful = 0x8b;
const int version_unsuitable = 0x9b;
const int argument_error = 0x4b;//argument of certain command is wrong!
const int error = 0x5b;//unknown error,detailed information is in return code info
const int busy_refuse = 0x6b;
//datagram category code:
const int set_security = 0x1d;
const int plugin = 0x2d;
const int file_info = 0x3d;
const int direction_info = 0x4d;
const int file_count = 0x5d;
//command return info
const int execute_success = 0x1e;
const int permission_erroe = 0x2e;
const int unknown_error = 0x3e;
struct head_apply {
    int type;
    int version;
    int need_password;
    unsigned int password_hash;
    char user_name[524];
};
struct head_datablock {
    int type;
    int version;
    int session_id;
    int eof;//1 represents this block is end of a file 0 represents this block is not end of file
    int valid_length;
    int check_code;
    int sequence_number;
    char data[512];
};//data block
struct head_return_code {
    int type;
    int version;
    int state;
    int info_count;
    char info1[264];
    char info2[260];
};//server return state
struct command_info {
    int type;
    int version;
    int argc;
    int pos_taker;//no use
    char command[16];//command is no longer than 16 byte
    char **argv;//16 args,each is shorter than 32 byte
};//send command to server
struct datagram {
    int type;
    int version;
    int category;
    char plugin_name[128];
    char arg1[200];
    char arg2[200];
};
struct head_transfering_info { //describe the whole transferring task
    int type;
    int version;
    int operation_count;
    char addition[528];
};
struct head_file_info {
    int type;
    int version;
    int file_name_length;
    unsigned long long file_size;
    char file_name[520];
};

INNER_FUNCTION void loadDatabase(QString file);
INNER_FUNCTION bool checkLogin(QString user,unsigned int pwd);
INNER_FUNCTION int fast_pow(int,int);
INNER_FUNCTION unsigned int hash(char*);
INNER_FUNCTION int check_code(const struct head_datablock&);
INNER_FUNCTION head_return_code* make_return_code(int state, int info_count, char *info1 = nullptr, int size1 = 0, char *info2 = nullptr, int size2 = 0);
INNER_FUNCTION head_transfering_info* make_trans_info(int operation_count);//test version
INNER_FUNCTION head_apply* make_apply_nopass(QString user_name);
INNER_FUNCTION head_apply* make_apply_pass(QString user_name,unsigned int pwdHash);
INNER_FUNCTION int get_version();
INNER_FUNCTION head_file_info* make_file_info(QString filename);
#endif // CORE_H

#include <xlnt/xlnt.hpp>
#include "core.h"
#include <cstring>
#include <QFile>
#include <QFileInfo>
int fast_pow(int base,int pow) {
    int ret = 1;
    while(pow) {
        if(pow&1)ret = ret * base % hash_mod;
        base = base * base % hash_mod;
        pow>>=1;
    }
    return ret;
}
unsigned int hash(char *str) {
    unsigned int len = (int)strlen(str),hash = 0;
    for(int i = 0; i<len; i++) {
        hash = ((hash * hash_base % hash_mod)+str[i]) % hash_mod;
    }
    return hash;
}
int check_code(const struct head_datablock & block) {
    int code = 0;
    for(int i = 0; i<block.valid_length; i++) {
        code+=block.data[i]*i;
        code%=check_code_mod;
    }
    return code;
}
int get_version() {
    return version_beta;
}
head_return_code *make_return_code(int state, int info_count, char *info1, int size1, char *info2, int size2) {
    auto *ret = new head_return_code;//TODO:maybe I should use shared_ptr!!!!!!
    memset(ret,0,sizeof(head_return_code));
    ret->type = type_head_return_code;
    ret->version = version_beta;
    ret->state = state;
    ret->info_count = info_count;
    if (info_count >= 1) {
        memcpy(ret->info1, info1, size1);
    }
    if (info_count == 2) {
        memcpy(ret->info2, info2, size2);
    }
    return ret;
}
head_transfering_info * make_trans_info(int operation_count) {
    auto * ret = new head_transfering_info;
    memset(ret,0,sizeof(head_transfering_info));
    ret->type = type_trans_info;
    ret->version = get_version();
    ret->operation_count = operation_count;
    return ret;
}
head_apply * make_apply_nopass(QString user_name) {
    auto * ret = new head_apply;
    memset(ret,0,sizeof(head_apply));
    ret->type = type_head_apply;
    ret->version = get_version();
    ret->need_password = 0;
    memcpy(ret->user_name,user_name.toLatin1().data(), strlen(user_name.toLatin1().data()));
    return ret;
}
head_apply * make_apply_pass(QString user_name,unsigned int pwdHash) {
    auto * ret = new head_apply;
    memset(ret,0,sizeof(head_apply));
    ret->type = type_head_apply;
    ret->version = get_version();
    ret->need_password = 1;
    memcpy(ret->user_name,user_name.toLatin1().data(), strlen(user_name.toLatin1().data()));
    ret->password_hash = pwdHash;
    return ret;
}
head_file_info * make_file_info(QString filePath) {
    QFile file(filePath);
    QFileInfo inf(filePath);
    QString filename = inf.fileName();
    auto * ret = new head_file_info;
    memset(ret,0,sizeof(head_file_info));
    ret->type = type_head_file_info;
    ret->version = get_version();
    //TODO I haven't considered too-long file name!!!!
    //watch out buffer overflow bug
    memcpy(ret->file_name,filename.toLatin1().data(), strlen(filename.toLatin1().data()));
    ret->file_name_length = strlen(filename.toLatin1().data());
    ret->file_size = file.size();
    return ret;
}
QMap<QString,unsigned int> users;
bool checkLogin(QString user, unsigned int pwd) {
    return users[user]==pwd;
}
void loadDatabase(QString file) {
    xlnt::workbook wb;
    wb.load(file.toStdString());
    auto sheet = wb.active_sheet();
    auto row = sheet.rows();
    int rows = sheet.rows().length();
    for(int i = 0; i<rows; i++) {
        std::string user = row[i][0].value<std::string>();
        unsigned int pwd = row[i][1].value<unsigned int>();
        users[QString::fromStdString(user)] = pwd;
    }
}

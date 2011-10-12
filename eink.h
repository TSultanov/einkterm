#include "common.h"
#include "s1d13521.h"
#include "s1d13521ioctl.h"

void put_glyph(unsigned char ch, int x, int y);
void put_char(char ch);
void put_str(char* str);
void put_buf(char* buf, int n);
int init_s1d13521(void);
int eink_cmd(int cmd, s1d13521_ioctl_cmd_params *params);
int eink_write_reg(int reg, int data);
int eink_read_reg(int reg);
int eink_write_buf(void *buf, int len);
void update_part(void);
void draw_bitmap(void);
void init_eink(void);

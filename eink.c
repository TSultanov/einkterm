#include "eink.h"
#include "font5x8.h"

static int size=800*600/2;
int cursor_y=0, cursor_x=0, blank=0;
char* bitmap;

int console=0;

void put_glyph(unsigned char ch, int x, int y)
{
    char raw_bitmap[5];
    char prep_bitmap[80];
    memcpy(raw_bitmap, (font5x8+ch*5), 5);
    int i,j, pos=0;
    for(i=0; i<5; i++)
    {
        for(j=0; j<8; j++)
        {
            if((raw_bitmap[i]<<j)&0b10000000)
            {
                prep_bitmap[pos] = 0x00;
            }
            else
            {
                prep_bitmap[pos] = 0xff;
            }
            pos++;
        }
    }
    if(blank)
    {
        memset(bitmap, 0xff, 800*600/2);
        blank = 0;
        draw_bitmap();
    }
    //draw_bitmap(bitmap, 800*600/2);
    for(i=0; i<8; i++)
    {
        memcpy(bitmap+x+(y*300)+(i*600), prep_bitmap+i*5, 5);
        memcpy(bitmap+x+(y*300)+(i*600)+300, prep_bitmap+i*5, 5);
    }
}

void put_char(char ch)
{
    if(ch=='\n')
    {
        cursor_y++;
        cursor_x=0;
    }
    else
    {
        put_glyph(ch, cursor_x*5, cursor_y*16);
        cursor_x++;
        if(cursor_x==60){
            cursor_x=0;
            cursor_y++;
        }

    }
    if(cursor_y>=50)
    {
        blank = 1;
        cursor_y=0;
        cursor_x=0;
    }
}

void put_str(char* str)
{
    while(*str)put_char(*str++);
}

void put_buf(char* buf, int n)
{
	int i;
	for(i=0;i<n;i++) put_char(buf[i]);
}

int init_s1d13521(void) 
{ 
    //struct fb_fix_screeninfo finfo; 
    console = open("/dev/fb0", O_RDWR, 0); 

    if(console<0) return 0;
    
    return 1; 
}

int eink_cmd(int cmd, s1d13521_ioctl_cmd_params *params)
{ 
    return ioctl(console, cmd, params); 
}

int eink_write_reg(int reg, int data) 
{ 
    s1d13521_ioctl_cmd_params params; 
    params.param[0]=reg; params.param[1]=data; 
    return eink_cmd(S1D13521_WR_REG,&params); 
}

int eink_read_reg(int reg) 
{ 
    s1d13521_ioctl_cmd_params params; 
    params.param[0]=reg; params.param[1]=0; 
    eink_cmd(S1D13521_RD_REG,&params); 
    return params.param[1]; 
} 
 
int eink_write_buf(void *buf, int len) 
{ 
    struct s1d13521_ioctl_hwc hwc; 
    hwc.buffer=buf; 
    hwc.value=len; 
    return ioctl(console,S1D13521_MEMBURSTWRITE,&hwc); 
} 

void wait_epd(void)
{
	static s1d13521_ioctl_cmd_params cmd_params;
	cmd_params.param[0] = (2 << 8);
	eink_cmd(S1D13521_WAIT_DSPE_TRG,&cmd_params); 
    eink_cmd(S1D13521_WAIT_DSPE_FREND,&cmd_params);
}

void update_part(void)
{
    static s1d13521_ioctl_cmd_params cmd_params;
    cmd_params.param[0] = (0x2 << 4);
    eink_cmd(S1D13521_LD_IMG, &cmd_params);
    eink_write_reg(0x154, 0);
    eink_write_buf(bitmap, size);
    eink_cmd(S1D13521_LD_IMG_END, &cmd_params);
    cmd_params.param[0] = (2 << 8);        // 0=init,1,2,3
    eink_cmd(S1D13521_UPD_PART, &cmd_params);
	wait_epd();
}

void draw_bitmap(void)
{
    static s1d13521_ioctl_cmd_params cmd_params;
    cmd_params.param[0] = (0x2 << 4);
    eink_cmd(S1D13521_LD_IMG, &cmd_params);
    eink_write_reg(0x154, 0);
    eink_write_buf(bitmap, size);
    eink_cmd(S1D13521_LD_IMG_END, &cmd_params);
    cmd_params.param[0] = (3 << 8);        // 0=init,1,2,3
    eink_cmd(S1D13521_UPD_FULL, &cmd_params);
    wait_epd();
}

void init_eink(void)
{
	bitmap = malloc(800*600/2);
    memset(bitmap, 0xff, 800*600/2);
    init_s1d13521();
    
    draw_bitmap();
}

int get_disp(void)
{
	return console;
}

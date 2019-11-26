#include "serial.h"

#define log_error printf

#define WAIT_TIMEOUT    (5 * 60)

#define BAUDRATE B115200 ///Baud rate : 115200
//#define BAUDRATE B460800
#define DEVICE "/dev/ttyUSB0"
//#define DEVICE "/dev/ttyS0"
#define RSIZE 1024

void dump_byte(char *buf, uint32_t len)
{
    int i = 0;
    printf("[0x%08x]: ", i);
    for(; i < len; i++) {

        if (i != 0 && (i % 8 == 0)) {
            printf("\n[0x%08x]: ", i);
        }

        printf("0x%02x ", buf[i]);
    }
    printf("\n");
}

void cfmakeraw(struct termios *termios_p);

int bindex = 0;
char buffer_cache[4096] = {0};

int sfd = 0;

struct termios st_new;
struct termios st_old;


/* block send */
int32_t serial_send(void *buf, uint32_t len)
{


    if( (fcntl(sfd, F_SETFL, 0)) < 0 )
    {
        log_error("fcntl F_SETFL fail!\n");
        return -1;
    }

#if 0
    uint32_t i;
    log_debug("%s:\n", __func__);
    for(i = 0; i < len; i++) {
        log_debug("[%d]: %02x \n", i, ((u8 *)buf)[i]);
        //log_debug("[%d]: %x [%c]\n", i, ((u8 *)buf)[i], ((u8 *)buf)[i]);
    }
#endif

    return write(sfd, buf, len);
}

int32_t serial_recv(void *buf, uint32_t len, uint32_t timeout_ms)
{
    fd_set rfds;  
    struct timeval timeout;

    memset(buf, 0, len);
    timeout.tv_sec  = 0;
    timeout.tv_usec = timeout_ms * 1000;

    FD_ZERO(&rfds);

    FD_SET(sfd, &rfds);

    switch(select(sfd + 1, &rfds, NULL, NULL, &timeout)) {
        case -1:
            log_error("fail: select error!\n");
            return -2;
            break;
        case 0: /* timeout */
            log_error("select timeout!\n");
            log_error("wait serial recv timeout.\n");
            return -1;
            break;
        default:
            //printf("select succ\n");
            break;
    }


    usleep(100 * 1000); /* wait 10ms for driver to recv the total data */

    return read(sfd, buf, len);
}

//open port & set port
int serial_init()
{
    log_error("open serial device [%s]\n", DEVICE);

    sfd = open(DEVICE, O_RDWR|O_NOCTTY|O_NDELAY);
    if(-1 == sfd)
    {
        perror("open serial port fail!\n");
        return -1;
    }

    if( (fcntl(sfd, F_SETFL, O_NONBLOCK)) < 0 )
    {
        perror("fcntl F_SETFL fail!\n");
        return -1;
    }

    if(tcgetattr(sfd, &st_old) != 0)
    {
        perror("tcgetattr fail!\n");
        return -1;
    }

    st_new = st_old;
    cfmakeraw(&st_new);//将终端设置为原始模式，该模式下所有的输入数据以字节为单位被处理

    //set speed
    cfsetispeed(&st_new, BAUDRATE);//115200
    cfsetospeed(&st_new, BAUDRATE);

    //set databits
    st_new.c_cflag |= (CLOCAL|CREAD);
    st_new.c_cflag &= ~CSIZE;
    st_new.c_cflag |= CS8;

    //set parity
    st_new.c_cflag &= ~PARENB;
    st_new.c_iflag &= ~INPCK;

    //set stopbits
    st_new.c_cflag &= ~CSTOPB;
    st_new.c_cc[VTIME]=0;    //指定所要读取字符的最小数量
    st_new.c_cc[VMIN]=1;     //指定读取第一个字符的等待时间，时间的单位为n*100ms
                            //如果设置VTIME=0，则无字符输入时read（）操作无限期的阻塞
    tcflush(sfd,TCIFLUSH);  //清空终端未完成的输入/输出请求及数据。
    if( tcsetattr(sfd,TCSANOW,&st_new) != 0 )
    {
        perror("tcsetattr fail!\n");
        return -1;
    }

    return 0;
}

#if 0
int main(int argc, char **argv)
{
    int nret = 0;
    int count = 0;
    char recv_buf[RSIZE];
    struct tlv *ptlv;

    int i;
    int ifd;
    struct stat st;
    u8 *mbuf;

    uint32_t crc0, crc1;

    if(serialinit() == -1)
    {
        error("serialinit fail!\n");
    }

    memset(recv_buf, 0, RSIZE);

    printf("sfd: %d\n", sfd);

    if (argv[1] == NULL) {
        printf("usage: %s xxx.gcode.bin\n", argv[0]);
    }

    if ((ifd = open(argv[1], O_RDWR)) == -1) {
        perror("open");
        exit(-1);
    }

    if ((fstat(ifd, &st)) == -1) {
        perror("fstat");
        exit(-1);
    }

    if (st.st_size >= (16 * 1024)) {

    }

    if ((mbuf = mmap(0, st.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, ifd, 0)) == MAP_FAILED) {
        perror("mmap");
        exit(-1);
    }

    printf("serial_send start...\n");

    nret = serial_send(mbuf, st.st_size);

    printf("send %d bytes\n", nret);

    printf("serial_recv start...\n");

    ptlv = (struct tlv *)recv_buf;
    nret = serial_recv(recv_buf, 8);   /* FIXME: hardcode */

    dump_byte(recv_buf, nret);

    if (nret != 8) {
        error("fail: serial_recv %d bytes, expect %d bytes\n", nret, sizeof(ptlv->type) + sizeof(ptlv->len));
    }

    if (ptlv->len % 4 != 0) {
        error("len must align by 4 bytes!\n");
    }

    count = ptlv->len / 4;

    for(i = 0; i < count; i++) {
        if ((nret = serial_recv(&recv_buf[8 + i * 4], 4)) != 4) {
            error("fail: [%d] serial_recv %d bytes, expect %d bytes\n", i, nret, 4);
        }
    }

    /* read the last word: crc sum*/
    if ((nret = serial_recv(&recv_buf[8 + i * 4], 4)) != 4) {
        error("fail: [%d] serial_recv %d bytes, expect %d bytes\n", i, nret, 4);
    }

    crc0 = readl(&recv_buf[8 + i * 4]);
    crc1 = crc32(recv_buf, 8 + i * 4);

    if (crc0 != crc1) {
        printf("crc0: 0x%08x\n", crc0);
        printf("crc1: 0x%08x\n", crc1);
        error("crc check fail!\n");
    }



    if (ptlv->type == TC_RESP_SUCC) {
        printf("cmd exec succ!\n");
    } else {
        error("cmd exec fail!\n");
    }

#if 0   /* read */
    while(1)
    {
        nret = read(sfd, recv_buf, RSIZE);
        if(-1 == nret)
        {
            perror("read data error!\n");
            break;
        }
        if(0 < nret)
        {
            buf[nret] = 0;
            printf("recv data: [%s]\n", buf);
        }
    }
#endif


    //nret = write(sfd, "hello, world!\r\n", 14);

    close(sfd);
    return 0;

}
#endif

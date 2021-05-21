#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h> 
#include <fcntl.h>
#include <math.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>


int fd;
int addr_max;


int i2c_inits(int8_t mode, char* dev);
int i2c_close(void);
int i2c_write(int16_t addr, int8_t len, unsigned char *data);
int i2c_read(int16_t addr, int8_t len, unsigned char *data);


int addr_check(int16_t addr){
    if(addr >= 0 && addr <= addr_max){
        return(0);
    }
    else{
        return(-1);
    }
}


int i2c_inits(int8_t mode, char* dev){
    if(fd == 0){
        if(mode == 7 || mode == 10){
            fd = open(dev, O_RDWR);
            addr_max = ((int)pow(2, mode)) - 1;
            return(0);
        }
        else{
            return(-1);
        }
    }
    else{
        return(-2);
    }
}


int i2c_close(void){
    if(fd != 0){
        int stat = close(fd);
        fd = 0;
        return(stat);
    }
    return(-2);
}


int i2c_write(int16_t addr, int8_t len, unsigned char *data){
    struct i2c_msg msg[1];
    struct i2c_rdwr_ioctl_data packets;
    int ret;
    msg[0].addr = addr; //* addrは16bit幅 */
    msg[0].flags = 0; //* read、writeやアドレス長の指定に利用 */
    msg[0].len = len; //* bufに指定するdataのサイズ */
    msg[0].buf = data;

    packets.msgs = msg;
    packets.nmsgs = 1; //* msgのサイズ指定 */
    ret = ioctl(fd, I2C_RDWR, &packets);

    return(ret);
}


int i2c_read(int16_t addr, int8_t len, unsigned char *data){
    if(!addr_check(addr)){
        struct i2c_msg msg[1];
        struct i2c_rdwr_ioctl_data packets;
        int ret;
        msg[0].addr = addr; //* addrは16bit幅 */
        msg[0].flags = I2C_M_RD; /* read、writeやアドレス長の指定に利用 */
        msg[0].len = len; //* bufに指定するdataのサイズ */
        msg[0].buf = data;

        packets.msgs = msg;
        packets.nmsgs = 1; //* msgのサイズ指定 */
        ret = ioctl(fd, I2C_RDWR, &packets);

        return(ret);
    }
    else{
        return(-1);
    }
}


int main(int argc, char **argv) {
    i2c_inits(7, "/dev/i2c-1");
    unsigned char datas[3] = {1, 2, 2};
    int stat = 0;
    stat = i2c_write(8, 3, datas);
    stat = i2c_read(8, 3, datas);
    printf("%d\n", stat);
    for (int i = 0; i < 3; i++){
        printf("%d ", datas[i]);
    }
    printf("\n");
    
    i2c_close();
       
}
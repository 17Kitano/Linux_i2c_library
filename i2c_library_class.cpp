#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <string>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>





class i2c_fanc{
public:
    i2c_fanc(int8_t mode, std::string dev){
        if(this -> fd == 0){
            if(mode == 7 || mode == 10){
                this -> fd = open(dev.c_str(), O_RDWR);
                this -> addr_max = ((int)pow(2, mode)) - 1;
            }
        }
    }


    ~i2c_fanc(void){
        if(this -> fd != 0){
            int stat = close(this -> fd);
            this -> fd = 0;
        }
    }

private:
    int addr_check(int16_t addr){
        if(addr >= 0 && addr <= this -> addr_max){
            return(0);
        }
        else{
            return(-1);
        }
    }


public:
    int i2c_write(int16_t addr, int8_t len, unsigned char *data){
        if(!addr_check(addr)){
            struct i2c_msg msg[1];
            struct i2c_rdwr_ioctl_data packets;
            int ret;
            msg[0].addr = addr; //* addrは16bit幅 */
            msg[0].flags = 0; //* read、writeやアドレス長の指定に利用 */
            msg[0].len = len; //* bufに指定するdataのサイズ */
            msg[0].buf = data;

            packets.msgs = msg;
            packets.nmsgs = 1; //* msgのサイズ指定 */
            ret = ioctl(this -> fd, I2C_RDWR, &packets);

            return(ret);
        }
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
            ret = ioctl(this -> fd, I2C_RDWR, &packets);

            return(ret);
        }
        else{
            return(-1);
        }
    }

    int fd = 0;
    int addr_max = 0;

};

int main(int argc, char **argv) {
    i2c_fanc* i2c = new i2c_fanc(7, "/dev/i2c-1");
    unsigned char datas[3] = {2, 3, 2};
    int stat = 0;
    stat = i2c -> i2c_write(8, 3, datas);
    stat += i2c -> i2c_read(8, 3, datas);
    printf("%d\n", stat);
    for (int i = 0; i < 3; i++){
        printf("%d ", datas[i]);
    }
    printf("\n");
    
    //i2c_close();
    delete i2c;
       
}
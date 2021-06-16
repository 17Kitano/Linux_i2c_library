#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include <string>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <boost/python.hpp>


class i2c_io_error : std::exception {
    public:
    char const* my_message = "";

    i2c_io_error(char const* message) {
        my_message = message;
    }
    char const* what() { 
        return my_message;
    }
};


void translate_i2c_io_error(i2c_io_error e) {
    PyErr_SetString(PyExc_RuntimeError, e.what() );
}


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
    int i2c_write(int16_t addr, int8_t len, boost::python::list data){
        if(!addr_check(addr)){
            unsigned char data1[32] = {0};
            for(int i = 0; i < len || i > 32; i++){
                data1[i] = boost::python::extract<unsigned char>(data[i]);
            }
            struct i2c_msg msg[1];
            struct i2c_rdwr_ioctl_data packets;
            int ret;
            msg[0].addr = addr; //* addrは16bit幅 */
            msg[0].flags = 0; //* read、writeやアドレス長の指定に利用 */
            msg[0].len = len; //* bufに指定するdataのサイズ */
            msg[0].buf = data1;

            packets.msgs = msg;
            packets.nmsgs = 1; //* msgのサイズ指定 */
            ret = ioctl(this -> fd, I2C_RDWR, &packets);
            if(ret != 0){
                throw i2c_io_error("i2c io error");
            }
            return(ret);
        }
        throw i2c_io_error("i2c address error");
        return(-2);
    }

    boost::python::list i2c_read(int16_t addr, int8_t len){
        boost::python::list data2;
        if(!addr_check(addr)){
            struct i2c_msg msg[1];
            struct i2c_rdwr_ioctl_data packets;
            //unsigned char *data;
            unsigned char data[32] = {0};
            int ret;
            msg[0].addr = addr; //* addrは16bit幅 */
            msg[0].flags = I2C_M_RD; /* read、writeやアドレス長の指定に利用 */
            msg[0].len = len; //* bufに指定するdataのサイズ */
            msg[0].buf = data;
            packets.msgs = msg;
            packets.nmsgs = 1; //* msgのサイズ指定 */
            ret = ioctl(this -> fd, I2C_RDWR, &packets);

            if(ret == 1){
                for(int i = 0; i < len || i > 32; i++){
                    data2.append(data[i]);
                }
            }
            else{
                //data2.append(-1);
                throw i2c_io_error("i2c io error");
            }
        }
        else{
            //data2.append(-1);
            throw i2c_io_error("i2c address error");
        }
        
        return(data2);
    }

    int fd = 0;
    int addr_max = 0;
};


BOOST_PYTHON_MODULE(i2c_fancs)
{
    using namespace boost::python;

    class_<i2c_fanc>("i2c_fanc", init<int8_t, std::string>())
        .def(init<int8_t, std::string>())
        .def("i2c_write", &i2c_fanc::i2c_write)
        .def("i2c_read", &i2c_fanc::i2c_read)
        ;

    boost::python::register_exception_translator<i2c_io_error>(&translate_i2c_io_error);
    
}


/*
コンパイルコマンド
g++ -I /usr/include/python3.7m -DPIC -shared -fPIC -o i2c_fancs.so i2c_library_class_py.cpp -lboost_python3
*/



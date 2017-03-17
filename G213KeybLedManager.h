#ifndef DEF_G213KEYBLEDMANAGER
#define DEF_G213KEYBLEDMANAGER

#include <libusb-1.0/libusb.h>

#define LOGITECH_VENDOR_ID (uint16_t)0x046d
#define LOGITECH_PRODUCT_ID (uint16_t)0xc336

class G213KeybLedManager {

  public:
    
    bool isAttached();
    bool commit();
    bool attach();
    void detach();
    void g213SetColor(char *hexColor,short field);
    void g213SetBreathe(char *hexColor,long timeInMills);
    void g213SetCycle(long timeInMills);

  private:
    libusb_device_handle *g213Handle = NULL;
    libusb_context *ctx = NULL;
    libusb_device_handle * g213GetDeviceHandle();
    void g213SendData(unsigned char *data,int16_t len);    
    int hexDigitToInt(char digit);
    unsigned char *decodeHexString(const char *hexStr,unsigned char *hexBin);
};

#endif

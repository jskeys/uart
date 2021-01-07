#include "ftd2xx.h"
#include <stdio.h>

int main() 
{
    unsigned int num_devices;
    FT_DEVICE_LIST_INFO_NODE info_node;
    FT_STATUS ft_status;
    FT_HANDLE ft_handle;

    // variables to hold device info detail
    unsigned int flags;
    unsigned int type;
    unsigned int id;
    unsigned int loc_id;
    char serial_number[16];
    char description[64];

    // Hold status
    unsigned int rx_bytes = 0;
    unsigned int tx_bytes = 0;
    unsigned int event_status = 0;

    // Determine the number of FTD devices
    ft_status = FT_CreateDeviceInfoList(&num_devices);
    if (ft_status)
    {
        printf("Error creating device info list.\n");
        return ft_status;
    }
    printf("Detected %d devices.\n", num_devices);

    ft_status = FT_GetDeviceInfoList(&info_node, &num_devices);
    if (ft_status)
    {
        printf("Error getting device info list.\n");
        return ft_status;
    }

    // Attempt to open device index 0 
    ft_status = FT_GetDeviceInfoDetail(0, &flags, &type, &id, &loc_id, serial_number, description, &ft_handle);
    if (ft_status)
    {
        printf("Error getting device info detail.\n");
        return ft_status;
    }

    printf("Location ID: %d\n", loc_id);
    printf("Serial Number: %s\n", serial_number);
    printf("Description: %s\n", description);

    ft_status = FT_OpenEx((void*)serial_number, FT_OPEN_BY_SERIAL_NUMBER, &ft_handle);
    if (ft_status)
    {
        printf("Error %ls opening device.\n", &ft_status);
        return ft_status;
    }

    printf("Handle: %p\n", ft_handle);

    ft_status = FT_GetStatus(ft_handle, &rx_bytes, &tx_bytes, &event_status);
    if (ft_status)
    {
        printf("Error %d\n", ft_status); 
        return ft_status;
    }
    printf("RX Bytes: %d\n", rx_bytes);
    printf("TX Bytes: %d\n", tx_bytes);

    // Write a byte and see if it ends up in the buffer.
    unsigned int num_to_write = 256;
    unsigned int num_written;
    char wt_bytes[256];

    for (int i = 0; i < 256; i++)
    {
        wt_bytes[i] = (char)i;
    }

    ft_status = FT_Write(ft_handle, wt_bytes, num_to_write, &num_written);
    if (ft_status)
    {
        printf("Error %d\n", ft_status); 
        return ft_status;
    }
    printf("Wrote %d bytes.\n", num_written);

    do  
    {
        ft_status = FT_GetStatus(ft_handle, &rx_bytes, &tx_bytes, &event_status);
        if (ft_status)
        {
            printf("Error %d\n", ft_status); 
            return ft_status;
        }
    }
    while (rx_bytes < num_written);

    printf("RX Bytes: %d\n", rx_bytes);
    printf("TX Bytes: %d\n", tx_bytes);

    unsigned int num_to_read = 256;
    unsigned int num_read;
    char rd_bytes[num_to_read + 1];
    rd_bytes[num_to_read] = '\0';

    ft_status = FT_Read(ft_handle, rd_bytes, num_to_read, &num_read);
    if (ft_status)
    {
        printf("Error %d\n", ft_status); 
        return ft_status;
    }

    printf("Read %d bytes\n", num_read);
    printf("Read %s\n", rd_bytes);

    ft_status = FT_Close(&ft_handle);

    return 0;
}

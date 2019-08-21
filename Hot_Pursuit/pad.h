#include<cell/pad.h>
#include <cell/sysmodule.h>
#define MAX_PAD   									(1)
void PadRead(uint32_t* pbutton1, uint32_t* pbutton2)
{
	int			i;
	int			ret;
	CellPadInfo2 pad_info;
	static uint32_t old_pad_info=0;
	CellPadData pad_data;
	uint32_t	button1 = 0;
	uint32_t	button2 = 0;

	ret = cellPadGetInfo2(&pad_info);
	if (ret != CELL_OK){
		//printf("cellPadGetInfo2() error (0x%x).\n", ret);
		return;
	}

     /*E Check info field for monitoring the INTERCEPTED state. */
    if((pad_info.system_info & CELL_PAD_INFO_INTERCEPTED) && 
       (!(old_pad_info & CELL_PAD_INFO_INTERCEPTED))) 
    {
        //printf ("This program lost the ownership of the game pad data\n");
        old_pad_info = pad_info.system_info;
    }
    else if((!(pad_info.system_info & CELL_PAD_INFO_INTERCEPTED)) &&
            (old_pad_info & CELL_PAD_INFO_INTERCEPTED))
    {
       // printf ("This program got the ownership of the game pad data\n");
        old_pad_info = pad_info.system_info;
    }

	for (i = 0; i < MAX_PAD; i ++)
	{
		if (pad_info.port_status[i] & CELL_PAD_STATUS_CONNECTED == 0)
			continue;

		ret = cellPadGetData(i, &pad_data);
		if (ret != CELL_PAD_OK || pad_data.len == 0)
			continue;

		button1 = pad_data.button [CELL_PAD_BTN_OFFSET_DIGITAL1];
		button2 = pad_data.button [CELL_PAD_BTN_OFFSET_DIGITAL2];
	}
	*pbutton1 = button1;
	*pbutton2 = button2;
	return;
}

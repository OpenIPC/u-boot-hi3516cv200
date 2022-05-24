#ifndef __JPEG_HDEC_RWREG_H__
#define __JPEG_HDEC_RWREG_H__


/*********************************add include here******************************/
#include  "hi_type.h"


/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C" 
{
#endif
#endif /* __cplusplus */


    /***************************** Macro Definition ******************************/
   	/*************************** Structure Definition ****************************/
 
    /******************************* API declaration *****************************/

	/** \addtogroup      JPEG RWREG API */
	 
	HI_VOID JPEG_HDEC_WriteReg(volatile HI_CHAR *pJpegRegVirAddr, const HI_S32 s32PhyOff, const HI_S32 s32Val);
	HI_S32 JPEG_HDEC_ReadReg(const volatile HI_CHAR *pJpegRegVirAddr, const HI_S32 s32PhyOff);

	HI_VOID JPEG_HDEC_CpyData2Reg(volatile HI_CHAR *pJpegRegVirAddr,const HI_VOID *pInMem,const HI_S32 s32PhyOff,const HI_U32 u32Bytes);


	HI_S32 JPEG_HDEC_CpyData2Buf(const volatile HI_CHAR *pJpegRegVirAddr,const HI_S32 s32PhyOff,const HI_U32 u32Bytes,HI_VOID *pOutMem);

	
    /****************************************************************************/



#ifdef __cplusplus
    
#if __cplusplus
   
}
#endif
#endif /* __cplusplus */

#endif /* __JPEG_HDEC_RWREG_H__*/

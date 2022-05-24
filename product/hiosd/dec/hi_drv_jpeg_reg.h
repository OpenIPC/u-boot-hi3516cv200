#ifndef __HI_DRV_JPEG_REG_H__
#define __HI_DRV_JPEG_REG_H__


/*********************************add include here******************************/
#include "hi_type.h"
#include <common.h>
/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C" 
{
#endif
#endif /* __cplusplus */


#define HARD_DEC

    /***************************** Macro Definition ******************************/
        /***************************** Macro Definition ******************************/
#ifdef CONFIG_HI3536
	#define JPGD_IRQ_NUM				      (71)
	#define JPGD_REG_BASEADDR			      (0x13060000)
	#define JPGD_CRG_REG_PHYADDR		      (0x12040064)
#elif (defined(CONFIG_HI3536C))
	#define JPGD_IRQ_NUM				      (66)
	#define JPGD_REG_BASEADDR			      (0x13070000)
	#define JPGD_CRG_REG_PHYADDR		      (0x120400d4)
#elif defined CONFIG_HI3536DV100
    #define JPGD_IRQ_NUM				      (63)
	#define JPGD_REG_BASEADDR			      (0x13110000)
	#define JPGD_CRG_REG_PHYADDR		      (0x120400a0)
#elif defined CONFIG_HI3535
    #define JPGD_IRQ_NUM				      (70)
	#define JPGD_REG_BASEADDR			      (0x20670000)
	#define JPGD_CRG_REG_PHYADDR		      (0x20030064)
#elif defined CONFIG_HI3521A
       #define JPGD_IRQ_NUM				      (66)
	#define JPGD_REG_BASEADDR			      (0x13070000)
	#define JPGD_CRG_REG_PHYADDR		      (0x12040054)
#elif defined CONFIG_HI3521D
       #define JPGD_IRQ_NUM				      (66)
	#define JPGD_REG_BASEADDR			      (0x13070000)
	#define JPGD_CRG_REG_PHYADDR		      (0x120400d4)
#elif ((defined CONFIG_HI3531A) || (defined CONFIG_HI3531D))
    #define JPGD_IRQ_NUM				      (66)
	#define JPGD_REG_BASEADDR			      (0x13070000)
	#define JPGD_CRG_REG_PHYADDR		      (0x120400d4)
#elif defined(CONFIG_HI3531) || defined(CONFIG_HI3532) || defined(CONFIG_HI3521) || defined(CONFIG_HI3520A)
    #define JPGD_IRQ_NUM				      (95)
	#define JPGD_REG_BASEADDR			      (0x10170000)
	#define JPGD_CRG_REG_PHYADDR		      (0x20030064)
#elif defined CONFIG_HI3520D
    #define JPGD_IRQ_NUM				      (74)
	#define JPGD_REG_BASEADDR			      (0x20670000)
	#define JPGD_CRG_REG_PHYADDR		      (0x20030064)
#endif

	#define JPGD_CLOCK_SELECT			      0x000 			  
	#define JPGD_CLOCK_ON				      0x2				 
	#define JPGD_CLOCK_OFF				      0xFFFFFFFD		
	#define JPGD_RESET_REG_VALUE 		      0x1				  
	#define JPGD_UNRESET_REG_VALUE		      0xFFFFFFFE		 

	#ifdef CONFIG_JPEG_TEST_CHIP_PRESS
	#define JPGD_REG_LENGTH                      0xFF30
	#else
	/** the length of register */
	#define JPGD_REG_LENGTH					   0x6BF			 /** <64K  **/
	#endif
	
	#define JPGD_CRG_REG_LENGTH				   0x4

	/** JPEG register that decoding start */
	#define JPGD_REG_START						 	 0x0
	/** JPEG continue stream register */
	#define JPGD_REG_RESUME 						 0x4
	/** the pic_vld_num register */
	#define JPGD_REG_PICVLDNUM					 	 0x8
	/** VHB stride register */
	#define JPGD_REG_STRIDE 						 0xC
	/** picture width register */
	#define JPGD_REG_PICSIZE						 0x10
	/** picture type register */
	#define JPGD_REG_PICTYPE						 0x14
	/** picture decode time cost */
	#define JPGD_REG_TIME                           0x18
	/** stream buffer start register */
	#define JPGD_REG_STADDR 						 0x20
	/** stream buffer end register */
	#define JPGD_REG_ENDADDR						 0x24
	/** stream start address and end address must in hard start and end address */
	#define JPGD_REG_STADD						 	 0x28
	/** stream save end register */
	#define JPGD_REG_ENDADD 						 0x2C
	/** luminance address register */
	#define JPGD_REG_YSTADDR						 0x30
	/** chrominance address register */
	#define JPGD_REG_UVSTADDR						 0x34
	/** scale register */
	#define JPGD_REG_SCALE						 	 0x40
	
	#define JPGD_REG_OUTTYPE						 0x44
	
	/** halt status register */
	#define JPGD_REG_INT							 0x100
	/** halt shield register */
	#define JPGD_REG_INTMASK						 0x104
	/** debug register */
	#define JPGD_REG_DEBUG						     0x108

	/** save lu pixle data register */
	#define JPGD_REG_LPIXSUM0						 0x114
	/** save lu pixle data register */
	#define JPGD_REG_LPIXSUM1						 0x118

	#define JPGD_REG_SAMPLINGFACTOR			     0x124

	#define JPGD_REG_DRI			     0x128

	/** dqt set register */
	#define JPGD_REG_QUANT						     0x200
	/** Huffman  set register */
	#define JPGD_REG_HDCTABLE						 0x300
	/** Huffman AC mincode memory register */
	#define JPGD_REG_HACMINTABLE					 0x340
	/** Huffman AC base memory register */
	#define JPGD_REG_HACBASETABLE				     0x360
	/** Huffman AC symbol memory register */
	#define JPGD_REG_HACSYMTABLE					 0x400
	
    /******************************* API declaration *****************************/
	/*****************************************************************************
	* func			  : JPGDRV_READ_REG
	* description	  : read register value
	* param[in] 	  : base
	* param[in] 	  : offset
	* retval		  : none
	* output		  : none
	* others:		  : nothing
	*****************************************************************************/
	HI_U32 JPGDRV_READ_REG(HI_U32 base,HI_U32 offset);
	/*****************************************************************************
	* func			  : JPGDRV_WRITE_REG
	* description	  : write register value
	* param[in] 	  : base
	* param[in] 	  : offset
	* param[in] 	  : value
	* retval		  : none
	* output		  : none
	* others:		  : nothing
	*****************************************************************************/
	HI_VOID  JPGDRV_WRITE_REG(HI_U32 base, HI_U32 offset, HI_U32 value);


	typedef unsigned int size_t;
	
	#define DCTSIZE2	    64	/* DCTSIZE squared; # of elements in a block */

	
		
	typedef struct tag_JQUANT_TBL{
	  /* This array gives the coefficient quantizers in natural array order
	   * (not the zigzag order in which they are stored in a JPEG DQT marker).
	   * CAUTION: IJG versions prior to v6a kept this array in zigzag order.
	   */
	  UINT16 quantval[DCTSIZE2];	/* quantization step for each coefficient */
	  /* This field is used only during compression.  It's initialized FALSE when
	   * the table is created, and set TRUE when it's been output to the file.
	   * You could suppress output of a table by setting this to TRUE.
	   * (See jpeg_suppress_tables for an example.)
	   */
	} JQUANT_TBL;
	
	
	/* Huffman coding tables. */
	
	typedef struct tag_JHUFF_TBL{
	  /* These two fields directly represent the contents of a JPEG DHT marker */
	  UINT8 bits[17];		/* bits[k] = # of symbols with codes of */
					/* length k bits; bits[0] is unused */
	  UINT8 huffval[256];		/* The symbols, in order of incr code length */
	  /* This field is used only during compression.  It's initialized FALSE when
	   * the table is created, and set TRUE when it's been output to the file.
	   * You could suppress output of a table by setting this to TRUE.
	   * (See jpeg_suppress_tables for an example.)
	   */
	} JHUFF_TBL;


#ifdef __cplusplus
    
#if __cplusplus
   
}
#endif
#endif /* __cplusplus */

#endif /* __HI_DRV_JPEG_REG_H__ */

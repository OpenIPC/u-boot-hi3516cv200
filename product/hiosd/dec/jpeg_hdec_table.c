/*********************************add include here******************************/
#include "jpeg_hdec_rwreg.h"
#include "hi_drv_jpeg_reg.h"
/***************************** Macro Definition ******************************/

/** luminance */
#define  LU         0
/** chrominance */
#define  CH         1
/** luminance and chrominance */
#define  MAX_TAB    2

/** swap the two value */
#define CAI_JPEG_SWAP(a,b) do{ a=a+b; b=a-b; a=a-b; } while(0)

/******************************* API realization *****************************/


HI_VOID	JPEG_HDEC_Uboot_SetDqt(int *YQtTable,int *UQtTable,int *VQtTable)
{

		int  *QCr = NULL;
		int  *QCb = NULL;
		int  *QY  = NULL;
		HI_S32 s32Cnt = 0;
		HI_S32 Q[DCTSIZE2];
		{
			QY  = YQtTable;
			QCb = UQtTable== NULL? QY  : UQtTable;
			QCr = VQtTable== NULL? QCb : VQtTable;
		}

		/** translate to YCbCr format */
		for(s32Cnt = 0; s32Cnt < DCTSIZE2; s32Cnt++)
		{
			Q[s32Cnt] = QY[s32Cnt] + (QCb[s32Cnt]<<8) + (QCr[s32Cnt]<<16);
		}

		/** set dqt */
		JPEG_HDEC_CpyData2Reg((HI_CHAR *)JPGD_REG_BASEADDR, (HI_VOID *)Q, JPGD_REG_QUANT, sizeof(Q));


}


static HI_U32 JPEG_HDEC_DecHuff(const JHUFF_TBL *huff_tbl, unsigned int bit[256])
{


	      HI_U32 idx     = 0;
		  HI_U32 cnt     = 0;
		  HI_U32 loc     = 0;
		  HI_U32 value   = 0;
		  HI_U32 max_idx = 0;

	      /** initial to zero */
		  memset(bit,0,sizeof(bit[0]));


	      for(idx = 1; idx < 17; idx++)
	      {
			   /** calc the none zero  */
	           if(0 != huff_tbl->bits[idx])
	           {
	                  /** remember the temproary max index **/
					  max_idx = idx;
	                  for(cnt = huff_tbl->bits[max_idx]; cnt>0; cnt--)
	                  {
	                      bit[loc] = value;
						  loc++;
						  value++;
	                  }
	           }
	           value<<=1;
	          
	      }
		  
	      return max_idx;
		  
}


HI_VOID JPEG_HDEC_Uboot_SetDC(JHUFF_TBL * dat)
{	
		/** 2 Huff tables(DC/AC): 0--luminance,1--chrominance */
		
		HI_U32 cnt		  = 0;
		HI_U32 u32Index   = 0;
		HI_U32 sum_syms    =0;
		HI_U32 syms 	  = 0;
		HI_U32 temp 	  = 0;
		HI_U32 index1	  = 0;

		/** dht table,256 is limit value */
		HI_U32 huffcode[MAX_TAB][256]  = {{0}};
		HI_U32 dc_hufflen[MAX_TAB][12] = {{0}};
		HI_U32 dc_sym[MAX_TAB][12]	    = {{0}};
		HI_U32 max_idx[MAX_TAB] 	    = {0};
		HI_U32 hdc_tab[12]             = {0};
		
		JHUFF_TBL huff_ptr[MAX_TAB];

		{
			/** luminance DC */
			memcpy(&(huff_ptr[LU]), &dat[0], sizeof(huff_ptr[LU]));
			/** chrominance DC */
            if(NULL == &dat[1])
            {
            	memcpy(&(huff_ptr[CH]),&dat[0],sizeof(huff_ptr[LU]));
            }
            else
			{
				memcpy(&(huff_ptr[CH]),&dat[1],sizeof(huff_ptr[LU]));
			}
		}
		/**
		** calculate the largest value of iHuffsize, here we can optimize
		** the all zero of iBits[] that after Ln
		**/
		max_idx[LU] = JPEG_HDEC_DecHuff(&huff_ptr[LU],huffcode[LU]);
		/** calculate the largest value of iHuffsize for chrominance **/
		max_idx[CH] = JPEG_HDEC_DecHuff(&huff_ptr[CH],huffcode[CH]);
		
		/** Store huff length and huff value number **/

		/** two, one is luminance, the other is chrominance */
		for(cnt = 0; cnt < MAX_TAB; cnt++)
		{

			temp = 0;
			sum_syms = 0;
			/** the largest value of iHuffsize**/
			for(u32Index = 0; u32Index < max_idx[cnt]; u32Index++)
			{
				syms = huff_ptr[cnt].bits[u32Index+1];
				if(syms)
				{
					/** calculate the number of node **/
					sum_syms +=syms; 
					while(syms--)
					{
						/** calclulate the value of iHuffsize **/
						dc_hufflen[cnt][temp] = u32Index + 1;
						/** calculate iHuffVal[] **/
						
						dc_sym[cnt][temp] = huff_ptr[cnt].huffval[temp];
						
						temp++;
					}
				}
				
			}

			/** sort */
			/** from less to large about iHuffVal[] **/
			for(u32Index = 0; u32Index < sum_syms; u32Index++)
			{
				for(index1=u32Index+1;index1<sum_syms;index1++)
				{
					if(dc_sym[cnt][u32Index]>dc_sym[cnt][index1])
					{
						CAI_JPEG_SWAP(dc_sym[cnt][u32Index],dc_sym[cnt][index1]);
						CAI_JPEG_SWAP(dc_hufflen[cnt][u32Index],dc_hufflen[cnt][index1]);
						CAI_JPEG_SWAP(huffcode[cnt][u32Index],huffcode[cnt][index1]);
					}
				}

			}
			
		  /** Generate DC table **/
		  /** this table make up with iHuffVal[],iHuffSize,iHuffcode **/
		  if(0 == cnt)
		  {/** LU **/
				for(u32Index = 0;u32Index < sum_syms; u32Index++)
				{

					if(dc_sym[0][u32Index] >= 12)
					{
						//ERREXIT(cinfo, JERR_BAD_HUFF_TABLE); /*lint !e740 ignore by y00181162, because this is needed */  
					}
					*(hdc_tab + dc_sym[0][u32Index]) = \
							  ( (*(hdc_tab + dc_sym[0][u32Index])) & 0xfffff000 ) \
							| ( (dc_hufflen[0][u32Index] & 0xf)<<8 ) \
							| ( (huffcode[0][u32Index] & 0xff) );
				}

		  }
		  else
		  {/** CH **/
		  
				for(u32Index = 0;u32Index < sum_syms; u32Index++)
				{
						if(dc_sym[cnt][u32Index] >= 12)
						{
							//ERREXIT(cinfo, JERR_BAD_HUFF_TABLE); /*lint !e740 ignore by y00181162, because this is needed */  
						}
						*(hdc_tab + dc_sym[cnt][u32Index]) = \
							  ( (*(hdc_tab + dc_sym[1][u32Index])) & 0xff000fff ) \
							| ( (dc_hufflen[1][u32Index] & 0xf)<<20 ) \
							| ( (huffcode[1][u32Index] & 0xff)<<12 );
				}

		  }

		}
		/** Set HW DC table **/
		JPEG_HDEC_CpyData2Reg((HI_CHAR *)JPGD_REG_BASEADDR,hdc_tab,JPGD_REG_HDCTABLE,48);

}

HI_VOID JPEG_HDEC_Uboot_SetAC(JHUFF_TBL * dat)
{
            /** 2 Huff tables(DC/AC): 0--luminance,1--chrominance */
            HI_U32 cnt        = 0;
            HI_U32 u32Index   = 0;
            HI_U32 pre_index  = 0;
            HI_U32 sum_syms    =0;
            HI_U32 syms       = 0;
            
            /** dht table,256 is limit value */
            HI_U32 huffcode[MAX_TAB][256]  = {{0}};
            HI_U32 min_tab[MAX_TAB][16]    = {{0}};
            HI_U32 base_tab[MAX_TAB][16]   = {{0}};
            HI_U32 max_idx[MAX_TAB]        = {0};
    
            HI_U32 hac_min_tab[8]   = {0};
            HI_U32 hac_base_tab[8]   = {0};
            HI_U32 hac_symbol_tab[256]    = {0};
    
    
            JHUFF_TBL huff_ptr[MAX_TAB];
            {
                memset(&(huff_ptr[LU].bits[0]),0,17);
                memset(&(huff_ptr[LU].huffval[0]),0,256);
                memset(&(huff_ptr[CH].bits[0]),0,17);
                memset(&(huff_ptr[CH].huffval[0]),0,256);
                memcpy(&(huff_ptr[LU]), &dat[0], sizeof(huff_ptr[LU]));
                if(NULL == &dat[1])
                {
                    memcpy(&(huff_ptr[CH]),&dat[0],sizeof(huff_ptr[LU]));
                }
                else
                {
                    memcpy(&(huff_ptr[CH]),&dat[1],sizeof(huff_ptr[LU]));
                }
            }

            max_idx[LU] = JPEG_HDEC_DecHuff(&huff_ptr[0],huffcode[LU]);
            max_idx[CH] = JPEG_HDEC_DecHuff(&huff_ptr[1],huffcode[CH]);
        
            /** Luminance and Chrominance: LU=0,CH=1;**/
            for(cnt = 0; cnt < MAX_TAB; cnt++)
            {
                sum_syms=0;
                for(u32Index = 0; u32Index < 16; u32Index++)
                {
                    /** the ibit[] has sixteen value,read from jpeg file */
	                 syms = huff_ptr[cnt].bits[u32Index+1];
                    pre_index = u32Index?(u32Index-1):(0);
            
                    if(u32Index < max_idx[cnt])
                    {
    
				        min_tab[cnt][u32Index] = (min_tab[cnt][pre_index]+huff_ptr[cnt].bits[u32Index])<<1;

                        if(syms)
                        {
                            base_tab[cnt][u32Index]=sum_syms-min_tab[cnt][u32Index];
                        }

                        sum_syms += huff_ptr[cnt].bits[u32Index+1];
            
                    }
                    else
                    {
                        min_tab[cnt][u32Index]=~0;
                    }
                    
                }
            
                if(0 == cnt)
                {/** Create LU symbol table **/
                    for(u32Index=0;u32Index<sum_syms;u32Index++)
                    {
			              hac_symbol_tab[u32Index] |= ( (unsigned int)(huff_ptr[0].huffval[u32Index]) );

                    }
                }
                else
                {/** Create CH symbol table **/
                    for(u32Index=0;u32Index<sum_syms;u32Index++)
                    {
			            hac_symbol_tab[u32Index] |= ( (unsigned int)(huff_ptr[1].huffval[u32Index])<<8 );

                    }
                }
                
            }
            
            /** Conbine Luminnance and Chrominance */
            for(u32Index=0;u32Index<8;u32Index++)
            {
            
                hac_min_tab[u32Index] =   ( (min_tab[CH][2*u32Index+1] & 0xff)<<24 )  \
                                     | ( (min_tab[CH][2*u32Index] & 0xff)<<16   )  \
                                     | ( (min_tab[LU][2*u32Index+1] & 0xff)<<8  )  \
                                     | (min_tab[LU][2*u32Index] & 0xff);
            
                hac_base_tab[u32Index] =   ((base_tab[CH][2*u32Index+1] & 0xff)<<24 ) \
                                      | ((base_tab[CH][2*u32Index] & 0xff)<<16   ) \
                                      | ((base_tab[LU][2*u32Index+1] & 0xff)<<8  ) \
                                      | (base_tab[LU][2*u32Index] & 0xff);
                
            }
            
            /** Write hac_min/base/symbol_table into  HW reisters */
            JPEG_HDEC_CpyData2Reg((HI_CHAR *)JPGD_REG_BASEADDR, hac_min_tab,   JPGD_REG_HACMINTABLE,  32);
            JPEG_HDEC_CpyData2Reg((HI_CHAR *)JPGD_REG_BASEADDR, hac_base_tab,  JPGD_REG_HACBASETABLE, 32);
            JPEG_HDEC_CpyData2Reg((HI_CHAR *)JPGD_REG_BASEADDR, hac_symbol_tab,JPGD_REG_HACSYMTABLE,  0x2c0);

}



/*
 * Copyright (C) 2020-2025 ASHINi corp. 
 * 
 * This library is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU Lesser General Public 
 * License as published by the Free Software Foundation; either 
 * version 2.1 of the License, or (at your option) any later version. 
 * 
 * This library is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * Lesser General Public License for more details. 
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library; if not, write to the Free Software 
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA 
 * 
 */

#ifndef _ASI_FIND_DOCF_H__

#define _ASI_FIND_DOCF_H__

#define READ_FIRST_FMT_BIN_MAX	16
#define READ_PK_FMT_BIN_MAX 	8
#define READ_HML_FMT_BIN_MAX	12
#define READ_HWP_FMT_BIN_MAX	32
#define READ_DOC_FMT_BIN_MAX	1680

class CDocFileFmtUtil
{
public:
	CDocFileFmtUtil(void);
	~CDocFileFmtUtil(void);

	INT32 ProcAnalysis(PASI_DFILE_FMT_INFO pADFFI, char *acLogMsg);		

protected:
	INT32 SetSearchFileList(PASI_DFILE_FMT_INFO pADFFI, char *acLogMsg);
	INT32 CheckPkFmtAnalysis(char *pcFilePath, char *szFmt, char *acLogMsg);
	INT32 CheckHmlFmtAnalysis(char *pcFilePath, char *szFmt, char *acLogMsg);
	INT32 CheckHwpFmtAnalysis(char *pcFilePath, char *szFmt, char *acLogMsg);
	INT32 CheckDocFmtAnalysis(char *pcFilePath, char *szFmt, char *acLogMsg);
	INT32 CheckDocumentFile(char *pcFilePath, char *acLogMsg);
	INT32 CheckFileExtAnalysis(char *pcFilePath, INT32 *pnFileFmtType);
	INT32 CheckZipFileContentAnalysis(char *pcFilePath, INT32 *pnFileFmtType);

protected:
	INT32	ConvertFmtTypeToName(PASI_DFILE_FMT_INFO pADFFI);


protected:
	INT32 m_n_zip_Count;
	INT32 m_n_pptx_Han_Count;
	INT32 m_n_pptx_Office_Count;
	INT32 m_n_pptx2_Office_Count;
	INT32 m_n_odt_Office_Count;
	INT32 m_n_odt_Han_Count;
	INT32 m_n_odp_Office_Count;
	INT32 m_n_odp_Han_Count;
	INT32 m_n_xlsx_Office_Count;
	INT32 m_n_xlsx_Han_Count;
	INT32 m_n_docx_Office_Count;
	INT32 m_n_docx_Han_Count;
	INT32 m_n_ods_Han_Count;
	INT32 m_n_ppsx_Han_Count;
	INT32 m_n_ppsx_Office_Count;
	INT32 m_n_show_Han_Count;
	INT32 m_n_xlsx_unKnown_Count;
	INT32 m_n_ods_Office_Count;
	INT32 m_n_doc_Han_Count;
	INT32 m_n_doc_Office_Count;
	INT32 m_n_pps_Office_Count;
	INT32 m_n_ppt_Han_Count;
	INT32 m_n_ppt_Office_Count;
	INT32 m_n_xls_Office_Count;
	INT32 m_n_xls_Han_Count;
	INT32 m_n_cell_Han_Count;
	INT32 m_n_nxl_Han_Count;
	INT32 m_n_hwp2_Han_Count;
	INT32 m_n_hwp_Han_Count;
	INT32 m_n_hwp3_Han_Count;	
	INT32 m_n_pdf_Count;	
	INT32 m_n_hml_Han_Count;
	INT32 m_n_hpt_Han_Count;		
	INT32 m_n_nxt_Han_Count;
	INT32 m_n_odb_Han_Count;
	INT32 m_n_odg_Han_Count;	
	INT32 m_n_pptx_Known_Count;
	INT32 m_n_odt_Known_Count;
	INT32 m_n_odp_Known_Count;
	INT32 m_n_xlsx_Known_Count;
	INT32 m_n_docx_Known_Count;
	INT32 m_n_ods_Known_Count;
	INT32 m_n_ppsx_Known_Count;
	INT32 m_n_show_Known_Count;
	INT32 m_n_doc_Known_Count;
	INT32 m_n_xls_Known_Count;
	INT32 m_n_ppt_Known_Count;
	INT32 m_n_pps_Known_Count;
	INT32 m_n_cell_Known_Count;
	INT32 m_n_nxl_Known_Count;
	INT32 m_n_pdf_Known_Count;
	INT32 m_n_hml_Known_Count;
	INT32 m_n_hwp_Known_Count;
	INT32 m_n_hpt_Known_Count;
	INT32 m_n_nxt_Known_Count;
	INT32 m_n_odb_Known_Count;
	INT32 m_n_odg_Known_Count;
	INT32 m_n_user_define_Count;
};

#endif /*_ASI_FIND_DOCF_H__*/

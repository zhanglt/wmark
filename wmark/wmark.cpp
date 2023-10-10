#include <iostream>
#include "StdAfx.h"
#include "pdflib.h"
#include "pdflib.hpp"
#include <string>
#include <locale>
#include <codecvt>
#include <stdio.h>

#include "getopt.h"



#include <vector>


#include <fcntl.h>
#include <io.h>
#include <Windows.h>
#include <sysinfoapi.h>
bool getpages(std::wstring filename);





using namespace std;
using namespace pdflib;

void usage() {
	printf("�÷�:\n");
	printf("\t -i Դ�ļ�\n");
	printf("\t o ����ļ�\n");
	printf("\t-t ˮӡ�ı�\n");
	printf("\t-p ͸����(��:10%)\n");
	printf("\t-c ˮӡ��ɫ(��:green)\n");
	printf("\t-t ��ת�Ƕ�(��:45)\n");
	printf("\t-t ˮӡ����(��:simkai��simhei��simli��simfang)\n");
	printf("\t-s ��ȡ�ļ���ҳ��\n");
}


std::wstring String2WString(const std::string &s)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const char *chSrc = s.c_str();
	size_t nDestSize = mbstowcs(NULL, chSrc, 0) + 1;
	wchar_t *wchDest = new wchar_t[nDestSize];
	wmemset(wchDest, 0, nDestSize);
	mbstowcs(wchDest, chSrc, nDestSize);
	std::wstring wstrResult = wchDest;
	delete[] wchDest;
	setlocale(LC_ALL, strLocale.c_str());
	return wstrResult;
}
std::wstring GetFontsFolder() {
	char buffer[MAX_PATH + 1];
	auto result = GetWindowsDirectoryA(buffer, MAX_PATH);
	if (result) {
		return String2WString(std::string(buffer).append("\\fonts"));
	}
	return L"c:/windows/fonts"; // or however you want to handle the error
}
std::string char2string( char*  c) {
	string str=c;
	return str;
}
wstring pageName ;
int main(int argc, char *argv[])

{
	
	wstring infile = L"in.pdf";
	wstring outfile = L"out.pdf";
	wstring mark_txt = L"��ͨ���ֿƼ����޹�˾�ܲ�Ͷ��ר���ĵ�";
	wstring mark_opacity = L"15%";
	wstring mark_color = L"gray";
	wstring mark_rotate = L"45";
	wstring mark_font = L"simkai";
	
	
	int ch;
	//printf("\n\n");
	//printf("optind:%d��opterr��%d\n", optind, opterr);
	while ((ch = getopt(argc, argv, "i:o:t:p:c:r:f:s:h")) != -1)
	{
		//printf("optind: %d\n", optind);
		switch (ch)
		{
		case 'i':
			//printf("The argument of -i is %s\n\n", optarg);
			infile = String2WString(char2string(optarg));
			break;
		case 'o':
			//printf("The argument of -o is %s\n\n", optarg);
			outfile= String2WString(char2string(optarg));
			break;
		case 't':
			//printf("The argument of -t is %s\n\n", optarg);
			mark_txt= String2WString(char2string(optarg));
			break;
		case 'p':
			//printf("The argument of -p is %s\n\n", optarg);
			mark_opacity = String2WString(char2string(optarg));
			break;
		case 'c':
			//printf("The argument of -c is %s\n\n", optarg);
			mark_color = String2WString(char2string(optarg));
			break;
		case 'r':
			//printf("The argument of -r is %s\n\n", optarg);
			mark_rotate = String2WString(char2string(optarg));
			break;
		case 'f':
			//printf("The argument of -f is %s\n\n", optarg);
			mark_font = String2WString(char2string(optarg));
			break;
		case 's':
			//printf("The argument of -f is %s\n\n", optarg);
			getpages(String2WString(optarg));
			exit(0);
			//	getpages;
			exit(0);
		case 'h':
			usage();
			exit(0);
	    	break;
			
		case '?':
			printf("�����ѡ����ʹ��-h�����鿴����: \n");
			exit(0);
			break;

		}
	}
	//return 0;

	
	
	int indoc,endpage,pageno,page,font ;
	
	int numArg = 0;
	for (int i = 1; i < argc; ++i) {
		++numArg;
	}
	//cout<<numArg<<endl;
	//��ȡ�����ļ�
	if (numArg==0){
		usage();
		exit(0);
	}
	/*
	// ��ȡ����ļ�
	if (numArg>1){
		string str(argv[2]);
		if(str.length()>0){
			outfile=String2WString(str);
		}
	}
	// ��ȡˮӡ�ı�
	if (numArg>2){
		string str(argv[3]);
		if(str.length()>0){
			mark_txt = String2WString(str);
		}
	}
	// ˮӡ͸����
	if (numArg>3){
		string str(argv[4]);
		if(str.length()>0){
			mark_opacity=String2WString(str);
		}


	}
	// ��ȡˮӡ��ɫ
	if (numArg>4){
		string str(argv[5]);
		if(str.length()>0){
			mark_color=String2WString(str);
		}
	}
	// ��ȡˮӡ��б��
	if (numArg>5){
		string str(argv[6]);
		if(str.length()>0){
			mark_rotate=String2WString(str);
		}
	}
	// ��ȡˮӡ����
	if (numArg>6){
		string str(argv[7]);
		if(str.length()>0){
			mark_font=String2WString(str);
		}
	}

	*/
	try {

		PDFlib p;
		const wstring searchpath = L"./PDFlib-CMap-5.0/resource/cmap";
		wostringstream optlist;
		optlist << L"searchpath={{" << searchpath << L"}";
		optlist << L" {" << GetFontsFolder() << L"}}";
		p.set_option(optlist.str());
		//wcout << optlist.str() << endl;
		if (p.begin_document(outfile, L"") == -1){
			wcerr << L"Error: " << p.get_errmsg() << endl;
			return 2;
		}
		p.set_info(L"Creator", L"����̬ҵ�񹤾߼�");
		p.set_info(L"Title", L"���ĵ������ڷ���̬ҵ��Ͷ�갸��");
		indoc = p.open_pdi_document(infile, L"");

		//font = p.load_font(L"Helvetica", L"winansi", L"unicode");
		// �������塢�ַ���
		p.set_option(L"FontOutline={"+mark_font+L"="+mark_font+L".ttf}");
		//wcout << L"font:" << mark_font << endl;
		font = p.load_font(mark_font , L"unicode", L"");
		if (font == -1) {
			wcerr << L"Error: " << p.get_errmsg() << endl;
			return 2;
		}

		endpage = (int) p.pcos_get_number(indoc, L"length:pages");
		//wcout << L"mark_opacity:" << mark_opacity << endl;
		p.begin_template_ext(0, 0,
			L"watermark={location=ontop opacity=" +mark_opacity + L"}");
		

		//p.fit_textline(L"���������Ժ��ҵ���ص������鹵ͨ����ȡ��ɹ�Ӯ����ģʽ", 0, 0,L"fontsize=10 fontname=STSong-Light encoding=unicode  fillcolor=red boxsize={595 842} rotate=15 stamp=ll2ur");
		//p.fit_textline(L"��ʮ���塱�䶼��ˮ�����ִ������칤����Ϣ��(���ǻ�����������)ϵͳ", 0, 0,L"fontsize=10 fontname=STSong-Light encoding=unicode  fillcolor=red boxsize={195 142}  rotate=-45 ");
		wstring mark=L"fontsize=10 fontname=" +mark_font+ L" encoding=unicode  fillcolor="+mark_color+L" boxsize={95 42}  rotate="+mark_rotate;
		//wcout<< mark<<endl;
		p.fit_textline(mark_txt, 100, 100,mark);
		p.end_template_ext(0, 0);

		for (pageno = 1; pageno <= endpage; pageno++)
		{
			page = p.open_pdi_page(indoc, pageno, L"");

			p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");

			p.fit_pdi_page(page, 0, 0, L"adjustpage");

			p.close_pdi_page(page);

			p.end_page_ext(L"");
		}
		
		p.end_document(L"");

	}

	catch (PDFlib::Exception &ex) {
		
		wcerr << L"PDFlib �����쳣: " << endl
			<< L"[" << ex.get_errnum() << L"] " << ex.get_apiname()
			<< L": " << ex.get_errmsg() << endl
			<< L": " << L"����Ĳ���ѡ����ʹ�� - h�����鿴����" << endl;
		return 2;
	}



	return 0;

}
// ��ȡ�ļ���ҳ��
bool getpages(wstring filename)
{
	PDFlib m_oPDF;
	try
	{

		//��Դpdf�ļ���ȡʵ��id
		//pageName = String2WString(char2string(optarg));
		int nSrcPDFID = m_oPDF.open_pdi_document(filename, L"");
		if (nSrcPDFID == -1)
		{
			printf(u8"##��Դpdf�ļ�ʧ��!");
			return false;
		}
		//m_oPDF.set_option(L"searchpath={{ print_cache }}");//����Ĭ��·��

		//����SrcPDFʵ��id��ȡpdf�е���Ϣ
		int endpage = (int)m_oPDF.pcos_get_number(nSrcPDFID, L"length:pages");//��ҳ��
		auto fWidth = (m_oPDF.pcos_get_number(nSrcPDFID, L"pages[0]/width"));//���595
		auto fHeight = m_oPDF.pcos_get_number(nSrcPDFID, L"pages[0]/height");//�߶�842

		//printf("%d", endpage);
		cout << endpage << endl;

		//		m_oPDF.close_font(font);
		m_oPDF.close_pdi_document(nSrcPDFID);
	

		return true;
	}
	catch (PDFlib::Exception& e)
	{
		//���pdflib�쳣��Ϣ
		wprintf(m_oPDF.get_errmsg().c_str());
		return false;
	}
	catch (...)
	{
		//�����쳣
		return false;
	}
}


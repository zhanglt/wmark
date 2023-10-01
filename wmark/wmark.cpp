#include <iostream>
#include "StdAfx.h"
#include "pdflib.h"
#include "pdflib.hpp"
#include <string>
#include <locale>
#include <codecvt>

using namespace std;
using namespace pdflib;


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

int main(int argc, char *argv[])

{
	const wstring searchpath =L"./PDFlib-CMap-5.0/resource/cmap";
	const wstring fontPath =L"c:/windows/fonts";
	wstring infile = L"in.pdf";
	wstring outfile = L"out.pdf";
	wstring mark_txt=L"联通数字科技有限公司总部投标专用文档";
	wstring mark_opacity=L"15%";
	wstring mark_color=L"gray"	;	
	wstring mark_rotate=L"45";
	wstring mark_font=L"simkai";

	wostringstream optlist;
	wostringstream buf;

	int indoc,endpage,pageno,page,font ;

	int numArg = 0;
	for (int i = 1; i < argc; ++i) {
		++numArg;
	}
	//cout<<numArg<<endl;
	//获取输入文件
	if (numArg>0){
		string str(argv[1]);
		if(str.length()>0){
			infile=String2WString(str);
		}
	}else{
		cout<<"watermark 输入文件 输出文件 水印文字 透明度  文字颜色  旋转角度 水印字体"<<endl;
		exit(0);
	}
	// 获取输出文件
	if (numArg>1){
		string str(argv[2]);
		if(str.length()>0){
			outfile=String2WString(str);
		}
	}
	// 获取水印文本
	if (numArg>2){
		string str(argv[3]);
		if(str.length()>0){
			mark_txt = String2WString(str);
		}
	}
	// 水印透明度
	if (numArg>3){
		string str(argv[4]);
		if(str.length()>0){
			mark_opacity=String2WString(str);
		}


	}
	// 获取水印颜色
	if (numArg>4){
		string str(argv[5]);
		if(str.length()>0){
			mark_color=String2WString(str);
		}
	}
	// 获取水印倾斜度
	if (numArg>5){
		string str(argv[6]);
		if(str.length()>0){
			mark_rotate=String2WString(str);
		}
	}
	// 获取水印字体
	if (numArg>6){
		string str(argv[7]);
		if(str.length()>0){
			mark_font=String2WString(str);
		}
	}
	try {

		PDFlib p;
		optlist.str(L"");
		optlist << L"searchpath={{" << searchpath << L"}";
		optlist << L" {" << fontPath << L"}}";
		p.set_option(optlist.str());
		//wcout<< optlist.str()<<endl;

		if (p.begin_document(outfile, L"") == -1){
			wcerr << L"Error: " << p.get_errmsg() << endl;
			return 2;
		}
		p.set_info(L"Creator", L"泛生态业务工具集");
		p.set_info(L"Title", L"本文档来自于泛生态业务投标案例");

		indoc = p.open_pdi_document(infile, L"");

		//font = p.load_font(L"Helvetica", L"winansi", L"unicode");
		// 设置字体、字符集
		p.set_option(L"FontOutline={"+mark_font+L"="+mark_font+L".ttf}");
		font = p.load_font(mark_font , L"unicode", L"");
		if (font == -1) {
			wcerr << L"Error: " << p.get_errmsg() << endl;
			return 2;
		}

		endpage = (int) p.pcos_get_number(indoc, L"length:pages");

		p.begin_template_ext(0, 0,
			L"watermark={location=ontop opacity=" +mark_opacity + L"}");

		//p.fit_textline(L"继续与设计院、业主重点合作伙伴沟通，争取达成共赢合作模式", 0, 0,L"fontsize=10 fontname=STSong-Light encoding=unicode  fillcolor=red boxsize={595 842} rotate=15 stamp=ll2ur");
		//p.fit_textline(L"“十四五”武都引水灌区现代化改造工程信息化(“智慧武引”二期)系统", 0, 0,L"fontsize=10 fontname=STSong-Light encoding=unicode  fillcolor=red boxsize={195 142}  rotate=-45 ");
		wstring mark=L"fontsize=10 fontname=" +mark_font+ L" encoding=unicode  fillcolor="+mark_color+L" boxsize={95 42}  rotate="+mark_rotate;
		wcout<< mark<<endl;
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
		wcerr << L"PDFlib exception occurred in hello sample: " << endl
			<< L"[" << ex.get_errnum() << L"] " << ex.get_apiname()
			<< L": " << ex.get_errmsg() << endl;
		return 2;
	}
	return 0;

}
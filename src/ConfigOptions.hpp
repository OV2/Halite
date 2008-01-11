
//         Copyright Eóin O'Callaghan 2006 - 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#define ID_CONFIG_BEGIN	 			12000
#define IDC_SECURITY_IPF_GB 		ID_CONFIG_BEGIN + 1
#define IDC_SECURITY_PE_GB	 		ID_CONFIG_BEGIN + 2
#define IDC_SECURITY_CP_GB	 		ID_CONFIG_BEGIN + 3
#define IDC_SECURITY_EL_GB	 		ID_CONFIG_BEGIN + 4
#define IDC_SECURITY_OUTGOING 		ID_CONFIG_BEGIN + 5
#define IDC_SECURITY_INCOMING	 	ID_CONFIG_BEGIN + 6
#define IDC_SECURITY_ENABLED	 	ID_CONFIG_BEGIN + 7
#define IDC_SECURITY_FORCED	 		ID_CONFIG_BEGIN + 8
#define IDC_SECURITY_DISABLED 		ID_CONFIG_BEGIN + 9
#define IDC_BITT_OPENPORT 			ID_CONFIG_BEGIN + 10
#define IDC_BITT_GDHT	 			ID_CONFIG_BEGIN + 11
#define IDC_BITT_DHT_NOTE		 	ID_CONFIG_BEGIN + 12
#define IDC_BITT_GSETTINGS	 		ID_CONFIG_BEGIN + 13
#define IDC_BITT_SELECT_TEXT 		ID_CONFIG_BEGIN + 14
#define IDC_CONFIGT_TOTALMAX 		ID_CONFIG_BEGIN + 15
#define IDC_CONFIGT_TOTALUP	 		ID_CONFIG_BEGIN + 16
#define IDC_CONFIGT_DOWNRATE	 	ID_CONFIG_BEGIN + 17
#define IDC_CONFIGT_UPRATE	 		ID_CONFIG_BEGIN + 18
#define IDC_CONFIGT_TORMAX 			ID_CONFIG_BEGIN + 19
#define IDC_CONFIGT_TORUP 			ID_CONFIG_BEGIN + 20
#define IDC_CONFIGT_TORDOWNRATE	 	ID_CONFIG_BEGIN + 21
#define IDC_CONFIGT_TORUPRATE		ID_CONFIG_BEGIN + 22
#define IDC_CONFIGT_GLOBAL_TEXT	 	ID_CONFIG_BEGIN + 23
#define IDC_CONFIGT_TOR_TEXT 		ID_CONFIG_BEGIN + 24
#define IDC_CONFIGT_DEF_FOLDER 		ID_CONFIG_BEGIN + 25
#define IDC_CONFIGT_GLOBALG 		ID_CONFIG_BEGIN + 26
#define IDC_CONFIGT_TORRENTG 		ID_CONFIG_BEGIN + 27
#define IDC_CONFIGT_TDOWNRATE 		ID_CONFIG_BEGIN + 28
#define IDC_GENERAL_GROUP			ID_CONFIG_BEGIN + 29
#define IDC_GENERAL_LANG			ID_CONFIG_BEGIN + 30
#define IDC_GENERAL_CLOSETRAY		ID_CONFIG_BEGIN + 31
#define IDC_GENERAL_CONFIRMCLOSE	ID_CONFIG_BEGIN + 32
#define IDC_ABOUT_GROUP     		ID_CONFIG_BEGIN + 33
#define IDC_ABOUT_VERSION       	ID_CONFIG_BEGIN + 34

#ifndef RC_INVOKED

#include "stdAfx.hpp"

#include "Halite.hpp"
#include "halConfig.hpp"
#include "HaliteWindow.hpp"
#include "HaliteEditCtrl.hpp"
#include "DdxEx.hpp"
#include "CSSFileDialog.hpp"

class GeneralOptions :
    public CPropertyPageImpl<GeneralOptions>,
	public CAutoSizeWindow<GeneralOptions, false>,
    public CWinDataExchange<GeneralOptions>
{
	typedef GeneralOptions thisClass;
	typedef CPropertyPageImpl propPageClass;
	typedef CAutoSizeWindow<thisClass, false> autosizeClass;
	
public:
    enum { IDD = IDD_CONFIGGENERAL };

	GeneralOptions(HaliteWindow* haliteWindow) :
		haliteWindow_(haliteWindow)
	{}

	~GeneralOptions()
	{}

    BEGIN_MSG_MAP_EX(GeneralOptions)
		MSG_WM_INITDIALOG(OnInitDialog)
     	CHAIN_MSG_MAP(CPropertyPageImpl<GeneralOptions>)
        CHAIN_MSG_MAP(autosizeClass)
    END_MSG_MAP()

    BEGIN_DDX_MAP(GeneralOptions)
    	DDX_CHECK(IDC_GENERAL_ONEINST, halite().oneInst)
    	DDX_CHECK(IDC_GENERAL_TRAY, haliteWindow_->use_tray)
    	DDX_CHECK(IDC_GENERAL_ADVGUI, haliteWindow_->advancedUI)
    	DDX_CHECK(IDC_GENERAL_CLOSETRAY, haliteWindow_->closeToTray)
    	DDX_CHECK(IDC_GENERAL_CONFIRMCLOSE, haliteWindow_->confirmClose)
    END_DDX_MAP()

#define GENERAL_CONFIG_LAYOUT \
	WMB_HEAD(WMB_COLNOMAX(_exp)), \
		WMB_ROW(_gap,	_d), \
		WMB_ROW(_auto,	IDC_GENERAL_ONEINST), \
		WMB_ROW(_auto,	IDC_GENERAL_ADVGUI), \
		WMB_ROW(_auto,	IDC_GENERAL_TRAY), \
		WMB_ROW(_auto,	IDC_GENERAL_CLOSETRAY), \
		WMB_ROW(_auto,	IDC_GENERAL_CONFIRMCLOSE), \
	WMB_END()

#define GENERAL_LANGUAGE_LAYOUT \
	WMB_HEAD(WMB_COLNOMAX(_exp)), \
		WMB_ROW(_auto,	IDC_GENERAL_LANG), \
		WMB_ROW(_auto,	IDC_LANGLIST), \
	WMB_END()

	BEGIN_WINDOW_MAP(thisClass, 6, 6, 3, 3)
		WMB_HEAD(WMB_COL(_gap), WMB_COLNOMAX(_exp), WMB_COLNOMAX(_exp), WMB_COL(_gap)),
			WMB_ROW(_gap|3,	IDC_GENERAL_GROUP, _r, _r, _r), 
			WMB_ROW(_auto,	_d, GENERAL_CONFIG_LAYOUT, GENERAL_LANGUAGE_LAYOUT), 
			WMB_ROW(_gap,	_d),
		WMB_END()
	END_WINDOW_MAP()	

    BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);

    int OnApply()
	{	
		size_t length = lang_list_.GetTextLen(lang_list_.GetCurSel());
		boost::scoped_array<wchar_t> buffer(new wchar_t[length+1]);
		
		lang_list_.GetText(lang_list_.GetCurSel(), buffer.get());
		std::wstring language(buffer.get(), length);

		if (language == L"English")
		{
			halite().dll_ = L"";
			hal::app().res_revert();
		}
		else
		{
			halite().dll_ = lang_map_[language].external_file_string();
			hal::app().res_set_dll(lang_map_[language].external_file_string());
		}
		
		return DoDataExchange(true);
	}

private:
	CListBox lang_list_;
	std::map<wstring, wpath> lang_map_;
	HaliteWindow* haliteWindow_;
};

class BitTorrentOptions :
    public CPropertyPageImpl<BitTorrentOptions>,
	public CAutoSizeWindow<BitTorrentOptions, false>,
    public CWinDataExchangeEx<BitTorrentOptions>
{
	typedef BitTorrentOptions thisClass;
	typedef CAutoSizeWindow<thisClass, false> autosizeClass;
	
public:
    enum { IDD = IDD_CONFIGBIT };

	BitTorrentOptions()
	{}

	~BitTorrentOptions()
	{}

    BEGIN_MSG_MAP_EX(BitTorrentOptions)
        MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER_EX(IDC_BC_PORTCHECK, onPortCheck)
		COMMAND_ID_HANDLER_EX(IDC_BC_HALFCONN_CHECK, onHalfCheck)
		COMMAND_ID_HANDLER_EX(IDC_BC_DHT, onDHTCheck)
		
        CHAIN_MSG_MAP(CPropertyPageImpl<BitTorrentOptions>)
        CHAIN_MSG_MAP(autosizeClass)
    END_MSG_MAP()

    BEGIN_DDX_MAP(BitTorrentOptions)
        DDX_CHECK(IDC_BC_PORTCHECK, hal::config().portRange)
        DDX_INT(IDC_BC_PORTFROM, hal::config().portFrom)
        DDX_INT(IDC_BC_PORTTO, hal::config().portTo)		
        DDX_CHECK(IDC_BC_HALFCONN_CHECK, hal::config().halfConn)
        DDX_EX_INT_POSITIVE(IDC_BC_HALFCONN_NUM, hal::config().halfConnLimit)
        DDX_CHECK(IDC_BC_DHT, hal::config().enableDHT)
        DDX_INT(IDC_BC_DHTPORT, hal::config().dhtServicePort)
    END_DDX_MAP()
	
#define BITT_GS_LAYOUT \
	WMB_HEAD(WMB_COLNOMAX(_exp|250), WMB_COLNOMAX(_exp), WMB_COLNOMAX(_exp), WMB_COLNOMAX(_exp)), \
		WMB_ROW(_auto,	IDC_BITT_SELECT_TEXT, _r, _r, _r), \
		WMB_ROW(_auto,	IDC_BITT_OPENPORT, IDC_BC_PORTFROM, IDC_BC_PORTCHECK, IDC_BC_PORTTO), \
		WMB_ROW(_auto,	IDC_BC_HALFCONN_CHECK, _r, IDC_BC_HALFCONN_NUM), \
	WMB_END()

#define BITT_DHT_LAYOUT \
	WMB_HEAD(WMB_COLNOMAX(_exp), WMB_COLNOMAX(_exp), WMB_COLNOMAX(_exp)), \
		WMB_ROW(_auto,	IDC_BC_DHT, IDC_BC_DHTPORT_S, IDC_BC_DHTPORT), \
		WMB_ROW(_auto,	IDC_BITT_DHT_NOTE, _r, _r), \
	WMB_END()

	BEGIN_WINDOW_MAP(thisClass, 6, 6, 3, 3)
		WMB_HEAD(WMB_COL(_gap), WMB_COLNOMAX(_exp), WMB_COL(_gap)),
			WMB_ROW(_gap|3,	IDC_BITT_GSETTINGS, _r, _r), 
			WMB_ROW(_auto,	_d, BITT_GS_LAYOUT),
			WMB_ROW(_gap,	_d),
			WMB_ROW(_gap|3,	IDC_BITT_GDHT, _r, _r),
			WMB_ROW(_auto,	_d, BITT_DHT_LAYOUT), 
			WMB_ROW(_gap,	_d),
		WMB_END()
	END_WINDOW_MAP()

    BOOL OnInitDialog (HWND hwndFocus, LPARAM lParam)
	{	
	//	helpLink.SubclassWindow(GetDlgItem(IDC_BC_HELP_LINK));
	//	helpLink.SetHyperLink(L"http://www.binarynotions.com/");
		
	//	whyHalfConn.SubclassWindow(GetDlgItem(IDC_BC_CON_WHY));
	//	whyHalfConn.SetHyperLink(L"http://www.binarynotions.com/");
		
		BOOL retval = DoDataExchange(false);

		onHalfCheck(0, 0, GetDlgItem(IDC_BC_HALFCONN_CHECK));
		onPortCheck(0, 0, GetDlgItem(IDC_BC_PORTCHECK));
		onDHTCheck(0, 0, GetDlgItem(IDC_BC_DHT));

		autosizeClass::CtrlsArrange();
		return retval;
	}

    int OnApply()
	{
		return DoDataExchange(true);
	}

	void onDHTCheck(UINT, int, HWND hWnd)
	{
		LRESULT result = ::SendMessage(hWnd, BM_GETCHECK, 0, 0);

		if (result == BST_CHECKED)
		{
			::EnableWindow(GetDlgItem(IDC_BC_DHTPORT), true);
			::EnableWindow(GetDlgItem(IDC_BC_DHTPORT_S), true);
		}
		else
		{
			::EnableWindow(GetDlgItem(IDC_BC_DHTPORT), false);
			::EnableWindow(GetDlgItem(IDC_BC_DHTPORT_S), false);
		}
	}

	void onPortCheck(UINT, int, HWND hWnd)
	{
		LRESULT result = ::SendMessage(hWnd, BM_GETCHECK, 0, 0);

		if (result == BST_CHECKED)
		{
			::EnableWindow(GetDlgItem(IDC_BC_PORTTO), true);
		}
		else
		{
			::EnableWindow(GetDlgItem(IDC_BC_PORTTO), false);
			boost::array<wchar_t, MAX_PATH> buffer;
			GetDlgItemText(IDC_BC_PORTFROM, buffer.elems, MAX_PATH);
			SetDlgItemText(IDC_BC_PORTTO, buffer.elems);
		}
	}

	void onHalfCheck(UINT, int, HWND hWnd)
	{
		LRESULT result = ::SendMessage(hWnd, BM_GETCHECK, 0, 0);

		if (result == BST_CHECKED)
		{
			::EnableWindow(GetDlgItem(IDC_BC_HALFCONN_NUM), true);
		}
		else
		{
			::EnableWindow(GetDlgItem(IDC_BC_HALFCONN_NUM), false);
			SetDlgItemText(IDC_BC_HALFCONN_NUM, L"");
		}
	}
private:
	CHyperLink helpLink;
	CHyperLink whyHalfConn;
};

class ProxyOptions :
    public CPropertyPageImpl<ProxyOptions>,
    public CWinDataExchangeEx<ProxyOptions>
{
public:
    enum { IDD = IDD_CONFIGPROXY };

	ProxyOptions()
	{}

	~ProxyOptions()
	{}

    BEGIN_MSG_MAP_EX(ProxyOptions)
        MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER_EX(IDC_BC_PROXYCHECK, onProxyCheck)
        CHAIN_MSG_MAP(CPropertyPageImpl<ProxyOptions>)
    END_MSG_MAP()

    BEGIN_DDX_MAP(ProxyOptions)
	
    END_DDX_MAP()

    BOOL OnInitDialog (HWND hwndFocus, LPARAM lParam)
	{
		BOOL retval =  DoDataExchange(false);

		onProxyCheck(0, 0, GetDlgItem(IDC_BC_PROXYCHECK));

		return retval;
	}

    int OnApply()
	{
		return DoDataExchange(true);
	}

	void onPortCheck(UINT, int, HWND hWnd)
	{
		LRESULT result = ::SendMessage(hWnd, BM_GETCHECK, 0, 0);

		if (result == BST_CHECKED)
		{
			::EnableWindow(GetDlgItem(IDC_BC_PORTTO), true);
		}
		else
		{
			::EnableWindow(GetDlgItem(IDC_BC_PORTTO), false);
			boost::array<wchar_t, MAX_PATH> buffer;
			GetDlgItemText(IDC_BC_PORTFROM, buffer.elems, MAX_PATH);
			SetDlgItemText(IDC_BC_PORTTO, buffer.elems);
		}
	}

	void onProxyCheck(UINT, int, HWND hWnd)
	{
		LRESULT result = ::SendMessage(hWnd, BM_GETCHECK, 0, 0);

		if (result == BST_CHECKED)
		{
			::EnableWindow(GetDlgItem(IDC_BC_PROXYIP), true);
			::EnableWindow(GetDlgItem(IDC_BC_PROXYPORT), true);
			::EnableWindow(GetDlgItem(IDC_BC_PROXYUSER), true);
			::EnableWindow(GetDlgItem(IDC_BC_PROXYPASS), true);
			::EnableWindow(GetDlgItem(IDC_BC_PROXYIP_S), true);
			::EnableWindow(GetDlgItem(IDC_BC_PROXYPORT_S), true);
			::EnableWindow(GetDlgItem(IDC_BC_PROXYUSER_S), true);
			::EnableWindow(GetDlgItem(IDC_BC_PROXYPASS_S), true);
		}
		else
		{
			::EnableWindow(GetDlgItem(IDC_BC_PROXYIP), false);
			::EnableWindow(GetDlgItem(IDC_BC_PROXYPORT), false);
			::EnableWindow(GetDlgItem(IDC_BC_PROXYUSER), false);
			::EnableWindow(GetDlgItem(IDC_BC_PROXYPASS), false);
			::EnableWindow(GetDlgItem(IDC_BC_PROXYIP_S), false);
			::EnableWindow(GetDlgItem(IDC_BC_PROXYPORT_S), false);
			::EnableWindow(GetDlgItem(IDC_BC_PROXYUSER_S), false);
			::EnableWindow(GetDlgItem(IDC_BC_PROXYPASS_S), false);
		}
	}
};

class SecurityOptions :
    public CPropertyPageImpl<SecurityOptions>,
	public CAutoSizeWindow<SecurityOptions, false>,
    public CWinDataExchangeEx<SecurityOptions>
{
	typedef SecurityOptions thisClass;
	typedef CAutoSizeWindow<thisClass, false> autosizeClass;
	
public:
    enum { IDD = IDD_CONFIGSECURITY };

    BEGIN_MSG_MAP_EX(thisClass)
        MSG_WM_INITDIALOG(OnInitDialog)
		
		COMMAND_ID_HANDLER_EX(IDC_BC_FILTERLOAD, onFilterImport)
		COMMAND_ID_HANDLER_EX(IDC_BC_FILTERCLEAR, onFilterClear)
		COMMAND_ID_HANDLER_EX(IDC_BC_FILTERCHECK, onFilterCheck)
		
		COMMAND_ID_HANDLER_EX(IDC_SC_ENABLE_PE, onPeCheck)
		
        CHAIN_MSG_MAP(autosizeClass)
        CHAIN_MSG_MAP(CPropertyPageImpl<thisClass>)
		MSG_WM_SIZE(OnSize)
    END_MSG_MAP()

    BEGIN_DDX_MAP(thisClass)
        DDX_CHECK(IDC_BC_FILTERCHECK, hal::config().enableIPFilter)
        DDX_CHECK(IDC_SC_ENABLE_PE, hal::config().enablePe)
        DDX_RADIO(IDC_SC_PE_ENC_PLAIN, hal::config().peEncLevel)
        DDX_CHECK(IDC_SC_PE_ENC_RC4_PERFER, hal::config().pePerferRc4)
        DDX_RADIO(IDC_SC_PE_CP_IN_FORCED, hal::config().peConInPolicy)
        DDX_RADIO(IDC_SC_PE_CP_OUT_FORCED, hal::config().peConOutPolicy)
    END_DDX_MAP()
	
	void OnSize(UINT type, CSize)
	{
		
	hal::event().post(shared_ptr<hal::EventDetail>(
		new hal::EventMsg(L"Me sizing :).", hal::Event::info)));
		SetMsgHandled(false);
	}

#define SECURITY_IPF_LAYOUT \
	WMB_HEAD(WMB_COLNOMAX(_exp), WMB_COLNOMAX(_exp), WMB_COLNOMAX(_exp)), \
		WMB_ROW(_auto,	IDC_BC_FILTERCHECK,	IDC_BC_FILTERCLEAR, IDC_BC_FILTERLOAD), \
	WMB_END()

#define SECURITY_ENCODING_LAYOUT \
	WMB_HEAD(WMB_COL(_gap), WMB_COLNOMAX(_exp), WMB_COLNOMAX(_exp), WMB_COL(_gap)), \
		WMB_ROW(_auto,	IDC_SC_ENABLE_PE,	_r,_r,_r), \
		WMB_ROW(_gap|3,	IDC_SECURITY_EL_GB,	_r,_r,_r), \
		WMB_ROW(_auto,	_d,	IDC_SC_PE_ENC_PLAIN, _r), \
		WMB_ROW(_auto,	_d,	IDC_SC_PE_ENC_RC4, _r), \
		WMB_ROW(_auto,	_d,	IDC_SC_PE_ENC_BOTH, IDC_SC_PE_ENC_RC4_PERFER), \
		WMB_ROW(_gap,	_d), \
	WMB_END()

#define SECURITY_CONNECTION_LAYOUT \
	WMB_HEAD(WMB_COL(_gap), WMB_COLNOMAX(_exp), WMB_COLNOMAX(_exp), WMB_COLNOMAX(_exp), WMB_COLNOMAX(_exp), WMB_COL(_gap)), \
		WMB_ROW(_gap|4,	IDC_SECURITY_CP_GB,	_r, _r, _r, _r, _r), \
		WMB_ROW(_auto,	_d,	IDC_SECURITY_INCOMING, _r, IDC_SECURITY_OUTGOING, _r), \
		WMB_ROW(_auto,	_d,	IDC_SC_PE_CP_IN_FORCED, IDC_SECURITY_FORCED, _r, IDC_SC_PE_CP_OUT_FORCED), \
		WMB_ROW(_auto,	_d,	IDC_SC_PE_CP_IN_ENABLED, IDC_SECURITY_ENABLED, _r, IDC_SC_PE_CP_OUT_ENABLED), \
		WMB_ROW(_auto,	_d,	IDC_SC_PE_CP_IN_DISABLED, IDC_SECURITY_DISABLED, _r, IDC_SC_PE_CP_OUT_DISABLED), \
		WMB_ROW(_gap,	_d), \
	WMB_END()
	
#define SECURITY_PE_LAYOUT \
	WMB_HEAD(WMB_COLNOMAX(_exp|150), WMB_COLNOMAX(_exp)), \
		WMB_ROW(_auto,	SECURITY_ENCODING_LAYOUT, SECURITY_CONNECTION_LAYOUT), \
	WMB_END()

	BEGIN_WINDOW_MAP(thisClass, 6, 6, 3, 3)
		WMB_HEAD(WMB_COL(_gap), WMB_COLNOMAX(_exp), WMB_COL(_gap)), 
			WMB_ROW(_gap|3,	IDC_SECURITY_IPF_GB, _r, _r), 
			WMB_ROW(_auto,	_d, SECURITY_IPF_LAYOUT), 
			WMB_ROW(_gap,	_d),  
			WMB_ROW(_gap|3,	IDC_SECURITY_PE_GB, _r, _r), 
			WMB_ROW(_auto,	_d, SECURITY_PE_LAYOUT),
			WMB_ROW(_gap,	_d),
		WMB_END()
	END_WINDOW_MAP()	

    BOOL OnInitDialog (HWND hwndFocus, LPARAM lParam)
	{
	//	helpLink.SubclassWindow(GetDlgItem(IDC_SC_HELP_LINK));
	//	helpLink.SetHyperLink(L"http://www.binarynotions.com/");

		BOOL retval =  DoDataExchange(false);

		onFilterCheck(0, 0, GetDlgItem(IDC_BC_FILTERCHECK));
		onPeCheck(0, 0, GetDlgItem(IDC_SC_ENABLE_PE));

		autosizeClass::CtrlsArrange();
		return retval;
	}

    int OnApply()
	{
		return DoDataExchange(true);
	}

	void onFilterCheck(UINT, int, HWND hWnd)
	{
		LRESULT result = ::SendMessage(hWnd, BM_GETCHECK, 0, 0);

		if (result == BST_CHECKED)
		{
			::EnableWindow(GetDlgItem(IDC_BC_FILTERCLEAR), true);
			::EnableWindow(GetDlgItem(IDC_BC_FILTERLOAD), true);
		}
		else
		{
			::EnableWindow(GetDlgItem(IDC_BC_FILTERCLEAR), false);
			::EnableWindow(GetDlgItem(IDC_BC_FILTERLOAD), false);
		}
	}

	void onPeCheck(UINT, int, HWND hWnd)
	{
		LRESULT result = ::SendMessage(hWnd, BM_GETCHECK, 0, 0);

		if (result == BST_CHECKED)
		{
			::EnableWindow(GetDlgItem(IDC_SC_PE_ENC_BOTH), true);
			::EnableWindow(GetDlgItem(IDC_SC_PE_ENC_RC4), true);
			::EnableWindow(GetDlgItem(IDC_SC_PE_ENC_PLAIN), true);
			::EnableWindow(GetDlgItem(IDC_SC_PE_ENC_RC4_PERFER), true);
			::EnableWindow(GetDlgItem(IDC_SC_PE_CP_IN_FORCED), true);
			::EnableWindow(GetDlgItem(IDC_SC_PE_CP_IN_DISABLED), true);
			::EnableWindow(GetDlgItem(IDC_SC_PE_CP_IN_ENABLED), true);
			::EnableWindow(GetDlgItem(IDC_SC_PE_CP_OUT_DISABLED), true);
			::EnableWindow(GetDlgItem(IDC_SC_PE_CP_OUT_ENABLED), true);
			::EnableWindow(GetDlgItem(IDC_SC_PE_CP_OUT_FORCED), true);
		}
		else
		{
			::EnableWindow(GetDlgItem(IDC_SC_PE_ENC_BOTH), false);
			::EnableWindow(GetDlgItem(IDC_SC_PE_ENC_RC4), false);
			::EnableWindow(GetDlgItem(IDC_SC_PE_ENC_PLAIN), false);
			::EnableWindow(GetDlgItem(IDC_SC_PE_ENC_RC4_PERFER), false);
			::EnableWindow(GetDlgItem(IDC_SC_PE_CP_IN_FORCED), false);
			::EnableWindow(GetDlgItem(IDC_SC_PE_CP_IN_DISABLED), false);
			::EnableWindow(GetDlgItem(IDC_SC_PE_CP_IN_ENABLED), false);
			::EnableWindow(GetDlgItem(IDC_SC_PE_CP_OUT_DISABLED), false);
			::EnableWindow(GetDlgItem(IDC_SC_PE_CP_OUT_ENABLED), false);
			::EnableWindow(GetDlgItem(IDC_SC_PE_CP_OUT_FORCED), false);
		}
	}

	void onFilterClear(UINT, int, HWND hWnd)
	{
		hal::bittorrent().clearIpFilter();
	}

	void onFilterImport(UINT, int, HWND hWnd);

private:
	CHyperLink helpLink;
};

class TorrentsOptions :
    public CPropertyPageImpl<TorrentsOptions>,
	public CAutoSizeWindow<TorrentsOptions, false>,
    public CWinDataExchangeEx<TorrentsOptions>
{
	typedef TorrentsOptions thisClass;
	typedef CAutoSizeWindow<thisClass, false> autosizeClass;
public:
    enum { IDD = IDD_CONFIGTORRENT };
	
	TorrentsOptions() :
		gTotalConnections_(0, false, false),
		gUploadConnections_(0, false, false),
		gDownloadRate_(0, false, false),
		gUploadRate_(0, false, false),
		tTotalConnections_(0, false, false),
		tUploadConnections_(0, false, false),
		tDownloadRate_(0, false, false),
		tUploadRate_(0, false, false)
	{}

    BEGIN_MSG_MAP_EX(TorrentsOptions)
        MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER_EX(IDC_BC_SAVEBROWSE, onFolderBrowse)
        CHAIN_MSG_MAP(CPropertyPageImpl<TorrentsOptions>)
        CHAIN_MSG_MAP(autosizeClass)
	   REFLECT_NOTIFICATIONS()
    END_MSG_MAP()
	
#define CONFIGT_GLOBAL_LAYOUT \
	WMB_HEAD(WMB_COLNOMAX(_exp), WMB_COLNOMAX(_exp|30), WMB_COLNOMAX(_eq|0), WMB_COLNOMAX(_eq|1)), \
		WMB_ROW(_auto,	IDC_CONFIGT_GLOBAL_TEXT, _r, _r, _r), \
		WMB_ROW(_auto,	IDC_CONFIGT_TOTALMAX, IDC_BC_MAXCONN, IDC_CONFIGT_DOWNRATE, IDC_BC_DOWNRATE), \
		WMB_ROW(_auto,	IDC_CONFIGT_TOTALUP, IDC_BC_MAXUP, IDC_CONFIGT_UPRATE, IDC_BC_UPRATE), \
	WMB_END()

#define CONFIGT_TORRENT_LAYOUT \
	WMB_HEAD(WMB_COLNOMAX(_exp), WMB_COLNOMAX(_exp|30), WMB_COLNOMAX(_eq|0), WMB_COLNOMAX(_eq|1)), \
		WMB_ROW(_auto,	IDC_CONFIGT_TOR_TEXT, _r, _r, _r), \
		WMB_ROW(_auto,	IDC_CONFIGT_TORMAX, IDC_BC_TMAXUP, IDC_CONFIGT_TDOWNRATE, IDC_BC_TDOWNRATE), \
		WMB_ROW(_auto,	IDC_CONFIGT_TORUP, IDC_BC_TUPRATE, IDC_CONFIGT_TORUPRATE, IDC_BC_TMAXCONN), \
	WMB_END()

#define CONFIGT_TORRENT_FOLDER_LAYOUT \
	WMB_HEAD(WMB_COLNOMAX(_exp|50), WMB_COLNOMAX(_exp), WMB_COLNOMAX(_auto)), \
		WMB_ROW(_auto,	IDC_CONFIGT_DEF_FOLDER, IDC_BC_SAVEFOLDER, IDC_BC_SAVEBROWSE), \
		WMB_ROW(_auto, 0, IDC_BC_PROMPTSAVE, _r), \
	WMB_END()

	BEGIN_WINDOW_MAP(thisClass, 6, 6, 3, 3)
		WMB_HEAD(WMB_COL(_gap), WMB_COLNOMAX(_exp), WMB_COL(_gap)), 
			WMB_ROW(_gap|3,	IDC_CONFIGT_GLOBALG, _r, _r), 
			WMB_ROW(_auto,	_d, CONFIGT_GLOBAL_LAYOUT), 
			WMB_ROW(_gap,	_d),  
			WMB_ROW(_gap|3,	IDC_CONFIGT_TORRENTG, _r, _r),
			WMB_ROW(_auto,	_d, CONFIGT_TORRENT_LAYOUT),
			WMB_ROW(_auto,	_d, CONFIGT_TORRENT_FOLDER_LAYOUT), 
			WMB_ROW(_gap,	_d),
		WMB_END()
	END_WINDOW_MAP()

    BEGIN_DDX_MAP(TorrentsOptions)
/*        DDX_EX_INT_POSITIVE_LIMIT(IDC_BC_MAXCONN, hal::config().maxConnections, 2, true)
        DDX_EX_INT_POSITIVE_LIMIT(IDC_BC_MAXUP, hal::config().maxUploads, 2, true)
        DDX_EX_FLOAT_POSITIVE(IDC_BC_DOWNRATE, hal::config().downRate)
        DDX_EX_FLOAT_POSITIVE(IDC_BC_UPRATE, hal::config().upRate)

        DDX_EX_INT_POSITIVE_LIMIT(IDC_BC_TMAXCONN, hal::config().torrentMaxConnections, 2, true)
        DDX_EX_INT_POSITIVE_LIMIT(IDC_BC_TMAXUP, hal::config().torrentMaxUploads, 2, true)
        DDX_EX_FLOAT_POSITIVE(IDC_BC_TDOWNRATE, hal::config().torrentDownRate)
        DDX_EX_FLOAT_POSITIVE(IDC_BC_TUPRATE, hal::config().torrentUpRate)
*/
		DDX_EX_STDWSTRING(IDC_BC_SAVEFOLDER, hal::config().defaultSaveFolder);
        DDX_CHECK(IDC_BC_PROMPTSAVE, hal::config().savePrompt)
    END_DDX_MAP()	

    BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam)
	{
 		gTotalConnections_.Attach(GetDlgItem(IDC_BC_MAXCONN));
		gUploadConnections_.Attach(GetDlgItem(IDC_BC_MAXUP));
		gDownloadRate_.Attach(GetDlgItem(IDC_BC_DOWNRATE));
		gUploadRate_.Attach(GetDlgItem(IDC_BC_UPRATE));
	
		tTotalConnections_.Attach(GetDlgItem(IDC_BC_TMAXCONN));
		tUploadConnections_.Attach(GetDlgItem(IDC_BC_TMAXUP));
		tDownloadRate_.Attach(GetDlgItem(IDC_BC_TDOWNRATE));
		tUploadRate_.Attach(GetDlgItem(IDC_BC_TUPRATE));

		gTotalConnections_ = hal::config().maxConnections;
		gUploadConnections_ = hal::config().maxUploads;
		gDownloadRate_ = hal::config().downRate;
		gUploadRate_ = hal::config().upRate;
	
		tTotalConnections_ = hal::config().torrentMaxConnections;
		tUploadConnections_ = hal::config().torrentMaxUploads;
		tDownloadRate_ = hal::config().torrentDownRate;
		tUploadRate_ = hal::config().torrentUpRate;
	
		BOOL retval =  DoDataExchange(false);

		autosizeClass::CtrlsArrange();
		return retval;
	}

	void onFolderBrowse(UINT, int, HWND hWnd)
	{
		hal::bittorrent().clearIpFilter();
		
		wstring folderSelect = hal::app().res_wstr(HAL_FOLDER_SELECT);

		CFolderDialog fldDlg (NULL, folderSelect.c_str(),
			BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE);

		if (IDOK == fldDlg.DoModal())
			SetDlgItemText(IDC_BC_SAVEFOLDER, fldDlg.m_szFolderPath);
	}

    int OnApply()
	{
		hal::config().maxConnections = gTotalConnections_;
		hal::config().maxUploads = gUploadConnections_;
		hal::config().downRate = gDownloadRate_;
		hal::config().upRate = gUploadRate_;

		hal::config().torrentMaxConnections = tTotalConnections_;
		hal::config().torrentMaxUploads = tUploadConnections_;
		hal::config().torrentDownRate = tDownloadRate_;
		hal::config().torrentUpRate = tUploadRate_;
		
		return DoDataExchange(true);
	}
	
private:
	CHaliteEditCtrl<int> gTotalConnections_;
	CHaliteEditCtrl<int> gUploadConnections_;
	CHaliteEditCtrl<float> gDownloadRate_;
	CHaliteEditCtrl<float> gUploadRate_;
	
	CHaliteEditCtrl<int> tTotalConnections_;
	CHaliteEditCtrl<int> tUploadConnections_;
	CHaliteEditCtrl<float> tDownloadRate_;
	CHaliteEditCtrl<float> tUploadRate_;
};

class RemoteOptions :
    public CPropertyPageImpl<RemoteOptions>,
    public CWinDataExchange<RemoteOptions>
{
public:
    enum { IDD = IDD_CONFIGREMOTE };

	RemoteOptions()
	{}

	~RemoteOptions()
	{}

    BEGIN_MSG_MAP_EX(RemoteOptions)
		MSG_WM_INITDIALOG(OnInitDialog)
     	CHAIN_MSG_MAP(CPropertyPageImpl<RemoteOptions>)
    END_MSG_MAP()

    BEGIN_DDX_MAP(RemoteOptions)
//    	DDX_CHECK(IDC_REMOTECTRL, INI().remoteConfig().isEnabled)
//    	DDX_INT(IDC_REMOTEPORT, INI().remoteConfig().port)
    END_DDX_MAP()

    BOOL OnInitDialog ( HWND hwndFocus, LPARAM lParam )
	{
		return DoDataExchange(false);
	}

    int OnApply()
	{
		return DoDataExchange(true);
	}
};

class AboutOptions :
    public CPropertyPageImpl<AboutOptions>,
	public CDialogResize<AboutOptions>
{
	typedef AboutOptions thisClass;
	typedef CDialogResize<thisClass> resizeClass;

public:
    enum { IDD = IDD_ABOUT };

    BEGIN_MSG_MAP_EX(thisClass)
		MSG_WM_INITDIALOG(OnInitDialog)
		CHAIN_MSG_MAP(resizeClass)
        CHAIN_MSG_MAP(CPropertyPageImpl<thisClass>)
    END_MSG_MAP()

    BEGIN_DLGRESIZE_MAP(thisClass)
		DLGRESIZE_CONTROL(IDC_RICHEDITABOUT, DLSZ_SIZE_X|DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_ABOUT_VERSION, DLSZ_MOVE_X|DLSZ_MOVE_Y)
	END_DLGRESIZE_MAP()

    BOOL OnInitDialog (HWND hwndFocus, LPARAM lParam)
	{
		richEdit_.Attach(GetDlgItem(IDC_RICHEDITABOUT));

		std::pair<void*, size_t> res = hal::app().res_find_lock(IDR_ABOUT_TEXT, HALITE_RTF_RES_TYPE);

		string rtf(static_cast<char*>(res.first), res.second);
		wstring wrtf = hal::from_utf8_safe(rtf);

		richEdit_.SendMessage(WM_SETTEXT, 0, (LPARAM)wrtf.c_str());
		richEdit_.SetSelNone();
//		richEdit_.SetEventMask(ENM_REQUESTRESIZE);

        resizeClass::DlgResize_Init(false, true, WS_CLIPCHILDREN);

		return true;
	}

private:

	CRichEditCtrl richEdit_;
	RECT richRect_;
};

class ConfigOptionsProp :
	public CPropertySheet,
	public CDialogResize<ConfigOptionsProp>
{
	typedef ConfigOptionsProp thisClass;
	typedef CDialogResize<thisClass> resizeClass;

public:
    ConfigOptionsProp(HaliteWindow* haliteWindow, LPCTSTR title = (LPCTSTR)NULL,
		UINT uStartPage = 0, HWND hWndParent = NULL) :
        CPropertySheet(title, uStartPage, hWndParent),
		m_bCentered(false),
		generalOptions(haliteWindow)
    {
		AddPage(generalOptions);
		AddPage(bitTorrentOptions);
		AddPage(securityOptions);
		AddPage(torrentsOptions);
	//	AddPage(remoteControlOptions);
		AddPage(aboutOptions);
    }

    BEGIN_MSG_MAP_EX(ConfigOptionsProp)
        MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		MSG_WM_SIZE(OnSize)
		CHAIN_MSG_MAP(resizeClass)
        CHAIN_MSG_MAP(CPropertySheet)
    END_MSG_MAP()

    BEGIN_DLGRESIZE_MAP(thisClass)
		DLGRESIZE_CONTROL(0x3020, DLSZ_SIZE_X|DLSZ_SIZE_Y)

		DLGRESIZE_CONTROL(0x1, DLSZ_MOVE_X|DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(0x2, DLSZ_MOVE_X|DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(0x3021, DLSZ_MOVE_X|DLSZ_MOVE_Y)
	END_DLGRESIZE_MAP()

	void OnSize(UINT, CSize)
	{
        SetMsgHandled(false);

        HWND propTabs = GetDlgItem(0x3020);

		RECT rect;
		::GetWindowRect(propTabs, &rect);

		::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rect, 2);
		::SendMessage(propTabs, TCM_ADJUSTRECT, false, (LPARAM)&rect);

		::MoveWindow(GetActivePage(), rect.left, rect.top,
            rect.right-rect.left, rect.bottom-rect.top, true);
	}

    LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
    {
        if (wParam == TRUE)
        {
            CMenuHandle pSysMenu = GetSystemMenu(FALSE);

        	if (pSysMenu != NULL)
                pSysMenu.InsertMenu(-1, MF_BYPOSITION|MF_STRING, SC_SIZE, L"&Size");

            ModifyStyle(0, WS_THICKFRAME, 0);
            Center();

            resizeClass::DlgResize_Init(false, true, WS_CLIPCHILDREN);
        }
		
		bHandled = FALSE;
		
        return 0;
    }

    void Center(void)
    {
        if (!m_bCentered)
        {
            CenterWindow();
            m_bCentered = true;
        }
    }

private:
    bool m_bCentered;

	GeneralOptions generalOptions;
	BitTorrentOptions bitTorrentOptions;
	SecurityOptions securityOptions;
	TorrentsOptions torrentsOptions;
	RemoteOptions remoteControlOptions;
	AboutOptions aboutOptions;
};

#endif // RC_INVOKED

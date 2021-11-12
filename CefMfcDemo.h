
// CefMfcDemo.h : main header file for the CefMfcDemo application
//
#pragma once

#ifndef __AFXWIN_H__
#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"  // main symbols
#include "include/cef_base.h"
#include "include/cef_app.h" 

// CCefMfcDemoApp:
// See CefMfcDemo.cpp for the implementation of this class
//

class CCefMfcDemoApp : public CWinApp {
 public:
  CCefMfcDemoApp() noexcept;

  // Overrides
 public:
  virtual BOOL InitInstance();
  virtual int ExitInstance();

 private:
  // Implementation
  BOOL PumpMessage() override;

  afx_msg void OnAppAbout();
  DECLARE_MESSAGE_MAP()
  afx_msg void OnOpenBrowser();

  void InitializeCef();

  void UninitializeCef();

 private:
  CefRefPtr<CefApp> m_app;
};

extern CCefMfcDemoApp theApp;

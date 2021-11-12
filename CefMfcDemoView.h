
// CefMfcDemoView.h : interface of the CCefMfcDemoView class
//

#pragma once

#include "ClientHandler.h"

class CCefMfcDemoView : public CView, public ClientHandler::Delegate {
 protected:  // create from serialization only
  CCefMfcDemoView() noexcept;
  DECLARE_DYNCREATE(CCefMfcDemoView)

  // Attributes
 public:
  CCefMfcDemoDoc* GetDocument() const;

  // Operations
 public:
  // Overrides
 public:
  virtual void OnDraw(CDC* pDC);  // overridden to draw this view
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

 protected:
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
  virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

  // Implementation
 public:
  virtual ~CCefMfcDemoView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

 protected:
  // Generated message map functions
 protected:
  DECLARE_MESSAGE_MAP()

 private:
  void OnInitialUpdate() override;

  void OnActivateView(BOOL bActivate,
                      CView* pActivateView,
                      CView* pDeactiveView) override;

  void OnSize(UINT nType, int cx, int cy);

  void InitStartUrl();

  // Called when the browser is created.
  void OnBrowserCreated(CefRefPtr<CefBrowser> browser) override;

  // Called when the browser is closing.
  void OnBrowserClosing(CefRefPtr<CefBrowser> browser) override;

  // Called when the browser has been closed.
  void OnBrowserClosed(CefRefPtr<CefBrowser> browser) override;

  // Set the window URL address.
  void OnSetAddress(std::string const& url) override;

  // Set the window title.
  void OnSetTitle(std::string const& title) override;

  // Set fullscreen mode.
  void OnSetFullscreen(bool const fullscreen) override;

  // Set the loading state.
  void OnSetLoadingState(bool const isLoading,
                         bool const canGoBack,
                         bool const canGoForward) override;

  ClientHandler* m_clientHandler = nullptr;

  CWnd* m_wndMain = nullptr;
  CString m_startUrl;

  CefRefPtr<CefBrowser> m_browser;
};

#ifndef _DEBUG  // debug version in CefMfcDemoView.cpp
inline CCefMfcDemoDoc* CCefMfcDemoView::GetDocument() const {
  return reinterpret_cast<CCefMfcDemoDoc*>(m_pDocument);
}
#endif


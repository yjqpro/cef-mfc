
// CefMfcDemoView.cpp : implementation of the CCefMfcDemoView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview,
// thumbnail and search filter handlers and allows sharing of document code with
// that project.
#ifndef SHARED_HANDLERS
#include "CefMfcDemo.h"
#endif


#include "CefWindowsHelpers.h"
#include "CefMfcDemoDoc.h"
#include "CefMfcDemoView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCefMfcDemoView

IMPLEMENT_DYNCREATE(CCefMfcDemoView, CView)

BEGIN_MESSAGE_MAP(CCefMfcDemoView, CView)
// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
ON_WM_SIZE()
END_MESSAGE_MAP()

// CCefMfcDemoView construction/destruction

CCefMfcDemoView::CCefMfcDemoView() noexcept {
  // TODO: add construction code here
}

CCefMfcDemoView::~CCefMfcDemoView() {}

void CCefMfcDemoView::OnInitialUpdate() {
  CView::OnInitialUpdate();
  InitStartUrl();

  auto rect = RECT{0};
  GetClientRect(&rect);

  CefWindowInfo info;
  info.SetAsChild(GetSafeHwnd(), CefRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top));

  CefBrowserSettings browserSettings;

  // Remove CefBrowserSettings.web_security since it's no longer useful
  // https://bitbucket.org/chromiumembedded/cef/issues/3058/remove-cefbrowsersettingsweb_security
  // browserSettings.web_security = STATE_DISABLED;

  m_clientHandler = new ClientHandler(this);
  m_clientHandler->CreateBrowser(info, browserSettings, CefString(m_startUrl));
}

void CCefMfcDemoView::InitStartUrl() {
  TCHAR path_buffer[_MAX_PATH] = {0};
  TCHAR drive[_MAX_DRIVE] = {0};
  TCHAR dir[_MAX_DIR] = {0};
  TCHAR fname[_MAX_FNAME] = {0};
  TCHAR ext[_MAX_EXT] = {0};

  ::GetModuleFileName(NULL, path_buffer, sizeof(path_buffer));
  auto err = _tsplitpath_s(path_buffer, drive, _MAX_DRIVE, dir, _MAX_DIR, fname,
                           _MAX_FNAME, ext, _MAX_EXT);
  if (err != 0) {
  }

  auto s = CString{dir};
  s += _T("html");
  err = _tmakepath_s(path_buffer, _MAX_PATH, drive, (LPCTSTR)s, _T("index"),
                     _T("html"));
  if (err != 0) {
  }

  m_startUrl = CString{path_buffer};
  m_startUrl.Replace(_T('\\'), _T('/'));
  m_startUrl = CString{_T("file:///")} + m_startUrl;
}

BOOL CCefMfcDemoView::PreCreateWindow(CREATESTRUCT& cs) {
  // TODO: Modify the Window class or styles here by modifying
  //  the CREATESTRUCT cs

  return CView::PreCreateWindow(cs);
}

// CCefMfcDemoView drawing

void CCefMfcDemoView::OnDraw(CDC* /*pDC*/) {
  CCefMfcDemoDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if (!pDoc)
    return;

  // TODO: add draw code for native data here
}

// CCefMfcDemoView printing

BOOL CCefMfcDemoView::OnPreparePrinting(CPrintInfo* pInfo) {
  // default preparation
  return DoPreparePrinting(pInfo);
}

void CCefMfcDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
  // TODO: add extra initialization before printing
}

void CCefMfcDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
  // TODO: add cleanup after printing
}

void CCefMfcDemoView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
   m_wndMain = AfxGetMainWnd();

   return CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CCefMfcDemoView::OnSize(UINT nType, int cx, int cy)
{
   CView::OnSize(nType, cx, cy);

   if(m_clientHandler != nullptr)
   {
      if(m_browser != nullptr)
      {
         auto hwnd = m_browser->GetHost()->GetWindowHandle();
         auto rect = RECT {0};
         GetClientRect(&rect);

         ::SetWindowPos(hwnd, HWND_TOP, rect.left, 
         rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
      }
   }   
}



// CCefMfcDemoView diagnostics

#ifdef _DEBUG
void CCefMfcDemoView::AssertValid() const {
  CView::AssertValid();
}

void CCefMfcDemoView::Dump(CDumpContext& dc) const {
  CView::Dump(dc);
}

CCefMfcDemoDoc* CCefMfcDemoView::GetDocument()
    const  // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCefMfcDemoDoc)));
  return (CCefMfcDemoDoc*)m_pDocument;
}
#endif  //_DEBUG

// Called when the browser is created.
void CCefMfcDemoView::OnBrowserCreated(CefRefPtr<CefBrowser> browser) {
m_browser = browser;
}

// Called when the browser is closing.
void CCefMfcDemoView::OnBrowserClosing(CefRefPtr<CefBrowser> browser) {}

// Called when the browser has been closed.
void CCefMfcDemoView::OnBrowserClosed(CefRefPtr<CefBrowser> browser) {
if(m_browser != nullptr && 
      m_browser->GetIdentifier() == browser->GetIdentifier())
   {
      m_browser = nullptr;

      m_clientHandler->DetachDelegate();
   }
}

// Set the window URL address.
void CCefMfcDemoView::OnSetAddress(std::string const& url) {
   auto main = static_cast<CMainFrame*>(m_wndMain);
   if(main != nullptr)
   {
      auto newurl = CString {url.c_str()};
      if(newurl.Find(m_startUrl) >= 0)
         newurl = "";

      main->SetUrl(newurl);
   }

}

// Set the window title.
void CCefMfcDemoView::OnSetTitle(std::string const& title) {
 ::SetWindowText(m_hWnd, CefString(title).ToWString().c_str());
}

// Set fullscreen mode.
void CCefMfcDemoView::OnSetFullscreen(bool const fullscreen) {
 if(m_browser != nullptr)
   {
      if(fullscreen)
      {
         CefWindowsHelpers::Maximize(m_browser);
      }
      else 
      {
         CefWindowsHelpers::Restore(m_browser);
      }
   }


}

// Set the loading state.
void CCefMfcDemoView::OnSetLoadingState(bool const isLoading,
                                        bool const canGoBack,
                                        bool const canGoForward) {}

// CCefMfcDemoView message handlers

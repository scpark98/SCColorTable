@../Common/claude.md

# SCColorTable 프로젝트 컨텍스트

GDI+ 명명 색상(140여 개) 목록을 두 개의 리스트로 보여주고, ARGB/RGBA/int 값 ↔ 색 이름을
상호 변환·검색하는 개인 도구. 디버거에서 복사한 정수 컬러값의 정체를 확인하는 것이 주 용도.

- 경로: `D:\1.Projects_C++\SCColorTable` (집·회사 동일)
- 활성 구성: Debug|x64
- 빌드·테스트는 **사용자가 F7/F5 로 직접 한다** (Common/claude.md §5B-0). Claude 는 지시받았을 때만 빌드한다.

## 주요 Common 의존

- `Common/colors.h` → `CSCColorList` (색 목록·`get_color()` 이름→색·`get_color_name()` 색→이름)
- `Common/CListCtrl/CSCListCtrl` → `m_list0`, `m_list1` (2026-07-23 `CVtListCtrlEx` 에서 전환)
- `Common/CEdit/SCEdit` → `m_edit_argb`, `m_edit_rgba`, `m_edit_int`, `m_edit_color`
- `Common/CIPAddressCtrl/SCIPAddressCtrl` → `m_ip_rgba`
- `Common/CComboBox/SCComboBox` → `CSearchDlg` 의 키워드 콤보

## Enter 키 처리 (비자명 — 세 곳이 얽힌다)

1. `CSCColorTableDlg::PreTranslateMessage` 가 `VK_RETURN` 을 잡아 `OnBnClickedOk()` 를 부른다.
   실제 IDOK 버튼은 없다. `OnBnClickedOk()` 는 `GetFocus()` 로 어느 입력 칸이었는지 분기한다.
2. `CSCEdit::PreTranslateMessage` 는 `m_is_dynamic_control` 이 true 일 때만 Enter 를 삼키고
   `Message_CSCEdit` 를 부모에 보낸다. 이 앱의 에디트는 전부 DDX 정적 생성이라 플래그가 false →
   Enter 가 dlg 까지 정상 도달한다. (Common `e96201e`, 2026-02-27. 그 이전에는 모든 CSCEdit 이
   Enter 를 삼켜 dlg 의 `OnOK` 가 호출되지 않았다.)
3. `m_ip_rgba` 는 내부 edit 4개가 실제 포커스를 가지므로 `GetParent()` 까지 올려 판정하고,
   값 반영은 `on_message_CSCIPAddressCtrl()` 이 담당한다.

## IDC_EDIT_COLOR

선택된 색의 **이름을 표시**하는 칸이지만 `ES_READONLY` 가 아니라 직접 입력할 수 있다.
2026-08-28 에 여기에 이름을 넣고 Enter 를 치면 `search()` 로 리스트에서 찾아 선택/EnsureVisible
하도록 배선했다 (`OnBnClickedOk()`). 그 전에는 이 배선이 **존재한 적이 없다** — git 전체 히스토리
확인. `CSCColorList::get_color()` 를 쓰지 않은 이유: 이름을 못 찾으면 조용히 Black 을 돌려주므로
오타를 쳐도 Black 이 잡히는 오동작이 된다. `search()` 는 부분일치·F3 순회·미발견 메시지를 이미 갖췄다.

---

## 자체 패치 (Self Patch)

2026-09-07 적용. 공통 모듈 `Common/system/CSCSelfPatch/SCSelfPatch.{h,cpp}` 를 쓴다 —
`InitInstance` 앞부분에서 `m_self_patch.startup()`, `ExitInstance` 에서 `m_self_patch.shutdown()`.

- 서버 폴더: `/download/tools/KoinoTools/SCColorTable` (`ss.linkmemine.com:443`)
- 실행한 그 자리에서 패치한다. 정해진 설치 폴더가 없으므로 exe 를 어디에 두든 바로가기·시작프로그램 등록이 그대로 유효하다.
- 부팅 자동 실행 등록은 하지 않는다.

원리·배포 절차는 <https://koinodoc.atlassian.net/wiki/spaces/2HfRugOJ0HVK/pages/48562184> 참조.

기존에 있던 `check_new_version_itself()` + `run_self_update_batch()` 실험 코드(`if (false)` 로 막혀 있었다)를 이것으로 대체했다.

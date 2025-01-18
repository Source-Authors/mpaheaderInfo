// GNU LESSER GENERAL PUBLIC LICENSE
// Version 3, 29 June 2007
//
// Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>
//
// Everyone is permitted to copy and distribute verbatim copies of this license
// document, but changing it is not allowed.
//
// This version of the GNU Lesser General Public License incorporates the terms
// and conditions of version 3 of the GNU General Public License, supplemented
// by the additional permissions listed below.

#ifndef MPA_HEADER_INFO_DPI_WND_BEHAVIOR_H_
#define MPA_HEADER_INFO_DPI_WND_BEHAVIOR_H_

#include <Windows.h>

/**
 * @brief Window behavior to handle DPI settings.
 */
class DpiWindowBehavior {
 public:
  explicit DpiWindowBehavior(bool applyDpiOnCreate = true);

  /**
   * @brief Callback on WM_CREATE.
   * @param window Window.
   * @return TRUE if success, FALSE otherwise.
   */
  BOOL OnCreateWindow(HWND window);

  /**
   * @brief Callback on WM_DESTROY.
   */
  void OnDestroyWindow();

  /**
   * @brief Callback on WM_DPICHANGED.
   * @param wParam WPARAM.
   * @param lParam LPARAM.
   * @return 0 on handled, non 0 otherwise.
   */
  LRESULT OnWindowDpiChanged(WPARAM wParam, LPARAM lParam);

  /**
   * @brief Apply current DPI to window and children.
   */
  BOOL ApplyDpiToWindow(bool recompute_window_size);

  /**
   * @brief Previous X DPI.
   * @return
   */
  [[nodiscard]] unsigned GetPreviousDpiX() const { return m_previous_dpi_x; }

  /**
   * @brief Previous Y DPI.
   * @return
   */
  [[nodiscard]] unsigned GetPreviousDpiY() const { return m_previous_dpi_y; }

  /**
   * @brief Current X DPI.
   * @return
   */
  [[nodiscard]] unsigned GetCurrentDpiX() const { return m_current_dpi_x; }

  /**
   * @brief Current Y DPI.
   * @return
   */
  [[nodiscard]] unsigned GetCurrentDpiY() const { return m_current_dpi_y; }

  /**
   * @brief Apply scaling on X axis.
   * @param value Value to scale.
   * @return Scaled value.
   */
  [[nodiscard]] int ScaleOnX(int value) const;

  /**
   * @brief Apply scaling on Y axis.
   * @param value Value to scale.
   * @return Scaled value.
   */
  [[nodiscard]] int ScaleOnY(int value) const;

 private:
  HWND m_window_handle;

  unsigned m_previous_dpi_x, m_previous_dpi_y;
  unsigned m_current_dpi_x, m_current_dpi_y;

  const bool m_apply_dpi_on_create;
};

#endif  // !MPA_HEADER_INFO_DPI_WND_BEHAVIOR_H_

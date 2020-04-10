#pragma once
#include "common/gl/program.h"
#include "common/gl/texture.h"
#include "core/host_display.h"
#include "core/host_interface.h"
#include "frontend-common/common_host_interface.h"
#include <SDL.h>
#include <array>
#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include <string>

class System;
class AudioStream;

class INISettingsInterface;

class SDLHostInterface final : public CommonHostInterface
{
public:
  SDLHostInterface();
  ~SDLHostInterface();

  static std::unique_ptr<SDLHostInterface> Create();

  void ReportError(const char* message) override;
  void ReportMessage(const char* message) override;
  bool ConfirmMessage(const char* message) override;

  bool Initialize() override;
  void Shutdown() override;

  void Run();

protected:
  void LoadSettings() override;

  bool AcquireHostDisplay() override;
  void ReleaseHostDisplay() override;
  std::unique_ptr<AudioStream> CreateAudioStream(AudioBackend backend) override;
  std::unique_ptr<ControllerInterface> CreateControllerInterface() override;

  void OnSystemCreated() override;
  void OnSystemPaused(bool paused) override;
  void OnSystemDestroyed() override;
  void OnRunningGameChanged() override;

  std::optional<HostKeyCode> GetHostKeyCode(const std::string_view key_code) const override;
  void UpdateInputMap() override;

private:
  bool HasSystem() const { return static_cast<bool>(m_system); }

#ifdef WIN32
  bool UseOpenGLRenderer() const { return m_settings.gpu_renderer == GPURenderer::HardwareOpenGL; }
#else
  bool UseOpenGLRenderer() const { return true; }
#endif

  static float GetDPIScaleFactor(SDL_Window* window);

  bool CreateSDLWindow();
  void DestroySDLWindow();
  bool CreateDisplay();
  void DestroyDisplay();
  void CreateImGuiContext();
  void UpdateFramebufferScale();

  /// Executes a callback later, after the UI has finished rendering. Needed to boot while rendering ImGui.
  void RunLater(std::function<void()> callback);

  void SaveSettings();
  void UpdateSettings();

  bool IsFullscreen() const override;
  bool SetFullscreen(bool enabled) override;

  // We only pass mouse input through if it's grabbed
  void DrawImGuiWindows() override;
  void DoStartDisc();
  void DoChangeDisc();
  void DoFrameStep();

  void HandleSDLEvent(const SDL_Event* event);
  void ProcessEvents();

  void DrawMainMenuBar();
  void DrawQuickSettingsMenu();
  void DrawDebugMenu();
  void DrawPoweredOffWindow();
  void DrawSettingsWindow();
  void DrawAboutWindow();
  bool DrawFileChooser(const char* label, std::string* path, const char* filter = nullptr);
  void ClearImGuiFocus();

  SDL_Window* m_window = nullptr;
  std::unique_ptr<HostDisplayTexture> m_app_icon_texture;
  std::unique_ptr<INISettingsInterface> m_settings_interface;
  u32 m_run_later_event_id = 0;

  bool m_fullscreen = false;
  bool m_quit_request = false;
  bool m_frame_step_request = false;
  bool m_settings_window_open = false;
  bool m_about_window_open = false;

  // this copy of the settings is modified by imgui
  Settings m_settings_copy;
};

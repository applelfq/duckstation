#include "host_interface_progress_callback.h"
#include "common/log.h"
Log_SetChannel(HostInterfaceProgressCallback);

HostInterfaceProgressCallback::HostInterfaceProgressCallback(HostInterface* intf)
  : BaseProgressCallback(), m_host_interface(intf)
{
}

void HostInterfaceProgressCallback::PushState()
{
  BaseProgressCallback::PushState();
}

void HostInterfaceProgressCallback::PopState()
{
  BaseProgressCallback::PopState();
  Redraw(true);
}

void HostInterfaceProgressCallback::SetCancellable(bool cancellable)
{
  BaseProgressCallback::SetCancellable(cancellable);
  Redraw(true);
}

void HostInterfaceProgressCallback::SetStatusText(const char* text)
{
  BaseProgressCallback::SetStatusText(text);
  Redraw(true);
}

void HostInterfaceProgressCallback::SetProgressRange(u32 range)
{
  u32 last_range = m_progress_range;

  BaseProgressCallback::SetProgressRange(range);

  if (m_progress_range != last_range)
    Redraw(false);
}

void HostInterfaceProgressCallback::SetProgressValue(u32 value)
{
  u32 lastValue = m_progress_value;

  BaseProgressCallback::SetProgressValue(value);

  if (m_progress_value != lastValue)
    Redraw(false);
}

void HostInterfaceProgressCallback::Redraw(bool force)
{
  const int percent =
    static_cast<int>((static_cast<float>(m_progress_value) / static_cast<float>(m_progress_range)) * 100.0f);
  if (percent == m_last_progress_percent && !force)
    return;

  m_last_progress_percent = percent;
  m_host_interface->DisplayLoadingScreen(m_status_text, 0, static_cast<int>(m_progress_range),
                                         static_cast<int>(m_progress_value));
}

void HostInterfaceProgressCallback::DisplayError(const char* message)
{
  Log_ErrorPrint(message);
}

void HostInterfaceProgressCallback::DisplayWarning(const char* message)
{
  Log_WarningPrint(message);
}

void HostInterfaceProgressCallback::DisplayInformation(const char* message)
{
  Log_InfoPrint(message);
}

void HostInterfaceProgressCallback::DisplayDebugMessage(const char* message)
{
  Log_DevPrint(message);
}

void HostInterfaceProgressCallback::ModalError(const char* message)
{
  m_host_interface->ReportError(message);
}

bool HostInterfaceProgressCallback::ModalConfirmation(const char* message)
{
  return m_host_interface->ConfirmMessage(message);
}

u32 HostInterfaceProgressCallback::ModalPrompt(const char* message, u32 num_options, ...)
{
  Log_InfoPrint(message);
  return 0;
}

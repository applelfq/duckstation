#include "generalsettingswidget.h"
#include "settingsdialog.h"
#include "settingwidgetbinder.h"

GeneralSettingsWidget::GeneralSettingsWidget(QtHostInterface* host_interface, QWidget* parent, SettingsDialog* dialog)
  : QWidget(parent), m_host_interface(host_interface)
{
  m_ui.setupUi(this);

  SettingWidgetBinder::BindWidgetToBoolSetting(m_host_interface, m_ui.pauseOnStart, "Main", "StartPaused", false);
  SettingWidgetBinder::BindWidgetToBoolSetting(m_host_interface, m_ui.startFullscreen, "Main", "StartFullscreen",
                                               false);
  SettingWidgetBinder::BindWidgetToBoolSetting(m_host_interface, m_ui.renderToMain, "Main", "RenderToMainWindow", true);
  SettingWidgetBinder::BindWidgetToBoolSetting(m_host_interface, m_ui.saveStateOnExit, "Main", "SaveStateOnExit", true);
  SettingWidgetBinder::BindWidgetToBoolSetting(m_host_interface, m_ui.confirmPowerOff, "Main", "ConfirmPowerOff", true);
  SettingWidgetBinder::BindWidgetToBoolSetting(m_host_interface, m_ui.loadDevicesFromSaveStates, "Main",
                                               "LoadDevicesFromSaveStates", false);
  SettingWidgetBinder::BindWidgetToBoolSetting(m_host_interface, m_ui.showOSDMessages, "Display", "ShowOSDMessages",
                                               true);
  SettingWidgetBinder::BindWidgetToBoolSetting(m_host_interface, m_ui.showFPS, "Display", "ShowFPS", false);
  SettingWidgetBinder::BindWidgetToBoolSetting(m_host_interface, m_ui.showVPS, "Display", "ShowVPS", false);
  SettingWidgetBinder::BindWidgetToBoolSetting(m_host_interface, m_ui.showSpeed, "Display", "ShowSpeed", false);

  SettingWidgetBinder::BindWidgetToBoolSetting(m_host_interface, m_ui.enableSpeedLimiter, "Main", "SpeedLimiterEnabled",
                                               true);
  SettingWidgetBinder::BindWidgetToBoolSetting(m_host_interface, m_ui.increaseTimerResolution, "Main",
                                               "IncreaseTimerResolution", true);
  SettingWidgetBinder::BindWidgetToNormalizedSetting(m_host_interface, m_ui.emulationSpeed, "Main", "EmulationSpeed",
                                                     100.0f, 1.0f);

  connect(m_ui.enableSpeedLimiter, &QCheckBox::stateChanged, this,
          &GeneralSettingsWidget::onEnableSpeedLimiterStateChanged);
  connect(m_ui.emulationSpeed, &QSlider::valueChanged, this, &GeneralSettingsWidget::onEmulationSpeedValueChanged);

  onEnableSpeedLimiterStateChanged();
  onEmulationSpeedValueChanged(m_ui.emulationSpeed->value());

  dialog->registerWidgetHelp(m_ui.confirmPowerOff, "Confirm Power Off", "Checked",
                             "Determines whether a prompt will be displayed to confirm shutting down the emulator/game "
                             "when the hotkey is pressed.");
  dialog->registerWidgetHelp(m_ui.saveStateOnExit, "Save State On Exit", "Checked",
                             "Automatically saves the emulator state when powering down or exiting. You can then "
                             "resume directly from where you left off next time.");
  dialog->registerWidgetHelp(m_ui.startFullscreen, "Start Fullscreen", "Unchecked",
                             "Automatically switches to fullscreen mode when a game is started.");
  dialog->registerWidgetHelp(m_ui.renderToMain, "Render To Main Window", "Checked",
                             "Renders the display of the simulated console to the main window of the application, over "
                             "the game list. If unchecked, the display will render in a seperate window.");
  dialog->registerWidgetHelp(m_ui.pauseOnStart, "Pause On Start", "Unchecked",
                             "Pauses the emulator when a game is started.");
  dialog->registerWidgetHelp(
    m_ui.loadDevicesFromSaveStates, "Load Devices From Save States", "Unchecked",
    "When enabled, memory cards and controllers will be overwritten when save states are loaded. This can "
    "result in lost saves, and controller type mismatches. For deterministic save states, enable this option, "
    "otherwise leave disabled.");
  dialog->registerWidgetHelp(m_ui.enableSpeedLimiter, "Enable Speed Limiter", "Checked",
                             "Throttles the emulation speed to the chosen speed above. If unchecked, the emulator will "
                             "run as fast as possible, which may not be playable.");
  dialog->registerWidgetHelp(m_ui.increaseTimerResolution, "Increase Timer Resolution", "Checked",
                             "Increases the system timer resolution when emulation is started to provide more accurate "
                             "frame pacing. May increase battery usage on laptops.");
  dialog->registerWidgetHelp(m_ui.emulationSpeed, "Emulation Speed", "100%",
                             "Sets the target emulation speed. It is not guaranteed that this speed will be reached, "
                             "and if not, the emulator will run as fast as it can manage.");
  dialog->registerWidgetHelp(m_ui.showOSDMessages, "Show OSD Messages", "Checked",
                             "Shows on-screen-display messages when events occur such as save states being "
                             "created/loaded, screenshots being taken, etc.");
  dialog->registerWidgetHelp(m_ui.showFPS, "Show FPS", "Unchecked",
                             "Shows the internal frame rate of the game in the top-right corner of the display.");
  dialog->registerWidgetHelp(m_ui.showVPS, "Show VPS", "Unchecked",
                             "Shows the number of frames (or v-syncs) displayed per second by the system in the "
                             "top-right corner of the display.");
  dialog->registerWidgetHelp(
    m_ui.showSpeed, "Show Speed", "Unchecked",
    "Shows the current emulation speed of the system in the top-right corner of the display as a percentage.");

  // Since this one is compile-time selected, we don't put it in the .ui file.
#ifdef WITH_DISCORD_PRESENCE
  {
    QCheckBox* enableDiscordPresence = new QCheckBox(tr("Enable Discord Presence"), m_ui.groupBox_4);
    SettingWidgetBinder::BindWidgetToBoolSetting(m_host_interface, enableDiscordPresence, "Main",
                                                 "EnableDiscordPresence");
    m_ui.formLayout_4->addWidget(enableDiscordPresence, m_ui.formLayout_4->rowCount(), 0);
    dialog->registerWidgetHelp(enableDiscordPresence, "Enable Discord Presence", "Unchecked",
                               "Shows the game you are currently playing as part of your profile in Discord.");
  }
#endif
}

GeneralSettingsWidget::~GeneralSettingsWidget() = default;

void GeneralSettingsWidget::onEnableSpeedLimiterStateChanged()
{
  m_ui.emulationSpeed->setDisabled(!m_ui.enableSpeedLimiter->isChecked());
}

void GeneralSettingsWidget::onEmulationSpeedValueChanged(int value)
{
  m_ui.emulationSpeedLabel->setText(tr("%1%").arg(value));
}

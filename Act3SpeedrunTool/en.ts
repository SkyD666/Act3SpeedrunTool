<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="en_US" sourcelanguage="zh_CN">
<context>
    <name>DescriptionDialog</name>
    <message>
        <location filename="DescriptionDialog.ui" line="+14"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_DescriptionDialog.h" line="+305"/>
        <source>工具说明</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_DescriptionDialog.h" line="+13"/>
        <source>断网</source>
        <translation type="unfinished">Firewall</translation>
    </message>
    <message>
        <location line="+15"/>
        <location line="+94"/>
        <location line="+260"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_DescriptionDialog.h" line="-11"/>
        <location line="+16"/>
        <location line="+23"/>
        <source>基本原理</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-348"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_DescriptionDialog.h" line="-40"/>
        <source>通过 Windows 防火墙阻止进程联网。</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+32"/>
        <location line="+134"/>
        <location line="+129"/>
        <location line="+91"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_DescriptionDialog.h" line="+11"/>
        <location line="+15"/>
        <location line="+11"/>
        <location line="+11"/>
        <source>常见问题</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-348"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_DescriptionDialog.h" line="-46"/>
        <source>1. 启动/停止热键能否相同？
答：可以相同。

2. 使用加速器的情况下如何设置防火墙断网范围？
答：请见“设置-断网”页面下方说明。

3. 防火墙启动后，发生了电脑死机/工具闪退/工具被强制杀死等情况，发现无法联网怎么办？
答：手动打开 Windows 防火墙页面，删除所有名称为“AutoFirewall”的规则。</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+35"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_DescriptionDialog.h" line="+25"/>
        <source>爆头数</source>
        <translation type="unfinished">Headshot</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>GTA5 进程内使用整数来存储爆头数，因此只需读取 GTA5 进程的内存即可得到爆头数据。

由于不同任务中记录爆头数的内存地址可能不同，或者根本没有专门记录爆头数，因此该功能只能在“末日将至：三号行动”中使用。</source>
        <oldsource>GTA5 进程内使用整数来存储爆头数，因此只需读取 GTA5 进程的内存即可得到爆头数据。由于不同任务中记录爆头数的内存地址可能不同，或者根本没有专门记录爆头数，因此该功能只能在“末日将至：三号行动”中使用。</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+34"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_DescriptionDialog.h" line="-8"/>
        <source>注意事项</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_DescriptionDialog.h" line="-1"/>
        <source>由于每次大更新后，GTA5 进程的内存布局可能会发生变化，因此在大更新后，此功能可能会失效，请及时更新最新版本。</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+38"/>
        <source>1. 每次结束任务后是否需要重启计数器？
答：不需要。计数器每隔一个“刷新间隔”便会去读取内存的数据，当重新开始任务后，内存中的数据还是爆头数，因此会自动显示最新数据。

2. 为什么在任务外爆头数会出现奇怪的数字？
答：在任务外，原来存放爆头数的那部分内存空间会被存放其他数据，因此在任务外，爆头数显示的数据没有意义。</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+47"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_DescriptionDialog.h" line="+11"/>
        <source>计时基本原理</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_DescriptionDialog.h" line="-1"/>
        <source>启动时记录当前时间，每隔一个“刷新间隔”便会计算当前时间与开始时间的差值，差值即为计时时间。</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+32"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_DescriptionDialog.h" line="+3"/>
        <source>停止归零策略基本原理</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_DescriptionDialog.h" line="-1"/>
        <source>使用状态机实现，状态机维护了当前计时器的状态。指定不同的停止归零策略代表连通/断开状态机的部分有向边。</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+38"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_DescriptionDialog.h" line="+2"/>
        <source>1. 启动/停止计时的热键能否相同？
答：可以。

2. 为什么启动/停止计时的热键可以相同，而暂停/恢复的热键不能与前两者相同？
答：若相同，则不能表示想要完成的动作。</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+53"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_DescriptionDialog.h" line="+7"/>
        <source>在本机启动了一个 HTTP 服务器，通过 WebSocket 实时向其他终端传输最新数据。您可以通过浏览器或其他方式来显示数据。</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+38"/>
        <source>1. 队友能否不开启本软件，通过网络直接访问本机（本工具）数据？
答：理论上可以，但不推荐。若您的计算机处于内网环境，您可以在路由器上配置端口映射，让队友使用公网 IP 和设定的端口访问本机。但不建议这样做，因为网络传输有一定的延迟，本地最新的数据传输到队友端可能需要一段时间，队友体验不佳。此外，这样做还可能会带来安全风险。

2. 不喜欢默认的网页显示布局，能否自行编写浏览器前端页面/客户端来显示数据？
答：可以。您可以开启自己的 WebSocket 客户端，用来接收实时数据，然后通过自己喜欢的方式显示数据。</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-188"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_DescriptionDialog.h" line="-1"/>
        <source>计时器</source>
        <translation type="unfinished">Timer</translation>
    </message>
    <message>
        <location line="+129"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_DescriptionDialog.h" line="+11"/>
        <source>多端显示</source>
        <translation type="unfinished">Multi-platform display</translation>
    </message>
    <message>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_DescriptionDialog.h" line="-36"/>
        <source>GTA5 进程内使用整数来存储爆头数，因此只需读取 GTA5 进程的内存即可得到爆头数据。

由于不同任务中记录爆头数的内存地址可能不同，或者根本没有专门记录爆头数，因此该功能只能在“末日将至���三号行动”中使用。</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>1. 每次结束任务后是否需要重启计数器？
答：不需要。计数器每隔一个“刷新间隔”便会去读取内存的数据，当重新开始任务后，内存中的数据还是爆头数，因此会自动显示最新数据。

2. 为什么���任务外爆头数会出现奇怪的数字？
答：在任务外，原来存放爆头数的那部分内存空间会被存放其他数据，因此在任务外，爆头数显示的数据没有意义。</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+21"/>
        <source>1. 队友能否不开启本软件，通过网络直接访问本机（本工具）数据？
答：理论上可以，但不推荐。若您的计算机处于内网环境，您可以在路由器上配置端口映射，让队友使用公网 IP 和设定的端口访���本机。但不建议这样做，因为网络传输有一定的延迟，本地最新的数据传输到队友端可能需要一段时间，队友体验不佳。此外，这样做还可能会带来安全风险。

2. 不喜欢默认的网页显示布局，能否自行编写浏览器前端页面/客户端来显示数据？
答：可以。您可以开启自己的 WebSocket 客户端，用来接收实时数据，然后通过自己喜欢的方式显示数据。</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DisplayInfoDialog</name>
    <message>
        <location filename="DisplayInfoDialog.ui" line="+20"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_DisplayInfoDialog.h" line="+93"/>
        <source>信息展示</source>
        <translation>Display Info</translation>
    </message>
</context>
<context>
    <name>LanguageUtil</name>
    <message>
        <location filename="LanguageUtil.cpp" line="+32"/>
        <source>跟随系统</source>
        <translation>Follow System</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>简体中文</source>
        <translation>Simplified Chinese</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>英文</source>
        <translation>English</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="MainWindow.ui" line="+22"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+349"/>
        <source>断网</source>
        <translation>Firewall</translation>
    </message>
    <message>
        <location line="+23"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="-1"/>
        <source>播放提示音</source>
        <translation>Play sound</translation>
    </message>
    <message>
        <location line="-12"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="-3"/>
        <source>代言人：Adstargod
星神断网，好用没烦恼
😋</source>
        <translation>Spokesperson: Adstargod
AdstarGod Firewall, easy to use
😋</translation>
    </message>
    <message>
        <location line="+39"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+6"/>
        <source>爆头数</source>
        <translation>Headshot</translation>
    </message>
    <message>
        <location line="+59"/>
        <location line="+65"/>
        <location filename="MainWindow.cpp" line="+107"/>
        <location line="+535"/>
        <location line="+22"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="-1"/>
        <location line="+2"/>
        <source>点击启动</source>
        <translation>Start counting</translation>
    </message>
    <message>
        <location line="-54"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+2"/>
        <source>计时器</source>
        <translation>Timer</translation>
    </message>
    <message>
        <location line="+67"/>
        <location filename="MainWindow.cpp" line="-20"/>
        <location line="+16"/>
        <location line="+20"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="-1"/>
        <source>点击暂停</source>
        <translation>Click to pause</translation>
    </message>
    <message>
        <location line="+21"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+2"/>
        <source>帮助(&amp;H)</source>
        <translation>&amp;Help</translation>
    </message>
    <message>
        <location line="+27"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+2"/>
        <source>工具(&amp;T)</source>
        <translation>&amp;Tool</translation>
    </message>
    <message>
        <location line="+6"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+1"/>
        <source>视图(&amp;V)</source>
        <translation>&amp;View</translation>
    </message>
    <message>
        <location line="+4"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+1"/>
        <source>信息展示</source>
        <translation>Display info</translation>
    </message>
    <message>
        <location line="+8"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+1"/>
        <source>多端显示</source>
        <translation>Multi-platform display</translation>
    </message>
    <message>
        <location line="+135"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="-20"/>
        <source>爱发电...</source>
        <translation>Ai Fa Dian...</translation>
    </message>
    <message>
        <location line="+5"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+1"/>
        <source>Buy Me A Coffee...</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+1"/>
        <source>支付宝...</source>
        <translation>Alipay...</translation>
    </message>
    <message>
        <location line="+5"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+1"/>
        <source>检查更新...</source>
        <translation>Check for update...</translation>
    </message>
    <message>
        <location line="+5"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+1"/>
        <source>工具说明...</source>
        <oldsource>说明...</oldsource>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-130"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="-21"/>
        <source>关于 Qt...</source>
        <translation>About Qt...</translation>
    </message>
    <message>
        <location line="+9"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+1"/>
        <source>关于本软件...</source>
        <translation>About...</translation>
    </message>
    <message>
        <location line="+5"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+1"/>
        <source>访问 GitHub 仓库...</source>
        <translation>GitHub...</translation>
    </message>
    <message>
        <location line="+9"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+1"/>
        <source>设置(&amp;S)...</source>
        <translation>&amp;Setting...</translation>
    </message>
    <message>
        <location line="+3"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+2"/>
        <source>Ctrl+P</source>
        <translation></translation>
    </message>
    <message>
        <location line="+9"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+2"/>
        <source>查看日志...</source>
        <translation>Open log file...</translation>
    </message>
    <message>
        <location line="+9"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+1"/>
        <source>打开日志文件夹...</source>
        <translation>Open log dir...</translation>
    </message>
    <message>
        <location line="+30"/>
        <location filename="MainWindow.cpp" line="-326"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+4"/>
        <source>复制地址</source>
        <translation>Copy link</translation>
    </message>
    <message>
        <location line="+8"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+1"/>
        <source>运行</source>
        <translation>Run</translation>
    </message>
    <message>
        <location line="+9"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+1"/>
        <source>在浏览器中打开...</source>
        <translation>Open in browser...</translation>
    </message>
    <message>
        <location line="+5"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+1"/>
        <source>退出(&amp;X)</source>
        <translation>E&amp;xit</translation>
    </message>
    <message>
        <location line="+9"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+1"/>
        <source>显示(&amp;D)...</source>
        <oldsource>显示(&amp;S)...</oldsource>
        <translation>&amp;Display...</translation>
    </message>
    <message>
        <location line="-53"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="-7"/>
        <source>显示...</source>
        <translation>Show...</translation>
    </message>
    <message>
        <location line="+8"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+1"/>
        <source>可拖动</source>
        <translation>Moveable</translation>
    </message>
    <message>
        <location line="+5"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+1"/>
        <source>帮助我们进行翻译...</source>
        <translation>Help us translate...</translation>
    </message>
    <message>
        <location filename="MainWindow.cpp" line="-293"/>
        <location line="+17"/>
        <source>已关闭</source>
        <translation>Stopped</translation>
    </message>
    <message>
        <location line="-9"/>
        <source>已开启</source>
        <translation>Running</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>点击关闭</source>
        <translation>Stop counting</translation>
    </message>
    <message>
        <location line="+256"/>
        <source>服务器未运行</source>
        <translation>Server is not running</translation>
    </message>
    <message>
        <location line="+11"/>
        <location line="+10"/>
        <source>获取服务器地址失败！</source>
        <translation>Failed to get the server address!</translation>
    </message>
    <message>
        <location line="-7"/>
        <source>已复制服务器地址！</source>
        <translation>Server address has been copied!</translation>
    </message>
    <message>
        <location line="+54"/>
        <source>支付宝收款二维码</source>
        <translation>Alipay QR code for receiving payments</translation>
    </message>
    <message>
        <location line="+247"/>
        <source>点击归零</source>
        <translation>Click to Zero</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>点击停止</source>
        <translation>Click to stop</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>点击恢复</source>
        <translation>Click to continue</translation>
    </message>
    <message>
        <location line="-463"/>
        <source>注册启用防火墙热键失败！</source>
        <oldsource>注册防火墙启动热键失败！</oldsource>
        <translation>Failed to register enable firewall hotkeys!</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>注册关闭防火墙热键失败！</source>
        <oldsource>注册防火墙停止热键失败！</oldsource>
        <translation>Failed to register disable firewall hotkey!</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>注册启动计时器热键失败！</source>
        <oldsource>注册计时器启动热键失败！</oldsource>
        <translation>Failed to register start timer hotkey!</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>注册停止计时器热键失败！</source>
        <oldsource>注册计时器停止热键失败！</oldsource>
        <translation>Failed to register stop timer hotkey!</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>暂停计时器热键与启动/停止计时器热键相同，暂停计时器热键将会无效！</source>
        <oldsource>暂停热键与启动/停止热键相同，暂停热键设置无效！</oldsource>
        <translation>The pause timer hotkey is the same as the start/stop timer hotkey; the pause timer hotkey is not valid!</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>注册暂停计时器热键失败！</source>
        <oldsource>注册计时器暂停热键失败！</oldsource>
        <translation>Failed to register the pause timer hotkey!</translation>
    </message>
    <message>
        <location line="+190"/>
        <source>获取窗口句柄失败，请启动或重启游戏后再进行尝试！</source>
        <oldsource>获取窗口句柄失败！</oldsource>
        <translation>Failed to get the game window handle, please start or restart the game and try again!</translation>
    </message>
    <message>
        <location line="+43"/>
        <source>已停止记录</source>
        <translation>Stopped</translation>
    </message>
    <message>
        <location filename="MainWindow.h" line="+105"/>
        <source>&lt;p&gt;一个 GTAOL 末日三速通工具，支持快速启动防火墙、自动统计爆头数、手动计时。本软件仅读取内存，不会修改任何内存。&lt;/p&gt;</source>
        <oldsource>&lt;p&gt;一个 GTAOL 末日三速通工具，支持快速启动防火墙、自动统计爆头数。本软件仅读取内存，不会修改任何内存。&lt;/p&gt;</oldsource>
        <translation>&lt;p&gt;A GTAOL doomsday scenario speedrun tool that supports quick startup of the firewall and automatic counting of headshots, and manual timing. This tool only reads the memory and does not write the memory.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>作者：%1</source>
        <oldsource>作者：</oldsource>
        <translation>Author: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>加入 Discord 一起划水~</source>
        <translation>Join Discord~</translation>
    </message>
    <message>
        <location filename="MainWindow.ui" line="-131"/>
        <location filename="MainWindow.h" line="+1"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_MainWindow.h" line="+22"/>
        <source>赞助作者</source>
        <translation>Sponsor</translation>
    </message>
    <message>
        <location filename="MainWindow.h" line="+1"/>
        <source>特别感谢 &lt;b&gt;ACT3 帮会&lt;/b&gt;对本工具的宣传和支持。特别感谢 &lt;b&gt;CZ9946 和 Adstargod&lt;/b&gt; 两位大佬，他们对本工具的研发提供了大量的帮助！</source>
        <translation>Special thanks to the &lt;b&gt;World Save Act Elite&lt;/b&gt; for promoting and supporting this tool. Special thanks to &lt;b&gt;CZ9946 and Adstargod&lt;/b&gt;, who provided a lot of help in the development of this tool!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>感谢所有提供赞助的朋友们！</source>
        <translation>Thanks to all those who sponsored!</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="main.cpp" line="+21"/>
        <source>末日三速通工具</source>
        <translation>Act3 Speedrun Tool</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>*** 应用版本变更，请认真检查各项配置（热键、外观、提示音等）是否需要重新设置！***</source>
        <translation>*** The application version has changed, please check carefully if the configurations (hotkeys, appearance, sounds, etc.) need to be reset! ***</translation>
    </message>
    <message>
        <location filename="DisplayInfoSubFunction.cpp" line="+13"/>
        <source>断网</source>
        <translation>Firewall</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>爆头</source>
        <translation>Headshot</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>计时器</source>
        <translation>Timer</translation>
    </message>
    <message>
        <location filename="GlobalData.cpp" line="+27"/>
        <source>停止后时间不归零</source>
        <translation>Time does not reset to zero after stopping</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>停止后时间立即归零</source>
        <translation>Time resets to zero immediately after stopping</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>停止后，再按一次停止键，时间才会归零</source>
        <oldsource>停止后，再按一次停止，时间才会归零</oldsource>
        <translation>After stopping, press stop again to reset the time to zero</translation>
    </message>
</context>
<context>
    <name>SettingDialog</name>
    <message>
        <location filename="SettingDialog.ui" line="+14"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+1119"/>
        <source>设置</source>
        <translation>Settings</translation>
    </message>
    <message>
        <location line="+92"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+4"/>
        <source>windowsvista 样式不支持深色模式，其他样式支持深色模式跟随系统</source>
        <translation>windowsvista style does not support dark mode, while other styles support dark mode following the system</translation>
    </message>
    <message>
        <location line="+593"/>
        <location line="+354"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+24"/>
        <location line="+24"/>
        <source>启动</source>
        <translation>Enable</translation>
    </message>
    <message>
        <location line="-339"/>
        <location line="+354"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="-23"/>
        <location line="+24"/>
        <source>停止</source>
        <translation>Disable</translation>
    </message>
    <message>
        <location line="-336"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="-23"/>
        <source>音效</source>
        <translation>Sound</translation>
    </message>
    <message>
        <location line="+93"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+5"/>
        <source>默认值为出站，设置将在下次启动防火墙时生效。</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+1"/>
        <source>断网范围</source>
        <translation>Firewall scope</translation>
    </message>
    <message>
        <location line="+21"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+3"/>
        <source>说明</source>
        <translation>Tips</translation>
    </message>
    <message>
        <location line="+6"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="-2"/>
        <source>若您想断整个系统的网络，则留空；若您只想断 GTA 的网络，请选择 GTA5.exe 文件；若您想断加速器的网络，请选择加速器的应用程序文件。</source>
        <translation>Leave it blank if you want to disconnect the entire system&apos;s network, select the GTA5.exe file if you only want to disconnect GTA&apos;s network, or select the game network agent&apos;s application file if you want to disconnect the game network agent&apos;s network.</translation>
    </message>
    <message>
        <location line="+10"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+1"/>
        <source>填写要断网程序的路径，留空表示断全部。设置将在下次启动防火墙时生效。</source>
        <oldsource>填写要断网程序的路径，留空表示断全部。设置将在下次启动防火墙后生效。</oldsource>
        <translation type="unfinished">Enter the path of the program you want to disconnect, leave it blank to disconnect all. The settings will take effect after the next firewall startup.</translation>
    </message>
    <message>
        <location line="+11"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+10"/>
        <source>加速器 (UU 用户必看)</source>
        <translation>Game network agent</translation>
    </message>
    <message>
        <location line="+6"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="-8"/>
        <source>使用 UU 加速器的用户请填写 GTA5.exe 的完整路径，然后在 UU 内使用“路由模式”进行加速。
GTA5.exe 的完整路径例如：
F:\Epic Games\GTAV\GTA5.exe

其他用户可以直接填写加速器可执行 EXE 文件的路径。
加速器可执行 EXE 文件的路径例如：
C:\Program Files (x86)\nn\nn.exe</source>
        <oldsource>使用 UU 加速器的用户请填写 GTA5.exe 的完整路径，然后在 UU 内使用”路由模式“进行加速。
GTA5.exe 的完整路径例如：
F:\Epic Games\GTAV\GTA5.exe

其他用户可以直接填写加速器可执行 EXE 文件的路径。
加速器可执行 EXE 文件的路径例如：
C:\Program Files (x86)\nn\nn.exe</oldsource>
        <translation>If you are using UU network agent, please fill in the full path of GTA5.exe, and then use the &quot;Route Mode&quot; in UU.
The full path of GTA5.exe is for example:
F:\Epic Games\GTAV\GTA5.exe

Other users can directly fill in the path of the game network agent executable EXE file.
The path to the game network agent executable EXE file is for example:
C:\Program Files (x86)\nn\nn.exe</translation>
    </message>
    <message>
        <location line="-73"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="-6"/>
        <source>防火墙规则方向</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+164"/>
        <location line="+157"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+16"/>
        <location line="+6"/>
        <source>建议设置为50-200之间，配置低的用户请适当增大数值</source>
        <translation>Suggested setting is between 50 to 200. Users with lower computer performance should consider increasing the value accordingly</translation>
    </message>
    <message>
        <location line="-96"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="-5"/>
        <source>计时器热键</source>
        <translation>Timer hotkeys</translation>
    </message>
    <message>
        <location line="+42"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+3"/>
        <source>暂停/恢复</source>
        <translation>Pause/resume</translation>
    </message>
    <message>
        <location line="-136"/>
        <location line="+157"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="-5"/>
        <location line="+6"/>
        <source>刷新间隔</source>
        <translation>Refresh data interval</translation>
    </message>
    <message>
        <location line="-632"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="-29"/>
        <source>窗口</source>
        <translation>Window</translation>
    </message>
    <message>
        <location line="+35"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="-7"/>
        <source>X 坐标</source>
        <translation>X</translation>
    </message>
    <message>
        <location line="+18"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+1"/>
        <source>Y 坐标</source>
        <translation>Y</translation>
    </message>
    <message>
        <location line="+21"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+1"/>
        <source>宽度</source>
        <translation>Width</translation>
    </message>
    <message>
        <location line="+18"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+1"/>
        <source>高度</source>
        <translation>Height</translation>
    </message>
    <message>
        <location line="-266"/>
        <location line="+106"/>
        <location line="+187"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="-12"/>
        <location line="+4"/>
        <location line="+9"/>
        <source>颜色</source>
        <translation>Color</translation>
    </message>
    <message>
        <location line="+25"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+2"/>
        <source>透明度</source>
        <translation>Alpha</translation>
    </message>
    <message>
        <location line="-460"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="-7"/>
        <source>内容</source>
        <translation>Content</translation>
    </message>
    <message>
        <location line="-73"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="-17"/>
        <source>最小化到托盘</source>
        <translation>Minimize to the system tray</translation>
    </message>
    <message>
        <location line="+7"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+1"/>
        <source>启动时自动检查更新</source>
        <translation>Automatically check for update on application startup</translation>
    </message>
    <message>
        <location line="+7"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+1"/>
        <source>外观样式</source>
        <oldsource>样式</oldsource>
        <translation>Style</translation>
    </message>
    <message>
        <location line="+106"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+2"/>
        <source>选择要设置的功能</source>
        <translation>Select function</translation>
    </message>
    <message>
        <location line="+25"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+1"/>
        <source>注：该页设置仅针对所选的功能</source>
        <oldsource>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;注：该页设置仅针对所选的功能&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</oldsource>
        <translation>NOTE: The settings on this page are only for the selected function</translation>
    </message>
    <message>
        <location line="+13"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+1"/>
        <source>在屏幕上显示</source>
        <translation>Display on screen</translation>
    </message>
    <message>
        <location line="+13"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+1"/>
        <source>字体</source>
        <translation>Font</translation>
    </message>
    <message>
        <location line="+17"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+1"/>
        <source>字号</source>
        <translation>Size</translation>
    </message>
    <message>
        <location line="+55"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+2"/>
        <source>文字阴影</source>
        <translation>Text shadow</translation>
    </message>
    <message>
        <location line="+100"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+4"/>
        <source>文字对齐</source>
        <translation>Text alignment</translation>
    </message>
    <message>
        <location line="+261"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+12"/>
        <source>断网热键</source>
        <translation>Firewall hotkey</translation>
    </message>
    <message>
        <location line="-347"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="-15"/>
        <source>模糊半径</source>
        <translation>Blur radius</translation>
    </message>
    <message>
        <location line="+27"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+1"/>
        <source>偏移</source>
        <translation>Offset</translation>
    </message>
    <message>
        <location line="+71"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+3"/>
        <source>水平</source>
        <translation>Horizontal</translation>
    </message>
    <message>
        <location line="+19"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+1"/>
        <source>垂直</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <location line="+303"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+14"/>
        <source>启动音效</source>
        <translation>Start</translation>
    </message>
    <message>
        <location line="+29"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+1"/>
        <source>停止音效</source>
        <translation>Stop</translation>
    </message>
    <message>
        <location line="+7"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+1"/>
        <source>操作失败音效</source>
        <translation>Operation failed</translation>
    </message>
    <message>
        <location line="+354"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+24"/>
        <source>停止策略</source>
        <translation>Stop strategy</translation>
    </message>
    <message>
        <location line="+9"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+1"/>
        <source>建议在停止（归零）计时后再更改此项，以便首页按钮上的提示信息含义准确</source>
        <translation>It is recommended to change this item after stopping (zeroing) the timer so that the message on the home button is more accurate</translation>
    </message>
    <message>
        <location line="+10"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+1"/>
        <source>注：无论选择何种策略，从“暂停”状态到“停止”状态后计时器都会归零</source>
        <oldsource>注：无论选择何种策略，从”暂停“状态到”停止“状态后计时器都会归零</oldsource>
        <translation>Note: The timer will be reset to zero after going from &quot;Pause&quot; to &quot;Stop&quot; no matter which strategy is selected</translation>
    </message>
    <message>
        <location line="+64"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_SettingDialog.h" line="+1"/>
        <source>切换语言后需重启才可生效</source>
        <translation>Application needs to be restarted after switching languages</translation>
    </message>
    <message>
        <location filename="SettingDialog.h" line="+14"/>
        <source>信息展示</source>
        <oldsource>全局热键</oldsource>
        <translation>Display info</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>语言</source>
        <translation>Language</translation>
    </message>
    <message>
        <location filename="SettingDialog.cpp" line="+82"/>
        <source>选择文件</source>
        <translation>Choice a sound</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>WAV 文件 (*.wav)</source>
        <translation>WAV file (*.wav)</translation>
    </message>
    <message>
        <location line="+93"/>
        <source>选择程序</source>
        <translation>Select a exe file</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>应用程序 (*.exe)</source>
        <translation>Executable file (*.exe)</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>入站</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>出站</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="SettingDialog.h" line="+0"/>
        <source>断网</source>
        <translation>Firewall</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>计时器</source>
        <translation>Timer</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>爆头</source>
        <translation>Headshot</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>常规</source>
        <translation>General</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>左对齐</source>
        <translation>Left alignment</translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+6"/>
        <source>居中</source>
        <translation>Horizontal centering</translation>
    </message>
    <message>
        <location line="-5"/>
        <source>右对齐</source>
        <translation>Right alignment</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>上对齐</source>
        <translation>Top alignment</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>下对齐</source>
        <translation>Bottom alignment</translation>
    </message>
</context>
<context>
    <name>UpdateDialog</name>
    <message>
        <location filename="UpdateDialog.ui" line="+14"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_UpdateDialog.h" line="+135"/>
        <source>检查更新</source>
        <translation>Check for update</translation>
    </message>
    <message>
        <location line="+90"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_UpdateDialog.h" line="+1"/>
        <source>下载...</source>
        <translation>Download...</translation>
    </message>
    <message>
        <location line="+16"/>
        <location filename="../build-MinSizeRel/Act3SpeedrunTool_autogen/include/ui_UpdateDialog.h" line="+1"/>
        <source>忽略此版本</source>
        <translation>Ignore this version</translation>
    </message>
    <message>
        <location filename="UpdateDialog.cpp" line="+19"/>
        <source>当前版本：%1</source>
        <translation>Current version: %1</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>错误码：%1
错误信息：%2</source>
        <oldsource>错误码：%1
错误信息：%s</oldsource>
        <translation>Error code: %1
Error message: %2</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>获取更新信息失败！</source>
        <translation>Failed to retrieve update information!</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>正在检查更新，请稍候...</source>
        <translation>Checking for update, please wait...</translation>
    </message>
    <message>
        <location line="+73"/>
        <source>发现新版本：%1</source>
        <translation>New version: %1</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>发布时间：%1</source>
        <translation>Release date: %1</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>当前已是最新版本！</source>
        <translation>You are already using the latest version!</translation>
    </message>
</context>
</TS>

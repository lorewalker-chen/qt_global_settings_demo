#include "global_settings.h"

#include <QFileSystemWatcher>
#include <QSettings>
#include <QFileInfo>
#include <QTextCodec>
#include <QMutex>

QScopedPointer<GlobalSettings> GlobalSettings::self;

GlobalSettings::~GlobalSettings() {
    //释放文件监视器
    file_watcher_->deleteLater();
    //保存设置
    SaveSettings();
}
//返回单例的实例
GlobalSettings* GlobalSettings::Instance() {
    //使用双检锁，防止多线程中出错
    if (self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new GlobalSettings);
        }
    }
    return self.data();
}

void GlobalSettings::SetSetting1(int setting_1) {
    setting_1_ = setting_1;
}

void GlobalSettings::SetSetting2(QString setting_2) {
    setting_2_ = setting_2;
}

int GlobalSettings::GetSetting1() {
    return setting_1_;
}

QString GlobalSettings::GetSetting2() {
    return setting_2_;
}
//响应文件变化，重新读取配置文件，发送配置改变信号
void GlobalSettings::ConfigurationFileChanged(const QString& path) {
    if (path == configuration_file_path_) {
        ReadConfigurationFile();
        emit SettingsChanged();
    }
}

GlobalSettings::GlobalSettings() {
    InitSettings();//初始化默认值
    CheckConfigurationFile();//检查配置文件是否存在
    ReadConfigurationFile();//读入配置文件
    InitFileWatcher();//初始化文件监视器
}
//初始化设置
void GlobalSettings::InitSettings() {
    //配置文件路径
    configuration_file_path_ = "./config/settings.ini";
    //设置初始值
    setting_1_ = 1;
    setting_2_ = "2";
}
//检查配置文件是否存在,不存在则用初始值创建
void GlobalSettings::CheckConfigurationFile() {
    bool is_exist = IsFileExist(configuration_file_path_);
    if (!is_exist) {
        CreateInitialConfigurationFile();
    }
}
//判断文件是否存在
bool GlobalSettings::IsFileExist(const QString& file_path) {
    bool is_exist = false;
    QFileInfo file_info(file_path);
    if (file_info.exists()) {
        is_exist = true;
    }
    return is_exist;
}
//用初始值创建配置文件
void GlobalSettings::CreateInitialConfigurationFile() {
    QSettings* configuration_file = new QSettings(configuration_file_path_, QSettings::IniFormat);
    //设置文件编码格式为utf-8
    configuration_file->setIniCodec(QTextCodec::codecForName("UTF-8"));
    //写入文件
    configuration_file->setValue("Settings/details", "Some settings");
    configuration_file->setValue("Settings/setting_1", setting_1_);
    configuration_file->setValue("Settings/setting_2", setting_2_);
    delete configuration_file;
}
//读取配置文件
void GlobalSettings::ReadConfigurationFile() {
    QSettings* configuration_file = new QSettings(configuration_file_path_, QSettings::IniFormat);
    //读取文件
    setting_1_ = configuration_file->value("Settings/setting_1").toInt();
    setting_2_ = configuration_file->value("Settings/setting_2").toString();
    delete configuration_file;
}
//初始化文件监视器，对配置文件添加监视
void GlobalSettings::InitFileWatcher() {
    file_watcher_ = new QFileSystemWatcher;
    connect(file_watcher_, SIGNAL(fileChanged(const QString&)),
            this, SLOT(ConfigurationFileChanged(const QString&)));
    file_watcher_->addPath(configuration_file_path_);
}
//保存设置
void GlobalSettings::SaveSettings() {
    QSettings* configuration_file = new QSettings(configuration_file_path_, QSettings::IniFormat);
    //设置文件编码格式为utf-8
    configuration_file->setIniCodec(QTextCodec::codecForName("UTF-8"));
    //写入文件
    configuration_file->setValue("Settings/setting_1", setting_1_);
    configuration_file->setValue("Settings/setting_2", setting_2_);
    delete configuration_file;
}

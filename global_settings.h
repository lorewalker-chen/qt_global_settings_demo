#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include <QObject>

class QFileSystemWatcher;
class GlobalSettings: public QObject {
    Q_OBJECT
  public:
    ~GlobalSettings();
    static GlobalSettings* Instance();

    //设置
    void SetSetting1(int setting_1);
    void SetSetting2(QString setting_2);
    //获取
    int GetSetting1();
    QString GetSetting2();

  private slots:
    //响应本地配置文件改变
    void ConfigurationFileChanged(const QString& path);

  private:
    static QScopedPointer<GlobalSettings> self;
    GlobalSettings();

    void InitSettings();//初始化设置
    void CheckConfigurationFile();//检查配置文件
    bool IsFileExist(const QString& file_path);//判断文件是否存在
    void CreateInitialConfigurationFile();//用初始值创建配置文件
    void ReadConfigurationFile();//读取配置文件
    void InitFileWatcher();//初始化文件监视器

    void SaveSettings();//保存设置

    QString configuration_file_path_;//配置文件路径
    QFileSystemWatcher* file_watcher_;//文件监视器

    int setting_1_;
    QString setting_2_;

  signals:
    void SettingsChanged();
};

#endif // GLOBALSETTINGS_H

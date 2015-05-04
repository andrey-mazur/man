#include <QtQml/QQmlExtensionPlugin>
#include <QtQml/qqml.h>
#include <manWaveform/manWaveformItem.h>


class manWaveformQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri)
    {
        Q_ASSERT(uri == QLatin1String("manWaveform"));
		qmlRegisterType<manWaveformItem>(uri, 1, 0, "ManWaveform");
    }
};

#include "manWaveformPlugin.moc"

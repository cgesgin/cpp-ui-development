#include <QProcess>
#include <QDebug>
#include <QRegularExpression>

class AudioRecorder {

public:
    AudioRecorder() {
        ffmpegPath = "C:/Program Files/ffmpeg/bin/ffmpeg.exe";
    }

    QString message;

    bool startRecording(const QString &filePath) {

        QString micName = findMicrophone(ffmpegPath);
        if (micName.isEmpty()) {
            qDebug() << "micrfone not found";
            message ="micrfone not found";
            return false;
        }

        QStringList arguments;
        arguments << "-y"
                  << "-f" << "dshow"
                  << "-i" << ("audio=" + micName)
                  << "-acodec" << "libmp3lame"
                  << "-q:a" << "2"
                  << filePath;

        process.start(ffmpegPath, arguments);

        if (!process.waitForStarted()) {
            qDebug() << "record failed";
            message ="record failed";
            return false;
        }

        qDebug() << "record started. microfone name : " << micName;
        message ="record started. microfone name : "+micName;
        return true;
    }

    void stopRecording() {
        if (process.state() == QProcess::Running) {
            process.write("q\n");
            process.waitForFinished();
            qDebug() << "record finished.";
            message ="record finished.";
        }
    }

private:
    QProcess process;
    QString ffmpegPath;
    QString findMicrophone(const QString &path) {
        QProcess proc;

        proc.start(path, {
                             "-list_devices", "true",
                             "-f", "dshow",
                             "-i", "dummy"
                         });

        proc.waitForFinished(3000);

        QString output = QString::fromLocal8Bit(proc.readAllStandardError());

        QRegularExpression regex(R"REGEX("([^"]+)"\s*\(audio\))REGEX");
        QRegularExpressionMatchIterator i = regex.globalMatch(output);

        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            QString deviceName = match.captured(1);

            if (!deviceName.isEmpty()) {
                qDebug() << "Found Device" << deviceName;
                return deviceName;
            }
        }
        return {};
    }
};
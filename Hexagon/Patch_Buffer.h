#ifndef PATCH_BUFFER_H
#define PATCH_BUFFER_H

#include <QByteArray>
#include <QFile>
#include <QMap>

class Patch_Writer;
class Value_Manipulator;

class Patch_Buffer {
public:
    Patch_Buffer(QFile *file);
    ~Patch_Buffer();
    void Add_Next_Patch(const qint64 offset, const QByteArray &value);
    bool Flush();
    bool Write_Checksum(const QString &checksum);
    bool Write_Size(qint64 sizeDifference);

private:
    bool Flush_Current_Buffer(qint64 offset, const QByteArray &bytes);

    Patch_Writer *patchWriter;
    Value_Manipulator *valueManipulator;
    QMap<qint64, char> *patchMap;
};

#endif // PATCH_BUFFER_H

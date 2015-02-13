/****************************************************************************
** Meta object code from reading C++ file 'datafileparser.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/datafileparser.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'datafileparser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DataFileParser_t {
    QByteArrayData data[5];
    char stringdata[67];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_DataFileParser_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_DataFileParser_t qt_meta_stringdata_DataFileParser = {
    {
QT_MOC_LITERAL(0, 0, 14),
QT_MOC_LITERAL(1, 15, 15),
QT_MOC_LITERAL(2, 31, 0),
QT_MOC_LITERAL(3, 32, 18),
QT_MOC_LITERAL(4, 51, 14)
    },
    "DataFileParser\0fileDataChanged\0\0"
    "addFileWatchFailed\0fileDataChange\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DataFileParser[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06,
       3,    1,   30,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       4,    1,   33,    2, 0x08,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void DataFileParser::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DataFileParser *_t = static_cast<DataFileParser *>(_o);
        switch (_id) {
        case 0: _t->fileDataChanged(); break;
        case 1: _t->addFileWatchFailed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->fileDataChange((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DataFileParser::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataFileParser::fileDataChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (DataFileParser::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataFileParser::addFileWatchFailed)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject DataFileParser::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DataFileParser.data,
      qt_meta_data_DataFileParser,  qt_static_metacall, 0, 0}
};


const QMetaObject *DataFileParser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataFileParser::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DataFileParser.stringdata))
        return static_cast<void*>(const_cast< DataFileParser*>(this));
    return QObject::qt_metacast(_clname);
}

int DataFileParser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void DataFileParser::fileDataChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DataFileParser::addFileWatchFailed(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE

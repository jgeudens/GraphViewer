/****************************************************************************
** Meta object code from reading C++ file 'graphviewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/graphviewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'graphviewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GraphViewer_t {
    QByteArrayData data[11];
    char stringdata[134];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_GraphViewer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_GraphViewer_t qt_meta_stringdata_GraphViewer = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 14),
QT_MOC_LITERAL(2, 27, 0),
QT_MOC_LITERAL(3, 28, 14),
QT_MOC_LITERAL(4, 43, 18),
QT_MOC_LITERAL(5, 62, 16),
QT_MOC_LITERAL(6, 79, 10),
QT_MOC_LITERAL(7, 90, 10),
QT_MOC_LITERAL(8, 101, 17),
QT_MOC_LITERAL(9, 119, 8),
QT_MOC_LITERAL(10, 128, 4)
    },
    "GraphViewer\0autoScaleXAxis\0\0autoScaleYAxis\0"
    "generateTickLabels\0selectionChanged\0"
    "mousePress\0mouseWheel\0axisDoubleClicked\0"
    "QCPAxis*\0axis\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GraphViewer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a,
       3,    0,   50,    2, 0x0a,
       4,    0,   51,    2, 0x08,
       5,    0,   52,    2, 0x08,
       6,    0,   53,    2, 0x08,
       7,    0,   54,    2, 0x08,
       8,    1,   55,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void GraphViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GraphViewer *_t = static_cast<GraphViewer *>(_o);
        switch (_id) {
        case 0: _t->autoScaleXAxis(); break;
        case 1: _t->autoScaleYAxis(); break;
        case 2: _t->generateTickLabels(); break;
        case 3: _t->selectionChanged(); break;
        case 4: _t->mousePress(); break;
        case 5: _t->mouseWheel(); break;
        case 6: _t->axisDoubleClicked((*reinterpret_cast< QCPAxis*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCPAxis* >(); break;
            }
            break;
        }
    }
}

const QMetaObject GraphViewer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GraphViewer.data,
      qt_meta_data_GraphViewer,  qt_static_metacall, 0, 0}
};


const QMetaObject *GraphViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GraphViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GraphViewer.stringdata))
        return static_cast<void*>(const_cast< GraphViewer*>(this));
    return QObject::qt_metacast(_clname);
}

int GraphViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

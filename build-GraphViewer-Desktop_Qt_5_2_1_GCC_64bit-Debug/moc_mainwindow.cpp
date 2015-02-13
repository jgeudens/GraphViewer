/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[24];
    char stringdata[330];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 19),
QT_MOC_LITERAL(2, 31, 0),
QT_MOC_LITERAL(3, 32, 9),
QT_MOC_LITERAL(4, 42, 15),
QT_MOC_LITERAL(5, 58, 14),
QT_MOC_LITERAL(6, 73, 14),
QT_MOC_LITERAL(7, 88, 16),
QT_MOC_LITERAL(8, 105, 4),
QT_MOC_LITERAL(9, 110, 18),
QT_MOC_LITERAL(10, 129, 9),
QT_MOC_LITERAL(11, 139, 20),
QT_MOC_LITERAL(12, 160, 20),
QT_MOC_LITERAL(13, 181, 13),
QT_MOC_LITERAL(14, 195, 6),
QT_MOC_LITERAL(15, 202, 22),
QT_MOC_LITERAL(16, 225, 27),
QT_MOC_LITERAL(17, 253, 14),
QT_MOC_LITERAL(18, 268, 16),
QT_MOC_LITERAL(19, 285, 1),
QT_MOC_LITERAL(20, 287, 9),
QT_MOC_LITERAL(21, 297, 11),
QT_MOC_LITERAL(22, 309, 15),
QT_MOC_LITERAL(23, 325, 3)
    },
    "MainWindow\0getDataFileSettings\0\0"
    "parseData\0exitApplication\0reloadDataFile\0"
    "fileDataChange\0addFileWatchFail\0path\0"
    "prepareImageExport\0showAbout\0"
    "showXAxisScaleDialog\0showYAxisScaleDialog\0"
    "showHideGraph\0bState\0enableWatchFileChanged\0"
    "enableDynamicSessionChanged\0dragEnterEvent\0"
    "QDragEnterEvent*\0e\0dropEvent\0QDropEvent*\0"
    "showContextMenu\0pos\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x08,
       3,    0,   95,    2, 0x08,
       4,    0,   96,    2, 0x08,
       5,    0,   97,    2, 0x08,
       6,    0,   98,    2, 0x08,
       7,    1,   99,    2, 0x08,
       9,    0,  102,    2, 0x08,
      10,    0,  103,    2, 0x08,
      11,    0,  104,    2, 0x08,
      12,    0,  105,    2, 0x08,
      13,    1,  106,    2, 0x08,
      15,    1,  109,    2, 0x08,
      16,    1,  112,    2, 0x08,
      17,    1,  115,    2, 0x08,
      20,    1,  118,    2, 0x08,
      22,    1,  121,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   14,
    QMetaType::Void, QMetaType::Bool,   14,
    QMetaType::Void, QMetaType::Bool,   14,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void, 0x80000000 | 21,   19,
    QMetaType::Void, QMetaType::QPoint,   23,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->getDataFileSettings(); break;
        case 1: _t->parseData(); break;
        case 2: _t->exitApplication(); break;
        case 3: _t->reloadDataFile(); break;
        case 4: _t->fileDataChange(); break;
        case 5: _t->addFileWatchFail((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->prepareImageExport(); break;
        case 7: _t->showAbout(); break;
        case 8: _t->showXAxisScaleDialog(); break;
        case 9: _t->showYAxisScaleDialog(); break;
        case 10: _t->showHideGraph((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->enableWatchFileChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->enableDynamicSessionChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->dragEnterEvent((*reinterpret_cast< QDragEnterEvent*(*)>(_a[1]))); break;
        case 14: _t->dropEvent((*reinterpret_cast< QDropEvent*(*)>(_a[1]))); break;
        case 15: _t->showContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

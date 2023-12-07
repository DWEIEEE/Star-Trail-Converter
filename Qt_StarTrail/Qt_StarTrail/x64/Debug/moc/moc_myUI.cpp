/****************************************************************************
** Meta object code from reading C++ file 'myUI.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../myUI.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'myUI.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_MyUI_t {
    uint offsetsAndSizes[22];
    char stringdata0[5];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[7];
    char stringdata4[9];
    char stringdata5[13];
    char stringdata6[14];
    char stringdata7[9];
    char stringdata8[10];
    char stringdata9[13];
    char stringdata10[8];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_MyUI_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_MyUI_t qt_meta_stringdata_MyUI = {
    {
        QT_MOC_LITERAL(0, 4),  // "MyUI"
        QT_MOC_LITERAL(5, 12),  // "getInputPath"
        QT_MOC_LITERAL(18, 0),  // ""
        QT_MOC_LITERAL(19, 6),  // "string"
        QT_MOC_LITERAL(26, 8),  // "getFiles"
        QT_MOC_LITERAL(35, 12),  // "input_browse"
        QT_MOC_LITERAL(48, 13),  // "output_browse"
        QT_MOC_LITERAL(62, 8),  // "pushLeft"
        QT_MOC_LITERAL(71, 9),  // "pushRight"
        QT_MOC_LITERAL(81, 12),  // "searchFolder"
        QT_MOC_LITERAL(94, 7)   // "loadImg"
    },
    "MyUI",
    "getInputPath",
    "",
    "string",
    "getFiles",
    "input_browse",
    "output_browse",
    "pushLeft",
    "pushRight",
    "searchFolder",
    "loadImg"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_MyUI[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   62,    2, 0x06,    1 /* Public */,
       4,    1,   65,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   68,    2, 0x08,    5 /* Private */,
       6,    0,   69,    2, 0x08,    6 /* Private */,
       7,    0,   70,    2, 0x08,    7 /* Private */,
       8,    0,   71,    2, 0x08,    8 /* Private */,
       9,    1,   72,    2, 0x08,    9 /* Private */,
      10,    1,   75,    2, 0x08,   11 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QStringList,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QStringList,    3,

       0        // eod
};

Q_CONSTINIT const QMetaObject MyUI::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_MyUI.offsetsAndSizes,
    qt_meta_data_MyUI,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_MyUI_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MyUI, std::true_type>,
        // method 'getInputPath'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'getFiles'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QStringList &, std::false_type>,
        // method 'input_browse'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'output_browse'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'pushLeft'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'pushRight'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'searchFolder'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'loadImg'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QStringList &, std::false_type>
    >,
    nullptr
} };

void MyUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MyUI *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->getInputPath((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->getFiles((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1]))); break;
        case 2: _t->input_browse(); break;
        case 3: _t->output_browse(); break;
        case 4: _t->pushLeft(); break;
        case 5: _t->pushRight(); break;
        case 6: _t->searchFolder((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->loadImg((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MyUI::*)(const QString & );
            if (_t _q_method = &MyUI::getInputPath; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MyUI::*)(const QStringList & );
            if (_t _q_method = &MyUI::getFiles; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *MyUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyUI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MyUI.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MyUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void MyUI::getInputPath(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MyUI::getFiles(const QStringList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

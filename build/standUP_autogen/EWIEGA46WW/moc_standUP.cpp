/****************************************************************************
** Meta object code from reading C++ file 'standUP.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../standUP.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'standUP.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
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
struct qt_meta_stringdata_standUP_t {
    uint offsetsAndSizes[24];
    char stringdata0[8];
    char stringdata1[12];
    char stringdata2[1];
    char stringdata3[17];
    char stringdata4[15];
    char stringdata5[20];
    char stringdata6[13];
    char stringdata7[13];
    char stringdata8[8];
    char stringdata9[13];
    char stringdata10[13];
    char stringdata11[15];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_standUP_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_standUP_t qt_meta_stringdata_standUP = {
    {
        QT_MOC_LITERAL(0, 7),  // "standUP"
        QT_MOC_LITERAL(8, 11),  // "updateClock"
        QT_MOC_LITERAL(20, 0),  // ""
        QT_MOC_LITERAL(21, 16),  // "onApplyInputTime"
        QT_MOC_LITERAL(38, 14),  // "onToggleGoStop"
        QT_MOC_LITERAL(53, 19),  // "onTogglePauseResume"
        QT_MOC_LITERAL(73, 12),  // "triggerAlert"
        QT_MOC_LITERAL(86, 12),  // "onAddMinutes"
        QT_MOC_LITERAL(99, 7),  // "minutes"
        QT_MOC_LITERAL(107, 12),  // "saveSettings"
        QT_MOC_LITERAL(120, 12),  // "showSettings"
        QT_MOC_LITERAL(133, 14)   // "showMainScreen"
    },
    "standUP",
    "updateClock",
    "",
    "onApplyInputTime",
    "onToggleGoStop",
    "onTogglePauseResume",
    "triggerAlert",
    "onAddMinutes",
    "minutes",
    "saveSettings",
    "showSettings",
    "showMainScreen"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_standUP[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   68,    2, 0x08,    1 /* Private */,
       3,    0,   69,    2, 0x08,    2 /* Private */,
       4,    0,   70,    2, 0x08,    3 /* Private */,
       5,    0,   71,    2, 0x08,    4 /* Private */,
       6,    0,   72,    2, 0x08,    5 /* Private */,
       7,    1,   73,    2, 0x08,    6 /* Private */,
       9,    0,   76,    2, 0x08,    8 /* Private */,
      10,    0,   77,    2, 0x08,    9 /* Private */,
      11,    0,   78,    2, 0x08,   10 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject standUP::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_standUP.offsetsAndSizes,
    qt_meta_data_standUP,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_standUP_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<standUP, std::true_type>,
        // method 'updateClock'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onApplyInputTime'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onToggleGoStop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTogglePauseResume'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'triggerAlert'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAddMinutes'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'saveSettings'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showSettings'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showMainScreen'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void standUP::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<standUP *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->updateClock(); break;
        case 1: _t->onApplyInputTime(); break;
        case 2: _t->onToggleGoStop(); break;
        case 3: _t->onTogglePauseResume(); break;
        case 4: _t->triggerAlert(); break;
        case 5: _t->onAddMinutes((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->saveSettings(); break;
        case 7: _t->showSettings(); break;
        case 8: _t->showMainScreen(); break;
        default: ;
        }
    }
}

const QMetaObject *standUP::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *standUP::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_standUP.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int standUP::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

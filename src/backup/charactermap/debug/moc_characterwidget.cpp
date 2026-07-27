/****************************************************************************
** Meta object code from reading C++ file 'characterwidget.h'
**
** Created: Sun Aug 5 18:23:26 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../characterwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'characterwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CharacterWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      27,   17,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      59,   54,   16,   16, 0x0a,
      86,   77,   16,   16, 0x0a,
     116,  106,   16,   16, 0x0a,
     144,  137,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CharacterWidget[] = {
    "CharacterWidget\0\0character\0"
    "characterSelected(QString)\0font\0"
    "updateFont(QFont)\0fontSize\0"
    "updateSize(QString)\0fontStyle\0"
    "updateStyle(QString)\0enable\0"
    "updateFontMerging(bool)\0"
};

void CharacterWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CharacterWidget *_t = static_cast<CharacterWidget *>(_o);
        switch (_id) {
        case 0: _t->characterSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->updateFont((*reinterpret_cast< const QFont(*)>(_a[1]))); break;
        case 2: _t->updateSize((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->updateStyle((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->updateFontMerging((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CharacterWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CharacterWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CharacterWidget,
      qt_meta_data_CharacterWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CharacterWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CharacterWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CharacterWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CharacterWidget))
        return static_cast<void*>(const_cast< CharacterWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CharacterWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void CharacterWidget::characterSelected(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

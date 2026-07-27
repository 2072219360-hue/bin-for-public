/****************************************************************************
** Meta object code from reading C++ file 'dvonnwindowimpl.h'
**
** Created: Fri Aug 31 15:23:05 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dvonnwindowimpl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dvonnwindowimpl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DvonnWindowImpl[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x0a,
      24,   16,   16,   16, 0x0a,
      31,   16,   16,   16, 0x0a,
      40,   16,   16,   16, 0x0a,
      48,   16,   16,   16, 0x0a,
      58,   16,   16,   16, 0x0a,
      65,   16,   16,   16, 0x0a,
      72,   16,   16,   16, 0x0a,
      79,   16,   16,   16, 0x0a,
      87,   16,   16,   16, 0x0a,
     113,   16,   16,   16, 0x0a,
     133,   16,   16,   16, 0x0a,
     162,   16,   16,   16, 0x0a,
     186,   16,   16,   16, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_DvonnWindowImpl[] = {
    "DvonnWindowImpl\0\0load()\0save()\0saveAs()\0"
    "print()\0newGame()\0undo()\0redo()\0help()\0"
    "about()\0randomlyFinishPlacement()\0"
    "randomlyMoveStack()\0play(dvonn::Game::Placement)\0"
    "play(dvonn::Game::Move)\0animateScore()\0"
};

void DvonnWindowImpl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DvonnWindowImpl *_t = static_cast<DvonnWindowImpl *>(_o);
        switch (_id) {
        case 0: _t->load(); break;
        case 1: _t->save(); break;
        case 2: _t->saveAs(); break;
        case 3: _t->print(); break;
        case 4: _t->newGame(); break;
        case 5: _t->undo(); break;
        case 6: _t->redo(); break;
        case 7: _t->help(); break;
        case 8: _t->about(); break;
        case 9: _t->randomlyFinishPlacement(); break;
        case 10: _t->randomlyMoveStack(); break;
        case 11: _t->play((*reinterpret_cast< dvonn::Game::Placement(*)>(_a[1]))); break;
        case 12: _t->play((*reinterpret_cast< dvonn::Game::Move(*)>(_a[1]))); break;
        case 13: _t->animateScore(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DvonnWindowImpl::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DvonnWindowImpl::staticMetaObject = {
    { &DvonnWindow::staticMetaObject, qt_meta_stringdata_DvonnWindowImpl,
      qt_meta_data_DvonnWindowImpl, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DvonnWindowImpl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DvonnWindowImpl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DvonnWindowImpl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DvonnWindowImpl))
        return static_cast<void*>(const_cast< DvonnWindowImpl*>(this));
    return DvonnWindow::qt_metacast(_clname);
}

int DvonnWindowImpl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DvonnWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

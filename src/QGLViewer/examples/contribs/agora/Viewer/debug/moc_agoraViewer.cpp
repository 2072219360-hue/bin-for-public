/****************************************************************************
** Meta object code from reading C++ file 'agoraViewer.h'
**
** Created: Fri Aug 31 15:21:52 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../agoraViewer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'agoraViewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AgoraViewer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      20,   12,   12,   12, 0x0a,
      27,   12,   12,   12, 0x0a,
      36,   12,   12,   12, 0x0a,
      46,   12,   12,   12, 0x0a,
      53,   12,   12,   12, 0x0a,
      60,   12,   12,   12, 0x0a,
      82,   79,   12,   12, 0x0a,
     107,   79,   12,   12, 0x0a,
     132,   12,   12,   12, 0x0a,
     155,   12,   12,   12, 0x0a,
     178,   79,   12,   12, 0x0a,
     200,   79,   12,   12, 0x0a,
     233,   12,   12,   12, 0x0a,
     248,   12,   12,   12, 0x0a,
     256,   12,   12,   12, 0x08,
     283,  269,   12,   12, 0x08,
     313,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AgoraViewer[] = {
    "AgoraViewer\0\0load()\0save()\0saveAs()\0"
    "newGame()\0undo()\0redo()\0finalizeUndoRedo()\0"
    "on\0blackPlayerIsHuman(bool)\0"
    "whitePlayerIsHuman(bool)\0"
    "configureBlackPlayer()\0configureWhitePlayer()\0"
    "toggleAnimation(bool)\0"
    "toggleDisplayPossibleMoves(bool)\0"
    "displayRules()\0about()\0simplePlay()\0"
    "move,duration\0playComputerMove(QString,int)\0"
    "playNextMove()\0"
};

void AgoraViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AgoraViewer *_t = static_cast<AgoraViewer *>(_o);
        switch (_id) {
        case 0: _t->load(); break;
        case 1: _t->save(); break;
        case 2: _t->saveAs(); break;
        case 3: _t->newGame(); break;
        case 4: _t->undo(); break;
        case 5: _t->redo(); break;
        case 6: _t->finalizeUndoRedo(); break;
        case 7: _t->blackPlayerIsHuman((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->whitePlayerIsHuman((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->configureBlackPlayer(); break;
        case 10: _t->configureWhitePlayer(); break;
        case 11: _t->toggleAnimation((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->toggleDisplayPossibleMoves((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->displayRules(); break;
        case 14: _t->about(); break;
        case 15: _t->simplePlay(); break;
        case 16: _t->playComputerMove((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 17: _t->playNextMove(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AgoraViewer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AgoraViewer::staticMetaObject = {
    { &QGLViewer::staticMetaObject, qt_meta_stringdata_AgoraViewer,
      qt_meta_data_AgoraViewer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AgoraViewer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AgoraViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AgoraViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AgoraViewer))
        return static_cast<void*>(const_cast< AgoraViewer*>(this));
    return QGLViewer::qt_metacast(_clname);
}

int AgoraViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLViewer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

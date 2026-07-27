/****************************************************************************
** Meta object code from reading C++ file 'blobWarViewer.h'
**
** Created: Fri Aug 31 15:22:35 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../blobWarViewer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'blobWarViewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BlobWarViewer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,
      22,   14,   14,   14, 0x0a,
      29,   14,   14,   14, 0x0a,
      38,   14,   14,   14, 0x0a,
      48,   14,   14,   14, 0x0a,
      55,   14,   14,   14, 0x0a,
      62,   14,   14,   14, 0x0a,
      84,   81,   14,   14, 0x0a,
     108,   81,   14,   14, 0x0a,
     131,   14,   14,   14, 0x0a,
     153,   14,   14,   14, 0x0a,
     174,   81,   14,   14, 0x0a,
     196,   81,   14,   14, 0x0a,
     229,   14,   14,   14, 0x0a,
     244,   14,   14,   14, 0x0a,
     252,   14,   14,   14, 0x08,
     265,   14,   14,   14, 0x08,
     291,  277,   14,   14, 0x08,
     321,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_BlobWarViewer[] = {
    "BlobWarViewer\0\0load()\0save()\0saveAs()\0"
    "newGame()\0undo()\0redo()\0finalizeUndoRedo()\0"
    "on\0bluePlayerIsHuman(bool)\0"
    "redPlayerIsHuman(bool)\0configureBluePlayer()\0"
    "configureRedPlayer()\0toggleAnimation(bool)\0"
    "toggleDisplayPossibleMoves(bool)\0"
    "displayRules()\0about()\0simplePlay()\0"
    "flipColor()\0move,duration\0"
    "playComputerMove(QString,int)\0"
    "playNextMove()\0"
};

void BlobWarViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BlobWarViewer *_t = static_cast<BlobWarViewer *>(_o);
        switch (_id) {
        case 0: _t->load(); break;
        case 1: _t->save(); break;
        case 2: _t->saveAs(); break;
        case 3: _t->newGame(); break;
        case 4: _t->undo(); break;
        case 5: _t->redo(); break;
        case 6: _t->finalizeUndoRedo(); break;
        case 7: _t->bluePlayerIsHuman((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->redPlayerIsHuman((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->configureBluePlayer(); break;
        case 10: _t->configureRedPlayer(); break;
        case 11: _t->toggleAnimation((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->toggleDisplayPossibleMoves((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->displayRules(); break;
        case 14: _t->about(); break;
        case 15: _t->simplePlay(); break;
        case 16: _t->flipColor(); break;
        case 17: _t->playComputerMove((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 18: _t->playNextMove(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BlobWarViewer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BlobWarViewer::staticMetaObject = {
    { &QGLViewer::staticMetaObject, qt_meta_stringdata_BlobWarViewer,
      qt_meta_data_BlobWarViewer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BlobWarViewer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BlobWarViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BlobWarViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BlobWarViewer))
        return static_cast<void*>(const_cast< BlobWarViewer*>(this));
    return QGLViewer::qt_metacast(_clname);
}

int BlobWarViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLViewer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

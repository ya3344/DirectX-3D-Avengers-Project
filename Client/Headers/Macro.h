#ifndef MACRO_H__
#define MACRO_H__

#define LIMIT_FPS65
//#define LIMIT_FPS400

#ifdef _DEBUG 
#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif 
#endif  // _DEBUG 
#endif // MACRO_H__

//#define _COLLISION_DEBUG
//#define _BULLET_DEBUG // DH_BulletAdd_180618  _BULLET_DEBUG ¼±¾ð
//#define _TERRAIN_INIT_DEBUG //TY_ChangeStage_180704_ Terrain Initialize Related Macro
//#define _AI_DEBUG // JH_NaturalAnim_1800630 _AI_DEBUG Delcare
#ifndef ENGINE_ENUM_H__
#define ENGINE_ENUM_H__

enum class UPDATE : int
{
	NORMAL = 0,
	DIE = 1,
	FAIL = -1,
	BULLET_WALL = 2, // TY_FixBullet_180701_ Add Enum
	BULLET_BOMB = 3,
	BULLET_METEOR = 4// TY_AddBullet_180704_ Add Meteor Enum
};

enum class WIN_MODE : size_t
{
	MODE_FULL = 0,
	MODE_WIN,
};

enum SCENE 
{ 
	SCENE_STATIC,
	SCENE_PLAYER,
	SCENE_BOSS,
	SCENE_THANOS,
	SCENE_BULLET, // DH_BulletAdd_180618  SCENE_BULLET enum Add
	SCENE_PARTICLE, //TY_PARTICLE_180623 Add ParticleCube Enum
	SCENE_STAGE,
	SCENE_TERRAIN, //TY_SCENE_TERRAIN_180615 Add Enum SCENE_TERRAIN
	SCENE_LOGO,
	SCENE_PLANENT,
	SCENE_END
};
		


#endif //  ENGINE_ENUM_H__
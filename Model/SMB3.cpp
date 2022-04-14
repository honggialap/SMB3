#pragma region INCLUDE
#include "SMB3.h"

#include "GameMaster.h"

#include "Menu/Menu.h"

#include "World/Background.h"
#include "World/Block.h"
#include "World/Brick.h"
#include "World/Coin.h"
#include "World/DeadZone.h"
#include "World/ExtraLifeMushroom.h"
#include "World/FireFlower.h"
#include "World/Goal.h"
#include "World/Goomba.h"
#include "World/Koopa.h"
#include "World/KoopaSensor.h"
#include "World/Mario.h"
#include "World/MarioFireball.h"
#include "World/MarioTail.h"
#include "World/MicroGoomba.h"
#include "World/Pipe.h"
#include "World/Plant.h"
#include "World/PlantFireball.h"
#include "World/PlantSensor.h"
#include "World/Platform.h"
#include "World/Relay.h"
#include "World/SpawnZone.h"
#include "World/SuperLeaf.h"
#include "World/SuperMushroom.h"
#include "World/TransportPipe.h"
#include "World/World.h"

#include "WorldMap/WorldMap.h"

#pragma endregion

pGameObject CSMB3::Create(
	pScene scene,
	unsigned int actorID, std::string name, std::string source,
	float posX, float posY,
	int gridX, int gridY,
	unsigned int layer
) {
	switch (actorID) {
	case ACT_GAME_MASTER: {
		auto gameObject = new CGameMaster(
			this, scene, 
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_MENU: {
		auto gameObject = new CMenu(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_WORLD_MAP: {
		auto gameObject = new CWorldMap(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_WORLD: {
		auto gameObject = new CWorld(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_MARIO: {
		auto gameObject = new CMario(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_MARIO_TAIL: {
		auto gameObject = new CMarioTail(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_MARIO_FIREBALL: {
		auto gameObject = new CMarioFireball(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_GOOMBA: {
		auto gameObject = new CGoomba(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_MICRO_GOOMBA: {
		auto gameObject = new CMicroGoomba(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_KOOPA: {
		auto gameObject = new CKoopa(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_KOOPA_SENSOR: {
		auto gameObject = new CKoopaSensor(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_PLANT: {
		auto gameObject = new CPlant(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_PLANT_SENSOR: {
		auto gameObject = new CPlantSensor(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_PLANT_FIREBALL: {
		auto gameObject = new CPlantFireball(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_COIN: {
		auto gameObject = new CCoin(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_SUPER_MUSHROOM: {
		auto gameObject = new CSuperMushroom(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_FIRE_FLOWER: {
		auto gameObject = new CFireFlower(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_SUPER_LEAF: {
		auto gameObject = new CSuperLeaf(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_EXTRA_LIFE_MUSHROOM: {
		auto gameObject = new CExtraLifeMushroom(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_PLATFORM: {
		auto gameObject = new CPlatform(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_BLOCK: {
		auto gameObject = new CBlock(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_BRICK: {
		auto gameObject = new CBrick(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_PIPE: {
		auto gameObject = new CPipe(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_TRANSPORT_PIPE: {
		auto gameObject = new CTransportPipe(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_RELAY: {
		auto gameObject = new CRelay(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_SPAWN_ZONE: {
		auto gameObject = new CSpawnZone(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_DEAD_ZONE: {
		auto gameObject = new CDeadZone(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_GOAL: {
		auto gameObject = new CGoal(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_BACKGROUND: {
		auto gameObject = new CBackground(
			this, scene,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	default: {
		return nullptr;
	} break;
	}
}

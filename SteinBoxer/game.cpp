#include "stdafx.h"
#include "game.h"
#include "constants.h"
using namespace std;

Entity* game::getPlayerEntity() {
	for (int i = 0; i < 254; i++) {
		if (entities[i] != 0) {
			if (entities[i]->getIsOwnPlayer() == 1 && entities[i]->getIsPlayerCharacter() == playerIdentifier) {
				//cout << "found own player: " << hex << entities[i] << endl;
				return entities[i];
			}
		}
	}
	return 0;
}

bool game::isPlayerAttackingMob() {
	int* mobUidAddress = (int*)mem::findDMAAddy(baseAdressMainMod + offsetToPlayerControlObject, { offsetToAttackUID });

	if (mem::IsBadReadPtr(mobUidAddress)) {
		return 0;
	}

	//cout << "Checking if player is attacking something. Value: " << std::dec << mobUid << endl;
	return *mobUidAddress != 0;
}

bool game::areOtherPlayersPresent() {

	int playerCount = 0;

	for (int i = 0; i < 255; i++) {
		if (entities[i] != 0) {
			if (entities[i]->getIsPlayerCharacter() == playerIdentifier) {
				playerCount++;
				//cout << "some player found at: 0x" << hex << entities[i] << endl;
			}
		}
	}

	return playerCount > 1;
}

Entity* game::getClosestMetinStone(Vector3 anchorPosition) {
	cout << "Searching for metin stones now..." << endl;

	Entity* closestMetinStone = nullptr;
	float distanceToClosestStone = INFINITY;
	int tempMobId;
	Vector3 tempMobPos;
	float tempDistanceToClosestStone;
	int stoneCount = 0;


	for (int i = 0; i < 255; i++) {
		if (entities[i] != 0) {
			
			tempMobId = entities[i]->getMobId();

			if (tempMobId > 100 && tempMobId < 200) {
				cout << "mob: " << hex << entities[i] << endl;
			}


			// check if mob is metin stone
			if (tempMobId >= metinIdStart && tempMobId <= metinIdEnd) {
				cout << "stone: " << hex << entities[i] << endl;
				//cout << "Stone found, mobID: " << dec << tempMobId << endl;
				//cout << "Stone found, mobAddress: 0x" << hex << entities[i] << endl;
				stoneCount++;
				tempMobPos = entities[i]->getPosition();
				//calculate distance to anchor position
				tempDistanceToClosestStone = sqrt(pow(anchorPosition.x - tempMobPos.x, 2) + pow(anchorPosition.y - tempMobPos.y, 2));
				//cout << "Distance to stone: " << dec << tempDistanceToClosestStone << endl;
				if (tempDistanceToClosestStone < distanceToClosestStone) {
					distanceToClosestStone = tempDistanceToClosestStone;
					closestMetinStone = entities[i];
				}
			}

		}
	}

	cout << "[i] I found " << dec << stoneCount << " stones." << endl;

	return closestMetinStone;
}

void game::enableWallhack() {
	BYTE* wallHackAddress = (BYTE*)mem::findDMAAddy(baseAdressMainMod + offsetWallHackBase, { offsetWallHackOne, offsetWallHackTwo });
	if (!mem::IsBadReadPtr(wallHackAddress)) {
		*wallHackAddress = 0x1;
	}

}

void game::disableWallhack() {
	BYTE* wallHackAddress = (BYTE*)mem::findDMAAddy(baseAdressMainMod + offsetWallHackBase, { offsetWallHackOne, offsetWallHackTwo });
	if (!mem::IsBadReadPtr(wallHackAddress)) {
		*wallHackAddress = 0x0;
	}
}

void game::playerAttackMobWithUid(Entity* mobEntity) {

	// make mob visible
	mobEntity->setIsRendered(0x1);

	// attack mob
	int* attackMobAddress = (int*)mem::findDMAAddy(baseAdressMainMod + offsetToPlayerControlObject, { offsetToAttackUID });
	if (!mem::IsBadReadPtr(attackMobAddress)) {
		*attackMobAddress = mobEntity->getUid();
	}
}


void game::resetPlayerAtatck() {

	int* playerAttackAddress = (int*)mem::findDMAAddy(baseAdressMainMod + offsetToPlayerControlObject, { offsetToAttackUID });

	if (!mem::IsBadReadPtr(playerAttackAddress)) {
		*playerAttackAddress = 0x0;
	}
}

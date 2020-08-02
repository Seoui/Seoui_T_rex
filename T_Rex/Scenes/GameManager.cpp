#include "stdafx.h"
#include "GameManager.h"
//HX 오브젝트 스폰시간 다 바꾸어야 함.
//구름, 달, 별의 높이범위도 바꾸어야 함.
GameManager::GameManager()
{
	// 6가지 종류의 선인장
	cactusXY[0] = { 228, 2, 244, 36 };
	cactusXY[1] = { 245, 2, 278, 36 };
	cactusXY[2] = { 279, 2, 329, 36 };
	cactusXY[3] = { 332, 2, 356, 51 };
	cactusXY[4] = { 357, 2, 406, 51 };
	cactusXY[5] = { 407, 2, 481, 51};
	// 3가지 종류의 별
	starXY[0] = { 644, 2, 652, 10 };
	starXY[1] = { 644, 11, 652, 19 };
	starXY[2] = { 644, 20, 652, 28 };

	/*
		새와 선인장은 동시에 나오면 안되기 때문에 묶어서 spawn한다.
		bc가 bird와 cactus를 뜻함. 
	*/
	uniform_real_distribution<float> bcTime(5.0f, 10.0f);
	bcRandTime = bcTime(randomEngine);
	uniform_real_distribution<float> cloudTime(5.0f, 10.0f);
	cloudRandTime = cloudTime(randomEngine);
	uniform_real_distribution<float> moonTime(5.0f, 10.0f);
	moonRandTime = moonTime(randomEngine);
	uniform_real_distribution<float> starTime(5.0f, 10.0f);
	starRandTime = starTime(randomEngine);
}

GameManager::~GameManager()
{
	for (auto& b : bird)
		SAFE_DELETE(b);
	for (auto& c : cactus)
		SAFE_DELETE(c);
	for (auto& c : cloud)
		SAFE_DELETE(c);
	for (auto& m : moon)
		SAFE_DELETE(m);
	for (auto& s : star)
		SAFE_DELETE(s);
}

void GameManager::Update(D3DXMATRIX& V, D3DXMATRIX& P)
{
	/*
		새, 선인장, 구름, 달, 별
		5개의 오브젝트를 랜덤시간에 생성한다.
		새, 선인장 오브젝트에는 collider를 추가한다.
	*/

	// spawn bird or cactus
	bcPlayTime += Time::Delta();
	if (bcRandTime < bcPlayTime)
	{
		SpawnBirdOrCactus();
		bcPlayTime = 0.0f;
	}
	if (bird.size() > 0)
	{
		int i = 0;
		for (auto& b : bird)
		{
			b->Update(V, P);
			// bird에 collider를 추가한다.
			birdColliders[i]->World(b->GetSprite()->World());
			birdColliders[i++]->Update(V, P);
		}
	}
	if(cactus.size() >0)
	{
		int i = 0;
		for (auto& c : cactus)
		{
			c->Update(V, P);
			// cactus에 collider를 추가한다.
			cactusColliders[i]->World(c->GetSprite()->World());
			cactusColliders[i++]->Update(V, P);
		}
	}

	// spawn cloud
	cloudPlayTime += Time::Delta();
	if (cloudRandTime < cloudPlayTime)
	{
		SpawnCloud();
		cloudPlayTime = 0.0f;
	}
	if (cloud.size() > 0)
	{
		for (auto& c : cloud)
			c->Update(V, P);
	}
	
	// spawn moon
	if(bNight)
	{
		moonPlayTime += Time::Delta();
		if (moonRandTime < moonPlayTime)
		{
			SpawnMoon();
			moonPlayTime = 0.0f;
		}
		if (moon.size() > 0)
		{
			for (auto& m : moon)
				m->Update(V, P);
		}
	}

	// spawn star
	if(bNight)
	{
		starPlayTime += Time::Delta();
		if (starRandTime < starPlayTime)
		{
			SpawnStar();
			starPlayTime = 0.0f;
		}
		if (star.size()>0)
		{
			for (auto& s : star)
				s->Update(V, P);
		}
	}

	// remove passed object
	RemoveObject();
}

void GameManager::Render()
{
	if (bird.size() > 0)
	{
		for (auto& b : bird)
			b->Render();
	}

	if (cactus.size() > 0)
	{
		for (auto& c : cactus)
			c->Render();
	}

	if (cloud.size() > 0)
	{
		for (auto& c : cloud)
			c->Render();
	}

	if (moon.size() > 0)
	{
		for (auto& m : moon)
			m->Render();
	}

	if (star.size() >0)
	{
		for (auto& s : star)
			s->Render();
	}

	if (birdColliders.size() > 0)
	{
		for (auto& c : birdColliders)
			c->Render();
	}

	if (cactusColliders.size() > 0)
	{
		for (auto& c : cactusColliders)
			c->Render();
	}
}

void GameManager::RemoveObject()
{
	if (bird.size() > 0)
	{
		if (bird[0]->Position().x < -400.0f)
		{
			bird.erase(bird.begin());
			birdColliders.erase(birdColliders.begin());
		}
	}
	if (cactus.size() > 0)
	{
		if (cactus[0]->Position().x < -400.0f)
		{
			cactus.erase(cactus.begin());
			cactusColliders.erase(cactusColliders.begin());
		}
	}
	if (cloud.size() > 0)
		if (cloud[0]->Position().x < -400.0f)
			cloud.erase(cloud.begin());
	if (moon.size() > 0)
		if (moon[0]->Position().x < -400.0f)
			moon.erase(moon.begin());
	if (star.size() > 0)
		if (star[0]->Position().x < -400.0f)
			star.erase(star.begin());
}

void GameManager::SpawnBirdOrCactus()
{
	uniform_int_distribution<int> distBC(0, 1);
	int bSelectBC = distBC(randomEngine);
	if (bSelectBC)
		SpawnBird();
	else
		SpawnCactus();
	uniform_real_distribution<float> BCTime(2.0f, 5.0f);
	bcRandTime = BCTime(randomEngine);
}

void GameManager::SpawnBird()
{
	uniform_int_distribution<int> distPositionY(0, 3);
	int nRandY = distPositionY(randomEngine);
	bird.push_back(new Bird(D3DXVECTOR2(400.0f, nRandY * 30.0f)));
	birdColliders.push_back(new Collider());
}

void GameManager::SpawnCactus()
{
	uniform_int_distribution<int> distIndex(0, 5);
	int nrandIndex = distIndex(randomEngine);
	cactus.push_back(new Cactus(D3DXVECTOR2(400.0f, 0.0f),
		cactusXY[nrandIndex].startX, cactusXY[nrandIndex].startY,
		cactusXY[nrandIndex].endX, cactusXY[nrandIndex].endY));
	cactusColliders.push_back(new Collider());
}

void GameManager::SpawnCloud()
{
	uniform_real_distribution<float> distPositionY(80.0f, 135.0f);
	float nRandY = distPositionY(randomEngine);
	cloud.push_back(new Cloud(D3DXVECTOR2(400.0f, nRandY)));
	uniform_real_distribution<float> cloudTime(5.0f, 10.0f);
	cloudRandTime = cloudTime(randomEngine);
}

void GameManager::SpawnMoon()
{
	uniform_real_distribution<float> distPositionX(-200.0f, 200.0f);
	float nRandX = distPositionX(randomEngine);
	uniform_real_distribution<float> distPositionY(90.0f, 135.0f);
	float nRandY = distPositionY(randomEngine);
	moon.push_back(new Moon(D3DXVECTOR2(nRandX, nRandY)));
	uniform_real_distribution<float> moonTime(0.0f, 1.0f);
	moonRandTime = moonTime(randomEngine);
}

void GameManager::SpawnStar()
{
	uniform_int_distribution<int> distIndex(0, 2);
	int nrandIndex = distIndex(randomEngine);
	uniform_real_distribution<float> distPositionX(-200.0f, 200.0f);
	float nRandX = distPositionX(randomEngine);
	uniform_real_distribution<float> distPositionY(80.0f, 135.0f);
	float nRandY = distPositionY(randomEngine);
	star.push_back(new Star(D3DXVECTOR2(nRandX, nRandY),
		starXY[nrandIndex].startX, starXY[nrandIndex].startY,
		starXY[nrandIndex].endX, starXY[nrandIndex].endY));
		uniform_real_distribution<float> starTime(5.0f, 10.0f);
	starRandTime = starTime(randomEngine);
}
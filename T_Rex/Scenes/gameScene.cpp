#include "stdafx.h"
#include "GameScene.h"
#include "Objects/Background.h"
#include "Scenes/GameManager.h"
#include "Objects/t_rex.h"
#include "Collider/Collider.h"
#include "UI/UI.h"
/*
	T-rex ����� (���бⰣ����)
*/
// ���� lambda �Լ� ����
Shader* shader = nullptr;
Background* background = nullptr;
GameManager* gm = nullptr;
T_rex* t_rex = nullptr;
Collider* collider = nullptr;
UI* ui = nullptr;

GameScene::GameScene(SceneValues * values)
	:Scene(values)
{
	background = new Background();
	gm = new GameManager();
	t_rex = new T_rex(D3DXVECTOR2(-290, 0), D3DXVECTOR2(1, 1));
	collider = new Collider();
	ui = new UI(D3DXVECTOR2(300.0f, 220.0f));
	bool (UI::*CallUICrash)(bool) = &UI::Crash;
}

GameScene::~GameScene()
{
	SAFE_DELETE(background);
	SAFE_DELETE(gm);
	SAFE_DELETE(t_rex);
	SAFE_DELETE(collider);
	SAFE_DELETE(ui);
}

void GameScene::Update()
{
	D3DXMATRIX V = values->MainCamera->View();
	D3DXMATRIX P = values->Projection;

	if (gameState == GAMESTATE::RUN) 
	{
		background->Update(V, P);
		bSwapGround = Collider::Aabb(t_rex->GetSprite()->World(), background->getLineTriggerWorld());
		background->SwapGround(bSwapGround);

		gm->Update(V, P);
		t_rex->Update(V, P);
		collider->World(t_rex->GetSprite()->World());
		collider->Update(V, P);
		ui->Update(V, P);

		// �浹 ����
		CheckCrash(V, P);

		// �㳷 ����
		ChangeDayNight();

		// �ð��� ���� �ӵ��� ��ȭ ����
		gamePlayTime += Time::Delta();
		if (gamePlayTime > gameTime)
		{
			if(moveSpeed < 350.0f)
				moveSpeed += moveSpeed * 0.2;
			background->SetMoveSpeed(moveSpeed);
			gm->setMoveSpeed(moveSpeed);
			gamePlayTime = 0.0f;
		}
	}
	// ���� �Ͻ�����(���ӿ���) -> ���� �ٽ� ����
	else if (gameState == GAMESTATE::STOP)
	{
		if (Key->Down(VK_SPACE) || Key->Down(VK_UP))
		{
			Reset();
			background->SetMoveSpeed(moveSpeed);
			gm->Reset();
			ui->Reset();
			ui->Update(V, P);
			
			gameState = GAMESTATE::RUN;
		}
	}
}

void GameScene::Render()
{
	//ImGui::Checkbox("crash", &bCrash);
	background->Render();
	gm->Render();
	t_rex->Render();
	collider->Render();
	collider->DrawColliderColor(bSwapGround);
	ui->Render();
}

void GameScene::CheckCrash(D3DXMATRIX& V, D3DXMATRIX& P)
{
	// ���� bird �Ǵ� cactus�� �ε����ٸ�
	if (gm->GetBird().size() > 0)
	{
		for (auto& b : gm->GetBird())
		{
			bCrash = Collider::Aabb(t_rex->GetSprite()->World(), b->GetSprite()->World());
			if (bCrash)
			{
				ui->Crash(bCrash);
				ui->SpawnGameOverSprite();
				ui->Update(V, P);
				gameState = GAMESTATE::STOP;
			}
		}
	}

	if (gm->GetCactus().size() > 0)
	{
		for (auto& c : gm->GetCactus())
		{
			bCrash = Collider::Aabb(t_rex->GetSprite()->World(), c->GetSprite()->World());
			if (bCrash)
			{
				ui->Crash(bCrash);
				ui->SpawnGameOverSprite();
				ui->Update(V, P);
				gameState = GAMESTATE::STOP;
			}
		}
	}
}

void GameScene::ChangeDayNight()
{
	// �� �� ����, ��->12��, ��->60��
	if (bDay == true)
	{
		dayPlayTime += Time::Delta();
		if (dayPlayTime > dayTime)
		{
			bNight = true;
			bDay = false;
			gm->setNight(bNight);
			dayPlayTime = 0.0f;
		}
	}

	if (bNight == true)
	{
		nightPlayTime += Time::Delta();
		if (nightPlayTime > nightTime)
		{
			bNight = false;
			bDay = true;
			gm->setNight(bNight);
			nightPlayTime = 0.0f;
		}
	}
}

void GameScene::Reset()
{
	multipleTime = 1; //�ӵ� �������
	moveSpeed = 200.0f;
	gamePlayTime = 0.0f;
	dayPlayTime = 0.0f;
	nightPlayTime = 0.0f;
	bNight = false;
	bDay = true;
}

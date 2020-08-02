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
	background->Update(V, P);
	bSwapGround = Collider::Aabb(t_rex->GetSprite()->World(), background->getLineTriggerWorld());
	background->SwapGround(bSwapGround);
	
	gm->Update(V, P);
	t_rex->Update(V, P);
	collider->World(t_rex->GetSprite()->World());
	collider->Update(V, P);
	ui->Update(V, P);

	// ���� bird �Ǵ� cactus�� �ε����ٸ�
	if(gm->GetBird().size() >0)
	{
		for (auto& b : gm->GetBird())
		{
			bCrash = Collider::Aabb(t_rex->GetSprite()->World(), b->GetSprite()->World());
			if (bCrash) {}
				//���� ����
		}
	}
	
	if (gm->GetCactus().size() > 0)
	{
		for (auto& c : gm->GetCactus())
		{
			Collider::Aabb(t_rex->GetSprite()->World(), c->GetSprite()->World());
			if (bCrash) {}
				//���� ����
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

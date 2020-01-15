#pragma once
#pragma comment(lib, "msimg32.lib") //TransparentBlt �� ����ϱ� ����.

#include<Windows.h>
#include<stdio.h>
#include"resource.h"
class CSceneInfo
{
private:
	int SceneINDEX;
	int BackGroundIndex;

	unsigned int MonsterCount; //���� ��
	int MonsterType[3];//���� ����

	bool isBoss;

	HBITMAP BackGroundBit[3]; //��� ��Ʈ��
	HBITMAP MonsterBit[4];    //���� ��Ʈ��
	HBITMAP BossBit;
	RECT GridRect_Monster[3]; //�׸��� ���� ��Ʈ
public:
	CSceneInfo();

	void SceneInit(HINSTANCE hInst);

	void BackGroundDraw(HDC hdc);
	void SceneIndexDraw(HDC hdc);
	void MonsterDraw(HDC hdc);
	void GridDraw(HDC hdc);

	void SetSceneIndex(int idx);
	const int GetSceneIndex() const;

	void SetGridRectPos(int idx, int left, int top, int right, int bottom);
	
	void SetMonsterCount(int count);
	void SwitchingIsBoss();
	bool GetIsBoss() const;
	int SelectRect_ChangeMonster(int x, int y);
	void SetMonsterType(int num, int type);

	void PrevBackGround();
	void NextBackGround();

	BOOL LoadSceneFile(char* strFileName);
	BOOL SaveSceneFile(char* strFileName);

};


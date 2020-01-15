#include "CSceneInfo.h"

CSceneInfo::CSceneInfo() : SceneINDEX(-1), BackGroundIndex(-1), MonsterCount(0),isBoss(false)
{
	for (int i = 0; i < 3; i++)
	{
		MonsterType[i] = -1;
	}
}

void CSceneInfo::SceneInit(HINSTANCE hInst)
{
	BackGroundBit[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_BACK1));
	BackGroundBit[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_BACK2));
	BackGroundBit[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_BACK3));

	MonsterBit[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_MONSTER1));
	MonsterBit[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_MONSTER2));
	MonsterBit[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_MONSTER3));
	MonsterBit[3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_MONSTER4));

	BossBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_BOSS));
}

void CSceneInfo::BackGroundDraw(HDC hdc)
{
	HDC MemDC;
	HBITMAP OldBit;
	MemDC = CreateCompatibleDC(hdc);
	switch (BackGroundIndex)
	{
	case 0:
		OldBit = (HBITMAP)SelectObject(MemDC, BackGroundBit[0]);
		break;
	case 1:
		OldBit = (HBITMAP)SelectObject(MemDC, BackGroundBit[1]);
		break;
	case 2:
		OldBit = (HBITMAP)SelectObject(MemDC, BackGroundBit[2]);
		break;
	default:
		DeleteDC(MemDC);
		return;
	}
	BitBlt(hdc, 0, 0, 650, 650, MemDC, 0, 0, SRCCOPY);
	SelectObject(MemDC, OldBit);
	DeleteDC(MemDC);
}

void CSceneInfo::SceneIndexDraw(HDC hdc)
{
	char str[30];
	sprintf(str, "< SceneINDEX :%d >", SceneINDEX);
	TextOut(hdc, 0, 0, str, strlen(str));
}

void CSceneInfo::MonsterDraw(HDC hdc)
{
	HDC MemDC;
	HBITMAP OldBit;
	MemDC = CreateCompatibleDC(hdc);

	if (isBoss)
	{
		OldBit = (HBITMAP)SelectObject(MemDC, BossBit);
		TransparentBlt(hdc, GridRect_Monster[0].left, GridRect_Monster[0].top,
			250, 250, MemDC, 0, 0, 250, 250,RGB(255,255,255));
		SelectObject(MemDC, OldBit);
		DeleteDC(MemDC);
		return;
	}
	OldBit = (HBITMAP)SelectObject(MemDC, MonsterBit[0]);
	for (int i = 0; i < MonsterCount; i++)
	{
		if (MonsterType[i] == -1)
		{
			continue;
		}
		OldBit = (HBITMAP)SelectObject(MemDC, MonsterBit[MonsterType[i]]);
		TransparentBlt(hdc, GridRect_Monster[i].left, GridRect_Monster[i].top,
			100, 200, MemDC, 0, 0, 100, 200, RGB(48, 120, 128));
	}
	SelectObject(MemDC, OldBit);
	DeleteDC(MemDC); 
}

void CSceneInfo::GridDraw(HDC hdc)
{
	int i = 0;
	HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
	HPEN myPen = CreatePen(BS_SOLID, 2, RGB(255, 0, 0));
	HPEN oldPen = (HPEN)SelectObject(hdc, myPen);
	while (i<MonsterCount)
	{
		Rectangle(hdc, GridRect_Monster[i].left, GridRect_Monster[i].top, GridRect_Monster[i].right, GridRect_Monster[i].bottom);
		i++;
	}
	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldPen);
	DeleteObject(myPen);
	DeleteObject(myBrush);

}

void CSceneInfo::SetSceneIndex(int idx)
{
	SceneINDEX = idx;
}

const int CSceneInfo::GetSceneIndex() const
{
	return SceneINDEX;
}

void CSceneInfo::SetGridRectPos(int idx, int left, int top, int right, int bottom)
{
	if (idx<0 || idx>MonsterCount)
		return;
	GridRect_Monster[idx].left = left;
	GridRect_Monster[idx].top = top;
	GridRect_Monster[idx].right = right;
	GridRect_Monster[idx].bottom = bottom;
}

void CSceneInfo::SetMonsterCount(int count)
{
	MonsterCount = count;
}

void CSceneInfo::SwitchingIsBoss()
{
	isBoss = !isBoss;
}

bool CSceneInfo::GetIsBoss() const
{
	return isBoss;
}

int CSceneInfo::SelectRect_ChangeMonster(int x, int y)
{
	int result = -1;
	for (int i = 0; i < MonsterCount; i++)
	{
		if (GridRect_Monster[i].left <= x && GridRect_Monster[i].right >= x && GridRect_Monster[i].top <= y && GridRect_Monster[i].bottom >= y)
		{
			return i;
		}
	}
	return result;
}

void CSceneInfo::SetMonsterType(int num,int type)
{
	MonsterType[num] = type;
}

void CSceneInfo::PrevBackGround()
{
	if (BackGroundIndex <= 0)
		return;
	BackGroundIndex--;
}

void CSceneInfo::NextBackGround()
{
	if (BackGroundIndex == 2)
		return;
	BackGroundIndex++;
}

BOOL CSceneInfo::LoadSceneFile(char* strFileName)
{
	FILE* fp;
	fp = fopen(strFileName, "rt");
	int bossis;

	fscanf(fp, "%d\n", &SceneINDEX);
	fscanf(fp, "%d\n", &BackGroundIndex);
	fscanf(fp, "%d\n", &MonsterCount);
	fscanf(fp, "%d\n", &bossis);
	bossis == 1 ? isBoss = true : isBoss = false;

	for (int i = 0; i < MonsterCount; i++)
	{
		int typetemp, left, top, right, bottom;
		fscanf(fp, "%d %d %d %d %d\n", &typetemp, &left, &top,
			&right, &bottom);
		MonsterType[i] = typetemp;
		GridRect_Monster[i].left = left;
		GridRect_Monster[i].top = top;
		GridRect_Monster[i].right = right;
		GridRect_Monster[i].bottom = bottom;
	}

	fclose(fp);

	return TRUE;
}

BOOL CSceneInfo::SaveSceneFile(char* strFileName)
{
	FILE* fp;
	fp = fopen(strFileName, "wt");

	fprintf(fp, "%d\n", SceneINDEX);
	fprintf(fp, "%d\n", BackGroundIndex);
	fprintf(fp, "%d\n", MonsterCount);
	fprintf(fp, "%d\n", isBoss?1:0);

	for (int i = 0; i < MonsterCount; i++)
	{
		fprintf(fp, "%d %d %d %d %d\n", MonsterType[i],GridRect_Monster[i].left, GridRect_Monster[i].top,
			GridRect_Monster[i].right, GridRect_Monster[i].bottom);
	}

	fclose(fp);

	return TRUE;
}

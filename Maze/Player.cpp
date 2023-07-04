#include "pch.h"
#include "Player.h"
#include "Board.h"
#include <stack>

void Player::Init(Board* board)
{
	_pos = board->GetEnterPos();
	_board = board;
	
	// RightHand();
	// Bfs();
	AStar();
}

void Player::Update(uint64 deltaTick)
{
	if (_pathIndex >= _path.size())
		return;
	
	_sumTick += deltaTick;
	if (_sumTick > MOVE_TICK)
	{
		_sumTick = 0;
		_pos = _path[_pathIndex];
		_pathIndex++;
	}
}

bool Player::CanGo(Pos pos)
{
	TileType tileType = _board->GetTileType(pos);
	return tileType == TileType::EMPTY;
}

void Player::SetPath(Pos& dest, map<Pos,Pos>& parent)
{
	_path.clear();
	_pathIndex = 0;
	
	Pos pos = dest;
	while (true)
	{
		_path.push_back(pos);
		// 시작점은 자신이 곧 부모이기때문
		if(pos == parent[pos])  
			break;
		pos = parent[pos];
	}
	std::reverse(_path.begin(), _path.end());
}

void Player::RightHand()
{
	Pos pos = _pos;

	_path.clear();
	_path.push_back(pos);
	
	Pos dest = _board->GetExitPos();

	Pos movementForDirection[4] = 
	{
		Pos {-1, 0},
		Pos {0, -1},
		Pos {1, 0},
		Pos {0, 1},
	};
	
	
	while (pos != dest)
	{
		// 현재 바라보는 방향을 기준으로 오른쪽으로 갈 수 있는지 확인
		int32 newDir = (_dir - 1 + DIR_COUNT) % DIR_COUNT;
		if (CanGo(pos + movementForDirection[newDir]))
		{
			// 오른쪽 방향으로 90도 회전
			_dir = newDir;
			// 앞으로 한보 전진
			pos += movementForDirection[_dir];
			_path.push_back(pos);
		}
		// 현재 바라보는 방향을 기준으로 앞쪽으로 전진할수 있는지 확인
		else if (CanGo(pos + movementForDirection[_dir]))
		{
			// 앞으로 한보 전진
			pos += movementForDirection[_dir];
			_path.push_back(pos);
		}
		else
		{
			// 왼쪽 방향으로 90도 회전
			_dir = (_dir + 1) % DIR_COUNT;
		}
	}

	// 스택사용해서 경로 걸러주기
	stack<Pos> s;
	for (int i = 0; i < _path.size() -1; i++)
	{
		if (!s.empty() && s.top() == _path[i + 1])
			s.pop();
		else
			s.push(_path[i]);
	}

	// 목적지 도착
	if (!_path.empty())
		s.push(_path.back());

	vector<Pos> path;
	while (!s.empty())
	{
		path.push_back(s.top());
		s.pop();
	}
	std::reverse(path.begin(), path.end());
	_path = path;
}

void Player::Bfs()
{
	Pos pos = _pos;

	Pos dest = _board->GetExitPos();

	Pos movementForDirection[4] = 
	{
		Pos {-1, 0},
		Pos {0, -1},
		Pos {1, 0},
		Pos {0, 1},
	};

	const int32 size = _board->GetSize();
	vector<vector<bool>> discovered(size, vector<bool>(size, false));
	// A는 B로 인해 발견함
	map<Pos, Pos> parent;
	
	queue<Pos> q;
	q.push(pos);
	discovered[pos.y][pos.x] = true;
	parent[pos] = pos;

	while (!q.empty())
	{
		pos = q.front();
		q.pop();

		if (pos == dest)
			break;

		for (int32 dir = 0; dir < 4; dir++)
		{
			Pos nextPos = pos + movementForDirection[dir];

			if (!CanGo(nextPos))
				continue;
			if (discovered[nextPos.y][nextPos.x])
				continue;
			
			q.push(nextPos);
			discovered[nextPos.y][nextPos.x] = true;
			parent[nextPos] = pos;
		}
	}
	
	SetPath(dest, parent);
}

struct PQNode
{
	bool operator < (const PQNode& other) const { return f < other.f; }
	bool operator > (const PQNode& other) const { return f > other.f; }
	int32 f;
	int32 g;
	Pos pos;
};

void Player::AStar()
{
	// 점수 매기기   F = G + H
	// F : 최종 점수 (작을수록 좋음)
	// G : 시작점에서 해당 좌표까지 이동하는데 드는 비용 
	// H : 목적지에서 얼마나 가까운지 , 고정
	Pos start = _pos;
	Pos dest = _board->GetExitPos();

	enum
	{
		// DIR_COUNT = 4,
		DIR_COUNT = 8,
	};
	
	Pos movementForDirection[] = 
	{
		Pos {-1, 0},
		Pos {0, -1},
		Pos {1, 0},
		Pos {0, 1},
		Pos {-1, -1},
		Pos { 1, -1},
		Pos {1, 1},
		Pos {-1, 1}, 
	};

	int32 cost[] =
	{
		10,
		10,
		10,
		10,
		14,
		14,
		14,
		14,
	};

	const int32 size = _board->GetSize();

	vector<vector<bool>> visited(size, vector<bool>(size, false));
	vector<vector<int32>> best(size, vector<int32>(size, INT32_MAX));
	map<Pos, Pos> parent;
	priority_queue<PQNode, vector<PQNode> ,greater<PQNode>> pq;
	// 예약(발견) 시스템 구현
	// 뒤늦게 더 좋은 경로가 발견될 수 있음 -> 예외 처리 필수

	// 초기값
	int32 g = 0;
	int32 h = 10 * (abs(dest.y - start.y) + abs(dest.x - start.x));
	pq.push(PQNode{g+h, g, start});
	best[start.y][start.x] = g + h;
	parent[start] = start;

	while (!pq.empty())
	{
		PQNode node = pq.top();
		pq.pop();

		// 더 빠른 경로로 인해서 이미 방문된 경우 스킵
		if (visited[node.pos.y][node.pos.x] == true)
			continue;
		visited[node.pos.y][node.pos.x] = true;

		if (node.pos == dest)
			break;

		for (int32 dir = 0; dir < DIR_COUNT; dir++)
		{
			Pos nextPos = node.pos + movementForDirection[dir];
			if(CanGo(nextPos) == false)
				continue;
			if(visited[nextPos.y][nextPos.x])
				continue;

			int32 g = node.g + cost[dir];
			int32 h = 10 * (abs(dest.y - nextPos.y) + abs(dest.x - nextPos.x));

			if(best[nextPos.y][nextPos.x] <= g + h)
				continue;

			// 예약 진행
			best[nextPos.y][nextPos.x] = g + h;
			pq.push( PQNode{ g+h, g, nextPos});
			parent[nextPos] = node.pos;
		}
	}

	SetPath(dest, parent);
}

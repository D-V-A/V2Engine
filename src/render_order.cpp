#include <cassert>
#include <queue>

#include "render_order.h"
#include "entity.h"
#include "isometric.h"

std::vector<const Entity*> BuildRenderOrder(const std::vector<const Entity*>& entities)
{
	std::vector<std::vector<size_t>> edges(entities.size());
	std::vector<int> incomingEdges(entities.size(), 0);

	std::vector<Rect> bounds;
	bounds.reserve(entities.size());

	for (const Entity* entity : entities)
	{
		bounds.push_back(entity->GetRenderOrderBounds());
	}

	for (size_t i = 0; i < entities.size(); ++i)
	{
		for (size_t j = i + 1; j < entities.size(); ++j)
		{
			const bool firstBehindSecond = IsBehind(bounds[i], bounds[j]);
			const bool secondBehindFirst = IsBehind(bounds[j], bounds[i]);

			if (firstBehindSecond && !secondBehindFirst)
			{
				edges[i].push_back(j);
				++incomingEdges[j];
			}
			else if (secondBehindFirst && !firstBehindSecond)
			{
				edges[j].push_back(i);
				++incomingEdges[i];
			}
		}
	}

	std::queue<size_t> ready;

	for (size_t i = 0; i < incomingEdges.size(); ++i)
	{
		if (incomingEdges[i] == 0)
			ready.push(i);
	}

	std::vector<const Entity*> sortedQueue;
	sortedQueue.reserve(entities.size());

	while (!ready.empty())
	{
		const size_t current = ready.front();
		ready.pop();

		sortedQueue.push_back(entities[current]);

		for (size_t next : edges[current])
		{
			--incomingEdges[next];

			if (incomingEdges[next] == 0)
				ready.push(next);
		}
	}
	assert(sortedQueue.size() == entities.size());
	return sortedQueue;
}
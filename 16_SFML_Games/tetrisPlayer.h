#pragma once

class TetrisPlayer
{
public:
	TetrisPlayer() {}

	int getBlockPerShapeNum() { return blockPerShapeNum; }

	int getBlockSpriteSize() { return blockSpriteSize; }

	int getTotalColourOptions() { return totalColourOptions; }

private:
	const int blockSpriteSize = 18;
	const int blockPerShapeNum = 4;
	const int totalColourOptions = 7;
};

#include "pch.h"


#include"../16_SFML_Games/tetrisPlayer.h"

TEST(TetrisPlayer, giveBlocks)
{
	TetrisPlayer playa;
	EXPECT_EQ(4, playa.getBlockPerShapeNum());
}
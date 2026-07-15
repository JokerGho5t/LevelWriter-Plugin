#include "Core/LWGameplayTags.h"

namespace LevelWriterTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Channel_System, "LevelWriter.Channel.System", "A channel for global signals");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Channel_Gameplay, "LevelWriter.Channel.Gameplay", "A channel for gameplay signals");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Start, "LevelWriter.Event.Start", "Event start signal");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_End, "LevelWriter.Event.End", "Event end signal");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Door_Open, "LevelWriter.Event.Door.Open", "Door opening/opened signal");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Door_Close, "LevelWriter.Event.Door.Close", "Door closing/closed signal");
};
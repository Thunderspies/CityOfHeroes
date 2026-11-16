#############################################################
## Enemy_Spawn.fx
#############################################################

FxInfo

Lifespan 30

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Bev
		Type	Local
		At	Root
		Bhvr	behaviors/fadein.bhvr
		Flags	AdoptParentEntity
	End

	Event
		Ename	GeyserHighlights
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 -7.0 0.0
		End
		Part	:GeyserHighlights_EnemySpawn.part
		Part	:GeyserCore_EnemySpawn.part
		Part	:GeyserFlash_EnemySpawn.part
		Lifespan 12
	End

	Event
		Ename	Steam
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 2.0 0
		End
		Part	:SteamAdditive_EnemySpawn.part
		Part	:SteamAlpha_EnemySpawn.part
		Lifespan 6
	End
End

#############################################################

End































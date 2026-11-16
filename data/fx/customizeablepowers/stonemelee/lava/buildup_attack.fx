#############################################################
## StoneMeleeFollowUpHit.fx
#############################################################

FxInfo

Lifespan 75

#########################################################

Condition
	Repeat		4
	RepeatJitter	2

	Event
		Type	Local
		At	Chest
		ChildFx	CustomizeablePowers\EarthControl\Lava\Child_RockChunkBrownScatter.fx
		Lifespan 1
	End

	Event
		HARDWAREONLY
		Type	Local
		At	Chest
		ChildFx	CustomizeablePowers\EarthControl\Lava\Child_RockChunkBrownScatter.fx
		Lifespan 1
	End
End

Condition
	Repeat		4
	RepeatJitter	2

	Event
		Type	Local
		At	Chest
		ChildFx	CustomizeablePowers\EarthControl\Lava\Child_RockDebrisDarkScatter.fx
		Lifespan 90
	End

	Event
		HARDWAREONLY
		Type	Local
		At	Chest
		ChildFx	CustomizeablePowers\EarthControl\Lava\Child_RockDebrisDarkScatter.fx
		Lifespan 90
	End
End

Condition
	Repeat		4
	RepeatJitter	2

	Event
		Type	Local
		At	Chest
		ChildFx	CustomizeablePowers\EarthControl\Lava\Child_RockDebrisGoldScatter.fx
		Lifespan 1
	End

	Event
		HARDWAREONLY
		Type	Local
		At	Chest
		ChildFx	CustomizeablePowers\EarthControl\Lava\Child_RockDebrisGoldScatter.fx
		Lifespan 1
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	root
		ChildFx	Powers\Child_BuildUp.fx
		Lifespan 80
	End
End

#############################################################

End
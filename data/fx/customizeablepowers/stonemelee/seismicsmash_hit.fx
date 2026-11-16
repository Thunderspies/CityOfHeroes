#############################################################
## SeismicSmash_Hit.fx
#############################################################

FxInfo

Lifespan 100

## FALLBACK NODES ###########################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		bhvr	Behaviors\CameraShake03Subtle.bhvr
		Part	CustomizeablePowers\StoneMelee\StalagtiteDustHit.part
		Part	CustomizeablePowers\StoneMelee\StalagtiteDustHit2.part
		Sound seismicblast 100.0 100.0 0.88
	End
End

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat	2

	Event
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0 0.5 0
		End
		ChildFX	CustomizeablePowers\EarthControl\Child_RockDebrisGold.fx
		Lifespan 100
	End


	Event
		HardwareOnly
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0 0.5 0
		End
		ChildFX	CustomizeablePowers\EarthControl\Child_RockDebrisGold.fx
		Lifespan 100
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	11

	Event
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0 0.5 0
		End
		ChildFX	CustomizeablePowers\EarthControl\Child_RockDebrisGoldScatter.fx
		Lifespan 100
	End


	Event
		HardwareOnly
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0 0.5 0
		End
		ChildFX	CustomizeablePowers\EarthControl\Child_RockDebrisGoldScatter.fx
		Lifespan 100
	End
End

#############################################################

End
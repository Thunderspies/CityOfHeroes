#############################################################
## Fault_Hit.fx
#############################################################

FxInfo
Lifespan 100

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Part	CustomizeablePowers\StoneMelee\StalagtiteDustHit.part
		Part	CustomizeablePowers\StoneMelee\StalagtiteDustHit2.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat	6

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
		End
		ChildFX	CustomizeablePowers\EarthControl\Child_RockDebrisGold.fx
		Lifespan 100
	End

	Event
		HardwareOnly
		Type	Local
		At	Root
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
	Repeat	6

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 0.5 0
		End
		ChildFX	CustomizeablePowers\EarthControl\Child_RockDebrisGoldExplode.fx
		Lifespan 100
	End

	Event
		HardwareOnly
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 0.5 0
		End
		ChildFX	CustomizeablePowers\EarthControl\Child_RockDebrisGoldExplode.fx
		Lifespan 100
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	6

	Event
		HardwareOnly
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 0.5 0
		End
		ChildFX	CustomizeablePowers\EarthControl\Child_RockDebrisGoldExplode.fx
		Lifespan 100
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	15

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
		End
		ChildFX	CustomizeablePowers\EarthControl\Child_RockDebrisGoldScatter.fx
		Lifespan 100
	End

	Event
		HardwareOnly
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 0.5 0
		End
		ChildFX	CustomizeablePowers\EarthControl\Child_RockDebrisGoldScatter.fx
		Lifespan 100
	End
End

#############################################################

End
#############################################################
## EarthWeaponHit.fx
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
		EName 	Mallet
		Type	Local
		At	Chest
		Part	:StalagtiteDustHit.part
		Part	:StalagtiteDustHit2.part
		Bhvr	Behaviors\CameraShake03Subtle.bhvr
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
		ChildFX	V_COV\PhysicsEnabled\RockDebrisGold.fx
		Lifespan 100
	End


	Event
		HardwareOnly
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0 0.5 0
		End
		ChildFX	V_COV\PhysicsEnabled\RockDebrisGold.fx
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
		ChildFX	V_COV\PhysicsEnabled\RockDebrisGoldScatter.fx
		Lifespan 100
	End


	Event
		HardwareOnly
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0 0.5 0
		End
		ChildFX	V_COV\PhysicsEnabled\RockDebrisGoldScatter.fx
		Lifespan 100
	End
End

#############################################################

End
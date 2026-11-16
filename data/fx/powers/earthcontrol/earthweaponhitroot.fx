#############################################################
## EarthWeaponHitRoot.fx
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
		Part	:StalagtiteDustHit.part
		Part	:StalagtiteDustHit2.part
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
		ChildFX	V_COV\PhysicsEnabled\RockDebrisGold.fx
		Lifespan 100
	End

	Event
		HardwareOnly
		Type	Local
		At	Root
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
	Repeat	6

	Event
		HardwareOnly
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 0.5 0
		End
		ChildFX	V_COV\PhysicsEnabled\RockDebrisGoldExplode.fx
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
		ChildFX	V_COV\PhysicsEnabled\RockDebrisGoldScatter.fx
		Lifespan 100
	End

	Event
		HardwareOnly
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 0.5 0
		End
		ChildFX	V_COV\PhysicsEnabled\RockDebrisGoldScatter.fx
		Lifespan 100
	End
End

#############################################################

End
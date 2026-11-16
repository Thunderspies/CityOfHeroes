#############################################################
## Halloween_Sigil_Spawn.fx
#############################################################

FxInfo
LifeSpan 300

#########################################################

Condition
	On 	Time
	Time	20

	Event
		EName	flash
		Type	start
		At	Root
		Bhvr	behaviors\CameraShake05Subtle.bhvr
		Sound Sigil_Spawn 500 500 1
		Lifespan 120
	End

	Event
		Ename	SpinOffset
		Type	Start
		At	Root
		BhvrOverride
			Spin	0 .2 0
		End
        End

	Event
		Ename	Offset
		Type	Local
		At 	SpinOffset
		BhvrOverride
			PositionOffset 0 0 20
		End
	End

	Event
		Ename 	Crack
		Type	StartPositOnly
		At	Root
		Bhvr	:Splat_FootStomp.bhvr
		BhvrOverride
			Alpha		255
			PyrRotateJitter 0 0 0
			StartColor 100 150 255
			SplatFlags ADDITIVE
			Scale 40 0.0 40
		End
		Splat	StoneCracks_Additive.tga #impactCracks.tga #crackedEarth2.tga
		Lifespan 150

	End



	Event
		Type	Start
		At	Root
		ChildFX	V_COV\PhysicsEnabled\RubbleBounce.fx
	End
End

Condition
	On	Cycle
	Time	5

	Event
		Type	Start
		At	Flash
		ChildFX	V_COV\PhysicsEnabled\RubbleBounce_Continuing.fx
		Lifespan 30
	End

End

Condition
	On 	Time
	Time	20
	Repeat 	4

	Event
		EName 	Prime
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 -10 0
			StartJitter	1 5 1
		End
		Part	WORLD/Lairs/cot/Base_Dust_Burst_Origin.part

		Lifespan 10
	End
End

Condition
	On 	Time
	Time 	25

	Event
		EName 	Prime
		Type	Start
		At	Root
		Part	WORLD/Lairs/cot/Base_Dust_Burst.part
		Lifespan 20
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName 	Prime
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 -13 0
		End
		Part	WORLD/Lairs/cot/Base_Fire_Burst.part
		Lifespan 30
	End
End

Condition
	On	Time
	Time	25

	Event
		EName 	Prime
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 -18 0
		End
		Part	WORLD/Lairs/cot/Base_Fire_Burst.part
		Lifespan 60
	End

	Event
		EName 	Prime
		Type	Start
		At	Root
		Part	WORLD/Lairs/cot/Base_Dust.part
		Part	WORLD/Lairs/cot/Base_Dust_Origin.part
		Lifespan 80
	End
End

Condition
	On	Time
	Time	40

	Event
		Ename 	Crack
		Type	StartPositOnly
		At	Root
		Bhvr	:Splat_FootStomp.bhvr
		BhvrOverride
			Alpha		255
			PyrRotateJitter 0 0 0
			StartColor 100 150 255
			SplatFlags ADDITIVE
			Scale 55 0.0 55
		End
		Splat	impactCracks.tga #crackedEarth2.tga
		Lifespan 90

	End

	Event
		Ename	Ring
		Type	Local
		At	Root #Offset
		BhvrOverride
			PositionOffset	0 -18 0
		End
		Part	WORLD/Lairs/cot/OuterRing_Fire_Burst.part
		Lifespan 35
	End


#########################################################

End
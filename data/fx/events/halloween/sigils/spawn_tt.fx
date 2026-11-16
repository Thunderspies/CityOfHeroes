#############################################################
## Halloween_Sigil_Spawn.fx
#############################################################

FxInfo

LifeSpan 300

#############################################################
#
#Condition
#	On 	Time
#	Time	0
#
#	Event
#		EName	Summon
#		Type	Local
#		At	Root
#		ChildFX	WORLD\Lairs\cot\Halloween_Sigil_Destruction.fx
#	End
#End

#########################################################

Condition
	On 	Time
	Time	0

	Event
		EName	flash
		Type	Local
		At	Root
		Bhvr	behaviors\CameraShake05Subtle.bhvr
		Sound Sigil_Spawn 500 500 1
		Lifespan 120
	End

	Event
		Ename 	Crack
		Type	Start
		At	Root
		Bhvr	WORLD\Lairs\cot\Splat_FootStomp.bhvr
		BhvrOverride
			Alpha			255
			StartColor		100 255 150
			SplatFlags		ADDITIVE
			Scale			55 75 55
		End
		Splat	StoneCracks_Additive.tga
		Lifespan 150
	End

	Event
		Type	Local
		At	Root
		ChildFX	V_COV\PhysicsEnabled\RubbleBounce.fx
	End
End

Condition
	On	Cycle
	Time	5

	Event
		Type	Local
		At	Flash
		ChildFX	V_COV\PhysicsEnabled\RubbleBounce_Continuing.fx
		Lifespan 30
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 -13 0
		End
		Part	WORLD\Lairs\cot\Base_Fire_Burst.part
		Lifespan 30
	End
End

Condition
	On 	Time
	Time	0
	Repeat 	4

	Event
		EName 	Prime
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 -10 0
			StartJitter	1 5 1
		End
		Part	WORLD\Lairs\cot\Base_Dust_Burst_Origin.part
		Lifespan 10
	End
End

Condition
	On 	Time
	Time 	5

	Event
		EName 	Prime
		Type	Local
		At	Root
		Part	WORLD\Lairs\cot\Base_Dust_Burst.part
		Lifespan 20
	End

	Event
		EName 	Prime
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 -18 0
		End
		Part	WORLD\Lairs\cot\Base_Fire_Burst_TT.part
		Lifespan 60
	End

	Event
		EName 	Prime
		Type	Local
		At	Root
		Part	WORLD\Lairs\cot\Base_Dust.part
		Part	WORLD\Lairs\cot\Base_Dust_Origin.part
		Lifespan 80
	End
End

#############################################################

Condition
	On	Time
	Time	20

	Event
		Ename 	Crack
		Type	Start
		At	Root
		Bhvr	WORLD\Lairs\cot\Splat_FootStomp.bhvr
		BhvrOverride
			Alpha		255
			StartColor	50 255 50
			SplatFlags	ADDITIVE
			Scale		55 55 55
		End
		Splat	impactCracks.tga
		Lifespan 90
	End

	Event
		Ename	Ring
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 -18 0
		End
		Part	WORLD\Lairs\cot\OuterRing_Fire_Burst_TT.part
		Lifespan 35
	End
End

#########################################################

End
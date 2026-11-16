#############################################################
## Gunfire_GernadeHit.fx
#############################################################

FxInfo

Lifespan 50

#############################################################

Input
	InpName	Chest
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Fire
		Type	Local
		At	Chest
		Part1	fx\WEAPONFX\FireExplosionSmall.part
		#Part2	powers/firecontrol/FireWeaponHitSpark.part
		#Sound	grexplo4 70 60 .73
		Lifespan 10
	End
End

Condition
	On	Time
	Time	0
	Repeat	1
	RepeatJitter	2

	Event
		EName	Sparkout
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\HotSparkOUT.fx
	End
End

Condition
	On	Time
	Time	0
	Repeat	2
	RepeatJitter	1
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\HotSparkUP.fx
	End
End

Condition
	On	Time
	Time	0
	Repeat	5
	RepeatJitter	1

	Event
		HardwareOnly
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\HotSparkUP.fx
	End

	Event
		HardwareOnly
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\HotSparkOUT.fx
	End
End

#############################################################

End
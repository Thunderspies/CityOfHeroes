#############################################################
## ShotgunHit.fx
#############################################################

FxInfo

LifeSpan 75

#############################################################

Input
	Inpname Chest

End

#############################################################

Condition
	On	Time
	Time	6

	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part1	WEAPONFX\Shotgunflash.part
		Part2	WEAPONFX\ShotgunflashStar.part
		Lifespan 9
	End
End

#############################################################

Condition
	On	Time
	Time	4
	Repeat	4
	RepeatJitter	2

	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\GunSparkOUT.fx
	End
End

#############################################################

Condition
	On	Time
	Time	3
	Repeat	8
	RepeatJitter	2

	Event
		HardwareOnly
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\GunSparkOUT.fx
	End
End

#############################################################

End
#############################################################
## PistolHit2.fx
#############################################################

FxInfo

LifeSpan 60

#############################################################

Input
	Inpname Chest
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part1	WEAPONFX\Pistolflash.part
		Part2	WEAPONFX\PistolflashStar2.part
		LifeSpan 10
		Sound DW_BulletHit_01 50 50 1
		Sound DW_BulletHit_02 50 50 1
		Sound DW_BulletHit_03 50 50 1
		Sound DW_BulletHit_04 50 50 1
		Sound DW_BulletHit_05 50 50 1
	End
End

#############################################################

Condition
	On	Time
	Time	0
	Repeat	2
	RepeatJitter	1

	Event
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\GunSparkOUT.fx
		Lifespan 60
	End

	Event
		HardwareOnly
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\GunSparkOUT.fx
		Lifespan 60
	End
End

#############################################################

End
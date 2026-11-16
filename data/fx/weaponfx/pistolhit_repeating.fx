#############################################################
## PistolHit2.fx
#############################################################

FxInfo

LifeSpan 76

#############################################################

Input
	Inpname Hips
End

#############################################################

Condition
	On	Cycle
	Time	6

	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part1	WEAPONFX\Pistolflash.part
		Part2	WEAPONFX\PistolflashStar2.part
		LifeSpan 10
#		Sound DW_BulletHit_01 50 50 1
#		Sound DW_BulletHit_02 50 50 1
#		Sound DW_BulletHit_03 50 50 1
#		Sound DW_BulletHit_04 50 50 1
#		Sound DW_BulletHit_05 50 50 1
	Lifespan 10

	End
End

#############################################################

End
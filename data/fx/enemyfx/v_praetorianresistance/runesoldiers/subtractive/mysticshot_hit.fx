#############################################################
## SingleShot_Hit.fx
#############################################################

FXInfo
LifeSpan 47

#############################################################

Condition
	On	Time
	Time	2

	Event
		Type	Local
		At	Origin
		Sound RES_PowerRifle_HitSingle_01 30 30 .75
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part	:Hit_Star.part
		Part	:Hit_Flash_Small.part
		LifeSpan 10
	End
End

Condition
	On	Time
	Time	1
	Repeat	4

	Event
		HardwareOnly
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\GunSparkOUT_Purple.fx
		Lifespan 30
	End
End

#############################################################

End
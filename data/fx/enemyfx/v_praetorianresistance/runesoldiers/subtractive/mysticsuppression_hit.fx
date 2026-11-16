#############################################################
## MachineGunHitLong.fx
#############################################################

FxInfo

LifeSpan 30

#############################################################

Condition
	On	Time
	Time	2

	Event
		Type	Local
		At	Origin
		Sound RES_PowerRifle_HitShort_01 30 30 .75
	End
End

Condition
	On 	cycle
	Time 	4

	Event
		EName	Prime
		Type 	Local
		At	Chest
		BhvrOverride
			StartJitter	0.3 0.3 0.3
		End
		Part	:Hit_Star.part
		Part	:Hit_Flash_Small.part
		LifeSpan 2
	End
End

Condition
	On	Time
	Time	5
	Repeat 9


	Event
		HardwareOnly
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\GunSparkOUT_Purple.fx
	End
End

#############################################################

End
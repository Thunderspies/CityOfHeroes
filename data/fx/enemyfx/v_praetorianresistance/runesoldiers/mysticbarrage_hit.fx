#############################################################
## MachineGunHitLong.fx
#############################################################

FxInfo

LifeSpan 45

#############################################################

Condition
	On	Time
	Time	2

	Event
		Type	Local
		At	Origin
		Sound RES_PowerRifle_HitLong_01 30 30 0.75
	End
End

Condition
	On 	cycle
	Time 	4
	Chance	0.75

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
	Time	3
	Chance	0.5

	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			StartJitter	0.3 0.3 0.3
		End
		Part	:Hit_Star.part
		Part	:Hit_Flash_Small.part
		Lifespan 2
	End
End

Condition
	On	Time
	Time	5
	Repeat	9

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
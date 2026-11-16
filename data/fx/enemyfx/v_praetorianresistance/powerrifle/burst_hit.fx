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
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\Hit_Star.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\Hit_Flash_Small.part
		LifeSpan 10
	End
End

Condition
	On	Time
	Time	1
	Repeat	4

	Event
		HardwareOnly
		EName	Prime
		Type 	Local
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\GunSparkOUT_Blue.fx
	End
End

#############################################################

End
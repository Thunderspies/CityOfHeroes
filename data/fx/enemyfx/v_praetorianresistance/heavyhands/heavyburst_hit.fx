#############################################################
## MachineGunHitLong.fx
#############################################################

FxInfo

LifeSpan 40

#############################################################

Condition
	On	Time
	Time	2

	Event
		Type	Local
		At	Origin
		Sound RES_ArmCannon_Hit_01 30 30 .75
	End
End

Condition
	On 	cycle
	Time 	5
	Chance	1

	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\Hit_Star.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\Hit_Flash_Small.part
	#	Part	ENEMYFX\V_PraetorianResistance\PowerCannon\Hit_Shockwave_Small.part
		LifeSpan 10
	End
End

Condition
	On	cycle
	Time	3
	Chance	0.5

	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\GunSparkOUT_Blue.fx
		Lifespan 60
	End
End

Condition
	On	cycle
	Time	3
	Chance	0.9

	Event
		HardwareOnly
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\GunSparkOUT_Blue.fx
		Lifespan 60
	End
End

#############################################################

End
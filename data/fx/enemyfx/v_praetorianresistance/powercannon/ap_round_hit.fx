#############################################################
## SnapShot_Hit.fx
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
	Time	0

	Event
		EName	Prime
		Type 	Local
		At	Chest
		Sound RES_PowerCannon_HitAPRound 50 50 .75
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\Hit_Star.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\Hit_Flash.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\Hit_Shockwave.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\Hit_Shockwave_Ring.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\Hit_Smoke.part
		Part	ENEMYFX\V_PraetorianResistance\PowerCannon\Hit_Smoke_Additive.part

		Lifespan 9
	End
End

#############################################################

Condition
	On	Time
	Time	0
	Repeat	2
	RepeatJitter	1

	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerCannon\Child_Sparks_Out.fx
	End
End

#############################################################

Condition
	On	Time
	Time	0
	Repeat	3
	RepeatJitter	2

	Event
		#HardwareOnly
		EName	Sparkup
		Type 	Start
		At	Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerCannon\Child_Sparks_Out.fx
	End
End

#############################################################

End
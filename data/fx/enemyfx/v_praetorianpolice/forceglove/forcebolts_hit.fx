#############################################################
## ForceBolts_Hit.fx
#############################################################

FxInfo

LifeSpan 15

#############################################################

Input
	InpName	Chest
End

###########################################################

Condition
	On 	Cycle
	Time 	5


	Event
		EName 	hit2
		Type	Start
		At	Chest
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Flash_Small.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Ring_Burst_Small.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Balls_Small.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Streaks_Small.part
		Lifespan 8
	End
End

Condition
	On	Time
	Time	0

	Event
		EName 	hit2
		Type	Start
		At	Chest
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Flash_Small.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Ring_Burst_Small.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Balls_Small.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Streaks_Small.part
		Sound PPD_ForceBolts_Hit_02 100 100 .75
		Lifespan 8
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		1000
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End
End

#############################################################

End
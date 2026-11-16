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
	On	Time
	Time	0

	Event
		EName 	hit2
		Type	Start
		At	Chest
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBrawl_Hit_Flash_Small.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBrawl_Hit_Ring_Burst_Small.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBrawl_Hit_Balls_Small.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBrawl_Hit_Streaks_Small.part
		Sound PPD_ForceBrawl_Hit_01 100 100 .75
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
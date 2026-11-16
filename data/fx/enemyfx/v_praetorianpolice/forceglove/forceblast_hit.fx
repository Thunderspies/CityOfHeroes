#############################################################
## ForceBolts_Hit.fx
#############################################################

FxInfo

LifeSpan 30

#############################################################

Input
	InpName	Chest
End

###########################################################

Condition
	On 	Time
	Time 	0


	Event
		EName 	hit2
		Type	Start
		At	Chest
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Flash.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Glow.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Rings.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Ring_Burst.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Rings_Linger.part

		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Balls.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Streaks.part
		Sound PPD_ForceBlast_Hit_01 100 100 .75
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
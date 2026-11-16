#############################################################
## ForceBolts_Hit.fx
#############################################################

FxInfo

LifeSpan 12

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
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Flash_Large.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Glow_Large.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Rings_Large.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Ring_Burst.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Rings_Linger.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Balls_Large.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Streaks_Large.part
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Streaks_Large_Detached.part
		Sound PPD_PlasmaBlast_Hit_01 100 100 .75
		Lifespan 8
	End
End

Condition
	On	Cycle
	Time	5


	Event
		Type	Start
		At	Chest
		BhvrOverride
			PyrRotateJitter	0 90 180
		End
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceBolts_Hit_Rings_Rotated.part
		Lifespan	1
	End
End

Condition
	On	Time
	Time	0

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
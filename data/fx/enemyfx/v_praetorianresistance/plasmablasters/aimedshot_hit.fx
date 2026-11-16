#############################################################
## AimedShot_Hit.fx
#############################################################

FxInfo

LifeSpan 5

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_Hit_Single.fx
		Sound RES_Pistols_HitAimed_01 30 30 .75
	End
End

#############################################################

End
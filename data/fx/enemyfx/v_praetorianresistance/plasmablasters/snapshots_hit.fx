#############################################################
## Fusilade_Hit.fx
#############################################################

FxInfo

LifeSpan 16

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_Hit_Single.fx
		Sound RES_Pistols_HitFireSnapShot_01 30 30 .75
		LifeSpan 5
	End
End

Condition
	On 	Time
	Time 	8

	Event
		Type	Start
		At	Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_Hit_Single.fx
		LifeSpan 5
	End
End

Condition
	On 	Time
	Time 	16

	Event
		Type	Start
		At	Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_Hit_Single.fx
		LifeSpan 5
	End
End

#############################################################

End
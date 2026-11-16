#############################################################
## Fusilade_Hit.fx
#############################################################

FxInfo

LifeSpan 18

#############################################################

Condition
	On Time
	Time 0
	
	Event
		Type Local
		At Origin
		Sound RES_Pistols_HitFusillade_01 30 30 .75
	End
End

Condition
	On 	Cycle
	Time 	6

	Event
		Type	Start
		At	Chest
		ChildFX	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Child_Hit_Single.fx
		LifeSpan 10
	End
End

#############################################################

End
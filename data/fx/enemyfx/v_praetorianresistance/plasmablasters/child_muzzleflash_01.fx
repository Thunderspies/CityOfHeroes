#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

Lifespan 1

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Origin
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Flash_Star01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Flash_Star02.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Flash_Energy01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Flash_Energy02.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\Flash_Glow01.part
		LifeSpan 1
	End
End

#############################################################

End
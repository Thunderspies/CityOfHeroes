#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

Lifespan 2

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Origin
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\LargeFlash_Star01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\LargeFlash_Star02.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\LargeFlash_Energy01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\LargeFlash_Energy02.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\LargeFlash_Glow01.part
		Part	ENEMYFX\V_PraetorianResistance\PlasmaBlasters\LargeFlash_Sparks01.part
		LifeSpan 2
	End
End

#############################################################

End
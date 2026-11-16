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
		Part	ENEMYFX\V_PraetorianResistance\PowerRifle\MuzzleFlash_Glow01.part
		Part	ENEMYFX\V_PraetorianResistance\PowerRifle\MuzzleFlash_Flare01.part
		Part	ENEMYFX\V_PraetorianResistance\PowerRifle\MuzzleFlash_Gas01.part
		Part	ENEMYFX\V_PraetorianResistance\PowerRifle\MuzzleFlash_Gas02.part
		Part	ENEMYFX\V_PraetorianResistance\PowerRifle\MuzzleFlash_Sparks01.part
		Lifespan 2
	End
End

#############################################################

End
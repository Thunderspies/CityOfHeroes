#############################################################
## RuneCircle_Enchant.fx
#############################################################

FxInfo
Lifespan 200

#########################################################

Condition
	On 	Time
	Time 	2

	Event
		EName 	HitFX
		Type	StartPositOnly
		At	Chest
		Part	:Enchant_Fire.part
		Part	:Enchant_Fire_Alpha.part
		Part	:Enchant_Rune_SparkTrail.part
		Part	:Hit_Whip_Glow.part
		Part	:Enchant_Shockwave.part
		Part	:Enchant_Fire_Geyser.part
		Part	:Enchant_Smoke_Geyser.part
		Lifespan 7
	End
End

#########################################################

End
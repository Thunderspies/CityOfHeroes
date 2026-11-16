#############################################################
## Halloween_Sigil_Spawn.fx
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

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset 		0 0.2 0
		End
		Part	:Hellfire_Origin.part
		Part	:Hellfire_Origin_Alpha.part
		Part	:Hellfire_Origin_Flat.part
		Part	:Hellfire_Origin_Flat_Alpha.part
		Lifespan 20
	End

End

#########################################################

End
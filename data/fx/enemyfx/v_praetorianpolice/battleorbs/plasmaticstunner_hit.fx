#############################################################
## PlasmaticStunner_Attack.fx
#############################################################

FxInfo
LifeSpan 180

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Flash
		Type	Local
		At	Chest
		Part	:Hit_Flash_Highlight.part
		Part	:Hit_Shockwave.part

		Part	:Muzzle_Buildup_Flash_Alpha.part
		Part	:Muzzle_Buildup_Rings_Alpha.part
		Sound IDF_BattleOrb_PlasmaStunner_Hit_01 100 100 .8
		Lifespan 1
	End


End


#############################################################

End
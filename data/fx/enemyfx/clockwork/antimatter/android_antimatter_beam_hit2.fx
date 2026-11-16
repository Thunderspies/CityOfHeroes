#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

#LifeSpan 180

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
	#	Part	:Hit_Beam_Flash.part
		Part	:Muzzle_Buildup_Flash_Highlight.part
		Lifespan 90

	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	Chest
		Part	:Hit_Beam_Flash.part
		Part	:Muzzle_Buildup_Flash_Highlight.part
		Lifespan 60

	End
End

Condition
	On 	Time
	Time 	45

	Event
		Type	Local
		At	Chest
		Part	:Hit_Beam_Flash.part
		Part	:Muzzle_Buildup_Flash_Highlight.part
		Part	:Hit_Targeting_Rings.part
		Lifespan 30

	End
End
#############################################################

End
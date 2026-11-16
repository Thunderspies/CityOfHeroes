#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

Lifespan 120

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
		At	Root
		sound Antimatter_Beam_Hit 200 200 .8
	End

	Event
		Type	Local
		At	Chest
		Part	:Hit_Rings_Subtractive.part
		Part	:Hit_Rings_Alpha.part
		Part	:Hit_Shockwave.part
		Part	:Hit_Flash_Large.part
		Part	:Muzzle_Release_Flash_Dim.part
		Lifespan 5
	End

	Event
		Type	Local
		At	Chest
		ChildFX	:Hit_Body_Antimatter.fx
		Lifespan 90
	End
End

#############################################################

End
#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 60

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
		At	Root
		Part	:Hit_Rings_Subtractive.part
		Part	:Hit_Rings_Alpha.part
		Part	:Hit_Shockwave.part
		Part	:Hit_Flash_Large.part

#		Part	:Hit_Tendril_Core.part
#		Part	:Hit_Rings.part
#		Part	:Hit_Rings_Blue.part

		ChildFX	:Hit_Body_Antimatter.fx

		Lifespan 5
	End
End

#############################################################

End
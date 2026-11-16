#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 30

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
		Sound Plasma_Charged_Blast_Hit 200 200 1
	End

	Event
		Type	Local
		At	Root
		Part	:Hit_Flash_Small_Chemical.part
		Part	:Hit_Glow_Small_Chemical.part
		Part	:Hit_Shockwave_Small_Chemical.part
		Part	:Hit_Tendril_Small_Chemical.part
		Lifespan 1
	End
End

#############################################################

End
#############################################################
## ChargedBrawl_Attack.fx
#############################################################

FxInfo

Lifespan 40

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WepR
		Part	:ElectricityControl01.part
		Part	:ElectricityControl02.part
		Part	:ElectricityInnerGlow.part
		Part	:ElectricityOuterGlow.part
		Part	:ElectricityArch.part
		Part	:ElectricityRing.part
		Part	:ElectricityArchInvert.part
		Sound eleccontrol2 50 50 0.4
	End

	Event
		Type	local
		At	WepL
		Part	:ElectricityControl01.part
		Part	:ElectricityControl02.part
		Part	:ElectricityInnerGlow.part
		Part	:ElectricityOuterGlow.part
		Part	:ElectricityArch.part
		Part	:ElectricityRing.part
		Part	:ElectricityArchInvert.part
	End
End

#############################################################

End
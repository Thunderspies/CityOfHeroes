#############################################################
## SonicPylons_Hit.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	WepR
		Part	:Sonic_Ripple.part
		Part	:Sonic_Glow.part
	End

	Event
		Type	Start
		At	WepL
		Part	:Sonic_Ripple.part
		Part	:Sonic_Glow.part
	End
End

#############################################################

End
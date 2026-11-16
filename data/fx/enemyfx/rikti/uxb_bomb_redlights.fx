#########################################################
##	Villain Base Portal
#########################################################

FxInfo

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	lights
		Type	Start
		At	Root
		
		Geom	UXB_lightHookup

	End
	
	Event
		Type	Start
		At	lights
		Altpiv	1
		
		Part	:SmallBlinkingLightCore.part
		Part	:SmallBlinkingLightsRed.part

	End
	
	Event
		Type	Start
		At	lights
		Altpiv	2
		
		Part	:SmallBlinkingLightCore.part
		Part	:SmallBlinkingLightsRed2.part

	End
	
End

End
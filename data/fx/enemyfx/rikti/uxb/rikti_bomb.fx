#########################################################
##	Villain Base Portal
#########################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	gravWell
		Type	Start
		At	Root
				
		Bhvr	Behaviors/UXB_Fade_UP.bhvr
		Geom	rikti_UXB_Red

	End
End

##########################################################

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
		#Part	:Light_Red1.part
		Part	:BlinkingLight_CoreRed1.part
		Part	:BlinkingLight_GlowRed1.part
	End
	
	Event
		Type	Start
		At	lights
		Altpiv	2
		#Part	:Light_Red1.part
		Part	:BlinkingLight_CoreRed1.part
		Part	:BlinkingLight_GlowRed1.part
	End
End

#########################################################

End
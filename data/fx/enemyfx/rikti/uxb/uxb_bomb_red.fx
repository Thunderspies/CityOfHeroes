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
		Type	Local
		At	Hips
		#Bhvr	Behaviors/UXB_Fade_UP.bhvr
		Geom	FX_Rikti_UXB_Red
	End
	
	Event
		Type	Local
		At	WepR
		#Part	:Light_Red1.part
		Part	:BlinkingLight_CoreRed1.part
		Part	:BlinkingLight_GlowRed1.part
	End
	
	Event
		Type	Local
		At	WepL
		#Part	:Light_Red1.part
		Part	:BlinkingLight_CoreRed1.part
		Part	:BlinkingLight_GlowRed1.part
	End
End

#########################################################

End
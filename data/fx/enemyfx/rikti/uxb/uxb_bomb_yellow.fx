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
		Geom	FX_Rikti_UXB_Yellow
	End
	
	Event
		Type	Local
		At	WepR
		#Part	:Light_Yellow1.part
		Part	:BlinkingLight_CoreYellow1.part
		Part	:BlinkingLight_GlowYellow1.part
	End
	
	Event
		Type	Local
		At	WepL
		#Part	:Light_Yellow1.part
		Part	:BlinkingLight_CoreYellow1.part
		Part	:BlinkingLight_GlowYellow1.part
	End
End

#########################################################

End
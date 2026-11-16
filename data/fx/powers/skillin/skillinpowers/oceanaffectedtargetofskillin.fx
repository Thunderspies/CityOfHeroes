#########################################################
##	template

FxInfo


LifeSpan	100
#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound Fast2 70 70 .7
	End
End



#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	#On	cycle
	Time	5
	
	
	Event
		EName 	Prime2
		Type	local
		At	T_chest
		Part 	:OceanSkillinGlow.part
		Part 	:OceanSkillinReactors.part
		Part 	:OceanSkillinGlowPulse.part
		Part 	:OceanSkillinSparks.part
	End

End

End		
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
		Part 	:PurpleSkillinGlow.part
		Part 	:PurpleSkillinReactors.part
		Part 	:PurpleSkillinGlowPulse.part
		Part 	:PurpleSkillinSparks.part
	End

End

End		
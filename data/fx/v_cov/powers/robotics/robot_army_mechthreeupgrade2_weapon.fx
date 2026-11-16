#########################################################
##	template

FxInfo


Flags    InheritAlpha
#LifeSpan		300
#########################################################
####################### AUDIO ###########################
#########################################################



Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		#Sound	Glow3_loop 80 30 .26
	End
End


#########################################################
#################### FX ########################
#########################################################

Condition
	On	Time
	Time	124
		
	Event	
		Type	local
		At	back
		Bhvr	Behaviors/DefenderbotrocketBay_Fade_UP.bhvr
		Geom	FX_Assault_Rocket_Bay
		
	
	End

	Event	
		Type	local
		At	back
		
		ChildFx		ENEMYFX/5thColumn/HoverBot/AssaultBotEyes.fx
		
	
	End

End

End		
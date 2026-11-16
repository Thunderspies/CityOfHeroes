#########################################################
##	template

FxInfo

LifeSpan	100
#########################################################
####################### AUDIO ###########################
#########################################################

#Condition
#	On	Time
#	Time	0
#	
#
#	Event
#		Type	Local
#		At 	Origin
#		Sound	Fast 80 60 .8
#	End
#End
#
#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition	
	On	Time
	Time	0
	
	Event	
		ENAME	HeadGlow
		Type	Local
		At	Root
		part	:SM_WhiteSmoke.part
		Lifespan 5
	
	End

End


End		
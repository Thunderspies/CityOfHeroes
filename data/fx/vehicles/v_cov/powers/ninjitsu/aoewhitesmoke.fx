#########################################################
##	template

FxInfo


#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound Fast 80 80 .8
	End
End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition	

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Root
		part	:WhiteSmoke.part
		part	:BlackBits.part
		Lifespan 5
	
	End


End


End		
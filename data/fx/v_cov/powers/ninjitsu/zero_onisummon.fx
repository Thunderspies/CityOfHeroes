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
		Sound Fast 80 80 .8
	End
End

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

		part	:Flame.part
		part	:Flame2.part

		Lifespan 10
	
	End

End


Condition	
	On	Time
	Time	1

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Root
		Part	:SnapCap.part
		Part	:SnapCapCore.part
		#Lifespan 5
	
	End

End


End		
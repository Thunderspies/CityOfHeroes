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
		Sound Firetransform 80 80 .9
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

		part	:FireProtectorsFlame.part
		part	:FireProtectorsFlame2.part

		Lifespan 50
	
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
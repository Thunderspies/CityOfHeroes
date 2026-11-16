#########################################################
##	template

FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	Root
End

Input  
	InpName	Chest
End



Lifespan 10000



#########################################################
#####################  CLOUD 1  #########################
#########################################################


Condition

	On 	Time	
	Time	0

	Event	
		ENAME	Overhead
		Type	Posit
		At	Root
		Geom	OverHeadDummy
	End

End

Condition

	On 	Time	
	Time	0

	Event	
		ENAME	Lightning
		Type	Local
		At	Overhead
		AltPiv	1
		PMagnet Target
		part	:SSLSLightningBoltAttack01.part
		LifeSpan 5
	End

End


End		
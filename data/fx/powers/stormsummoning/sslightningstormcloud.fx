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
		ENAME	CloudPrime
		Type	Posit
		At	Root
		Geom	OverHeadDummy
		Sound storm_loop 120 120 .8
	End

End

Condition

	On 	Time	
	Time	45

	Event	
		ENAME	Lightning
		Type	Local
		At	CloudPrime
		AltPiv	1
		part	:SSLSCloudLightning02.part
	End

End

Condition

	On 	Time	
	Time	45

	Event	
		ENAME	Cloud
		Type	Local
		At	CloudPrime
		AltPiv	1
		part	:SSClouds01.part
		part	:SSClouds01move.part
		part	:SSClouds02.part
		part	:SSClouds02sml.part
		#Bhvr	Behaviors/SSCloudScale.bhvr	
	End	

End

Condition

	On 	Time	
	Time	45

	Event	
		ENAME	Lightning
		Type	Local
		At	CloudPrime
		AltPiv	1
		part	:SSLSCloudLightning01.part
	End
End


End		
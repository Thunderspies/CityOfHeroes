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
	End

	Event	
		ENAME	Cloud
		Type	Local
		At	CloudPrime
		AltPiv	1
		part	:SSLightningGlow2.part
		part	:SSClouds01.part
		part	:SSClouds01move.part
		part	:SSClouds02.part
		part	:SSClouds02sml.part
		part	:SSRainDrops01.part
		#Bhvr	Behaviors/SSCloudScale.bhvr	
	End

	Event	
		ENAME	Lightning
		Type	Local
		At	CloudPrime
		AltPiv	1
		part	:SSLightning.part
		PMagnet	Root	
	End
	
	Event	
		ENAME	LightningGlow
		Type	Local
		At	CloudPrime
		AltPiv	1
		part	:SSLightningGlow.part
		#PMagnet	Chest	
	End	

	Event	
		ENAME	RainSplash
		Type	Posit
		At	Root
		part	:SSRainSplash01.part
		part	:SSRainDropsSplash01.part
	End	

End


End		
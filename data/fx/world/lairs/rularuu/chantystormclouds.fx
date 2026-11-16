#########################################################
##	template

FxInfo

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

End

Condition

	#On 	time
	#Time 	0
	#Chance	.50

	Event	
		ENAME	Lightning
		Type	Local
		At	CloudPrime
		AltPiv	1
		part	:CloudLightning01a.part
		part	:CloudLightning02a.part
		#part	:CloudRaina.part
		part	:CloudLightning03a.part
		#part	:ElectricityArchLarge2.part
		
	End

End



End		
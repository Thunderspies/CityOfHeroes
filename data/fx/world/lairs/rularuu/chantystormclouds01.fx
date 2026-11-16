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
		part	:CloudLightning01b.part
		part	:CloudLightning02b.part
		#part	:CloudRainb.part
		part	:CloudLightning03b.part
		#part	:ElectricityArchLarge2.part
		
	End

End



End		
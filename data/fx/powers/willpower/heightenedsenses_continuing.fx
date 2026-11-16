#########################################################
##  Temporary Invulnerability Continuing FX
#########################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0
	
	Event
		Type	Local
		At	Hips
		Sound glow5_loop 46 46 .11
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	400
	End
End

## RICHOCETS #######################################################

#Condition
#	On 	cycle
#	Time 	17
#	Chance	0.125
#
#	Event
#		EName	3
#		Type	Local
#		At	LlegL
#		part	:FortitudeRaysSmall2.part
#		part	:FortitudeRing2.part
#		lifespan	25
#	End
#End
#
#Condition
#	On 	cycle
#	Time 	18
#	Chance	0.125
#
#	Event
#		EName	3
#		Type	Local
#		At	LlegR
#		part	:FortitudeRaysSmall2.part
#		part	:FortitudeRing2.part
#		lifespan	25
#	End
#End
#
#Condition
#	On 	cycle
#	Time 	19
#	Chance	0.125
#	
#	Event
#		EName	3
#		Type	Local
#		At	LarmR
#		part	:FortitudeRaysSmall2.part
#		part	:FortitudeRing2.part
#		lifespan	25
#	End
#End
#
#Condition
#	On 	cycle
#	Time 	20
#	Chance	0.125
#	
#	Event
#		EName	3
#		Type	Local
#		At	LarmL
#		part	:FortitudeRaysSmall2.part
#		part	:FortitudeRing2.part
#		lifespan	25
#	End
#End
#
#Condition
#	On 	cycle
#	Time 	21
#	Chance	0.125
#	
#	Event
#		EName	3
#		Type	Local
#		At	UarmL
#		part	:FortitudeRaysSmall2.part
#		part	:FortitudeRing2.part
#		lifespan	25
#	End
#End
#
#Condition
#	On 	cycle
#	Time 	22
#	Chance	0.125
#	
#	Event
#		EName	3
#		Type	Local
#		At	UarmR
#		part	:FortitudeRaysSmall2.part
#		part	:FortitudeRing2.part
#		lifespan	25
#	End
#End

#########################################################

End	
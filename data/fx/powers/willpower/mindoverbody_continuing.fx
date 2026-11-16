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
		Sound regen5b 60 60 .5
	End

	Event
		Type	Local
		At	Hips
		Sound glow5_loop 46 46 .11
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	400
	End
End

##Condition
##	On	Time
##	Time	31
##
##	Event
##		Ename	Ring
##		Type	Local
##		At	Hips
##		Part3 POWERS/Regeneration/ERing.part
##		Part5 POWERS/Regeneration/ERing2.part
##		Part1 POWERS/Regeneration/Cloud01.part
##		Part2 POWERS/Regeneration/Cloud02.part
##		Part4 POWERS/Regeneration/Cloud03.part
##	End
##End
##
##Condition
##	On	Time
##	Time	44
##
##	Event
##		Ename	Ring
##		Type	Local
##		At	Hips
##		Part3 POWERS/Regeneration/ERing.part
##		Part5 POWERS/Regeneration/ERing2.part
##		Part1 POWERS/Regeneration/Cloud01.part
##		Part2 POWERS/Regeneration/Cloud02.part
##		Part4 POWERS/Regeneration/Cloud03.part
##	End
##End

## RICHOCETS #######################################################

Condition
	On 	cycle
	Time 	17
	Chance	0.0625

	Event
		Type	Local
		At	LlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:FortitudeRaysSmall2.part
		part	:FortitudeRing2.part
		lifespan	15
	End
End

Condition
	On 	cycle
	Time 	18
	Chance	0.0625

	Event
		Type	Local
		At	LlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:FortitudeRaysSmall2.part
		part	:FortitudeRing2.part
		lifespan	15
	End
End

Condition
	On 	cycle
	Time 	19
	Chance	0.0625
	
	Event
		Type	Local
		At	LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:FortitudeRaysSmall2.part
		part	:FortitudeRing2.part
		lifespan	15
	End
End

Condition
	On 	cycle
	Time 	20
	Chance	0.0625
	
	Event
		Type	Local
		At	LarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:FortitudeRaysSmall2.part
		part	:FortitudeRing2.part
		lifespan	15
	End
End

Condition
	On 	cycle
	Time 	21
	Chance	0.0625
	
	Event
		Type	Local
		At	UarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:FortitudeRaysSmall2.part
		part	:FortitudeRing2.part
		lifespan	15
	End
End

Condition
	On 	cycle
	Time 	22
	Chance	0.0625
	
	Event
		Type	Local
		At	UarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:FortitudeRaysSmall2.part
		part	:FortitudeRing2.part
		lifespan	15
	End
End

#########################################################

End	
#########################################################
##	chill touch hit
FxInfo

LifeSpan	20

##################################

Condition
	On	Time
	Time	0

	Event
		EName	1
		Type	Local
		At	WepR
		part	:DroneHandGlows.part
		part	:DroneHandGlowsPurple.part
		part	:DroneHandGlowsBlue.part
		part	:Sparks.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	1
		Type	Local
		At	WepL
		part	:DroneHandGlows.part
		part	:DroneHandGlowsPurple.part
		part	:DroneHandGlowsBlue.part
		part	:Sparks.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

End


End				
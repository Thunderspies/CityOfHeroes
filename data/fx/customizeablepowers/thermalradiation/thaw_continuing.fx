#########################################################
##	FireAura
FxInfo

##################################

Condition
	On	Time
	Time	0

########################################################################
##SOUND
##########################################


	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Sound Firehand_loop 70.0 70.0 .45
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420



	End

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepL
		Sound flasharrowhit 70 70 .85


	End

	Event
		Type	Local
		At	root
		geom	HeatWaveSphere2
		bhvr	CustomizeablePowers\CorruptorBuff\ThermalRadiation\ScaleUp.bhvr
		LifeSpan	25
	End

End

##################################

#Condition
#	On	Time
#	Time	23
#
#	Event
#		Type	Local
#		At	root
#		geom	HeatWaveSphere
#		bhvr	:Scale.bhvr
#	End
#
#End


Condition
	On	Time
	Time	10

	Event
		EName	InitialSwordFire
		Type	Local
		At	Head
		bhvr	CustomizeablePowers\CorruptorBuff\ThermalRadiation\Fadeinandout.bhvr
		Part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\FireHands.part
		Part	CustomizeablePowers\CorruptorBuff\ThermalRadiation\FireHandzSparks.part

	End


End


End
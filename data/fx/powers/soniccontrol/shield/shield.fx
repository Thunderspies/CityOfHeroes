#########################################################
##	TemperatureControl
########################################################

FxInfo

##########################
Condition
	On 	Time
	Time 	0

	Event
		EName	core
		Type	Local
		At	Root
		Geom	LocalPivot01
		bhvr	:Offset.bhvr
		Sound maestro1 60 60 .33
	End

	Event
		EName	core1
		Type	Local
		At	core
		Geom	LocalPivot01
		bhvr	:OffsetSpin.bhvr
		Sound phase_loop 60 60 .28
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 180
	End


	Event
		EName	core3
		Type	Local
		At	core1
		  Altpiv   4
		part	:SonicRingCore.part
		part	:SonicRingz.part	
		
	
	End

End

Condition
	On 	Time
	Time 	13


	Event
		EName	corea
		Type	Local
		At	Root
		Geom	LocalPivot01
		bhvr	:Offset.bhvr
		Sound maestro1 60 60 .33
	End

	Event
		EName	core1a
		Type	Local
		At	corea
		Geom	LocalPivot01
		bhvr	:OffsetSpin.bhvr
		Sound phase_loop 60 60 .28
	End


	Event
		EName	core3a
		Type	Local
		At	core1a
		  Altpiv   4
		part	:SonicRingCore.part
		part	:SonicRingz.part	
		
	
	End

End





End
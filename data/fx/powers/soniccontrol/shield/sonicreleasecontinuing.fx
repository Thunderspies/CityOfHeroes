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
		At	head
		Geom	LocalPivot01
		bhvr	:HeadOffset.bhvr
		Sound maestro1 60 60 .33
	End

	Event
		EName	core1
		Type	Local
		At	core
		Geom	LocalPivot01
		bhvr	:HeadOffsetSpin.bhvr
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
		part	:ContinuingSonicRingCore.part
		part	:ContinuingSonicRing.part		
		
	
	End

End

Condition
	On 	Time
	Time 	20

	Event
		EName	corea
		Type	Local
		At	head
		Geom	LocalPivot01
		bhvr	:HeadOffset.bhvr
	End

	Event
		EName	core1a
		Type	Local
		At	corea
		Geom	LocalPivot01
		bhvr	:HeadOffsetSpin.bhvr
	End


	Event
		EName	core3a
		Type	Local
		At	core1a
		  Altpiv   4
		part	:ContinuingSonicRingCore.part
		part	:ContinuingSonicRing.part		
		
	
	End

End



End
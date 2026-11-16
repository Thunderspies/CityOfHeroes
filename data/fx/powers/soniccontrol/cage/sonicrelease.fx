#########################################################
##	TemperatureControl
########################################################

FxInfo

LifeSpan	25

##########################
Condition
	On 	Time
	Time 	0

	Event
		EName	core
		Type	Local
		At	Root
		Geom	LocalPivot01
		bhvr	:CageTopOffsetRelease.bhvr
		Sound maestro1 60 60 .33
	End

	Event
		EName	core2
		Type	Local
		At	Root
		Geom	LocalPivot01
		bhvr	:CageTopOffsetReleaseTop.bhvr
		#Sound	maestro1 60 60 .33
	End

	Event
		EName	bottemRings
		Type	Local
		At	Core2
		AltPiv	4
		Part	:SonicRingUpRelease.part
		Part	:SonicRingCoreUpRelease.part

	End
######################################################################


	Event
		EName	bottemRings
		Type	Local
		At	Core
		AltPiv	4

		Part	:SonicRingRelease.part
		Part	:SonicRingCoreRelease.part

	End
End

End
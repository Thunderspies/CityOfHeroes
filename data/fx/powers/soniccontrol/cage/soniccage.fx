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
		bhvr	:CageTopOffset.bhvr
		Sound soniccage_loop 70 70 .77
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 180
	End

	Event
		EName	bottemRings
		Type	Local
		At	Core
		AltPiv	4
		Part	:SonicRing.part
		Part	:SonicRingCore.part

	End
	
	Event
		Type	Local
		At	root

		bhvr	:CageOffset2.bhvr
		Part	:CoreHold.part
		Part	:Core.part


	End

	Event
		Type	Local
		At	root
		
		BHVR	:CageOffset2.bhvr

		Part	:CoreHoldGlow.part
		Part	:CoreGlow.part


	End

######################################################################


	Event
		EName	bottemRings
		Type	Local
		At	Root
		Part	:SonicRingUp.part
		Part	:SonicRingCoreUp.part

	End
End

End
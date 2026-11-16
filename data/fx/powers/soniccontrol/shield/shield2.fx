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
		bhvr	:Shield2Offset.bhvr
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

######################################################################


	Event
		EName	core2
		Type	Local
		At	Root
		Geom	LocalPivot01
		bhvr	:Shield2aOffset.bhvr
		#Sound	maestro1 60 60 .33
	End

	Event
		EName	core2a
		Type	Local
		At	core2
		Geom	LocalPivot01
		bhvr	:OffsetSpin.bhvr
		#Sound	phase_loop 60 60 .28
	End

	Event
		Type	Local
		At	Chest
		At	core1
		  Altpiv   4
		part	:SonicRingCore2.part
		part	:SonicRing2.part		
		
	
	End
	
#	Event
#		Type	Local
#		At	Chest
#		At	core2a
#		  Altpiv   4
#		part	:SonicRingCore2.part
#		#part	:SonicRing2.part		
#		
#	
#	End

#############################################################################

	Event
		EName	2core
		Type	Local
		At	Root
		Geom	LocalPivot01
		bhvr	:2Shield2Offset.bhvr
		Sound maestro1 60 60 .33
	End

	Event
		EName	2core1
		Type	Local
		At	2core
		Geom	LocalPivot01
		bhvr	:OffsetSpin.bhvr
		Sound phase_loop 60 60 .28
	End


	Event
		EName	2core2
		Type	Local
		At	Root
		Geom	LocalPivot01
		bhvr	:2Shield2aOffset.bhvr
		Sound maestro1 60 60 .33
	End

	Event
		EName	2core2a
		Type	Local
		At	2core2
		Geom	LocalPivot01
		bhvr	:OffsetSpin.bhvr
		Sound phase_loop 60 60 .28
	End

	Event
		Type	Local
		At	Chest
		At	2core1
		  Altpiv   4
		part	:SonicRingCore2.part
		part	:SonicRing2.part		
		
	
	End
	
#	Event
#		Type	Local
#		At	Chest
#		At	2core2a
#		  Altpiv   4
#		part	:SonicRingCore2.part
##		part	:SonicRing2.part		
#		
#	
#	End
#
End


End
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
		bhvr	:OffsetSlow.bhvr
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
		part	:SonicRingCoreSlow.part
		part	:SonicRingSlow.part	
		
	
	End

End
##########################
Condition
	On 	Time
	Time 	13

	Event
		EName	2core
		Type	Local
		At	Root
		Geom	LocalPivot01
		bhvr	:OffsetSlow.bhvr
		#Sound	maestro1 60 60 .33
	End

	Event
		EName	2core1
		Type	Local
		At	2core
		Geom	LocalPivot01
		bhvr	:OffsetSpin.bhvr
		#Sound	phase_loop 60 60 .28
	End


	Event
		EName	core3
		Type	Local
		At	2core1
		  Altpiv   4
		part	:SonicRingCoreSlow.part
		part	:SonicRingSlow.part	
		
	
	End

End
##########################
Condition
	On 	Time
	Time 	23

	Event
		EName	3core
		Type	Local
		At	Root
		Geom	LocalPivot01
		bhvr	:OffsetSlow.bhvr
		#Sound	maestro1 60 60 .33
	End

	Event
		EName	3core1
		Type	Local
		At	3core
		Geom	LocalPivot01
		bhvr	:OffsetSpin.bhvr
		#Sound	phase_loop 60 60 .28
	End


	Event
		EName	3core3
		Type	Local
		At	3core1
		  Altpiv   4
		part	:SonicRingCoreSlow.part
		part	:SonicRingSlow.part	
		
	
	End

End
##########################
Condition
	On 	Time
	Time 	33

	Event
		EName	4core
		Type	Local
		At	Root
		Geom	LocalPivot01
		bhvr	:OffsetSlow.bhvr
		#Sound	maestro1 60 60 .33
	End

	Event
		EName	4core1
		Type	Local
		At	4core
		Geom	LocalPivot01
		bhvr	:OffsetSpin.bhvr
		#Sound	phase_loop 60 60 .28
	End


	Event
		EName	4core3
		Type	Local
		At	4core1
		  Altpiv   4
		part	:SonicRingCoreSlow.part
		part	:SonicRingSlow.part	
		
	
	End

End
##########################
Condition
	On 	Time
	Time 	43

	Event
		EName	5core
		Type	Local
		At	Root
		Geom	LocalPivot01
		bhvr	:OffsetSlow.bhvr
		#Sound	maestro1 60 60 .33
	End

	Event
		EName	5core1
		Type	Local
		At	5core
		Geom	LocalPivot01
		bhvr	:OffsetSpin.bhvr
		#Sound	phase_loop 60 60 .28
	End


	Event
		EName	5core3
		Type	Local
		At	5core1
		  Altpiv   4
		part	:SonicRingCoreSlow.part
		part	:SonicRingSlow.part	
		
	
	End

End

##########################
Condition
	On 	Time
	Time 	53

	Event
		EName	6core
		Type	Local
		At	Root
		Geom	LocalPivot01
		bhvr	:OffsetSlow.bhvr
		#Sound	maestro1 60 60 .33
	End

	Event
		EName	6core1
		Type	Local
		At	6core
		Geom	LocalPivot01
		bhvr	:OffsetSpin.bhvr
		#Sound	phase_loop 60 60 .28
	End


	Event
		EName	6core3
		Type	Local
		At	6core1
		  Altpiv   4
		part	:SonicRingCoreSlow.part
		part	:SonicRingSlow.part	
		
	
	End

End


#####################################################################################################################


Condition
	On 	Time
	Time 	0

	Event
		EName	bore
		Type	Local
		At	Root
		Geom	LocalPivot01
		bhvr	:Offset.bhvr
		#Sound	maestro1 60 60 .33
	End

	Event
		EName	bore1
		Type	Local
		At	bore
		Geom	LocalPivot01
		bhvr	:OffsetSpin.bhvr
		#Sound	phase_loop 60 60 .28
	End


	Event
		EName	bore3
		Type	Local
		At	bore1
		  Altpiv   4
		part	:SonicRingCore.part
		part	:SonicRing.part		
		
	
	End

End


#####################################################################################################################


Condition
	On 	Time
	Time 	0

	Event
		EName	dore
		Type	Local
		At	Root
		Geom	LocalPivot01
		bhvr	:Offset.bhvr
		#Sound	maestro1 60 60 .33
	End

	Event
		EName	dore1
		Type	Local
		At	dore
		Geom	LocalPivot01
		bhvr	:OffsetSpin.bhvr
		#Sound	phase_loop 60 60 .28
	End


	Event
		EName	dore3
		Type	Local
		At	dore1
		  Altpiv   4
		part	:SonicRingCore.part
		part	:SonicRing.part		
		
	
	End

End





End
#########################################################
##	
##
FxInfo


Input  
	InpName	Root
End

####################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	root
		Sound Ice1_loop 60 60 .66
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End
End


Condition
	On 	Time
	Time 	8

	Event
		Type	Local 
		At	root
		Bhvr	CustomizeablePowers\IceMelee\IceRoot.bhvr
		geom	IceRoot
		
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	root
		
		Part1	CustomizeablePowers\IceMelee\IceRootRing.part
		#Sound	Ice1_loop 60 25 .56
		
	End

	Event
		Type	Local 
		At	root
		
		Part	CustomizeablePowers\IceMelee\IceRootRing2.part
		Part	CustomizeablePowers\IceMelee\IceRootChunks.part
		part	CustomizeablePowers\IceMelee\coldMistRoot.part
		Part	CustomizeablePowers\IceMelee\Sparklies.part
		Part	CustomizeablePowers\IceMelee\IceRootChunksSmall.part
		part	CustomizeablePowers\IceMelee\coldMistRootFlat.part
		Part	CustomizeablePowers\IceMelee\Sparklies.part
	
	End


End


End			
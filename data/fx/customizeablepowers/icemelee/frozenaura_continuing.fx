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
		Bhvr	CustomizeablePowers\IceMelee\IceRootflashFreeze.bhvr
		geom	IceRootFlashFreeze
		
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	root
		
		#Part1	:IceRootRing.part
		#Sound	Ice1_loop 60 25 .66
		
		#Part2	:IceRootRing2.part
	End

	Event
		Type	Local 
		At	root
		
		Part	CustomizeablePowers\IceMelee\IceRootChunks.part
		Part	CustomizeablePowers\IceMelee\IceRootChunksSmall.part
		part	CustomizeablePowers\IceMelee\FlashFreezecoldMistRoot.part
		Part	CustomizeablePowers\IceMelee\Sparklies.part
		part	CustomizeablePowers\IceMelee\FlashFreezecoldMistRootFlat.part
	End

	
End


End			
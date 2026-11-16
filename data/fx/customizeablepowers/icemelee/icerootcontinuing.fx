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
		bhvr	behaviors/soundFade3.bhvr
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
		Bhvr 	behaviors/IceRoot.bhvr
		geom	IceRoot
		
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	root
		
		Part1	:IceRootRing.part
		#Sound	Ice1_loop 60 25 .56
		
	End

	Event
		Type	Local 
		At	root
		
		Part	:IceRootRing2.part
		Part	:IceRootChunks.part
		part	:coldMistRoot.part
		Part	:Sparklies.part
		Part	:IceRootChunksSmall.part
		part	:coldMistRootFlat.part
		Part	:Sparklies.part
	
	End


End


End			
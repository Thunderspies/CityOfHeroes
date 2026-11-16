#########################################################
##	
##
FxInfo


Input  
	InpName	Root
End

####################################################

Condition
	On 	Time
	Time 	5

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
		Part2	:IceRootRing2.part
		Part	:IceRootChunks.part
		part	:coldMistRoot.part
		Part	:Sparklies.part
		part	:coldMistRootFlat.part
		Part	:Sparklies.part
		Part	:IceRootChunksSmall.part
		part	:coldMistRootFlat.part
	End


End


End			
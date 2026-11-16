#############################################################
## FlashFreezeContinuing.fx
#############################################################

FxInfo

#########################################################

Input
	InpName	Hips
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
		Bhvr 	behaviors/IceRootflashFreeze.bhvr
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

		Part	:IceRootChunks.part
		Part	:IceRootChunksSmall.part
		part	:FlashFreezecoldMistRoot.part
		Part	:Sparklies.part
		part	:FlashFreezecoldMistRootFlat.part
	End


End


End
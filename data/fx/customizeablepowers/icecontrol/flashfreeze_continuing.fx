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
		bhvr	behaviors\soundFade3.bhvr
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
		Bhvr	CustomizeablePowers\IceControl\IceRootflashFreeze.bhvr
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

		Part	CustomizeablePowers\IceControl\IceRootChunks.part
		Part	CustomizeablePowers\IceControl\IceRootChunksSmall.part
		part	CustomizeablePowers\IceControl\FlashFreezecoldMistRoot.part
		Part	CustomizeablePowers\IceControl\Sparklies.part
		part	CustomizeablePowers\IceControl\FlashFreezecoldMistRootFlat.part
	End


End


End
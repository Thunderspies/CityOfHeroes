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
		bhvr	Behaviors\soundFade3.bhvr
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
		Bhvr	CustomizeablePowers\IceBlast\IceRoot.bhvr
		geom	IceRoot

	End

End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	root

		Part1	CustomizeablePowers\IceBlast\IceRootRing.part
		#Sound	Ice1_loop 60 25 .56

	End

	Event
		Type	Local
		At	root

		Part	CustomizeablePowers\IceBlast\IceRootRing2.part
		Part	CustomizeablePowers\IceBlast\IceRootChunks.part
		part	CustomizeablePowers\IceBlast\coldMistRoot.part
		Part	CustomizeablePowers\IceBlast\Sparklies.part
		Part	CustomizeablePowers\IceBlast\IceRootChunksSmall.part
		part	CustomizeablePowers\IceBlast\coldMistRootFlat.part
		Part	CustomizeablePowers\IceBlast\Sparklies.part

	End


End


End
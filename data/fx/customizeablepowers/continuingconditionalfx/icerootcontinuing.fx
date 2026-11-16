#############################################################
## IceRootContinuing.fx
#############################################################

FxInfo

ClampMinScale 0.75 0.75 0.75
ClampMaxScale 1.25 1.25 1.25

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	root
		Sound Ice1_loop 60 60 0.66
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	root
		Part1	:IceRootRing.part
		Part	:IceRootRing2.part
		Part	:IceRootChunks.part
		Part	:IceRootChunksSmall.part
	End

	Event
		Type	Local
		At	root
		part	:coldMistRoot.part
		part	:coldMistRootFlat.part
		Part	:Sparklies.part
	End
End

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	root
		Bhvr 	behaviors\IceRoot.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	tintable_IceRoot
	End
End

#############################################################

End
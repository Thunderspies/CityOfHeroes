#############################################################
## FlashFreezeContinuing.fx
#############################################################

FxInfo
LifeSpan 50

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
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Part	:IceRootChunks.part
		Part	:IceRootChunksSmall.part
		Part	:Sparklies.part
		part	:FlashFreezecoldMistRoot.part
		part	:FlashFreezecoldMistRootFlat.part
		Sound Ice1_loop 60 60 0.66
	End
End

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	root
		Bhvr 	behaviors\IceRootflashFreeze.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_IceRootFlashFreeze
	End
End

#############################################################

End
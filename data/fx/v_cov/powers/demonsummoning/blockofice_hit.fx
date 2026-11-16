#############################################################
## FrozenAura_Hit.fx
#############################################################

FxInfo

#############################################################

Input
	InpName	Hips
End

#############################################################

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

	Event
		Type	Local
		At	Root
		Part	:BlockOfIce_Ring.part
		Part	:BlockOfIce_Ring2.part

		Part	:FrozenAura_IceChunks.part
		Part	:FrozenAura_IceChunks_Small.part
		part	:FrozenAura_Mist.part
		Part	:FrozenAura_Sparkles.part
		Part	:FrozenAura_Sparkles.part
		part	:FrozenAura_Mist_Flat.part
	End
End

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	Root
		Bhvr 	behaviors/IceRoot.bhvr
		BhvrOverride
			StartColor	180 230 255
		End
		geom	Tintable_IceRoot
	End

End

#############################################################

End
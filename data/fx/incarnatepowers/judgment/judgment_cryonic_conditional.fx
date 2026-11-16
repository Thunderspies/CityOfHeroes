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
		Sound Incarnate_CryonicConditional_Loop 60 60 .66
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 420
	End

	Event
		Type	Local
		At	Root

		Part	:FrozenAura_IceChunks.part
		Part	:FrozenAura_IceChunks_Small.part
		part	:FrozenAura_Mist.part
		Part	:FrozenAura_Sparkles.part
		part	:FrozenAura_Mist_Flat.part
	End
End

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	root
		Bhvr 	behaviors/IceRootflashFreeze.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_IceRootFlashFreeze
	End

End

#############################################################

End
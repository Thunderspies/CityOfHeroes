#############################################################
## Spawn_Dust.fx
#############################################################

FxInfo

#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	StartPositOnly
		At	Root
		Bhvr	behaviors/CameraShakeHydra.bhvr
		BhvrOverride
			PyrRotate	0 0 0
			PositionOffset	0 50 0
		End
		Part	:Glow_Ground.part
	End

	Event
		Type	StartPositOnly
		At	Root
		Bhvr	behaviors/CameraShakeHydra.bhvr
		BhvrOverride
			PyrRotate	-90 0 0
			PositionOffset	0 50 0
		End
		Part	:Glow.part
		Part	:Glow2.part
	End

End

#############################################################

End
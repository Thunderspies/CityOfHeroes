#############################################################
## Spawn_Dust.fx
#############################################################

FxInfo
LifeSpan 90

#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	On 	time
	Time 	0

	Event
		Type	StartPositOnly
		At	Root
		Bhvr	behaviors/CameraShakeHydra.bhvr
		BhvrOverride
			PositionOffset	0.0 -5.0 0.0
		End
		Part	:Dust_Root.part
		Lifespan	90
	End


	Event
		Type	StartPositOnly
		At	HandL
		BhvrOverride
			PositionOffset	0.0 -5.0 0.0
		End
		Part	:Dust_Hand.part
		Lifespan	90
	End

	Event
		Type	StartPositOnly
		At	HAndR
		BhvrOverride
			PositionOffset	0.0 -5.0 0.0
		End
		Part	:Dust_Hand.part
		Lifespan	90
	End
End

#############################################################

End
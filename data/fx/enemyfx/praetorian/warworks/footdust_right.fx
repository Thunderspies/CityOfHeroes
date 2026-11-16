#############################################################
## SweepingRay_Attack.fx
#############################################################

FxInfo
LifeSpan 400

#############################################################

Input
	InpName Hips
End

#############################################################


Condition
	On	Time
	Time	1

	Event
		Type	StartPositOnly
		At	FootR
		Bhvr	behaviors/CameraShake01Subtle.bhvr
		BhvrOverride
			PositionOffset	0.0 -3.0 0.0
		End
		Part	:Dust_Circle_Lingering_Light.part
		Lifespan 5
	End
End

#############################################################

End
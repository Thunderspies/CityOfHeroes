#############################################################
## TargetingReticle.fx
#############################################################

FxInfo
LifeSpan 80

#############################################################

Input
	InpName Hips
End

#############################################################


Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	T_Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.5 0.0
		End
		Part	:Target_Reticle_Small.part
		Part	:White_Noise_Small.part
		Lifespan 80
	End
End

#############################################################

End
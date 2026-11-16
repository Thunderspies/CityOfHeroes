#############################################################
## OrbitalLance_Tell.fx
#############################################################

FxInfo
LifeSpan 180

#############################################################

Input
	InpName Hips
End

#############################################################


Condition
	On 	Time
	Time 	20

	Event
		Type	Local
		At	Root
		Sound 	BAF_TargetingReticle_01 100 100 .8
	End


	Event
		Type	Local
		At	T_Root
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			Spin		0.0 0.025 0.0
			PositionOffset	0.0 0.5 0.0
		End
		Part	:Target_Reticle_Small.part
		Part	:White_Noise_Small.part
		Lifespan 80
	End
End

#############################################################

End
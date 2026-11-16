#############################################################
## Costume Aura - Flies (Body)
#############################################################

FxInfo
LIfespan 100
########################################################

Condition
	On 	Time
	Time 	0
	Random	1

	Event
		Type	Local
		At	Root
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PyrRotateJitter	90 90 90
			Spin	0 0.05 0.0
			PositionOffset	0.0 4 0.0
			StartJitter	2 2 2
			InitialVelocityJitter	0.03 0.03 0.03
		End
		Part	:Microbe_01.part
		Lifespan 5
	End

	Event
		Type	Local
		At	Root
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PyrRotateJitter	90 90 90
			Spin	0 0.05 0.0
			PositionOffset	0.0 4 0.0
			StartJitter	2 2 2
			InitialVelocityJitter	0.03 0.03 0.03
		End
		Part	:Microbe_02.part
		Lifespan 5
	End

	Event
		Type	Local
		At	Root
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PyrRotateJitter	90 90 90
			Spin	0 0.05 0.0
			PositionOffset	0.0 4 0.0
			StartJitter	2 2 2
			InitialVelocityJitter	0.03 0.03 0.03
		End
		Part	:Microbe_04.part
		Lifespan 5
	End
End



#############################################################

End
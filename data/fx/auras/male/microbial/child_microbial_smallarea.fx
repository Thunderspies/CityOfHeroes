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
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PyrRotateJitter	90 90 90
			Spin	0 0.05 0.0
			#PositionOffset	0.0 4 0.0
			#StartJitter	1 1 1
			InitialVelocityJitter	0.01 0.01 0.01
		End
		Part	:Microbe_05.part
		Lifespan 5
	End

	Event
		Type	Local
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PyrRotateJitter	90 90 90
			Spin	0 0.05 0.0
			#PositionOffset	0.0 4 0.0
			#StartJitter	1 1 1
			InitialVelocityJitter	0.01 0.01 0.01
		End
		Part	:Microbe_06.part
		Lifespan 5
	End

	Event
		Type	Local
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PyrRotateJitter	90 90 90
			Spin	0 0.05 0.0
		#	PositionOffset	0.0 4 0.0
			#StartJitter	1 1 1
			InitialVelocityJitter	0.01 0.01 0.01
		End
		Part	:Microbe_07.part
		Lifespan 5
	End
End



#############################################################

End
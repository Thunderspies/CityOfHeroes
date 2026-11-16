#############################################################
## Costume Aura - Flies (Body)
#############################################################

FxInfo
LIfespan 60
########################################################

Condition
	On 	Time
	Time 	0

#	Event
#		EName	SpinnerAnchor
#		Type	Local
#		At	Origin
#		BhvrOverride
#			PyrRotateJitter	90 90 90
#			Spin	0 0.5 0.0
#		End
#		Lifespan 20
#	End

	Event
		Type	Local
		At	Origin #SpinnerAnchor
		BhvrOverride
			PyrRotateJitter	90 90 90
			Spin	0 0.5 0.0
		End
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:Electron_Trail_Stationary_Hand.part
	End
End

#############################################################

End
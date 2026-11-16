#############################################################
## Costume Aura - Flies (Body)
#############################################################

FxInfo
LIfespan 60
########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	SpinnerAnchor
		Type	Local
		At	Chest
		BhvrOverride
			PyrRotateJitter	90 90 90
			Spin	0 0.15 0.0
		End
	End

	Event
		Type	Local
		At	SpinnerAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	2.0 0.0 0.0
			StartJitter	0.5 0.0 0.0
		End
		part	:Electron_Head_Core_Large.part
		part	:Electron_Head_Flare_Large.part
		part	:Electron_Trail_Large.part
		Part	:Electron_Trail_Bright.part
	End

	Event
		Type	Local
		At	SpinnerAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr

		part	:Electron_Trail_Stationary.part
	End
End

#############################################################

End
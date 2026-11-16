#
FxInfo

	LifeSpan 100


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Local
			EName SpinnerAnchor1
			BhvrOverride
				#PyrRotateJitter 20 90 20
				#StartJitter 0 1.5 0
				PositionOffset 0 1 0
				Spin 0 -0.10 0
				Scale 1 1 1
			End
		End

		Event
			At SpinnerAnchor1
			Type Local
			EName SpinnerTrail
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 2 0 0
				StartJitter 0.5 0 0
			End
			Part :Electron_Trail_Large.part
			Part :Electron_Trail_Bright.part
			Part :Electron_Head_Core_Large.part
			Part :Electron_Head_Flare_Large.part
			Part :Electron_Head_Core_Largest.part
			Part :Electron_Head_Core_Largest_Add.part
		End

		Event
			At SpinnerAnchor1
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Electron_Trail_Stationary.part
		End

		Event
			At Origin
			Type Local
			EName SpinnerAnchor2
			BhvrOverride
				PyrRotate 0 180 0
				#StartJitter 0 1.5 0
				PositionOffset 0 -1 0
				Spin 0 -0.10 0
				Scale 1 1 1
			End
		End

		Event
			At SpinnerAnchor2
			Type Local
			EName SpinnerTrail
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 2 0 0
				StartJitter 0.5 0 0
			End
			Part :Electron_Trail_Large.part
			Part :Electron_Trail_Bright.part
			Part :Electron_Head_Core_Large.part
			Part :Electron_Head_Flare_Large.part
			Part :Electron_Head_Core_Largest.part
			Part :Electron_Head_Core_Largest_Add.part
		End

		Event
			At SpinnerAnchor2
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Electron_Trail_Stationary.part
		End

	End

End

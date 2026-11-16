Fxinfo

	Condition
		On Cycle
		Time 120
		Chance 0.5

		Event
			At Root
			Type Local
			BhvrOverride
				StartJitter 3000 0 3000
			End
#			childfx World/City/BlueLight01.fx
			SoundNoRepeat 3
			Sound DA_EarthShift_01 6000 3000 0.9
			Sound DA_EarthShift_02 6000 3000 0.9
			Sound DA_EarthShift_03 6000 3000 0.9
			Sound DA_EarthShift_04 6000 3000 0.9
			Sound DA_EarthShift_05 6000 3000 0.9
			Sound DA_EarthShift_06 6000 3000 0.9
			Sound DA_EarthShift_07 6000 3000 0.9
			lifespan 1
		End

	End
End
#                                                          
FxInfo

	Flags DontSuppress


###########################################################

	Condition
		On TimeOfDay
		DayStart 6.0000
		DayEnd 16.0000
		DoMany 1

		Event
			At Root
			Type StartPositOnly
			CameraSpace 
			EName StormParts
			Flags OneAtATime
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part WORLD\Ruins\Dust_Fullscreen_01.part
			Part WORLD\Ruins\Dust_Particles.part
			Part WORLD\Ruins\Dust_Fullscreen_02.part
		End

	End

	Condition
		On TimeOfDay
		DayStart 17.0000
		DayEnd 5.0000
		DoMany 1

		Event
			At Root
			Type StartPositOnly
			CameraSpace 
			EName OneAtATime2
			Flags OneAtATime
			Part WORLD\Ruins\Dust_Particles_Dark.part
			Part WORLD\Ruins\Dust_Fullscreen_01.part
		End

	End

End

#
FxInfo

	LifeSpan 250


###########################################################

#	Condition
#		On Time
#		Time 5
#
#		Event
#			At Root
#			Type Local
#			Sound Explosion_Fullscreen 100 100 0.8
#		End
#
#	End

	Condition
		On Time
		Time 0

		Event
			Type StartPositOnly
			CameraSpace
			EName Holocaust
			BhvrOverride
				PositionOffset 0 0 20
			End
			Part Events\EndGame\Magisterium\CAMERA_AirStreaks.part
			LifeSpan 25
		End

		Event
			Type StartPositOnly
			CameraSpace
			EName Holocaust
			Part WORLD\City\CAMERA_Debris.part
			LifeSpan 25
		End

		Event
			Type StartPositOnly
			CameraSpace
			EName flash
			Flags OneAtATime
			Part :Nuke_Flash2.part
		End

	End

	Condition
		On Time
		Time 25

		Event
			Type StartPositOnly
			CameraSpace
			EName Holocaust
			Part :CAMERA_Dark_Clouds.part
			Part Events\EndGame\Magisterium\CAMERA_Dust2.part
			#Part :CAMERA_Dust.part
			LifeSpan 25
		End

	End

	Condition
		On Time
		Time 70

		Event
			Type Start
			CameraSpace
			EName Dust
			#Part WORLD\City\Explosion_FullScreen_Smoke.part
			LifeSpan 40
		End

	End

End

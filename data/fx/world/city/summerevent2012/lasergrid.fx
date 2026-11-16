#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0 0 0
				Alpha 60
				StartColor 15 205 255
			End
			Geom LaserGrid
		End

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 20 0
			End
			Part WORLD\City\Summerevent2012\Cold_Mist_Rising.part
		End

		Event
			At Root
			Type StartPositOnly
			#Geom LaserGrid_Base
		End

	End

End

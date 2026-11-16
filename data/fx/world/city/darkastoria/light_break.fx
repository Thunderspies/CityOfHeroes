#
FxInfo


###########################################################


Condition
	On 	Time
	Time	25
	Event
		Type	Local
		At	Root
		Sound 	DA_Lightbreak 100 100 .8
	End
End


	Condition
		On Time
		Time 0

		Event
			At Root
			Type Start
			CameraSpace
			Part WORLD\City\DarkAstoria\Black.part
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 25

		Event
			At Root
			Type Start
			CameraSpace
			BhvrOverride
				PositionOffset 0 5 0
			End
			Part WORLD\City\DarkAstoria\Light_BreakOut.part
		End

	End

	Condition
		On Time
		Time 35

		Event
			At Root
			Type Start
			CameraSpace
			BhvrOverride
				PositionOffset 0 5 0
			End
			Part WORLD\City\DarkAstoria\Light_BreakOut.part
		End

	End

	Condition
		On Time
		Time 50

		Event
			At Root
			Type Start
			CameraSpace
			BhvrOverride
				PositionOffset 0 5 0
			End
			Part WORLD\City\DarkAstoria\Light_BreakOut_Flash.part
			Part WORLD\City\DarkAstoria\Light_BreakOut_Flash_hilite.part
			Part WORLD\City\DarkAstoria\Light_BreakOut.part
		End

	End

End

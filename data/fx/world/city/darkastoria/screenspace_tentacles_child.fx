#
FxInfo


###########################################################
Condition
	On Time
	Time 0

	Event
		Ename TopAnchor
		At Origin
		Type Local
		BhvrOverride
			PositionOffset 0 5 0
		End
	End

	Event
		Ename BottomAnchor
		At Origin
		Type Local
		BhvrOverride
			PositionOffset 0 -2 0
		End
	End
End


	Condition
		On Cycle
		Time 60
		#Chance 0.8

		Event
			At root
			Type Start
			CameraSpace
			BhvrOverride
				PositionOffset 0 -2 0
			End
			Part :ScreenTentacle_Up.part
			LifeSpan 1
		End

		Event
			At Root
			Type Start
			CameraSpace
			BhvrOverride
				PositionOffset 0 2 0
			End
			Part :ScreenTentacle_Down.part
			LifeSpan 50
		End

	End

End

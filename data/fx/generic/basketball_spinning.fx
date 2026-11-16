#############################################################
## Basketball_Spinning.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Basketball
		Type	Local
		At	Mystic
		BhvrOverride
			Scale		0.5 0.5 0.5
			Spin		0.075 0.075 0.075
		End

		Geom	Basketball
	End
End

#############################################################

End
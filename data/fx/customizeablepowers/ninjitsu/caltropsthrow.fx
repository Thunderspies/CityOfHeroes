#############################################################
## CaltropsThrow.fx
#############################################################

FxInfo
Lifespan		200

#################################################################################

Condition
	On	Time
	Time	17

	Event
		Type	Local
		At	origin
		Sound caltropstoss 60 60 .8
	End
End

Condition
	On 	Time
	Time 	19

	Event
		EName	Prime
		Type	start
		At	WepR

		Bhvr	CustomizeablePowers\Ninjitsu\CaltropsLobb.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Caltropsthrow

		Magnet	T_Root
	End

	Event
		EName	Prime2
		Type	start
		At	WepR

		Bhvr	CustomizeablePowers\Ninjitsu\CaltropsLobb2.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Caltropsthrow

		Magnet	T_Root
	End
End

Condition
	On	PrimeHit


	Event
		Type    Destroy
		EName 	Prime

	End

End

Condition
	On	Prime2Hit


	Event
		Type    Destroy
		EName 	Prime2

	End

End

End
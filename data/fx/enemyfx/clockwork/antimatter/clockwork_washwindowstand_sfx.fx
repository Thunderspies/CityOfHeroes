#############################################################
## Behavior SFX
#############################################################

FxInfo

LifeSpan 50

Condition
	On Time
	Time 0
	Event
		Type	Local
		At	Root
		Sound ClockworkLoyalists_Idle_Headset1 50 50 .5
	End
End

Condition
	On Time
	Time 10
	Event
		Type	Local
		At	Root
		Sound ClockworkLoyalists_Attack_Wrist_Resume 50 50 .2
	End
End


Condition
	On Time
	Time 50
	Event
		Type	Local
		At	Root
		Sound ClockworkLoyalists_Idle_Headset5 50 50 .5
	End
End

#########################################################
## Player Emote Howl - Sound FX
#########################################################

FxInfo

LifeSpan 20


Condition
	On	Time
	Time	0
	Event
		Type Local
		At origin
		Sound Pushup_Down_1 100 100 .06
		Sound Pushup_Down_2 100 100 .045
		Sound Pushup_Down_3 100 100 .06
		Sound Pushup_Down_4 100 100 .05
		Sound Pushup_Down_5 100 100 .055
	End

End
Condition
	On	Time
	Time	15
	Event
		Type Local
		At origin
		Sound Pushup_Up_1 100 100 .04
		Sound Pushup_Up_2 100 100 .03
		Sound Pushup_Up_3 100 100 .025
		Sound Pushup_Up_4 100 100 .03
		Sound Pushup_Up_5 100 100 .035
	End

End
#########################################################
## 	WeaveWhoosh
#########################################################

FxInfo

Flags SoundOnly

Input
	InpName Origin
End

Lifespan 65

Condition
	On	Time
	Time	0
	Event
		Type Local
		At origin
		Sound WHoosh_01 30 30 .2
		Sound WHoosh_02 30 30 .2
		Sound WHoosh_03 30 30 .2
		Sound WHoosh_04 30 30 .2
		Sound WHoosh_05 30 30 .2
		Sound WHoosh_06 30 30 .2
	End
End
#########################################################
## 	Huge Left Foot
#########################################################

FxInfo

Input
	InpName Origin
End

LifeSpan 30

Condition
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 3
		Sound Dog_Snarl_01 50 50 .45
		Sound Dog_Snarl_02 50 50 .45
		Sound Dog_Snarl_03 50 50 .45
		Sound Dog_Snarl_04 50 50 .45
		Sound Dog_Snarl_05 50 50 .45
		Sound Dog_Snarl_06 50 50 .45
	End

End

Condition
	On 	Time
	Time	30
	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound 	Dog_GrowlShort_01 100 100 .5
		Sound 	Dog_GrowlShort_02 100 100 .5
		Sound 	Dog_GrowlShort_03 100 100 .5
		Sound 	Dog_GrowlShort_04 100 100 .5
		Sound 	Dog_GrowlShort_05 100 100 .5
	End
End

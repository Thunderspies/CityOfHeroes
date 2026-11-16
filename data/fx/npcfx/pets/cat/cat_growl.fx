#############################################################
## Cat_Roar.fx
#############################################################

FxInfo

FxInfo
Lifespan 1



Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound 	Cat_Growl_01 50 50 .45
		Sound 	Cat_Growl_02 50 50 .45
		Sound 	Cat_Growl_03 50 50 .45
		Sound 	Cat_Growl_04 50 50 .45
		Sound 	Cat_Growl_05 50 50 .45
	End
End


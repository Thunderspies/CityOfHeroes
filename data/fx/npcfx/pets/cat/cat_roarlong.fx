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
		SoundNoRepeat 1
		Sound 	Cat_RoarLong_01 50 50 .45
		Sound 	Cat_RoarLong_02 50 50 .45	
	End
End


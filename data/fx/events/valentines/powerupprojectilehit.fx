#########################################################
##	
##

FxInfo

LifeSpan	20

##################################

COndition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound VialSparklehit 70 70 .66
	End
End


Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Hips

		Part	:1PowerUpSteamAdditive.part
		Part	:1PowerUpStreamDust02.part
		Part	:1PowerUpMainStreamChunks01.part

	End

End


End

	
###########################################################

FxInfo
Lifespan 60

###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
#		Sound 	DD_EnervatingStrike_Hit_01 100 100 .8
	End
End


	Condition
		On Time
		Time 11

		Event
			At Jaw
			Type Local
			Part :MaimingBite_Sparks.part
			Part :MaimingBite_Splash.part

			LifeSpan 50
		End
	End
End

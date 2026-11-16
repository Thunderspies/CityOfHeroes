#########################################################
##	forceBubble
########################################################
FxInfo

Lifespan 60

Condition
	On 	Time
	Time	0

	Event
		Ename	Whack

		Type	Local
		At	Target
		Part1	:LightningStrikeHaze.part
		Part2	:ShockFieldArchs2a.part
		Part3	:ShockFieldArchs1a.part

		lifespan	30
	End

End

Condition
	On 	Time
	Time 	6

	Event
		EName 	Whack
		Type	Destroy
	End


End
